CResetMgr::CResetMgr() {

}

CResetMgr::~CResetMgr() {
	LIST_CLEAR(ResetDataList::iterator, this->m_reset_data);
}

void CResetMgr::LoadResetData(char* pchFileName) {
	sLog->outInfo(LOG_DEFAULT, "Loading Reset System Data...");
	LIST_CLEAR(ResetDataList::iterator, this->m_reset_data);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("ResetSystem");
	sGameServer->SetResetSystemEnbale(Main.attribute("Enable").as_bool());

	pugi::xml_node RsSetting = Main.child("Setting");
	sGameServer->SetResetAutoCommand(RsSetting.attribute("ResetAutoCmd").as_bool());
	sGameServer->SetResetMasterLevel(RsSetting.attribute("ResetLvMasterAndMajestic").as_bool());
	sGameServer->SetResetMajesticLevel(RsSetting.attribute("ResetLvMasterAndMajestic").as_bool());
	sGameServer->SetResetReward(RsSetting.attribute("ResetReward").as_bool());
	sGameServer->SetResetKeepPoints(RsSetting.attribute("KeepPoints").as_bool());
	sGameServer->SetResetMoveChar(RsSetting.attribute("MoveChar").as_bool());
	sGameServer->SetResetRelog(RsSetting.attribute("Relog").as_bool());
	sGameServer->SetMaxReset(RsSetting.attribute("MaxReset").as_int());

	uint32 count = 0;

	for (pugi::xml_node reset = Main.child("Reset"); reset; reset = reset.next_sibling()) {
		reset_data* add_reset_data = new reset_data;

		add_reset_data->SetId(reset.attribute("Index").as_int());
		add_reset_data->SetMinLevelRequire(reset.attribute("MinLevelReq").as_int());
		add_reset_data->SetMinRSTime(reset.attribute("MinRsTime").as_int());
		add_reset_data->SetMaxRSTime(reset.attribute("MaxRsTime").as_int());
		add_reset_data->SetPointsPerReset(reset.attribute("PointsPerRs").as_int());
		add_reset_data->SetZenRequire(reset.attribute("ZenRq").as_int());
		add_reset_data->SetChaosRequire(reset.attribute("ChaosRq").as_int());
		add_reset_data->SetCreRequire(reset.attribute("CreRq").as_int());
		add_reset_data->SetBlueRequire(reset.attribute("FeatherRq").as_int());
		add_reset_data->SetRewardItemType(reset.attribute("RewardItemType").as_int());
		add_reset_data->SetRewardItemIndex(reset.attribute("RewardItemIndex").as_int());

		this->m_reset_data.push_back(add_reset_data);

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u Reset System definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool CResetMgr::PlayerReset(Player * pPlayer) {
	uint32 playerRSCount = pPlayer->GetResetSystem(RESET_TOTAL)->GetReset() + 1;
	uint32 playerLevel = pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel();
	uint32 playerClass = pPlayer->GetClass();
	uint32 playerZen = pPlayer->MoneyGet();
	uint32 playerChaos = pPlayer->GetInventory()->GetItemCount(6159, 0);
	uint32 playerCre = pPlayer->GetInventory()->GetItemCount(7190, 0);
	uint32 playerBlue = pPlayer->GetInventory()->GetItemCount(6670, 0);

	if (playerRSCount >= sGameServer->GetMaxReset()) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Your Reset Is Max");
		return false;
	}

	uint32 id = 0;
	uint32 min_level_require = 0;
	uint32 min_rs_time = 0;
	uint32 max_rs_time = 0;
	uint32 points_per_rs = 0;
	uint32 zen_require = 0;
	uint32 chaos_require = 0;
	uint32 cre_require = 0;
	uint32 blue_require = 0;
	uint32 reward_item_type = 0;
	uint32 reward_item_index = 0;
	uint32 old_points_stats = 0;

	for (ResetDataList::const_iterator it = this->m_reset_data.begin(); it != this->m_reset_data.end(); it++) {
		reset_data const* rsdata = (*it);

		if (!rsdata) {
			continue;
		}

		if (playerRSCount >= rsdata->GetMinRSTime() && playerRSCount <= rsdata->GetMaxRSTime()) {
			id = rsdata->GetId();
			min_level_require = rsdata->GetMinLevelRequire();
			min_rs_time = rsdata->GetMinRSTime();
			max_rs_time = rsdata->GetMaxRSTime();
			points_per_rs = rsdata->GetPointsPerReset();
			zen_require = rsdata->GetZenRequire();
			chaos_require = rsdata->GetChaosRequire();
			cre_require = rsdata->GetCreRequire();
			blue_require = rsdata->GetBlueRequire();
			reward_item_type = rsdata->GetRewardItemType();
			reward_item_index = rsdata->GetRewardItemIndex();
			//old_points_stats = rsdata->GetPointsOldIdRS();
			break;
		}
	}

	if (sGameServer->IsResetAutoCommand()) {
		if (playerLevel < min_level_require) {
			return false;
		}
	}

	if (min_rs_time == 0 && max_rs_time == 0)
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "No Reset Data");
		return false;
	}

	if (playerLevel < min_level_require) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Character is not level enough to reset");
		return false;
	}

	if (playerZen < zen_require)
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Not Enough Zen Require");
		return false;
	}

	if (playerChaos < chaos_require)
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Not Enough Jewel of Chaos Require");
		return false;
	}

	if (playerCre < cre_require) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Not Enough Jewel of Creation Require");
		return false;
	}

	if (playerBlue < blue_require) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Not Enough Loch's Feather Require");
		return false;
	}

	if (zen_require > 0) {
		pPlayer->MoneySet(playerZen - zen_require);
	}

	if (chaos_require > 0) {
		pPlayer->ItemDelete(6159, 0, chaos_require);
	}

	if (cre_require > 0) {
		pPlayer->ItemDelete(7190, 0, cre_require);
	}

	if (blue_require > 0) {
		pPlayer->ItemDelete(6670, 0, blue_require);
	}

	if (sGameServer->IsResetMajesticLevel()) {
		pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->Reset();
		pPlayer->MajesticTreeReset();
	}


	if (sGameServer->IsResetMasterLevel()) {
		pPlayer->GetLevelData(LEVEL_DATA_MASTER)->Reset();
		pPlayer->MasterSkillReset(0);
	}

	if (sGameServer->IsResetReward()) {
		if (reward_item_index >= 0 && reward_item_type >= 0) {
			sItemMgr->ItemSerialCreateGremoryCase(pPlayer, ITEMGET(reward_item_type, reward_item_index), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
		}
	}
	
	pPlayer->GetResetSystem(RESET_TOTAL)->SetReset(pPlayer->GetResetSystem(RESET_TOTAL)->GetReset() + 1);

	if (sGameServer->IsResetKeepPoints()) {
		pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetLevel(1);
		pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetExperience(0);
		pPlayer->NormalSendLevelUp(false);
		if (sGameServer->IsResetMoveChar()) {
			if (playerClass == Character::FAIRY_ELF || playerClass == Character::RUNE_WIZARD) {
				pPlayer->MoveToGate(27);
			}
			else if (playerClass == Character::SUMMONER || playerClass == Character::GUN_CRUSHER) {
				pPlayer->MoveToGate(267);
			}
			else {
				pPlayer->MoveToGate(17);
			}
		}
		//pPlayer->Close(6, CLOSE_FLAG_SELECT_CHAR);
	}
	else {
		pPlayer->SetStat(STRENGTH, sCharacterBase->GetCharacterBase(playerClass)->GetStat(STRENGTH));
		pPlayer->SetStat(AGILITY, sCharacterBase->GetCharacterBase(playerClass)->GetStat(AGILITY));
		pPlayer->SetStat(VITALITY, sCharacterBase->GetCharacterBase(playerClass)->GetStat(VITALITY));
		pPlayer->SetStat(ENERGY, sCharacterBase->GetCharacterBase(playerClass)->GetStat(ENERGY));

		if (playerClass == Character::DARK_LORD) {
			pPlayer->SetStat(LEADERSHIP, sCharacterBase->GetCharacterBase(playerClass)->GetStat(LEADERSHIP));
		}

		pPlayer->GetLevelData(LEVEL_DATA_MASTER_BAK)->SetLevel(pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetLevel());
		pPlayer->GetLevelData(LEVEL_DATA_MASTER_BAK)->SetPoints(pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetPoints());
		pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->SetLevel(pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel());
		pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->SetPoints(pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints());
		pPlayer->GetLevelData(LEVEL_DATA_MASTER)->SetLevel(0);
		pPlayer->GetLevelData(LEVEL_DATA_MASTER)->SetPoints(0);
		pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->SetLevel(0);
		pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->SetPoints(0);

		pPlayer->MasterSendLevelUp();

		pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetLevel(1);
		pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetExperience(0);
		pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetNextExperience(100);

		//if (min_rs_time == 0) {
		pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetPoints(playerRSCount * points_per_rs);
		//}
		//else {
		//	pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetPoints(old_points_stats + (playerRSCount - min_rs_time + 1) * points_per_rs);
		//}

		pPlayer->CalculateCharacter();
		pPlayer->CalculateItemStat();
		pPlayer->Calculate380Item();
		pPlayer->CalculateAncientItem();
		pPlayer->CalculateAncientItemStat();
		pPlayer->CalculateExeItem();
		pPlayer->CalculateHarmonyItem();
		pPlayer->CalculateMajesticStatPoints();
		pPlayer->CalculateMaxHP();
		pPlayer->CalculateMaxMana();
		pPlayer->CalculateMaxShield();
		pPlayer->CalculateMaxStamina();
		pPlayer->NormalSendLevelUp(false);

		if (sGameServer->IsResetMoveChar()) {
			if (playerClass == Character::FAIRY_ELF || playerClass == Character::RUNE_WIZARD) {
				pPlayer->MoveToGate(27);
			}
			else if (playerClass == Character::SUMMONER || playerClass == Character::GUN_CRUSHER) {
				pPlayer->MoveToGate(267);
			}
			else {
				pPlayer->MoveToGate(17);
			}
		}
	}

	if (sGameServer->IsResetRelog()) {
		pPlayer->Close(6, CLOSE_FLAG_SELECT_CHAR);
	}
	
	uint16 char_id = pPlayer->GetGUID();
	std::string char_name = pPlayer->GetName();
	uint16 old_rs = pPlayer->GetResetSystem(RESET_TOTAL)->GetReset() - 1;
	uint16 new_rs = pPlayer->GetResetSystem(RESET_TOTAL)->GetReset();
	MuLogDatabase.PExecute("INSERT IGNORE INTO `log_reset`(char_id, char_name, old_rs, new_rs, time_rs) VALUES (%d, '%s', %d, %d, NOW())", char_id, char_name.c_str(), old_rs, new_rs);
	
	return true;
}

void CResetMgr::AutoReset(Player* pPlayer) {
	if (sGameServer->IsResetAutoCommand()) {
		if (pPlayer->GetResetSystem(RESET_AUTO)->GetReset() == 1) {
			if (!sResetSystemMgr->PlayerReset(pPlayer)) {
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Reset Failed!");
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Auto Reset Disable");
				pPlayer->GetResetSystem(RESET_AUTO)->SetReset(0);
			}
		}
	}
}