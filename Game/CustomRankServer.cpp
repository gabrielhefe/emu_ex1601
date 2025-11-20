CCustomRankServer::CCustomRankServer() {

}

CCustomRankServer::~CCustomRankServer() {

}

void CCustomRankServer::LoadRankServer() {
	if (!sGameServer->IsRankServerEnable())
		return;

	LIST_CLEAR(RankData::iterator, this->sv_rank_list_data);

	QueryResult result = MuDatabase.Query("SELECT guid, name, race, reset, level, level_master, level_majestic from character_info where authority=0 order by `reset` desc, `level_majestic` desc, `level_master` desc, `level` desc");

	uint8 count = 0;

	if (result) {
		do {
			FieldReader reader(result->Fetch());

			RankServerData* data = new RankServerData;

			data->SetRank(count);
			data->SetGUID(reader.GetInt16());
			data->SetpPlayerName(reader.GetString());
			data->SetClass(reader.GetInt16());
			data->SetReset(reader.GetInt16());
			data->SetLevel(reader.GetInt16());
			data->SetMasterLevel(reader.GetInt16());
			data->SetMajesticLevel(reader.GetInt16());

			this->sv_rank_list_data.push_back(data);
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

	for (RankData::const_iterator itr = sv_rank_list_data.begin(); itr != sv_rank_list_data.end(); ++itr) {
		if (!(*itr))
			return;

		if (head->count >= MAX_VIEW_RANK_PLAYER)
			break;

		strcpy(body[head->count].pPlayerName, (*itr)->GetpPlayerName().c_str());
		body[head->count].Class = (*itr)->GetClass();
		body[head->count].Reset = (*itr)->GetReset();
		body[head->count].Level = (*itr)->GetLevel();
		body[head->count].MasterLevel = (*itr)->GetMasterLevel();
		body[head->count].MajesticLevel = (*itr)->GetMajesticLevel();
		++head->count;
	}

	head->Set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_SV_RANK_INFO, sizeof(RankServer_Head) + (head->count * sizeof(RankServer_Body)));
	pPlayer->sendPacket(buffer, head->GetSize());
}