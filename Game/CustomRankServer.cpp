CCustomRankServer::CCustomRankServer() {

}

CCustomRankServer::~CCustomRankServer() {

}

void CCustomRankServer::LoadRankServer() {
	if (!sGameServer->IsRankServerEnable())
		return;

	LIST_CLEAR(RankData::iterator, this->m_SvRankListData);

	QueryResult result = MuDatabase.Query("SELECT guid, name, race, reset, level, level_master, level_majestic from character_info where authority=0 order by `reset` desc, `level_majestic` desc, `level_master` desc, `level` desc");

	uint8 count = 0;

	if (result) {
		do {
			FieldReader reader(result->Fetch());

			RankServerData* data = new RankServerData;

			data->m_Rank = count;
			data->m_GUID = reader.GetInt16();
			data->m_PlayerName = reader.GetString();
			data->m_Class = reader.GetInt16();
			data->m_Reset = reader.GetInt16();
			data->m_Level = reader.GetInt16();
			data->m_MasterLevel = reader.GetInt16();
			data->m_MajesticLevel = reader.GetInt16();

			this->m_SvRankListData.push_back(data);
			count++;
		} while (result->NextRow());
	}
}

void CCustomRankServer::SendRankServerData(Player* pPlayer) {
	if (!sGameServer->IsRankServerEnable())
		return;

	if (!pPlayer)
		return;

	uint8 buffer[4092];
	POINTER_PCT(RankServer_Head, head, buffer, 0);
	POINTER_PCT(RankServer_Body, body, buffer, sizeof(RankServer_Head));
	head->count = 0;

	for (RankData::const_iterator itr = this->m_SvRankListData.begin(); itr != this->m_SvRankListData.end(); ++itr) {
		if (!(*itr))
			return;

		if (head->count >= MAX_VIEW_RANK_PLAYER)
			break;

		strcpy(body[head->count].pPlayerName, (*itr)->m_PlayerName.c_str());
		body[head->count].Class = (*itr)->m_Class;
		body[head->count].Reset = (*itr)->m_Reset;
		body[head->count].Level = (*itr)->m_Level;
		body[head->count].MasterLevel = (*itr)->m_MasterLevel;
		body[head->count].MajesticLevel = (*itr)->m_MajesticLevel;
		++head->count;
	}

	head->Set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_SV_RANK_INFO, sizeof(RankServer_Head) + (head->count * sizeof(RankServer_Body)));
	pPlayer->sendPacket(buffer, head->GetSize());
}
