CDynamicExpMgr::CDynamicExpMgr() {

}

CDynamicExpMgr::~CDynamicExpMgr() {
	LIST_CLEAR(DynamicExpData::iterator, this->m_dynamic_exp_data);
}

void CDynamicExpMgr::LoadDynamicExperienceData(char* pchFileName) {
	sLog->outInfo(LOG_DEFAULT, "Load Dynamic Experience Data");

	LIST_CLEAR(DynamicExpData::iterator, this->m_dynamic_exp_data);
	
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("ExpSystem");
	pugi::xml_node DynamicExpNode = Main.child("DynamicExp");

	sGameServer->SetDynamicExperience(DynamicExpNode.attribute("Enabled").as_bool());

	//load static exp
	pugi::xml_node StaticExp = Main.child("StaicExp");
	sGameServer->SetNormalExperience(StaticExp.attribute("Normal").as_int());
	sGameServer->SetMasterExperience(StaticExp.attribute("Master").as_int());
	sGameServer->SetMajesticExperience(StaticExp.attribute("Majestic").as_int());

	//load party exp
	pugi::xml_node PartyExp = Main.child("PartyExp");
	sGameServer->SetPartyExperience(0, PartyExp.attribute("One").as_int());
	sGameServer->SetPartyExperience(1, PartyExp.attribute("Two").as_int());
	sGameServer->SetPartyExperience(2, PartyExp.attribute("Three").as_int());
	sGameServer->SetPartyExperience(3, PartyExp.attribute("Four").as_int());
	sGameServer->SetPartyExperience(4, PartyExp.attribute("Five").as_int());

	pugi::xml_node PartyExpBonus = Main.child("PartyExpBonus");
	sGameServer->SetPartyBonusExperience(0, PartyExpBonus.attribute("One").as_int());
	sGameServer->SetPartyBonusExperience(1, PartyExpBonus.attribute("Two").as_int());
	sGameServer->SetPartyBonusExperience(2, PartyExpBonus.attribute("Three").as_int());
	sGameServer->SetPartyBonusExperience(3, PartyExpBonus.attribute("Four").as_int());
	sGameServer->SetPartyBonusExperience(4, PartyExpBonus.attribute("Five").as_int());

	uint32 count = 0;

	for (pugi::xml_node DynamicExp = DynamicExpNode.child("Range"); DynamicExp; DynamicExp = DynamicExp.next_sibling()) {
		dynamic_exp_data* data = new dynamic_exp_data;

		data->SetId(DynamicExp.attribute("Index").as_int());
		data->SetMinTotalLevel(DynamicExp.attribute("MinLevel").as_int());
		data->SetMaxTotalLevel(DynamicExp.attribute("MaxLevel").as_int());
		data->SetMinResetTime(DynamicExp.attribute("MinRs").as_int());
		data->SetMaxResetTime(DynamicExp.attribute("MaxRs").as_int());
		data->SetNormalExpRate(DynamicExp.attribute("NormalExp").as_int());
		data->SetMasterExpRate(DynamicExp.attribute("MasterExp").as_int());
		data->SetMajesticExpRate(DynamicExp.attribute("MajesticExp").as_int());

		this->m_dynamic_exp_data.push_back(data);
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u Dynamic Experience definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

int CDynamicExpMgr::ExpToPlayer(Player* pPlayer) {

	uint32 playerLevelTotal = 0;
	uint32 playerLevelNormal = pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel();
	uint32 playerLevelMaster = pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetLevel();
	uint32 playerLevelMajestic = pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel();
	uint32 playerReset = pPlayer->GetResetSystem(RESET_TOTAL)->GetReset();

	uint32 id = 0;
	uint32 min_total_level = 0;
	uint32 max_total_level = 0;
	uint32 min_reset_time = 0;
	uint32 max_reset_time = 0;
	uint32 normal_exp_rate = 0;
	uint32 master_exp_rate = 0;
	uint32 majestic_exp_rate = 0;

	uint32 experience = 0;

	for (DynamicExpData::const_iterator it = this->m_dynamic_exp_data.begin(); it != this->m_dynamic_exp_data.end(); it++) {
		dynamic_exp_data const* exp_data = (*it);

		if (!exp_data) {
			continue;
		}

		playerLevelTotal = playerLevelNormal;

		if (pPlayer->IsMaster()) {
			playerLevelTotal += playerLevelMaster;
		}

		if (pPlayer->IsMajestic()) {
			playerLevelTotal += playerLevelMajestic;
		}

		if (playerLevelTotal >= exp_data->GetMinTotalLevel() && playerLevelTotal <= exp_data->GetMaxTotalLevel() && playerReset >= exp_data->GetMinResetTime() && playerReset <= exp_data->GetMaxResetTime()) {
			id = exp_data->GetId();
			min_total_level = exp_data->GetMinTotalLevel();
			max_total_level = exp_data->GetMaxTotalLevel();
			min_reset_time = exp_data->GetMinResetTime();
			max_reset_time = exp_data->GetMaxResetTime();
			normal_exp_rate = exp_data->GetNormalExpRate();
			master_exp_rate = exp_data->GetMasterExpRate();
			majestic_exp_rate = exp_data->GetMajesticExpRate();
			break;
		}
	}

	if (pPlayer->IsMajestic()) {
		return experience = majestic_exp_rate;
	}
	else if (pPlayer->IsMaster()) {
		return experience = master_exp_rate;
	}
	else {
		return experience = normal_exp_rate;
	}


}