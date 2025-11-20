/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerDB.cpp"
*
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// LOAD FROM DB /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::LoadDBNew()
{
	this->LoadDBItemAllBox();
	//this->LoadDBItemNew();
	//this->LoadDBMuunNew();
	//this->LoadDBEventInventoryNew();
	//this->LoadDBPentagramInfoNew();
	//this->LoadDBItemRecoveryNew();
	//this->LoadDBItemSellNew();
	//this->LoadDBItemPersonalStoreNew();

	this->LoadDBSkillNew();
	this->LoadDBGameOptionNew();
	this->LoadDBQuestEvoNew();
	this->LoadDBFriendListNew();
	this->LoadDBMailListNew();
	this->LoadDBGuildNew();
	this->LoadDBGenNew(false);
	this->LoadDBGenKillNew();
	this->LoadDBHelperNew();
	this->LoadDBBuffChar();
	this->LoadDBBuffAcc();
	this->LoadDBRestrictionNew();
	this->LoadDBStatisticsNew();
	this->LoadDBItemDelayNew();
	this->GetWarehouse()->LoadDBDataNew();
	this->GetWarehouse()->LoadDBItemListNew();
	this->LoadDBCashShopItemListNew();
	this->LoadDBCashShopCoinNew();
	this->LoadDBChatBlockNew();
	
	this->LoadDBGremoryCaseChar();
	this->LoadDBGremoryCaseAcc();
	this->LoadDBGremoryCaseMobile();
	this->LoadDBGremoryCasePS();
	this->LoadDBHuntingRecordNew();
	this->LoadDBLabyrinthNew();
	this->LoadDBLabyrinthZoneNew();
	this->LoadDBLabyrinthLevelNew();
	this->LoadDBEventCountNew();
	this->LoadDBMiniBombNew();
	this->LoadDBMiniBombGridNew();
	this->LoadDBMuRoomyNew();
	this->LoadDBMuRoomyDeckNew();
	this->LoadDBWarpFavoriteListNew();
	this->LoadDBQuestGuidedNew();
	this->LoadDBJewelBingoNew();
	this->LoadDBJewelBingoGridNew();
	this->LoadDBDateNew();
	this->LoadDBMajesticTreeNew();
	this->LoadDBMajesticStatsNew();
	this->LoadDBNumericBaseballNew();
	//this->LoadDBMixRecoveryNew();
	this->LoadDBQuestMUNew();
	this->LoadMonsterSoulNew();

	for ( QuestEvolutionMap::const_iterator itr = sQuestMgr->m_quest_evolution.begin(); itr != sQuestMgr->m_quest_evolution.end(); ++itr )
	{
		QuestEvolution const* pQuest = itr->second;

		if ( pQuest )
		{
			QuestEvolutionStatus & quest =  this->quest_evolution[pQuest->GetID()];

			if ( quest.GetState() == QUEST_EVOLUTION_STATE_NONE )
			{
				quest.SetID(pQuest->GetID());
				quest.SetState(QUEST_EVOLUTION_STATE_CLEAR);
			}
		}
	}

	if ( this->IsAdministrator() )
	{
		if ( this->GetChangeUP(0) == 1 )
		{
			this->QuestEvolutionSetState(0, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(1, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(2, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(3, QUEST_EVOLUTION_STATE_COMPLETE);
		}

		if ( this->IsMaster() )
		{
			this->QuestEvolutionSetState(4, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(5, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(6, QUEST_EVOLUTION_STATE_COMPLETE);
		}

		if ( this->IsMajestic() )
		{
			this->QuestEvolutionSetState(7, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(8, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(9, QUEST_EVOLUTION_STATE_COMPLETE);
		}
	}
	else
	{
		if ( this->IsMaster() )
		{
			this->QuestEvolutionSetState(0, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(1, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(2, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(3, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(4, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(5, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(6, QUEST_EVOLUTION_STATE_COMPLETE);
		}

		if ( this->IsMajestic() )
		{
			this->QuestEvolutionSetState(0, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(1, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(2, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(3, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(4, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(5, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(6, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(7, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(8, QUEST_EVOLUTION_STATE_COMPLETE);
			this->QuestEvolutionSetState(9, QUEST_EVOLUTION_STATE_COMPLETE);
		}
	}

	if ( CharacterBaseData* pCharacterData = sCharacterBase->GetCharacterBase(this->GetClass()) )
	{
		for ( int64 i = 0; i < MAX_STAT_TYPE; ++i )
		{
			if ( this->GetStat(i) < pCharacterData->GetStat(i) )
			{
				this->SetStat(i, pCharacterData->GetStat(i));
			}
		}
	}

	this->CheckAddInfinityArrow();
	this->WearedItemApplySkill();
	this->MailDBUpdate();

	this->CalculateMajesticStatPoints();

	this->CheckSkill();
}

void Player::LoadDBInfoNew(uint32 charID)
{
	QueryResult result = MuDatabase.PQuery("SELECT c.guid, c.slot, c.authority, c.race, c.name, c.level, c.level_master, c.experience, c.experience_master, "
		"c.points, c.points_master, c.strength, c.agility, c.vitality, c.energy, c.leadership, c.world, c.world_x, c.world_y, c.direction, c.money, c.life, c.mana, c.shield, c.stamina, "
		"c.add_fruit_points, c.dec_fruit_points, c.expanded_inventory, c.mute_time, c.admin_flags, c.pk_level, c.pk_count, c.pk_points, c.first_time, "
		"c.santa_claus_gift, c.personal_store_name, "
		"c.goblin_points, c.kick_time, c.account_id, c.post_count, c.post_day, c.post_month, c.ruud_money, c.level_majestic, c.experience_majestic, c.points_majestic, UNIX_TIMESTAMP(c.create_date), c.hunting_log_visible, c.reset, c.monster_soul_purchase "
		"FROM character_info c WHERE c.account_id = %d AND c.guid = %d", this->GetAccountData()->GetGUID(), charID);

	if ( !result )
	{
		this->CloseSocket();
		return;
	}

	uint32 account_id = 0;

	FieldReader reader(result->Fetch());

	this->SetGUID(reader.GetUInt32());
	this->SetSlot(reader.GetUInt8());
	this->SetAuthority(reader.GetUInt8());
	this->SetDBClass(reader.GetUInt8());
	this->SetName(reader.GetCString());

	this->GetLevelData(LEVEL_DATA_NORMAL)->SetLevel(reader.GetInt16());
	this->GetLevelData(LEVEL_DATA_MASTER)->SetLevel(reader.GetInt16());
	this->GetLevelData(LEVEL_DATA_NORMAL)->SetExperience(reader.GetInt64());
	this->GetLevelData(LEVEL_DATA_MASTER)->SetExperience(reader.GetInt64());
	this->GetLevelData(LEVEL_DATA_NORMAL)->SetPoints(reader.GetInt32());
	this->GetLevelData(LEVEL_DATA_MASTER)->SetPoints(reader.GetInt32());

	if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < 400) {
		this->GetLevelData(LEVEL_DATA_MASTER_BAK)->SetLevel(this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel());
		this->GetLevelData(LEVEL_DATA_MASTER_BAK)->SetPoints(this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints());
		this->GetLevelData(LEVEL_DATA_MASTER)->SetLevel(0);
		this->GetLevelData(LEVEL_DATA_MASTER)->SetExperience(0);
		this->GetLevelData(LEVEL_DATA_MASTER)->SetPoints(0);
	}

	
	for (uint64 i = 0; i < MAX_STAT_TYPE; ++i)
	{
		this->SetStat(i, reader.GetInt32());
	}

	this->SetWorldId(reader.GetUInt16());
	this->SetX(reader.GetInt16());
	this->SetY(reader.GetInt16());
	this->SetDirection(reader.GetUInt8());
	this->MoneySet(reader.GetUInt32());

	POWER_LOOP(i)
	{
		this->PowerSet(i, reader.GetInt32());
	}

	this->SetFruitPointsAdd(reader.GetInt32());
	this->SetFruitPointsDec(reader.GetInt32());

	this->GetInventory()->SetExpanded(reader.GetUInt8());

	this->SetMuteTime(reader.GetInt64());

	this->SetAdministratorFlag(reader.GetUInt32());

	this->SetPKLevel(reader.GetUInt8());
	this->SetPKCount(reader.GetInt32());
	this->SetPKPoints(reader.GetInt32());
	this->SetFirstTime(reader.GetBool());
	this->SetSantaClausGiftDate(reader.GetInt64());
	this->GetPersonalStore()->SetName(reader.GetBase64String().c_str());
	this->GetPersonalStore()->SetOff(false);

	this->SetGoblinPoints(reader.GetUInt32());
	this->SetKickTime(reader.GetInt64());
	
	account_id = reader.GetUInt32();

	this->SetPostCount(reader.GetInt32());
	this->SetPostDay(reader.GetUInt8());
	this->SetPostMonth(reader.GetUInt8());

	this->SetRuudMoney(reader.GetUInt32());

	this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetLevel(reader.GetInt16());
	this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetExperience(reader.GetInt64());
	this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetPoints(reader.GetInt32());

	if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() <= 400 && this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() < 400) {
		this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->SetLevel(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel());
		this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->SetPoints(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints());
		this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetLevel(0);
		this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetExperience(0);
		this->GetLevelData(LEVEL_DATA_MAJESTIC)->SetPoints(0);
	}

	this->SetCreateDate(reader.GetInt64());
	this->SetHuntingRecordVisible(reader.GetUInt8() == 1 ? true : false);
	this->GetResetSystem(RESET_TOTAL)->SetReset(reader.GetInt32());
	this->SetMonsterSoulPurchase(reader.GetInt8());

	QueryResult result2 = MuDatabase.PQuery("SELECT * FROM character_gremory_case WHERE guid=%d", this->GetGUID());

	if (!result2) {
		QueryResult insert = MuDatabase.PQuery("INSERT INTO character_gremory_case(guid) VALUES(%d)", this->GetGUID());
	}

	if (account_id != this->GetAccountData()->GetGUID())
	{
		this->CloseSocket();
		return;
	}

	if ( this->GetKickTime() <= 0 )
	{
		this->SetKickTime(time(nullptr));
	}
}
	
void Player::LoadDBItemNew()
{
	QueryResult load = MuDatabase.PQuery("SELECT inventory FROM inventory_data where id=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (inventory_range(atoi(slot.c_str())) && sItemMgr->IsItem(atoi(itemEntry.c_str())) && !Item::IsEventArchangelWeapon(atoi(itemEntry.c_str())))
			{
				this->GetInventory()->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_INVENTORY);
			}
		}
	}

}

void Player::LoadDBSkillNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.type, c.skill, c.skill_level FROM character_skill c WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	do
	{
		Field* fields = result->Fetch();

		uint8 type = fields[0].GetUInt8();

		if (type == 0 || type == 1)
		{
			if (sSkillMgr->IsSkill(fields[1].GetUInt16()))
			{
				if (type == 0)
				{
					this->MagicAdd(fields[1].GetUInt16(), fields[2].GetUInt8());
				}
				else if (type == 1)
				{
					this->AddSkillTreeSkill(fields[1].GetUInt16(), fields[2].GetUInt8());
				}
			}
		}
		//		else if ( type >= 2 )
		//		{
		//			this->AddMajesticSkillTreeSkill(0, type - 2, fields[1].GetUInt16(), fields[2].GetUInt8());
		//		}
	} while (result->NextRow());
}

void Player::LoadDBGameOptionNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.game_option, c.chat_window, c.q_key, c.w_key, c.e_key, c.r_key, c.qwer_level, "
		"c.skill_bind_0, c.skill_bind_1, c.skill_bind_2, c.skill_bind_3, c.skill_bind_4, c.skill_bind_5, c.skill_bind_6, c.skill_bind_7, c.skill_bind_8, c.skill_bind_9, c.extra_data, c.change_skin, c.additional_options, "
		"c.button_bind_1, c.button_bind_2, c.button_bind_3, c.button_bind_4, c.button_bind_5 FROM character_gameoption c WHERE c.char_id = %d", this->GetGUID());
		
	if (!result)
		return;

	FieldReader reader(result->Fetch());

	this->GetGameOption()->SetOption(reader.GetUInt8());
	this->GetGameOption()->SetChatWindow(reader.GetUInt8());
	this->GetGameOption()->SetKeyQ(reader.GetUInt8());
	this->GetGameOption()->SetKeyW(reader.GetUInt8());
	this->GetGameOption()->SetKeyE(reader.GetUInt8());
	this->GetGameOption()->SetKeyR(reader.GetUInt8());
	this->GetGameOption()->SetQWERLevel(reader.GetInt32());

	for (uint8 i = 0; i < SKILL_KEY_COUNT; ++i)
	{
		this->GetGameOption()->SetSkillKey(i, reader.GetUInt16());
	}

	this->GetGameOption()->SetExtraFlag(reader.GetUInt64());
	this->GetGameOption()->SetChangeSkin(reader.GetUInt8());
	this->GetGameOption()->SetAdditionalOptions(reader.GetUInt16());

	for (int32 i = 0; i < BUTTON_KEY_COUNT; ++i)
	{
		this->GetGameOption()->SetButtonLink(i, reader.GetUInt8());
	}
}
	
void Player::LoadDBQuestEvoNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.id, c.state, c.kill_count_1, c.kill_count_2, c.kill_count_3, c.kill_count_4, c.kill_count_5 "
		"FROM character_quest_evo c WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	do
	{
		Field* fields = result->Fetch();

		QuestEvolutionStatus& add_quest = this->quest_evolution[fields[0].GetUInt8()];
		add_quest.SetID(fields[0].GetUInt8());
		add_quest.SetState(fields[1].GetUInt8());

		for (uint8 i = 0; i < QUEST_EVOLUTION_MONSTER_COUNT; ++i)
			add_quest.SetMonsterCount(i, fields[2 + i].GetInt32());

		if (add_quest.GetState() > QUEST_EVOLUTION_STATE_CLEAR)
			add_quest.SetState(QUEST_EVOLUTION_STATE_CLEAR);
	} while (result->NextRow());
}


void Player::LoadDBFriendListNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.friend_name, i.server_code, i.online "
		"FROM character_friend c LEFT JOIN character_info i ON c.friend_name = i.name WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	do
	{
		Field* fields = result->Fetch();

		this->AddFriend(fields[0].GetCString(), fields[2].GetBool() ? fields[1].GetUInt16() : -1);
	} while (result->NextRow());
}

void Player::LoadDBMailListNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.from_id, c.subject, c.message, c.direction, c.action, c.photo, c.window_guid, c.date, c.opened, c.id "
		"FROM character_mail c WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	MailData Mail;

	do
	{
		Field* fields = result->Fetch();

		Mail.Reset();

		Mail.SetFrom(fields[0].GetCString());
		Mail.SetSubject(fields[1].GetBase64String().c_str());
		Mail.SetText(fields[2].GetBase64String().c_str());
		Mail.SetDirection(fields[3].GetUInt8());
		Mail.SetAction(fields[4].GetUInt8());

		const char* data = fields[5].GetCString();

		if (data)
		{
			Tokens tokens(data, ' ');

			for (uint8 i = 0; i < MAX_PREVIEW_DATA; ++i)
			{
				if (tokens.size() > i)
				{
					Mail.SetPhoto(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}

		Mail.SetWindowGUID(fields[6].GetUInt32());
		Mail.SetDate(fields[7].GetInt64());
		Mail.SetStatus(fields[8].GetUInt8());
		Mail.SetID(fields[9].GetUInt32());
		Mail.SetInUse(true);

		this->MailAdd(Mail);
	} while (result->NextRow());

	this->MailDBUpdate();
}

void Player::LoadDBGuildNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT g.guild_id FROM guild_members g WHERE g.char_id = %d", this->GetGUID());

	if (!result)
	{
		return;
	}

	Field* fields = result->Fetch();

	this->SetGuildID(fields[0].GetUInt32());

	Guild* pGuild = sGuildMgr->GetGuild(this->GetGuildID());

	if (!pGuild)
	{
		this->SetGuildID(0);
	}
	else
	{
		if (!pGuild->GetMemberData(this->GetGUID()))
		{
			this->SetGuildID(0);
		}
	}
}

void Player::LoadDBGenNew(bool update)
{
	if (update)
	{
		if (!Player::IsPlayerBasicState(this, false, false))
		{
			return;
		}
	}

	QueryResult result = MuDatabase.PQuery("SELECT c.family, c.level, c.contribution, c.reward_date, c.join_date, c.left_date, h.ranking "
		"FROM character_gens AS c LEFT JOIN gen_ranking AS h ON c.char_id = h.char_id WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	Field* fields = result->Fetch();

	this->GetGen()->SetFamily(fields[0].GetUInt8());
	this->GetGen()->SetLevel(fields[1].GetUInt8());
	this->GetGen()->SetContribution(fields[2].GetInt32());
	this->GetGen()->SetRewardDate(fields[3].GetInt64());
	this->GetGen()->SetJoinDate(fields[4].GetInt64());
	this->GetGen()->SetLeftDate(fields[5].GetInt64());
	this->GetGen()->SetRanking(fields[6].GetInt32());

	if (update)
	{
		this->GenUpdateLevel();
	}
}

void Player::LoadDBGenKillNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.killed_id, c.count, c.date FROM character_gens_kill c WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	do
	{
		Field* fields = result->Fetch();

		GenVictimData& gen_victim = this->m_gen_victim_map[fields[0].GetUInt32()];
		gen_victim.count.set(fields[1].GetUInt32());
		gen_victim.last_hit.set(fields[2].GetInt64());
	} while (result->NextRow());
}

void Player::LoadDBHelperNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.option_flag_1, c.option_flag_2, c.item_pick_flag, c.hunting_range, c.item_pick_range, c.distance, c.attack_skill_1, c.attack_sec_skill_1, c.attack_sec_skill_2, "
		"c.attack_sec_delay_1, c.attack_sec_delay_2, c.buff_skill_1, c.buff_skill_2, c.buff_skill_3, c.time_space_casting, c.percent_autopot, c.percent_autoheal, c.percent_partyheal, c.percent_drainlife, "
		"c.item_list, c.buff_item_1, c.buff_item_2, c.buff_item_3 FROM character_helper c WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	Field* fields = result->Fetch();

	this->GetHelper()->SetOptionFlags(0, fields[0].GetUInt32());
	this->GetHelper()->SetOptionFlags(1, fields[1].GetUInt32());
	this->GetHelper()->SetItemPickFlags(fields[2].GetUInt8());
	this->GetHelper()->SetHuntingRange(fields[3].GetUInt8());
	this->GetHelper()->SetPickRange(fields[4].GetUInt8());
	this->GetHelper()->SetMovementTime(fields[5].GetUInt16());
	this->GetHelper()->SetAttackSkill(0, fields[6].GetUInt16());
	this->GetHelper()->SetAttackSkill(1, fields[7].GetUInt16());
	this->GetHelper()->SetAttackSkill(2, fields[8].GetUInt16());
	this->GetHelper()->SetDelay(0, fields[9].GetUInt16());
	this->GetHelper()->SetDelay(1, fields[10].GetUInt16());
	this->GetHelper()->SetBuffSkill(0, fields[11].GetUInt16());
	this->GetHelper()->SetBuffSkill(1, fields[12].GetUInt16());
	this->GetHelper()->SetBuffSkill(2, fields[13].GetUInt16());
	this->GetHelper()->SetTimeSpaceCasting(fields[14].GetUInt16());
	this->GetHelper()->SetAutoPotPercent(fields[15].GetUInt8());
	this->GetHelper()->SetAutoHealPercent(fields[16].GetUInt8());
	this->GetHelper()->SetPartyHealPercent(fields[17].GetUInt8());
	this->GetHelper()->SetDrainLifePercent(fields[18].GetUInt8());

	Tokenizer tokens(fields[19].GetBase64String(), ';');

	for (int32 i = 0; i < tokens.size(); ++i)
	{
		if (i < PLAYER_HELPER_MAX_ITEMS)
		{
			strncpy(this->GetHelper()->item_name[i], tokens[i], strlen(tokens[i]) > HELPER_ITEM_LENGTH ? HELPER_ITEM_LENGTH : strlen(tokens[i]));
		}
	}

	this->GetHelper()->SetItemBuff(0, fields[20].GetUInt16());
	this->GetHelper()->SetItemBuff(1, fields[21].GetUInt16());
	this->GetHelper()->SetItemBuff(2, fields[22].GetUInt16());

	this->GetHelper()->SetLoaded(true);

	this->HelperSettingsUpdate();
}

void Player::LoadDBBuffChar()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.buff, c.effect_1, c.value_1, c.effect_2, c.value_2, c.effect_3, c.value_3, c.duration, c.flags "
		"FROM character_buff c WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	do
	{
		Field* fields = result->Fetch();
		uint16 buff_id = fields[0].GetUInt16();
		uint8 effect_1 = (fields[1].GetUInt8());
		float value_1 = static_cast<float>(fields[2].GetInt32());
		uint8 effect_2 = (fields[3].GetUInt8());
		float value_2 = static_cast<float>(fields[4].GetInt32());
		uint8 effect_3 = (fields[5].GetUInt8());
		float value_3 = static_cast<float>(fields[6].GetInt32());
		time_t duration = fields[7].GetInt64();
		uint8 flags = fields[8].GetUInt8();

		if (sGameServer->IsItemBuffCheck())
		{
			auto pData = sSkillMgr->GetBuff(buff_id);

			if (pData && sItemMgr->IsItem(pData->GetItem()))
			{
				effect_1 = pData->GetEffect(0);
				effect_2 = pData->GetEffect(1);
				effect_3 = 0;

				value_1 = pData->GetValue(0);
				value_2 = pData->GetValue(1);
				value_3 = 0;
			}
		}

		this->AddBuff(buff_id, BuffEffect(effect_1, value_1), BuffEffect(effect_2, value_2), BuffEffect(effect_3, value_3), duration, flags);
	} while (result->NextRow());
}

void Player::LoadDBBuffAcc()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.buff, a.effect_1, a.value_1, a.effect_2, a.value_2, a.effect_3, a.value_3, a.duration, a.flags FROM account_buff a WHERE a.account_id = %d", this->GetAccountData()->GetGUID());

	if (!result)
		return;

	do
	{
		Field* fields = result->Fetch();
		uint16 buff_id = fields[0].GetUInt16();
		uint8 effect_1 = (fields[1].GetUInt8());
		float value_1 = static_cast<float>(fields[2].GetInt32());
		uint8 effect_2 = (fields[3].GetUInt8());
		float value_2 = static_cast<float>(fields[4].GetInt32());
		uint8 effect_3 = (fields[5].GetUInt8());
		float value_3 = static_cast<float>(fields[6].GetInt32());
		time_t duration = fields[7].GetInt64();
		uint8 flags = fields[8].GetUInt8();

		if (sGameServer->IsItemBuffCheck())
		{
			auto pData = sSkillMgr->GetBuff(buff_id);

			if (pData && sItemMgr->IsItem(pData->GetItem()))
			{
				effect_1 = pData->GetEffect(0);
				effect_2 = pData->GetEffect(1);
				effect_3 = 0;

				value_1 = pData->GetValue(0);
				value_2 = pData->GetValue(1);
				value_3 = 0;
			}
		}

		this->AddBuff(buff_id, BuffEffect(effect_1, value_1), BuffEffect(effect_2, value_2), BuffEffect(effect_3, value_3), duration, flags);
	} while (result->NextRow());
}

void Player::LoadDBRestrictionNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.restriction, c.time, c.admin_name, c.reason FROM character_restriction c WHERE c.char_id = %d", this->GetGUID());

	if (!result)
		return;

	do
	{
		Field* fields = result->Fetch();

		uint32 i = fields[0].GetUInt32();

		if (i >= PlayerAction::PLAYER_ACTION_MAX)
			continue;

		this->GetRestriction(i)->SetTime(fields[1].GetInt64());
		this->GetRestriction(i)->SetAdmin(fields[2].GetString());
		this->GetRestriction(i)->SetReason(fields[3].GetString());
	} while (result->NextRow());
}

void Player::LoadDBStatisticsNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.statistic_id, c.count FROM character_statistics c WHERE c.char_id = %d", this->GetGUID());

	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();

		if ( fields[0].GetUInt32() < STATISTIC_MAX )
		{
			this->GetStatisticData(fields[0].GetUInt32())->SetCount(fields[1].GetUInt64());
		}
	}
	while(result->NextRow());
}

void Player::LoadDBItemDelayNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.item, c.date FROM character_item_delay c WHERE c.char_id = %d", this->GetGUID());

	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();
			
		ItemDelayData & item_delay = this->m_item_delay[fields[0].GetUInt16()];
		item_delay.SetDate(fields[1].GetInt64());
	}
	while(result->NextRow());
}

void Player::LoadDBMuunNew()
{
	QueryResult load = MuDatabase.PQuery("SELECT muun_inventory FROM inventory_data where id=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (MUUN_INVENTORY_RANGE(atoi(slot.c_str())) && sItemMgr->GetItemInventoryType(atoi(itemEntry.c_str())) == ITEM_INVENTORY_TYPE_MUUN && sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->GetMuunInventory()->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_MUUN);
			}
		}
	}
}

void Player::LoadDBEventInventoryNew()
{
	QueryResult load = MuDatabase.PQuery("SELECT event_inventory FROM inventory_data where id=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (EVENT_INVENTORY_RANGE(atoi(slot.c_str())) && sItemMgr->GetItemInventoryType(atoi(itemEntry.c_str())) == ITEM_INVENTORY_TYPE_EVENT) {
				this->GetEventInventory()->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_EVENT_INVENTORY);
			}
		}
	}
}

void Player::LoadDBPentagramInfoNew()
{
	QueryResult load = MuDatabase.PQuery("SELECT pentagram_inventory FROM inventory_data where id=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');

			std::string type = tokens2[0];
			std::string index = tokens2[1];

			if (atoi(index.c_str()) >= MAX_PENTAGRAM_JEWEL_INFO)
			{
				continue;
			}

			if (atoi(type.c_str()) != PENTAGRAM_JEWEL_TYPE_INVENTORY && atoi(type.c_str()) != PENTAGRAM_JEWEL_TYPE_WAREHOUSE)
			{
				continue;
			}

			PentagramJewelInfo* pPentagramJewelInfoStr = (atoi(type.c_str()) == PENTAGRAM_JEWEL_TYPE_INVENTORY) ? this->GetPentagramJewelInfoInventoryPtr() : this->GetPentagramJewelInfoWarehousePtr();

			if (pPentagramJewelInfoStr[atoi(index.c_str())].GetIndex() != uint8(-1))
			{
				continue;
			}

			std::string atribute = tokens2[2];
			std::string section = tokens2[3];
			std::string itemType = tokens2[4];
			std::string itemLevel = tokens2[5];
			std::string rankindex1 = tokens2[6];
			std::string rankdata1 = tokens2[7];
			std::string rankindex2 = tokens2[8];
			std::string rankdata2 = tokens2[9];
			std::string rankindex3 = tokens2[10];
			std::string rankdata3 = tokens2[11];
			std::string rankindex4 = tokens2[12];
			std::string rankdata4 = tokens2[13];
			std::string rankindex5 = tokens2[14];
			std::string rankdata5 = tokens2[15];
			std::string serialServer = tokens2[16];
			std::string serial = tokens2[17];
			std::string dbflag = tokens2[18];
			std::string rawdata1 = tokens2[19];
			std::string rawdata2 = tokens2[20];
			std::string rawdata3 = tokens2[21];

			pPentagramJewelInfoStr[atoi(index.c_str())].SetIndex(atoi(index.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetType(atoi(type.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetAttribute(atoi(atribute.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetItemSection(atoi(section.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetItemType(atoi(itemType.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetItemLevel(atoi(itemLevel.c_str()));

			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(0)->SetIndex(atoi(rankindex1.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(0)->SetLevel(atoi(rankdata1.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(1)->SetIndex(atoi(rankindex2.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(1)->SetLevel(atoi(rankdata2.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(2)->SetIndex(atoi(rankindex3.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(2)->SetLevel(atoi(rankdata3.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(3)->SetIndex(atoi(rankindex4.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(3)->SetLevel(atoi(rankdata4.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(4)->SetIndex(atoi(rankindex5.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(4)->SetLevel(atoi(rankdata5.c_str()));


			pPentagramJewelInfoStr[atoi(index.c_str())].SetSerialServer(atoi(serialServer.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetSerial(atoi(serial.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetDBFlag(atoi(dbflag.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetData(0, atoi(rawdata1.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetData(1, atoi(rawdata2.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetData(2, atoi(rawdata3.c_str()));
		}
	}
}

void Player::LoadDBCashShopItemListNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.product, a.option, a.serial, a.serial_cash_shop, a.server, a.gift, a.date "
		"FROM account_cash_shop_item a WHERE a.account_id = %d", this->GetAccountData()->GetGUID());

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			CashShopItemData* pItem = new CashShopItemData;
			pItem->SetProduct(fields[0].GetUInt32());
			pItem->SetOption(fields[1].GetUInt32());
			pItem->SetSerial(fields[2].GetUInt32());
			pItem->SetSerialCashShop(fields[3].GetUInt32());
			pItem->SetServer(fields[4].GetUInt32());
			pItem->SetDate(fields[6].GetInt64());

			uint8 gift = fields[5].GetUInt8();

			this->CashShopInsertItem(gift, pItem, false);
		} while (result->NextRow());
	}
}

void Player::LoadDBCashShopCoinNew()
{
	this->SetAccountDataLoaded(true);

	this->GetAccountData()->SetVIPStatus(-1);
	this->GetAccountData()->SetVIPDuration(0);
	this->GetWarehouse()->SetExpanded(0);
	this->GetWarehouse()->SetExpandedTime(0);
	this->GetAccountData()->SetSpecialCharacter(SPECIAL_CHARACTER_SUMMONER);
	this->SetCredits(0);

	QueryResult result = MuDatabase.PQuery("SELECT a.vip_status, a.vip_duration, a.expanded_warehouse, a.expanded_warehouse_time, a.special_character, a.credits, a.web_credits, a.cash_shop_discount_wc, a.cash_shop_discount_gp, UNIX_TIMESTAMP(a.cash_shop_discount_date) FROM account_data a WHERE a.account_id = %d", this->GetAccountData()->GetGUID());

	if (!result)
	{
		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_DATA_CREATE);
		stmt->setUInt32(0, this->GetAccountData()->GetGUID());
		stmt->setString(1, this->GetAccountData()->GetIP());
		stmt->setString(2, this->GetAccountData()->GetMac());
		stmt->setUInt32(3, this->GetAccountData()->GetDiskSerial());
		stmt->setUInt8(4, sGameServer->GetServerCode());
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
		return;
	}

	Field* fields = result->Fetch();

	this->GetAccountData()->SetVIPStatus(fields[0].GetInt32());
	this->GetAccountData()->SetVIPDuration(fields[1].GetInt64());
	this->GetWarehouse()->SetExpanded(fields[2].GetUInt8());
	this->GetWarehouse()->SetExpandedTime(fields[3].GetInt64());
	this->GetAccountData()->SetSpecialCharacter(fields[4].GetUInt16());
	this->SetCredits(fields[5].GetUInt32());
	uint32 web_credits = fields[6].GetUInt32();

	this->SetCashShopDiscountWC(fields[7].GetUInt8());
	this->SetCashShopDiscountGP(fields[8].GetUInt8());
	this->SetCashShopDiscountDate(fields[9].GetInt64());

	if (this->GetCashShopDiscountWC() > 100)
	{
		this->SetCashShopDiscountWC(100);
	}

	if (this->GetCashShopDiscountGP() > 100)
	{
		this->SetCashShopDiscountGP(100);
	}

	this->IncreaseCredits(web_credits);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_COIN_CLEAR);
	stmt->setUInt32(0, web_credits);
	stmt->setUInt32(1, this->GetAccountData()->GetGUID());
	trans->Append(stmt);

	this->SaveDBAccount(trans);

	MuDatabase.CommitTransaction(trans);

	sLog->outInfo(LOG_CASHSHOP, "LoadDBCashShopCoin() - %s Success: [%u, %u]",
		this->BuildLog().c_str(), this->GetCredits(), this->GetGoblinPoints());
}

void Player::LoadDBChatBlockNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT c.blocked FROM character_chat_block c WHERE c.char_id = %d", this->GetGUID());

	if ( !result )
		return;

	do
	{
		Field* fields = result->Fetch();
			
		this->ChatBlockAdd(fields[0].GetString().c_str());
	}
	while(result->NextRow());
}

void Player::LoadDBGremoryCaseChar()
{
	QueryResult load = MuDatabase.PQuery("SELECT gremory_case_char FROM character_gremory_case where guid=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_GREMORY_CASE_INV);
			}
		}
	}

}

void Player::LoadDBGremoryCaseAcc()
{
	QueryResult load = MuDatabase.PQuery("SELECT gremory_case_acc FROM account_warehouse where account_id=%d", this->GetAccountData()->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_GREMORY_CASE_ACC);
			}
		}
	}
}


void Player::LoadDBGremoryCaseMobile()
{
	QueryResult load = MuDatabase.PQuery("SELECT gremory_case_mobile FROM character_gremory_case where guid=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_GREMORY_CASE_MOBILE);
			}
		}
	}
}


void Player::LoadDBGremoryCasePS()
{
	QueryResult load = MuDatabase.PQuery("SELECT gremory_case_ps FROM character_gremory_case where guid=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_GREMORY_CASE_PS);
			}
		}
	}
}

void Player::LoadDBLabyrinthNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.world, a.year, a.month, a.day, a.level, a.duration, a.damage, a.elemental_damage, a.healing, a.killed_count, a.earned_experience FROM character_hunting_record a WHERE a.char_id = %d ORDER BY a.year ASC, a.month ASC, a.day ASC", this->GetGUID());

	if ( !result )
	{
		return;
	}

	Field* fields = result->Fetch();
	
	this->GetLabyrinthData()->SetStage(fields[0].GetUInt8());
	this->GetLabyrinthData()->SetID(fields[1].GetUInt8());
	this->GetLabyrinthData()->SetLevel(fields[2].GetUInt16());
	this->GetLabyrinthData()->SetStatus(fields[3].GetUInt8());
	this->GetLabyrinthData()->SetKilledMonsters(fields[4].GetInt32());
	this->GetLabyrinthData()->SetEarnedExperience(fields[5].GetInt64());
	this->GetLabyrinthData()->SetCompletedMissions(fields[6].GetInt32());
	this->GetLabyrinthData()->SetDate(fields[7].GetInt64());
	this->GetLabyrinthData()->SetGoblinState(fields[8].GetUInt8());
	this->GetLabyrinthData()->SetDayFirst(fields[9].GetUInt8());
}
	
void Player::LoadDBLabyrinthZoneNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.idx, a.subidx, a.mission_id_1, a.mission_id_2, a.mission_id_3, a.mission_id_4, a.mission_id_5, "
		"a.mission_count_1, a.mission_count_2, a.mission_count_3, a.mission_count_4, a.mission_count_5 "
		"FROM character_labyrinth_zone AS a WHERE a.char_id = %d", this->GetGUID());

	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		int32 loop = 0;

		PlayerLabyrinthGate * pGate = new PlayerLabyrinthGate;
		pGate->SetIndex(fields[loop++].GetUInt8());
		pGate->SetSubIndex(fields[loop++].GetUInt8());

		for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
		{
			pGate->SetMissionID(i, fields[loop++].GetUInt8());
		}

		for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
		{
			pGate->SetMissionCount(i, fields[loop++].GetInt32());
		}

		this->GetLabyrinthData()->m_GateList.push_back(pGate);
	} while (result->NextRow());
}

void Player::LoadDBLabyrinthLevelNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.level, a.amount FROM character_labyrinth_level AS a WHERE a.char_id = %d", this->GetGUID());

	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		this->GetLabyrinthData()->m_LevelMap[fields[0].GetUInt16()] = fields[1].GetInt32();
	} while (result->NextRow());
}

void Player::LoadDBEventCountNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.event_id, a.count, a.day FROM character_event_count a WHERE a.char_id = %d", this->GetGUID());

	if ( !result )
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		PlayerEventEnterCount * pData = new PlayerEventEnterCount;
		pData->SetEventID(fields[0].GetUInt8());
		pData->SetCount(fields[1].GetUInt8());
		pData->SetDay(fields[2].GetUInt8());

		this->event_enter_count_map[pData->GetEventID()] = pData;
	}
	while(result->NextRow());
}

void Player::LoadDBItemRecoveryNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.item, a.level, a.durability, a.skill, a.luck, a.option, a.excellent, a.ancient, a.option_380, a.harmony, "
		"a.socket_1, a.socket_2, a.socket_3, a.socket_4, a.socket_5, a.socket_bonus, a.data_1, a.data_2, a.data_3, a.serial_server, a.serial, a.serial_cash_shop, a.flags, a.id FROM item_recovery a WHERE a.char_id = %d", this->GetGUID());

	if ( !result )
	{
		return;
	}

	bool full = false;

	SQLTransaction trans = MuDatabase.BeginTransaction();
	
	do
	{
		Field* fields = result->Fetch();

		int32 loop = 0;

		Item item = Item();
		item.SetItem(fields[loop++].GetUInt16());
		item.SetLevel(fields[loop++].GetUInt8());
		item.SetDurability(fields[loop++].GetUInt8());
		item.SetSkill(fields[loop++].GetUInt8());
		item.SetLuck(fields[loop++].GetUInt8());
		item.SetOption(fields[loop++].GetUInt8());
		item.SetExe(fields[loop++].GetUInt8());
		item.SetAncient(fields[loop++].GetUInt8());
		item.Set380(fields[loop++].GetUInt8());
		item.SetHarmony(fields[loop++].GetUInt8());

		SOCKET_SLOT_LOOP(i)
		{
			item.SetSocket(i, fields[loop++].GetUInt16());
		}

		item.SetSocketBonus(fields[loop++].GetUInt8());
		item.raw.data_extra[0] = fields[loop++].GetInt32();
		item.raw.data_extra[1] = fields[loop++].GetInt32();
		item.raw.data_extra[2] = fields[loop++].GetInt32();
		item.SetExpireDate(time(nullptr) + (WEEK * 2));

		item.SetSerialServer(fields[loop++].GetUInt16());
		item.SetSerial(fields[loop++].GetUInt32());
		item.SetSerialShop(fields[loop++].GetUInt32());
		item.SetDBFlag(fields[loop++].GetUInt32());
		
		uint32 id = fields[loop++].GetUInt32();

		if ( this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetEmptySlot() != uint8(-1) )
		{
			sItemMgr->ItemSerialCreateGremoryCase(this, item, GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT, 1);

			/*PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_RECOVERY_DELETE);
			stmt->setUInt32(0, this->GetGUID());
			stmt->setUInt32(1, id);
			trans->Append(stmt);*/

			QueryResult delitem = MuDatabase.PQuery("DELETE FROM item_recovery WHERE char_id = %d AND item = %d", this->GetGUID(), item.GetItem());
		}
		else
		{
			full = true;
			break;
		}
	}
	while(result->NextRow());

	MuDatabase.CommitTransaction(trans);

	if ( full )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "Gremory Case is full. You have some items in queue.");
	}
}

void Player::LoadDBItemSellNew()
{
	QueryResult load = MuDatabase.PQuery("SELECT sell_npc_inventory FROM inventory_data where id=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (atoi(slot.c_str()) < MAX_ITEM_SOLD && sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->GetNpcSell()->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_SELL_NPC);
			}
		}
	}
}

void Player::LoadDBHuntingRecordNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.world, a.year, a.month, a.day, a.level, a.duration, a.damage, a.elemental_damage, a.healing, a.killed_count, a.earned_experience FROM character_hunting_record a WHERE a.char_id = %d ORDER BY a.year ASC, a.month ASC, a.day ASC", this->GetGUID());

	if (!result)
	{
		return;
	}

	Custom::SystemTimer cur_time = Custom::SystemTimer();

	do
	{
		Field* fields = result->Fetch();
		int32 loop = 0;
		uint16 world = fields[loop++].GetUInt16();

		HuntingRecordData* pData = new HuntingRecordData;
		pData->SetYear(fields[loop++].GetUInt16());
		pData->SetMonth(fields[loop++].GetUInt8());
		pData->SetDay(fields[loop++].GetUInt8());
		pData->SetLevel(fields[loop++].GetUInt32());
		pData->SetDuration(fields[loop++].GetUInt32());
		pData->SetDamage(fields[loop++].GetUInt64());
		pData->SetElementalDamage(fields[loop++].GetUInt64());
		pData->SetHealing(fields[loop++].GetUInt32());
		pData->SetKilledCount(fields[loop++].GetUInt32());
		pData->SetEarnedExperience(fields[loop++].GetUInt64());

		if (sGameServer->IsHuntingRecordClear() && cur_time.GetMonth() != pData->GetMonth())
		{
			delete pData;
			continue;
		}

		this->hunting_record_data[world].push_back(pData);
	} while (result->NextRow());
}

void Player::LoadDBMiniBombNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.state, a.score, a.total_score, a.cdate FROM character_mini_bomb a WHERE a.char_id = %d", this->GetGUID());

	if ( result )
	{
		Field* fields = result->Fetch();

		this->SetMiniBombState(fields[0].GetUInt8());
		this->SetMiniBombCurrentScore(fields[1].GetUInt16());
		this->SetMiniBombTotalScore(fields[2].GetInt32());
		this->SetMiniBombDate(fields[3].GetInt64());
		
		if ( time(nullptr) > (this->GetMiniBombDate() + WEEK) )
		{
			this->SetMiniBombState(MINI_BOMB_GAME_START);
			this->SetMiniBombCurrentScore(0);
			this->SetMiniBombTotalScore(0);
			this->SetMiniBombDate(time(nullptr));
		}
	}
}

void Player::LoadDBMiniBombGridNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.cell, a.value, a.status FROM character_mini_bomb_grid a WHERE a.char_id = %d", this->GetGUID());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 cell = fields[0].GetUInt8();

			if ( cell < MINI_BOMB_SIZE )
			{
				this->GetMiniBombData(cell)->SetValue(fields[1].GetUInt8());
				this->GetMiniBombData(cell)->SetStatus(fields[2].GetUInt8());
			}
		}
		while(result->NextRow());
	}
}

void Player::LoadDBMuRoomyNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.playing, a.type, a.score, a.card_count, a.special_card_count, a.date FROM character_mu_roomy a WHERE a.char_id = %d", this->GetGUID());

	if ( result )
	{
		Field* fields = result->Fetch();
	
		this->GetMuRoomyInfo()->SetPlayCard(fields[0].GetBool());
		this->GetMuRoomyInfo()->SetType(fields[1].GetUInt8());
		this->GetMuRoomyInfo()->SetScore(fields[2].GetUInt16());
		this->GetMuRoomyInfo()->SetCardDeckCount(fields[3].GetUInt8());
		this->GetMuRoomyInfo()->SetSpecialCardDeckCount(fields[4].GetUInt8());
		this->GetMuRoomyInfo()->SetDate(fields[5].GetInt64());
	}
}

void Player::LoadDBMuRoomyDeckNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.type, a.slot, a.color, a.number, a.state, a.play_slot FROM character_mu_roomy_deck a WHERE a.char_id = %d", this->GetGUID());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 card_type = fields[0].GetUInt8();
			uint8 card_slot = fields[1].GetUInt8();
			uint8 play_slot = fields[5].GetUInt8();

			switch ( card_type )
			{
			case MU_ROOMY_CARD_TYPE_NORMAL:
				{
					if ( MU_ROOMY_CARD_SLOT_RANGE(card_slot) )
					{
						this->GetMuRoomyInfo()->GetCardInfo(card_slot)->SetColor(fields[2].GetUInt8());
						this->GetMuRoomyInfo()->GetCardInfo(card_slot)->SetNumber(fields[3].GetUInt8());
						this->GetMuRoomyInfo()->GetCardInfo(card_slot)->SetState(fields[4].GetUInt8());
						this->GetMuRoomyInfo()->GetCardInfo(card_slot)->SetSlot(play_slot);

						if ( play_slot < MU_ROOMY_MAX_SLOT )
						{
							this->GetMuRoomyInfo()->GetSlotInfo(play_slot)->SetCardSlot(card_slot);
							this->GetMuRoomyInfo()->GetSlotInfo(play_slot)->SetCardType(card_type);
						}
					}
				} break;

			case MU_ROOMY_CARD_TYPE_SPECIAL:
				{
					if ( MU_ROOMY_SPECIALCARD_SLOT_RANGE(card_slot) )
					{
						this->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetColor(fields[2].GetUInt8());
						this->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetNumber(fields[3].GetUInt8());
						this->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetState(fields[4].GetUInt8());
						this->GetMuRoomyInfo()->GetSpecialCardInfo(card_slot)->SetSlot(play_slot);

						if ( play_slot < MU_ROOMY_MAX_SLOT )
						{
							this->GetMuRoomyInfo()->GetSlotInfo(play_slot)->SetCardSlot(card_slot);
							this->GetMuRoomyInfo()->GetSlotInfo(play_slot)->SetCardType(card_type);
						}
					}
				} break;
			}
		}
		while(result->NextRow());
	}

	if ( time(nullptr) > (this->GetMuRoomyInfo()->GetDate() + WEEK) )
	{
		this->GetMuRoomyInfo()->Init();
	}
}

void Player::LoadDBWarpFavoriteListNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.slot, a.data, a.warp_id FROM character_warp_favorite_list a WHERE a.char_id = %d", this->GetGUID());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 slot = fields[0].GetUInt8();

			if ( slot < WARP_FAVORITE_COUNT )
			{
				this->GetWarpFavoriteList(slot)->SetData(fields[1].GetUInt16());
				this->GetWarpFavoriteList(slot)->SetWarpID(fields[2].GetUInt16());
			}
		}
		while(result->NextRow());
	}
}

void Player::LoadDBQuestGuidedNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.quest, a.count, a.state, a.date FROM character_quest_guided a WHERE a.char_id = %d", this->GetGUID());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			PlayerQuestGuided * pData = new PlayerQuestGuided;
			pData->SetID(fields[0].GetUInt16());
			pData->SetCount(fields[1].GetInt32());
			pData->SetState(fields[2].GetUInt8());
			pData->SetDate(fields[3].GetInt64());

			this->quest_guided_map[pData->GetID()] = pData;

			if ( this->GetQuestGuidedID() <= pData->GetID() )
			{
				this->SetQuestGuidedID(pData->GetID());
			}
		}
		while(result->NextRow());
	}
}

void Player::LoadDBJewelBingoNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.state, a.box, a.count, a.jewel, a.score1, a.score2, a.score3, a.date FROM character_jewel_bingo a WHERE a.char_id = %d", this->GetGUID());

	if ( result )
	{
		Field* fields = result->Fetch();
	
		this->SetJewelBingoState(fields[0].GetUInt8());
		this->SetJewelBingoBox(fields[1].GetUInt8());
		this->SetJewelBingoCount(fields[2].GetUInt8());
		this->SetJewelBingoCurrentJewel(fields[3].GetUInt8());
		this->SetJewelBingoScore1(fields[4].GetInt32());
		this->SetJewelBingoScore2(fields[5].GetInt32());
		this->SetJewelBingoScore3(fields[6].GetInt32());
		this->SetJewelBingoDate(fields[7].GetInt64());
	}
}
	
void Player::LoadDBJewelBingoGridNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.type, a.slot, a.value FROM character_jewel_bingo_grid a WHERE a.char_id = %d", this->GetGUID());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 type = fields[0].GetUInt8();
			uint8 slot = fields[1].GetUInt8();
			uint8 value = fields[2].GetUInt8();

			if ( type >= 3 )
			{
				continue;
			}
			
			switch ( type )
			{
			case 0:
				{
					if ( slot < JEWEL_BINGO_GRID_SIZE )
					{
						this->GetJewelBingoGrid(slot)->SetType(type);
						this->GetJewelBingoGrid(slot)->SetSlot(slot);
						this->GetJewelBingoGrid(slot)->SetValue(value);
					}
				} break;

			case 1:
				{
					if ( slot < JEWEL_BINGO_JEWEL_COUNT )
					{
						this->GetJewelBingoJewel(slot)->SetType(type);
						this->GetJewelBingoJewel(slot)->SetSlot(slot);
						this->GetJewelBingoJewel(slot)->SetValue(value);
					}
				} break;

			case 2:
				{
					if ( slot < JEWEL_BINGO_MATCHING_SIZE )
					{
						this->GetJewelBingoMatching(slot)->SetType(type);
						this->GetJewelBingoMatching(slot)->SetSlot(slot);
						this->GetJewelBingoMatching(slot)->SetValue(value);
					}
				} break;
			}
		}
		while(result->NextRow());
	}

	if ( time(nullptr) > (this->GetJewelBingoDate() + WEEK) )
	{
		this->SetJewelBingoState(0);
		this->SetJewelBingoDate(0);
	}
}

void Player::LoadDBDateNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT i.id, i.date FROM character_date AS i WHERE i.char_id = %d", this->GetGUID());

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint32 id = fields[0].GetUInt32();
			time_t date = fields[1].GetInt64();
			
			if ( id >= PLAYER_DATE_MAX )
			{
				continue;
			}

			this->SetDate(id, date);
		}
		while(result->NextRow());
	}
}

void Player::LoadDBMajesticTreeNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT i.type, i.section, i.id, i.level FROM character_majestic_tree AS i WHERE i.char_id = %d", this->GetGUID());

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			this->AddMajesticSkillTreeSkill(fields[0].GetUInt8(), fields[1].GetUInt8(), fields[2].GetUInt16(), fields[3].GetUInt8());
		} while (result->NextRow());
	}
}

void Player::LoadDBMajesticStatsNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT i.id, i.level FROM character_majestic_stats AS i WHERE i.char_id = %d", this->GetGUID());

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			uint8 id = fields[0].GetUInt8();

			if (id < MAX_SKILL_TREE_MAJESTIC_PASSIVE)
			{
				this->SetMajesticStat(id, fields[1].GetUInt16());
			}
		} while (result->NextRow());
	}
}

void Player::LoadDBNumericBaseballNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT a.state, a.score, a.number_1, a.number_2, a.number_3, a.numbers, a.strikes, a.balls FROM character_numeric_baseball AS a WHERE a.char_id = %d", this->GetGUID());

	if (result)
	{
		Field* fields = result->Fetch();

		this->SetNumericBaseballState(fields[0].GetUInt8());
		this->SetNumericBaseballScore(fields[1].GetInt32());

		for (int32 i = 0; i < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++i)
		{
			this->SetNumericBaseballNumber(i, fields[2 +  i].GetUInt8());
		}

		const char* numbers = fields[5].GetCString();

		if (numbers)
		{
			Tokens tokens(numbers, ' ');

			for (int32 i = 0; i < (NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW); ++i)
			{
				if (tokens.size() > i)
				{
					this->SetNumericBaseballUsedNumber(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}

		const char* strikes = fields[6].GetCString();

		if (strikes)
		{
			Tokens tokens(strikes, ' ');

			for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
			{
				if (tokens.size() > i)
				{
					this->SetNumericBaseballStrike(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}

		const char* balls = fields[7].GetCString();

		if (balls)
		{
			Tokens tokens(balls, ' ');

			for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
			{
				if (tokens.size() > i)
				{
					this->SetNumericBaseballBall(i, cast(uint8, atoi(tokens[i])));
				}
			}
		}
	}
}

void Player::LoadDBMixRecoveryNew()
{
	QueryResult load = MuDatabase.PQuery("SELECT mix_recovery_inventory FROM inventory_data where id=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (shop_range(atoi(slot.c_str())) && sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->GetMixRecoveryInventory()->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_MIX_RECOVERY);
			}
		}
	}
}

void Player::LoadDBQuestMUNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT i.quest_id, i.state, i.objective, i.date FROM character_quest_mu AS i WHERE i.char_id = %d", this->GetGUID());

	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		QuestMUData* pData = new QuestMUData;
		pData->SetID(fields[0].GetUInt16());
		pData->SetState(fields[1].GetUInt8());
		pData->SetObjective(fields[2].GetUInt16());
		pData->SetDate(fields[3].GetInt64());

		this->m_QuestMUDataMap[pData->GetID()] = pData;
	} while (result->NextRow());
}

void Player::LoadMonsterSoulNew()
{
	QueryResult result = MuDatabase.PQuery("SELECT i.type, i.id, i.amount FROM character_monster_soul AS i WHERE i.char_id = %d", this->GetGUID());

	if (!result)
	{
		return;
	}

	do
	{
		Field* fields = result->Fetch();

		uint32 type = fields[0].GetUInt32();

		MonsterSoulData* pData = this->GetMonsterSoulData(type);

		if (!pData)
		{
			pData = new MonsterSoulData;
			pData->SetType(type);
			this->m_MonsterSoulDataMap[type] = pData;
		}

		pData->m_AmountMap[fields[1].GetUInt32()] = fields[2].GetInt32();
	} while (result->NextRow());
}

void Player::LoadDBItemPersonalStoreNew()
{
	QueryResult load = MuDatabase.PQuery("SELECT personal_inventory FROM inventory_data where id=%d", this->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];
			if (sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->GetPersonalStore()->LoadDBItemNew(buffer.c_str(), this->GetGUID(), ITEM_BOX_PERSONAL_STORE);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// SAVE TO DB /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Player::SaveCharacter(uint8 operation)
{
	if ( !this->GetGUID() || this->IsServerQuit() )
		return;

	this->GetTimer(PLAYER_TIMER_SAVE_DB)->Start();

	SQLTransaction trans = MuDatabase.BeginTransaction();

	this->SaveDBInfo(trans);
	this->SaveDBItem(trans);
	this->SaveDBSkill(trans);
	this->SaveDBQuestEvo(trans);
	this->SaveDBFriendList(trans);
	this->SaveDBMailList(trans);
	this->SaveDBGen(trans);
	this->SaveDBGenKill(trans);
	this->SaveDBHelper(trans);
	this->SaveDBBuff(trans);
	this->SaveDBRestriction(trans);
	this->SaveDBStatistics(trans);
	this->SaveDBItemDelay(trans);
	this->SaveDBCashShopItem(trans);
	this->SaveDBAccount(trans);
	this->SaveDBPentagramInfo(trans);
	this->SaveDBChatBlock(trans);

	this->SaveDBHuntingRecord(trans);
	this->SaveDBLabyrinth(trans);
	this->SaveDBLabyrinthZone(trans);
	this->SaveDBLabyrinthLevel(trans);
	this->SaveDBEventCount(trans);
	this->SaveDBMiniBomb(trans);
	this->SaveDBMuRoomy(trans);
	this->SaveDBWarpFavoriteList(trans);
	this->SaveDBQuestGuided(trans);
	this->SaveDBJewelBingo(trans);
	this->SaveDBDate(trans);
	this->SaveDBMajesticTree(trans);
	this->SaveDBMajesticStats(trans);
	this->SaveDBNumericBaseball(trans);
	this->SaveDBQuestMU(trans);
	this->SaveDBMonsterSoul(trans);
	this->SaveDBGameOption(trans);

	SQLTransactionCallBack save_callback(new PlayerSaveTransaction(this, operation));

	MuDatabase.CommitTransaction(trans, save_callback);
}

void Player::SaveDBInfo(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_UPDATE_INFO);

	stmt->setUInt8(this->GetAuthority());
	stmt->setUInt8(this->GetDBClass());
	stmt->setString(this->GetName());

	stmt->setInt16(this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel());
	if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < 400) {
		stmt->setInt16(this->GetLevelData(LEVEL_DATA_MASTER_BAK)->GetLevel());
	}
	else if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() == 400) {
		if (this->GetLevelData(LEVEL_DATA_MASTER_BAK)->GetLevel() == 0) {
			stmt->setInt16(this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel());
		}
		else {
			this->GetLevelData(LEVEL_DATA_MASTER)->SetLevel(this->GetLevelData(LEVEL_DATA_MASTER_BAK)->GetLevel());
			stmt->setInt16(this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel());
		}
	}

	stmt->setInt64(this->GetLevelData(LEVEL_DATA_NORMAL)->GetExperience());
	stmt->setInt64(this->GetLevelData(LEVEL_DATA_MASTER)->GetExperience());
	stmt->setInt32(this->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints());

	if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < 400) {
		stmt->setInt16(this->GetLevelData(LEVEL_DATA_MASTER_BAK)->GetPoints());
	}
	else if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() == 400) {
		if (this->GetLevelData(LEVEL_DATA_MASTER_BAK)->GetLevel() == 0) {
			stmt->setInt16(this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints());
		}
		else {
			this->GetLevelData(LEVEL_DATA_MASTER)->SetPoints(this->GetLevelData(LEVEL_DATA_MASTER_BAK)->GetPoints());
			stmt->setInt16(this->GetLevelData(LEVEL_DATA_MASTER)->GetPoints());
		}
	}

	for (uint64 i = 0; i < MAX_STAT_TYPE; ++i)
	{
		stmt->setUInt32(this->GetStat(i));
	}

	stmt->setUInt16(this->GetWorldId());
	stmt->setInt16(this->GetX());
	stmt->setInt16(this->GetY());
	stmt->setUInt8(this->GetDirection());
	stmt->setUInt32(this->MoneyGet());

	for (uint8 i = 0; i < POWER_MAX; ++i)
	{
		stmt->setInt32(this->PowerGet(i));
	}

	stmt->setInt32(this->GetFruitPointsAdd());
	stmt->setInt32(this->GetFruitPointsDec());
	stmt->setUInt8(this->GetInventory()->GetExpanded());
	stmt->setInt64(this->GetMuteTime());
	stmt->setUInt32(this->GetAdministratorFlag());
	stmt->setUInt8(this->GetPKLevel());
	stmt->setInt32(this->GetPKCount());
	stmt->setInt32(this->GetPKPoints());
	stmt->setUInt8(this->IsFirstTime());
	stmt->setInt64(this->GetSantaClausGiftDate());
	stmt->setString(base64::encode((uint8*)this->GetPersonalStore()->GetName(), PERSONAL_STORE_NAME_LENGTH));
	stmt->setBool(this->GetPersonalStore()->IsOpen() && this->GetPersonalStore()->IsOff());
	stmt->setUInt32(this->GetGoblinPoints());
	stmt->setInt64(time(nullptr));
	stmt->setInt64(this->GetKickTime());
	stmt->setInt32(this->GetPostCount());
	stmt->setUInt8(this->GetPostDay());
	stmt->setUInt8(this->GetPostMonth());
	stmt->setUInt32(this->GetRuudMoney());
	if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() < 400 || this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() < 400) {
		stmt->setInt16(this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->GetLevel());
		stmt->setInt64(this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->GetExperience());
		stmt->setInt32(this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->GetPoints());
	}
	else if (this->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() == 400 && this->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() == 400) {
		if (this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->GetLevel() == 0) {
			stmt->setInt16(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel());
			stmt->setInt64(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetExperience());
			stmt->setInt32(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints());
		}
		else if(this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->GetLevel() > 0){
			stmt->setInt16(this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->GetLevel());
			stmt->setInt64(this->GetLevelData(LEVEL_DATA_MAJESTIC)->GetExperience());
			stmt->setInt32(this->GetLevelData(LEVEL_DATA_MAJESTIC_BAK)->GetPoints());
		}
	}

	stmt->setUInt8(this->IsHuntingRecordVisible() ? 1 : 0);
	stmt->setUInt32(this->GetResetSystem(RESET_TOTAL)->GetReset());
	stmt->setUInt8(this->GetMonsterSoulPurchase());
	stmt->setUInt32(this->GetGUID());

	trans->Append(stmt);

	this->GetWarehouse()->SaveDBData(trans);
}

void Player::SaveDBItem(SQLTransaction& trans)
{
	this->SaveDBAllItemBox();

	//this->SaveItemDbBinary();
	this->GetWarehouse()->WarehouseSaveItemDbBinary();
	//this->SaveMuunDbBinary();
	//this->SaveEventInventoryDbBinary();
	//this->SaveSellNpcInventoryBinary();
	//this->SaveMixRecoveryInventoryBinary();
	//this->SavePersonalStoreDbBinary();
	this->SaveGremoryCaseAccBinary();
	this->SaveGremoryCaseCharaBinary();
	this->SaveGremoryCaseMobileBinary();
	this->SaveGremoryCasePSBinary();
	//this->SavePentagramBinary();
}
	
void Player::SaveDBSkill(SQLTransaction& trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_SKILL);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	SKILL_LOOP(i)
	{
		if ( this->GetSkill(i)->GetSkill() == 0 )
			continue;

		if ( this->GetSkill(i)->IsWeapon() )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_SKILL);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(0);
		stmt->setUInt16(this->GetSkill(i)->GetSkill());
		stmt->setUInt8(this->GetSkill(i)->GetLevel());
		
		trans->Append(stmt);
	}

	for ( PlayerSkillTreeMap::const_iterator it = this->skill_tree_map.begin(); it != this->skill_tree_map.end(); ++it )
	{
		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_SKILL);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(1);
		stmt->setUInt16(it->second->GetSkill());
		stmt->setUInt8(it->second->GetLevel());
		
		trans->Append(stmt);
	}
}

void Player::SaveDBGameOption(SQLTransaction& trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GAMEOPTION);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_GAME_OPTION);
	stmt->setUInt32(this->GetGUID());
	stmt->setUInt8(this->GetGameOption()->GetOption());
	stmt->setUInt8(this->GetGameOption()->GetChatWindow());
	stmt->setUInt8(this->GetGameOption()->GetKeyQ());
	stmt->setUInt8(this->GetGameOption()->GetKeyW());
	stmt->setUInt8(this->GetGameOption()->GetKeyE());
	stmt->setUInt8(this->GetGameOption()->GetKeyR());
	stmt->setInt32(this->GetGameOption()->GetQWERLevel());

	for (int32 i = 0; i < SKILL_KEY_COUNT; ++i)
	{
		stmt->setUInt16(this->GetGameOption()->GetSkillKey(i));
	}

	stmt->setUInt64(this->GetGameOption()->GetExtraFlag());
	stmt->setUInt8(this->GetGameOption()->GetChangeSkin());
	stmt->setUInt16(this->GetGameOption()->GetAdditionalOptions());

	for (int32 i = 0; i < BUTTON_KEY_COUNT; ++i)
	{
		stmt->setUInt8(this->GetGameOption()->GetButtonLink(i));
	}

	trans->Append(stmt);
}
	
void Player::SaveDBQuestEvo(SQLTransaction& trans)
{
	PreparedStatement *stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_QUEST_EVOLUTION);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( QuestEvolutionStatusMap::const_iterator it = this->quest_evolution.begin(); it != this->quest_evolution.end(); ++it )
	{
		if ( it->second.GetState() == QUEST_EVOLUTION_STATE_NONE )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_QUEST_EVOLUTION);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, it->second.GetID());
		stmt->setUInt8(2, it->second.GetState());

		for ( uint8 i = 0; i < QUEST_EVOLUTION_MONSTER_COUNT; ++i )
			stmt->setInt32(3 + i, it->second.GetMonsterCount(i));

		trans->Append(stmt);
	}
}

void Player::SaveDBFriendList(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_FRIEND);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	FRIEND_LOOP(i)
	{
		if ( !strlen(this->GetFriend(i)->GetName()) )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_FRIEND);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setString(1, this->GetFriend(i)->GetName());
		trans->Append(stmt);
	}
}

void Player::SaveDBMailList(SQLTransaction& trans)
{
	MAIL_LOOP(i)
	{
		if ( !this->GetMailData(i)->IsInUse() )
			continue;

		auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_UPDATE);
		stmt->setUInt8(0, this->GetMailData(i)->GetStatus());
		stmt->setUInt32(1, this->GetMailData(i)->GetID());

		trans->Append(stmt);
	}
}

void Player::SaveDBGen(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GENS);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_GEN);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetGen()->GetFamily());
	stmt->setUInt8(2, this->GetGen()->GetLevel());
	stmt->setInt32(3, this->GetGen()->GetContribution());
	stmt->setInt64(4, this->GetGen()->GetRewardDate());
	stmt->setInt64(5, this->GetGen()->GetJoinDate());
	stmt->setInt64(6, this->GetGen()->GetLeftDate());
	
	trans->Append(stmt);
}

void Player::SaveDBGenKill(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_GENS_KILL);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( GenVictimDataMap::const_iterator it = this->m_gen_victim_map.begin(); it != this->m_gen_victim_map.end(); ++it )
	{
		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_GEN_KILL);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt32(1, it->first);
		stmt->setUInt32(2, it->second.count.get());
		stmt->setInt64(3, it->second.last_hit.get());

		trans->Append(stmt);
	}
}

void Player::SaveDBHelper(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_HELPER);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	this->HelperSettingsUpdate();

	stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_HELPER);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt32(1, this->GetHelper()->GetOptionFlags(0));
	stmt->setUInt32(2, this->GetHelper()->GetOptionFlags(1));
	stmt->setUInt8(3, this->GetHelper()->GetItemPickFlags());
	stmt->setUInt8(4, this->GetHelper()->GetHuntingRange());
	stmt->setUInt8(5, this->GetHelper()->GetPickRange());
	stmt->setUInt16(6, this->GetHelper()->GetMovementTime());
	stmt->setUInt16(7, this->GetHelper()->GetAttackSkill(0));
	stmt->setUInt16(8, this->GetHelper()->GetAttackSkill(1));
	stmt->setUInt16(9, this->GetHelper()->GetAttackSkill(2));
	stmt->setUInt16(10, this->GetHelper()->GetDelay(0));
	stmt->setUInt16(11, this->GetHelper()->GetDelay(1));
	stmt->setUInt16(12, this->GetHelper()->GetBuffSkill(0));
	stmt->setUInt16(13, this->GetHelper()->GetBuffSkill(1));
	stmt->setUInt16(14, this->GetHelper()->GetBuffSkill(2));
	stmt->setUInt16(15, this->GetHelper()->GetTimeSpaceCasting());
	stmt->setUInt8(16, this->GetHelper()->GetAutoPotPercent());
	stmt->setUInt8(17, this->GetHelper()->GetAutoHealPercent());
	stmt->setUInt8(18, this->GetHelper()->GetPartyHealPercent());
	stmt->setUInt8(19, this->GetHelper()->GetDrainLifePercent());

	std::ostringstream sstream;

	for ( uint8 i = 0; i < PLAYER_HELPER_MAX_ITEMS; ++i )
	{
		if ( strlen(this->GetHelper()->item_name[i]) )
		{
			sstream << this->GetHelper()->item_name[i] << ";";
		}
	}

	stmt->setString(20, base64::encode((uint8*)sstream.str().c_str(), sstream.str().size()));

	stmt->setUInt16(21, this->GetHelper()->GetItemBuff(0));
	stmt->setUInt16(22, this->GetHelper()->GetItemBuff(1));
	stmt->setUInt16(23, this->GetHelper()->GetItemBuff(2));

	trans->Append(stmt);
}

void Player::SaveDBBuff(SQLTransaction& trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_BUFF);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_DELETE_BUFF);
	stmt->setUInt32(this->GetAccountData()->GetGUID());
	trans->Append(stmt);

	BUFF_LOOP(i)
	{
		BuffData * data = this->GetBuffData(i);

		if (data->GetID() == 0)
			continue;

		if (!data->IsFlag(BUFF_FLAG_DB_STORE))
			continue;

		if (data->IsFlag(BUFF_FLAG_ACCOUNT))
		{
			stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ACCOUNT_INSERT_BUFF);
			stmt->setUInt32(this->GetAccountData()->GetGUID());
		}
		else
		{
			stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_BUFF);
			stmt->setUInt32(this->GetGUID());
		}

		stmt->setUInt16(data->GetID());
		stmt->setUInt8(data->GetEffect(0)->GetEffect());
		stmt->setInt32(static_cast<int32>(data->GetEffect(0)->GetValue()));
		stmt->setUInt8(data->GetEffect(1)->GetEffect());
		stmt->setInt32(static_cast<int32>(data->GetEffect(1)->GetValue()));
		stmt->setUInt8(data->GetEffect(2)->GetEffect());
		stmt->setInt32(static_cast<int32>(data->GetEffect(2)->GetValue()));
		stmt->setInt64(data->GetDuration());
		stmt->setUInt8(data->GetFlag());

		trans->Append(stmt);
	}
}

void Player::SaveDBRestriction(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_RESTRICTION);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for ( int32 i = 0; i < PlayerAction::PLAYER_ACTION_MAX; i++ )
	{
		if ( !this->GetRestriction(i)->IsActive() )
			continue;

		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_RESTRICTION);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt32(i);
		stmt->setInt64(this->GetRestriction(i)->GetTime());
		stmt->setString(this->GetRestriction(i)->GetAdmin());
		stmt->setString(this->GetRestriction(i)->GetReason());

		trans->Append(stmt);
	}
}

void Player::SaveDBStatistics(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_STATISTICS);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	STATISTIC_LOOP(i)
	{
		if ( this->GetStatisticData(i)->GetCount() > 0 )
		{
			stmt = MuDatabase.GetPreparedStatement(QUERY_CHARACTER_STATISTICS_INSERT);
			stmt->setUInt32(this->GetGUID());
			stmt->setUInt32(i);
			stmt->setUInt64(this->GetStatisticData(i)->GetCount());
			trans->Append(stmt);
		}
	}
}

void Player::SaveDBItemDelay(SQLTransaction& trans)
{
	/*PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_ITEM_DELAY);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);*/

	for ( ItemDelayDataMap::const_iterator it = this->m_item_delay.begin(); it != this->m_item_delay.end(); it++ )
	{
		if ( it->second.GetDate() <= 0 )
			continue;

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_ITEM_DELAY);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt16(it->first);
		stmt->setInt64(it->second.GetDate());

		trans->Append(stmt);
	}
}

void Player::SaveDBRegister(uint32 register_id, std::string register_data, Item const* item)
{
	/*PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CHARACTER_REGISTER_INSERT);

	stmt->setUInt32(0, this->GetGUID());
	stmt->setString(1, this->GetName());
	stmt->setUInt32(2, register_id);
	stmt->setString(3, register_data);
	stmt->setString(4, item ? item->BuildRegister(): "");
	stmt->setString(5, this->GetAccountData()->GetIP());

	MuDatabase.Execute(stmt);*/
}

void Player::SaveDBPentagramInfo(SQLTransaction & trans)
{
	//this->SavePentagramBinary();
}

void Player::SaveDBChatBlock(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_DELETE_CHAT_BLOCK);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for ( PlayerChatBlockList::const_iterator it = this->chat_block_list.begin(); it != this->chat_block_list.end(); ++it )
	{
		stmt = MuDatabase.GetPreparedStatement(CHARACTER_INSERT_CHAT_BLOCK);
		stmt->setUInt32(this->GetGUID());
		stmt->setString(*it);

		trans->Append(stmt);
	}
}

void Player::SaveDBCashShopItem(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_DELETE);
	stmt->setUInt32(0, this->GetAccountData()->GetGUID());
	trans->Append(stmt);

	for (CashShopItemDataList::const_iterator itr = this->m_CashShopItemList.begin(); itr != this->m_CashShopItemList.end(); ++itr)
	{
		CashShopItemData const* pItemData = *itr;

		if (!pItemData)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_INSERT);
		stmt->setUInt32(this->GetAccountData()->GetGUID());
		stmt->setUInt32(pItemData->GetProduct());
		stmt->setUInt32(pItemData->GetOption());
		stmt->setUInt32(pItemData->GetSerial());
		stmt->setUInt32(pItemData->GetSerialCashShop());
		stmt->setUInt16(pItemData->GetServer());
		stmt->setUInt8(0);
		stmt->setInt64(pItemData->GetDate());
		trans->Append(stmt);
	}

	for (CashShopItemDataList::const_iterator itr = this->m_CashShopGiftList.begin(); itr != this->m_CashShopGiftList.end(); ++itr)
	{
		CashShopItemData const* pItemData = *itr;

		if (!pItemData)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_INSERT);
		stmt->setUInt32(this->GetAccountData()->GetGUID());
		stmt->setUInt32(pItemData->GetProduct());
		stmt->setUInt32(pItemData->GetOption());
		stmt->setUInt32(pItemData->GetSerial());
		stmt->setUInt32(pItemData->GetSerialCashShop());
		stmt->setUInt16(pItemData->GetServer());
		stmt->setUInt8(1);
		stmt->setInt64(pItemData->GetDate());
		trans->Append(stmt);
	}
}

void Player::SaveDBHuntingRecord(SQLTransaction& trans)
{
	Custom::SystemTimer cur_time = Custom::SystemTimer();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( HuntingRecordDataMap::const_iterator itr = this->hunting_record_data.begin(); itr != this->hunting_record_data.end(); ++itr )
	{
		for ( HuntingRecordDataList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			HuntingRecordData const* pData = (*it);

			if ( !pData )
			{
				continue;
			}

			if ( sGameServer->IsHuntingRecordClear() && cur_time.GetMonth() != pData->GetMonth() )
			{
				continue;
			}

			stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_HUNTING_RECORD_INSERT);
			stmt->setUInt32(this->GetGUID());
			stmt->setUInt16(itr->first);
			stmt->setUInt16(pData->GetYear());
			stmt->setUInt8(pData->GetMonth());
			stmt->setUInt8(pData->GetDay());
			stmt->setUInt32(pData->GetLevel());
			stmt->setUInt32(pData->GetDuration());
			stmt->setUInt64(pData->GetDamage());
			stmt->setUInt64(pData->GetElementalDamage());
			stmt->setUInt32(pData->GetHealing());
			stmt->setUInt32(pData->GetKilledCount());
			stmt->setUInt64(pData->GetEarnedExperience());

			trans->Append(stmt);
		}
	}
}

void Player::SaveDBLabyrinth(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_INSERT);
	stmt->setUInt32(this->GetGUID());
	stmt->setUInt8(this->GetLabyrinthData()->GetStage());
	stmt->setUInt8(this->GetLabyrinthData()->GetID());
	stmt->setUInt16(this->GetLabyrinthData()->GetLevel());
	stmt->setUInt8(this->GetLabyrinthData()->GetStatus());
	stmt->setInt32(this->GetLabyrinthData()->GetKilledMonsters());
	stmt->setInt64(this->GetLabyrinthData()->GetEarnedExperience());
	stmt->setInt32(this->GetLabyrinthData()->GetCompletedMissions());
	stmt->setInt64(this->GetLabyrinthData()->GetDate());
	stmt->setUInt8(this->GetLabyrinthData()->GetGoblinState());
	stmt->setUInt8(this->GetLabyrinthData()->GetDayFirst());

	trans->Append(stmt);
}
	
void Player::SaveDBLabyrinthZone(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_DELETE);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for (PlayerLabyrinthGateList::const_iterator it = this->GetLabyrinthData()->m_GateList.begin(); it != this->GetLabyrinthData()->m_GateList.end(); ++it)
	{
		PlayerLabyrinthGate const* pGate = (*it);

		if (!pGate)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_ZONE_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(pGate->GetIndex());
		stmt->setUInt8(pGate->GetSubIndex());

		for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
		{
			stmt->setUInt8(pGate->GetMissionID(i));
		}

		for (int32 i = 0; i < MAX_LABYRINTH_PLAYER_MISSION; ++i)
		{
			stmt->setInt32(pGate->GetMissionCount(i));
		}

		trans->Append(stmt);
	}
}

void Player::SaveDBLabyrinthLevel(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for (PlayerLabyrinthLevelMap::const_iterator it = this->GetLabyrinthData()->m_LevelMap.begin(); it != this->GetLabyrinthData()->m_LevelMap.end(); ++it)
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_LABYRINTH_LEVEL_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt16(1, it->first);
		stmt->setInt32(2, it->second);
		trans->Append(stmt);
	}
}

void Player::SaveDBEventCount(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( PlayerEventEnterCountMap::const_iterator itr = this->event_enter_count_map.begin(); itr != this->event_enter_count_map.end(); ++itr )
	{
		PlayerEventEnterCount const* pData = itr->second;

		if ( !pData )
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_EVENT_COUNT_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, pData->GetEventID());
		stmt->setUInt8(2, pData->GetCount());
		stmt->setUInt8(3, pData->GetDay());
		trans->Append(stmt);
	}
}

void Player::SaveDBMiniBomb(SQLTransaction & trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_INSERT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetMiniBombState());
	stmt->setUInt16(2, this->GetMiniBombCurrentScore());
	stmt->setInt32(3, this->GetMiniBombTotalScore());
	stmt->setInt64(4, this->GetMiniBombDate());
	trans->Append(stmt);

	if ( this->GetMiniBombState() == MINI_BOMB_GAME_START )
	{
		return;
	}

	for ( int32 i = 0; i < MINI_BOMB_SIZE; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MINI_BOMB_GRID_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, i);
		stmt->setUInt8(2, this->GetMiniBombData(i)->GetValue());
		stmt->setUInt8(3, this->GetMiniBombData(i)->GetStatus());
		trans->Append(stmt);
	}
}

void Player::SaveDBMuRoomy(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_INSERT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetMuRoomyInfo()->IsPlayCard() ? 1 : 0);
	stmt->setUInt8(2, this->GetMuRoomyInfo()->GetType());
	stmt->setUInt16(3, this->GetMuRoomyInfo()->GetScore());
	stmt->setUInt8(4, this->GetMuRoomyInfo()->GetCardDeckCount());
	stmt->setUInt8(5, this->GetMuRoomyInfo()->GetSpecialCardDeckCount());
	stmt->setInt64(6, this->GetMuRoomyInfo()->GetDate());
	trans->Append(stmt);

	if ( !this->GetMuRoomyInfo()->IsPlayCard() )
	{
		return;
	}

	for ( int32 i = 0; i < MU_ROOMY_MAX_CARD; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, MU_ROOMY_CARD_TYPE_NORMAL);
		stmt->setUInt8(2, i);
		stmt->setUInt8(3, this->GetMuRoomyInfo()->GetCardInfo(i)->GetColor());
		stmt->setUInt8(4, this->GetMuRoomyInfo()->GetCardInfo(i)->GetNumber());
		stmt->setUInt8(5, this->GetMuRoomyInfo()->GetCardInfo(i)->GetState());
		stmt->setUInt8(6, this->GetMuRoomyInfo()->GetCardInfo(i)->GetSlot());
		trans->Append(stmt);
	}

	for ( int32 i = 0; i < MU_ROOMY_MAX_CARD_SPECIAL; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MU_ROOMY_DECK_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, MU_ROOMY_CARD_TYPE_SPECIAL);
		stmt->setUInt8(2, i);
		stmt->setUInt8(3, this->GetMuRoomyInfo()->GetSpecialCardInfo(i)->GetColor());
		stmt->setUInt8(4, this->GetMuRoomyInfo()->GetSpecialCardInfo(i)->GetNumber());
		stmt->setUInt8(5, this->GetMuRoomyInfo()->GetSpecialCardInfo(i)->GetState());
		stmt->setUInt8(6, this->GetMuRoomyInfo()->GetSpecialCardInfo(i)->GetSlot());
		trans->Append(stmt);
	}
}

void Player::SaveDBWarpFavoriteList(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( int32 i = 0; i < WARP_FAVORITE_COUNT; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_WARP_FAVORITE_LIST_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, i);
		stmt->setUInt16(2, this->GetWarpFavoriteList(i)->GetData());
		stmt->setUInt16(3, this->GetWarpFavoriteList(i)->GetWarpID());
		trans->Append(stmt);
	}
}

void Player::SaveDBQuestGuided(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for ( PlayerQuestGuidedMap::const_iterator itr = this->quest_guided_map.begin(); itr != this->quest_guided_map.end(); ++itr )
	{
		PlayerQuestGuided const* pData = itr->second;

		if ( !pData )
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_QUEST_GUIDED_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt16(1, pData->GetID());
		stmt->setInt32(2, pData->GetCount());
		stmt->setUInt8(3, pData->GetState());
		stmt->setInt64(4, pData->GetDate());
		trans->Append(stmt);
	}
}

void Player::SaveDBJewelBingo(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_INSERT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetJewelBingoState());
	stmt->setUInt8(2, this->GetJewelBingoBox());
	stmt->setUInt8(3, this->GetJewelBingoCount());
	stmt->setUInt8(4, this->GetJewelBingoCurrentJewel());
	stmt->setInt32(5, this->GetJewelBingoScore1());
	stmt->setInt32(6, this->GetJewelBingoScore2());
	stmt->setInt32(7, this->GetJewelBingoScore3());
	stmt->setInt64(8, this->GetJewelBingoDate());
	trans->Append(stmt);

	if ( !this->GetJewelBingoState() )
	{
		return;
	}

	for ( int32 i = 0; i < JEWEL_BINGO_GRID_SIZE; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, this->GetJewelBingoGrid(i)->GetType());
		stmt->setUInt8(2, this->GetJewelBingoGrid(i)->GetSlot());
		stmt->setUInt8(3, this->GetJewelBingoGrid(i)->GetValue());
		trans->Append(stmt);
	}

	for ( int32 i = 0; i < JEWEL_BINGO_JEWEL_COUNT; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, this->GetJewelBingoJewel(i)->GetType());
		stmt->setUInt8(2, this->GetJewelBingoJewel(i)->GetSlot());
		stmt->setUInt8(3, this->GetJewelBingoJewel(i)->GetValue());
		trans->Append(stmt);
	}

	for ( int32 i = 0; i < JEWEL_BINGO_MATCHING_SIZE; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_JEWEL_BINGO_GRID_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt8(1, this->GetJewelBingoMatching(i)->GetType());
		stmt->setUInt8(2, this->GetJewelBingoMatching(i)->GetSlot());
		stmt->setUInt8(3, this->GetJewelBingoMatching(i)->GetValue());
		trans->Append(stmt);
	}
}

void Player::SaveDBDate(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_DATE_DELETE);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for ( int32 i = 0; i < PLAYER_DATE_MAX; ++i )
	{
		if ( !this->GetDate(i) )
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_DATE_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt32(i);
		stmt->setInt64(this->GetDate(i));
		trans->Append(stmt);
	}
}

void Player::SaveDBMajesticTree(SQLTransaction& trans)
{
	auto stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_DELETE);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for (PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_list.begin(); it != this->majestic_skill_tree_list.end(); ++it)
	{
		MajesticSkill* pSkill = *it;

		if (!pSkill)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(0);
		stmt->setUInt8(pSkill->GetSection());
		stmt->setUInt16(pSkill->GetID());
		stmt->setUInt8(pSkill->GetLevel());

		trans->Append(stmt);
	}

	for (PlayerMajesticSkillTreeList::const_iterator it = this->majestic_skill_tree_stat_list.begin(); it != this->majestic_skill_tree_stat_list.end(); ++it)
	{
		MajesticSkill* pSkill = *it;

		if (!pSkill)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_TREE_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(1);
		stmt->setUInt8(pSkill->GetSection());
		stmt->setUInt16(pSkill->GetID());
		stmt->setUInt8(pSkill->GetLevel());

		trans->Append(stmt);
	}
}

void Player::SaveDBMajesticStats(SQLTransaction& trans)
{
	PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_DELETE);
	stmt->setUInt32(this->GetGUID());
	trans->Append(stmt);

	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_PASSIVE; ++i)
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_MAJESTIC_STATS_INSERT);
		stmt->setUInt32(this->GetGUID());
		stmt->setUInt8(i);
		stmt->setUInt16(this->GetMajesticStat(i));

		trans->Append(stmt);
	}
}

void Player::SaveDBNumericBaseball(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_NUMERIC_BASEBALL_INSERT);
	stmt->setUInt32(0, this->GetGUID());
	stmt->setUInt8(1, this->GetNumericBaseballState());
	stmt->setInt32(2, this->GetNumericBaseballScore());

	for (int32 i = 0; i < NUMERIC_BASEBALL_NUMBERS_PER_ROW; ++i)
	{
		stmt->setUInt8(3 + i, this->GetNumericBaseballNumber(i));
	}

	std::ostringstream numbers;
	for (int32 i = 0; i < (NUMERIC_BASEBALL_ROWS * NUMERIC_BASEBALL_NUMBERS_PER_ROW); ++i)
	{
		numbers << (uint32)this->GetNumericBaseballUsedNumber(i) << " ";
	}

	stmt->setString(6, numbers.str());
	
	std::ostringstream strikes;
	for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
	{
		strikes << (uint32)this->GetNumericBaseballStrike(i) << " ";
	}

	stmt->setString(7, strikes.str());

	std::ostringstream balls;
	for (int32 i = 0; i < NUMERIC_BASEBALL_ROWS; ++i)
	{
		balls << (uint32)this->GetNumericBaseballBall(i) << " ";
	}

	stmt->setString(8, balls.str());

	trans->Append(stmt);
}

void Player::SaveDBQuestMU(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_QUEST_MU_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for (QuestMUDataMap::const_iterator itr = this->m_QuestMUDataMap.begin(); itr != this->m_QuestMUDataMap.end(); ++itr)
	{
		QuestMUData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_QUEST_MU_INSERT);
		stmt->setUInt32(0, this->GetGUID());
		stmt->setUInt16(1, pData->GetID());
		stmt->setUInt8(2, pData->GetState());
		stmt->setUInt16(3, pData->GetObjective());
		stmt->setInt64(4, pData->GetDate());
				
		trans->Append(stmt);
	}
}

void Player::SaveDBMonsterSoul(SQLTransaction& trans)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_SOUL_DELETE);
	stmt->setUInt32(0, this->GetGUID());
	trans->Append(stmt);

	for (MonsterSoulDataMap::const_iterator itr = this->m_MonsterSoulDataMap.begin(); itr != this->m_MonsterSoulDataMap.end(); ++itr)
	{
		MonsterSoulData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		for (MonsterSoulAmountDataMap::const_iterator it = pData->m_AmountMap.begin(); it != pData->m_AmountMap.end(); ++it)
		{
			stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_SOUL_INSERT);
			stmt->setUInt32(0, this->GetGUID());
			stmt->setUInt32(1, pData->GetType());
			stmt->setUInt32(2, it->first);
			stmt->setInt32(3, it->second);

			trans->Append(stmt);
		}
	}
}

void Player::SaveGremoryCaseAccBinary() {
	if (!this)
		return;

	std::ostringstream itemGremoryCaseAcc;

	for (uint32 i = 0; i < GREMORY_CASE_INVENTORY_SIZE; i++) {
		if (!this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->IsItem())
			continue;

		char buffacc[150];

		uint32 Slot = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetItem();
		uint8 SerialServer = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSerialServer();
		uint32 Serial = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSerial();
		uint32 SerialShop = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSkill();
		uint8 ItemLuck = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetLuck();
		uint8 ItemOption = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetOption();
		uint8 ItemExe = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetExe();
		uint8 ItemAncient = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetAncient();
		uint8 Item380 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->Get380();
		uint8 ItemHarmony = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetHarmony();
		uint16 Socket1 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSocket(0);
		uint16 Socket2 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSocket(1);
		uint16 Socket3 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSocket(2);
		uint16 Socket4 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSocket(3);
		uint16 Socket5 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint32 dataExtra0 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->raw.data_extra[0];
		uint32 dataExtra1 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->raw.data_extra[1];
		uint32 dataExtra2 = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->raw.data_extra[2];
		uint32 PersonalStorePrice = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetPersonalStorePrice();
		uint16 PersonalStoreJoB = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetPersonalStorePriceJoB();
		uint16 PersonalStoreJoS = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetExpireDate();
		uint32 DBFlag = this->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(i)->GetDBFlag();

		sprintf(buffacc, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
		itemGremoryCaseAcc << buffacc;
	}

	char bufferacc[20000];
	strcpy(bufferacc, base64::encode((uint8*)itemGremoryCaseAcc.str().c_str(), itemGremoryCaseAcc.str().size()).c_str());

	QueryResult result1 = MuDatabase.PQuery("UPDATE account_warehouse SET gremory_case_acc='%s' where account_id=%d", bufferacc, this->GetAccountData()->GetGUID());
}

void Player::SaveGremoryCaseMobileBinary() {
	if (!this)
		return;

	std::ostringstream itemGremoryCaseMobile;

	//save gremory case mobile
	for (uint32 i = 0; i < GREMORY_CASE_INVENTORY_SIZE; i++) {
		if (!this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->IsItem())
			continue;

		char buffmobile[150];

		uint32 Slot = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetItem();
		uint8 SerialServer = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSerialServer();
		uint32 Serial = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSerial();
		uint32 SerialShop = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSkill();
		uint8 ItemLuck = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetLuck();
		uint8 ItemOption = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetOption();
		uint8 ItemExe = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetExe();
		uint8 ItemAncient = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetAncient();
		uint8 Item380 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->Get380();
		uint8 ItemHarmony = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetHarmony();
		uint16 Socket1 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSocket(0);
		uint16 Socket2 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSocket(1);
		uint16 Socket3 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSocket(2);
		uint16 Socket4 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSocket(3);
		uint16 Socket5 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint32 dataExtra0 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->raw.data_extra[0];
		uint32 dataExtra1 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->raw.data_extra[1];
		uint32 dataExtra2 = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->raw.data_extra[2];
		uint32 PersonalStorePrice = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetPersonalStorePrice();
		uint16 PersonalStoreJoB = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetPersonalStorePriceJoB();
		uint16 PersonalStoreJoS = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetExpireDate();
		uint32 DBFlag = this->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(i)->GetDBFlag();

		sprintf(buffmobile, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
		itemGremoryCaseMobile << buffmobile;
	}

	char buffermobile[20000];
	strcpy(buffermobile, base64::encode((uint8*)itemGremoryCaseMobile.str().c_str(), itemGremoryCaseMobile.str().size()).c_str());

	QueryResult result3 = MuDatabase.PQuery("UPDATE character_gremory_case SET gremory_case_mobile='%s' where guid=%d", buffermobile, this->GetGUID());
}

void Player::SaveGremoryCaseCharaBinary() {
	if (!this)
		return;

	std::ostringstream itemGremoryCaseChar;

	//save gremory case character
	for (uint32 i = 0; i < GREMORY_CASE_INVENTORY_SIZE; i++) {
		if (!this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->IsItem())
			continue;

		char buffchara[150];

		uint32 Slot = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetItem();
		uint8 SerialServer = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSerialServer();
		uint32 Serial = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSerial();
		uint32 SerialShop = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSkill();
		uint8 ItemLuck = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetLuck();
		uint8 ItemOption = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetOption();
		uint8 ItemExe = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetExe();
		uint8 ItemAncient = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetAncient();
		uint8 Item380 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->Get380();
		uint8 ItemHarmony = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetHarmony();
		uint16 Socket1 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSocket(0);
		uint16 Socket2 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSocket(1);
		uint16 Socket3 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSocket(2);
		uint16 Socket4 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSocket(3);
		uint16 Socket5 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint32 dataExtra0 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->raw.data_extra[0];
		uint32 dataExtra1 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->raw.data_extra[1];
		uint32 dataExtra2 = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->raw.data_extra[2];
		uint32 PersonalStorePrice = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetPersonalStorePrice();
		uint16 PersonalStoreJoB = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetPersonalStorePriceJoB();
		uint16 PersonalStoreJoS = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetExpireDate();
		uint32 DBFlag = this->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->GetItem(i)->GetDBFlag();

		sprintf(buffchara, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);

		itemGremoryCaseChar << buffchara;
	}

	char bufferchar[20000];
	strcpy(bufferchar, base64::encode((uint8*)itemGremoryCaseChar.str().c_str(), itemGremoryCaseChar.str().size()).c_str());

	QueryResult result2 = MuDatabase.PQuery("UPDATE character_gremory_case SET gremory_case_char='%s' where guid=%d", bufferchar, this->GetGUID());
}

void Player::SaveGremoryCasePSBinary() {
	//save gremory case ps
	std::ostringstream itemGremoryCasePS;

	if (sGameServer->IsPersonalStoreEnabled()) {
		for (uint32 i = 0; i < GREMORY_CASE_PERSONAL_STORE_SIZE; i++) {
			if (!this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->IsItem())
				continue;

			char buffps[150];

			uint32 Slot = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetOwnerSlot();
			uint32 ItemId = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetItem();
			uint8 SerialServer = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSerialServer();
			uint32 Serial = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSerial();
			uint32 SerialShop = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSerialShop();
			uint8 ItemLevel = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetLevel();
			uint8 ItemDurability = static_cast<uint8>(this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetDurability());
			uint32 ItemDurabilityState = static_cast<int32>(this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetDurabilityState());
			uint8 ItemSkill = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSkill();
			uint8 ItemLuck = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetLuck();
			uint8 ItemOption = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetOption();
			uint8 ItemExe = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetExe();
			uint8 ItemAncient = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetAncient();
			uint8 Item380 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->Get380();
			uint8 ItemHarmony = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetHarmony();
			uint16 Socket1 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSocket(0);
			uint16 Socket2 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSocket(1);
			uint16 Socket3 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSocket(2);
			uint16 Socket4 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSocket(3);
			uint16 Socket5 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSocket(4);
			uint16 SocketBonus = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetSocketBonus();
			uint16 IsInventoryActive = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->IsInventoryActive() ? 1 : 0;
			uint32 dataExtra0 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->raw.data_extra[0];
			uint32 dataExtra1 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->raw.data_extra[1];
			uint32 dataExtra2 = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->raw.data_extra[2];
			uint32 PersonalStorePrice = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetPersonalStorePrice();
			uint16 PersonalStoreJoB = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetPersonalStorePriceJoB();
			uint16 PersonalStoreJoS = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetPersonalStorePriceJoS();
			uint64 ExpireDate = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetExpireDate();
			uint32 DBFlag = this->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->GetItem(i)->GetDBFlag();

			sprintf(buffps, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
				ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
				PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
			itemGremoryCasePS << buffps;
		}
	}
	char bufferps[20000];
	strcpy(bufferps, base64::encode((uint8*)itemGremoryCasePS.str().c_str(), itemGremoryCasePS.str().size()).c_str());

	QueryResult result4 = MuDatabase.PQuery("UPDATE character_gremory_case SET gremory_case_ps='%s' where guid=%d", bufferps, this->GetGUID());
}

void Player::SaveDBAllItemBox() {
	if (!this)
		return;

	std::ostringstream itemString;
	std::ostringstream itempentagram;

	full_inventory_loop(i) {
		if (!this->GetInventory()->GetItem(i)->IsItem())
			continue;

		char buff[250];

		uint16 Box = ITEM_BOX_INVENTORY;
		uint32 Slot = this->GetInventory()->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = this->GetInventory()->GetItem(i)->GetItem();
		uint8 SerialServer = this->GetInventory()->GetItem(i)->GetSerialServer();
		uint32 Serial = this->GetInventory()->GetItem(i)->GetSerial();
		uint32 SerialShop = this->GetInventory()->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = this->GetInventory()->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(this->GetInventory()->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(this->GetInventory()->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = this->GetInventory()->GetItem(i)->GetSkill();
		uint8 ItemLuck = this->GetInventory()->GetItem(i)->GetLuck();
		uint8 ItemOption = this->GetInventory()->GetItem(i)->GetOption();
		uint8 ItemExe = this->GetInventory()->GetItem(i)->GetExe();
		uint8 ItemAncient = this->GetInventory()->GetItem(i)->GetAncient();
		uint8 Item380 = this->GetInventory()->GetItem(i)->Get380();
		uint8 ItemHarmony = this->GetInventory()->GetItem(i)->GetHarmony();
		uint16 Socket1 = this->GetInventory()->GetItem(i)->GetSocket(0);
		uint16 Socket2 = this->GetInventory()->GetItem(i)->GetSocket(1);
		uint16 Socket3 = this->GetInventory()->GetItem(i)->GetSocket(2);
		uint16 Socket4 = this->GetInventory()->GetItem(i)->GetSocket(3);
		uint16 Socket5 = this->GetInventory()->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = this->GetInventory()->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = this->GetInventory()->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint32 dataExtra0 = this->GetInventory()->GetItem(i)->raw.data_extra[0];
		uint32 dataExtra1 = this->GetInventory()->GetItem(i)->raw.data_extra[1];
		uint32 dataExtra2 = this->GetInventory()->GetItem(i)->raw.data_extra[2];
		uint32 PersonalStorePrice = this->GetInventory()->GetItem(i)->GetPersonalStorePrice();
		uint16 PersonalStoreJoB = this->GetInventory()->GetItem(i)->GetPersonalStorePriceJoB();
		uint16 PersonalStoreJoS = this->GetInventory()->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = this->GetInventory()->GetItem(i)->GetExpireDate();
		uint32 DBFlag = this->GetInventory()->GetItem(i)->GetDBFlag();

		sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Box, Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
		itemString << buff;
	}

	EVENT_INVENTORY_LOOP(i) {
		if (!this->GetEventInventory()->GetItem(i)->IsItem())
			continue;

		char buff[250];

		uint16 Box = ITEM_BOX_EVENT_INVENTORY;
		uint32 Slot = this->GetEventInventory()->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = this->GetEventInventory()->GetItem(i)->GetItem();
		uint8 SerialServer = this->GetEventInventory()->GetItem(i)->GetSerialServer();
		uint32 Serial = this->GetEventInventory()->GetItem(i)->GetSerial();
		uint32 SerialShop = this->GetEventInventory()->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = this->GetEventInventory()->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(this->GetEventInventory()->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(this->GetEventInventory()->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = this->GetEventInventory()->GetItem(i)->GetSkill();
		uint8 ItemLuck = this->GetEventInventory()->GetItem(i)->GetLuck();
		uint8 ItemOption = this->GetEventInventory()->GetItem(i)->GetOption();
		uint8 ItemExe = this->GetEventInventory()->GetItem(i)->GetExe();
		uint8 ItemAncient = this->GetEventInventory()->GetItem(i)->GetAncient();
		uint8 Item380 = this->GetEventInventory()->GetItem(i)->Get380();
		uint8 ItemHarmony = this->GetEventInventory()->GetItem(i)->GetHarmony();
		uint16 Socket1 = this->GetEventInventory()->GetItem(i)->GetSocket(0);
		uint16 Socket2 = this->GetEventInventory()->GetItem(i)->GetSocket(1);
		uint16 Socket3 = this->GetEventInventory()->GetItem(i)->GetSocket(2);
		uint16 Socket4 = this->GetEventInventory()->GetItem(i)->GetSocket(3);
		uint16 Socket5 = this->GetEventInventory()->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = this->GetEventInventory()->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = this->GetEventInventory()->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint32 dataExtra0 = this->GetEventInventory()->GetItem(i)->raw.data_extra[0];
		uint32 dataExtra1 = this->GetEventInventory()->GetItem(i)->raw.data_extra[1];
		uint32 dataExtra2 = this->GetEventInventory()->GetItem(i)->raw.data_extra[2];
		uint32 PersonalStorePrice = this->GetEventInventory()->GetItem(i)->GetPersonalStorePrice();
		uint16 PersonalStoreJoB = this->GetEventInventory()->GetItem(i)->GetPersonalStorePriceJoB();
		uint16 PersonalStoreJoS = this->GetEventInventory()->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = this->GetEventInventory()->GetItem(i)->GetExpireDate();
		uint32 DBFlag = this->GetEventInventory()->GetItem(i)->GetDBFlag();

		sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Box, Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
		itemString << buff;
	}

	if (sGameServer->IsPersonalStoreEnabled()) {
		for (uint8 i = 0; i < max_shop_item; i++) {
			if (!this->GetPersonalStore()->GetItem(i)->IsItem())
				continue;

			char buff[250];

			uint16 Box = ITEM_BOX_PERSONAL_STORE;
			uint32 Slot = this->GetPersonalStore()->GetItem(i)->GetOwnerSlot();
			uint32 ItemId = this->GetPersonalStore()->GetItem(i)->GetItem();
			uint8 SerialServer = this->GetPersonalStore()->GetItem(i)->GetSerialServer();
			uint32 Serial = this->GetPersonalStore()->GetItem(i)->GetSerial();
			uint32 SerialShop = this->GetPersonalStore()->GetItem(i)->GetSerialShop();
			uint8 ItemLevel = this->GetPersonalStore()->GetItem(i)->GetLevel();
			uint8 ItemDurability = static_cast<uint8>(this->GetPersonalStore()->GetItem(i)->GetDurability());
			uint32 ItemDurabilityState = static_cast<int32>(this->GetPersonalStore()->GetItem(i)->GetDurabilityState());
			uint8 ItemSkill = this->GetPersonalStore()->GetItem(i)->GetSkill();
			uint8 ItemLuck = this->GetPersonalStore()->GetItem(i)->GetLuck();
			uint8 ItemOption = this->GetPersonalStore()->GetItem(i)->GetOption();
			uint8 ItemExe = this->GetPersonalStore()->GetItem(i)->GetExe();
			uint8 ItemAncient = this->GetPersonalStore()->GetItem(i)->GetAncient();
			uint8 Item380 = this->GetPersonalStore()->GetItem(i)->Get380();
			uint8 ItemHarmony = this->GetPersonalStore()->GetItem(i)->GetHarmony();
			uint16 Socket1 = this->GetPersonalStore()->GetItem(i)->GetSocket(0);
			uint16 Socket2 = this->GetPersonalStore()->GetItem(i)->GetSocket(1);
			uint16 Socket3 = this->GetPersonalStore()->GetItem(i)->GetSocket(2);
			uint16 Socket4 = this->GetPersonalStore()->GetItem(i)->GetSocket(3);
			uint16 Socket5 = this->GetPersonalStore()->GetItem(i)->GetSocket(4);
			uint16 SocketBonus = this->GetPersonalStore()->GetItem(i)->GetSocketBonus();
			uint8 IsInventoryActive = this->GetPersonalStore()->GetItem(i)->IsInventoryActive() ? 1 : 0;
			uint32 dataExtra0 = this->GetPersonalStore()->GetItem(i)->raw.data_extra[0];
			uint32 dataExtra1 = this->GetPersonalStore()->GetItem(i)->raw.data_extra[1];
			uint32 dataExtra2 = this->GetPersonalStore()->GetItem(i)->raw.data_extra[2];
			uint32 PersonalStorePrice = this->GetPersonalStore()->GetItem(i)->GetPersonalStorePrice();
			uint16 PersonalStoreJoB = this->GetPersonalStore()->GetItem(i)->GetPersonalStorePriceJoB();
			uint16 PersonalStoreJoS = this->GetPersonalStore()->GetItem(i)->GetPersonalStorePriceJoS();
			uint64 ExpireDate = this->GetPersonalStore()->GetItem(i)->GetExpireDate();
			uint32 DBFlag = this->GetPersonalStore()->GetItem(i)->GetDBFlag();

			sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Box, Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
				ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
				PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
			itemString << buff;
		}
	}

	for (uint8 i = 0; i < MAX_ITEM_SOLD; i++) {
		if (!this->GetNpcSell()->GetItem(i)->IsItem())
			continue;

		char buff[250];

		uint16 Box = ITEM_BOX_SELL_NPC;
		uint32 Slot = this->GetNpcSell()->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = this->GetNpcSell()->GetItem(i)->GetItem();
		uint8 SerialServer = this->GetNpcSell()->GetItem(i)->GetSerialServer();
		uint32 Serial = this->GetNpcSell()->GetItem(i)->GetSerial();
		uint32 SerialShop = this->GetNpcSell()->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = this->GetNpcSell()->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(this->GetNpcSell()->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(this->GetNpcSell()->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = this->GetNpcSell()->GetItem(i)->GetSkill();
		uint8 ItemLuck = this->GetNpcSell()->GetItem(i)->GetLuck();
		uint8 ItemOption = this->GetNpcSell()->GetItem(i)->GetOption();
		uint8 ItemExe = this->GetNpcSell()->GetItem(i)->GetExe();
		uint8 ItemAncient = this->GetNpcSell()->GetItem(i)->GetAncient();
		uint8 Item380 = this->GetNpcSell()->GetItem(i)->Get380();
		uint8 ItemHarmony = this->GetNpcSell()->GetItem(i)->GetHarmony();
		uint16 Socket1 = this->GetNpcSell()->GetItem(i)->GetSocket(0);
		uint16 Socket2 = this->GetNpcSell()->GetItem(i)->GetSocket(1);
		uint16 Socket3 = this->GetNpcSell()->GetItem(i)->GetSocket(2);
		uint16 Socket4 = this->GetNpcSell()->GetItem(i)->GetSocket(3);
		uint16 Socket5 = this->GetNpcSell()->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = this->GetNpcSell()->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = this->GetNpcSell()->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint32 dataExtra0 = this->GetNpcSell()->GetItem(i)->raw.data_extra[0];
		uint32 dataExtra1 = this->GetNpcSell()->GetItem(i)->raw.data_extra[1];
		uint32 dataExtra2 = this->GetNpcSell()->GetItem(i)->raw.data_extra[2];
		uint32 PersonalStorePrice = this->GetNpcSell()->GetItem(i)->GetPersonalStorePrice();
		uint16 PersonalStoreJoB = this->GetNpcSell()->GetItem(i)->GetPersonalStorePriceJoB();
		uint16 PersonalStoreJoS = this->GetNpcSell()->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = this->GetNpcSell()->GetItem(i)->GetExpireDate();
		uint32 DBFlag = this->GetNpcSell()->GetItem(i)->GetDBFlag();


		sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Box, Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
		itemString << buff;
	}

	for (uint8 i = 0; i < max_shop_item; i++) {
		if (!this->GetMixRecoveryInventory()->GetItem(i)->IsItem())
			continue;

		char buff[250];

		uint16 Box = ITEM_BOX_MIX_RECOVERY;
		uint32 Slot = this->GetMixRecoveryInventory()->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = this->GetMixRecoveryInventory()->GetItem(i)->GetItem();
		uint8 SerialServer = this->GetMixRecoveryInventory()->GetItem(i)->GetSerialServer();
		uint32 Serial = this->GetMixRecoveryInventory()->GetItem(i)->GetSerial();
		uint32 SerialShop = this->GetMixRecoveryInventory()->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = this->GetMixRecoveryInventory()->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(this->GetMixRecoveryInventory()->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(this->GetMixRecoveryInventory()->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = this->GetMixRecoveryInventory()->GetItem(i)->GetSkill();
		uint8 ItemLuck = this->GetMixRecoveryInventory()->GetItem(i)->GetLuck();
		uint8 ItemOption = this->GetMixRecoveryInventory()->GetItem(i)->GetOption();
		uint8 ItemExe = this->GetMixRecoveryInventory()->GetItem(i)->GetExe();
		uint8 ItemAncient = this->GetMixRecoveryInventory()->GetItem(i)->GetAncient();
		uint8 Item380 = this->GetMixRecoveryInventory()->GetItem(i)->Get380();
		uint8 ItemHarmony = this->GetMixRecoveryInventory()->GetItem(i)->GetHarmony();
		uint16 Socket1 = this->GetMixRecoveryInventory()->GetItem(i)->GetSocket(0);
		uint16 Socket2 = this->GetMixRecoveryInventory()->GetItem(i)->GetSocket(1);
		uint16 Socket3 = this->GetMixRecoveryInventory()->GetItem(i)->GetSocket(2);
		uint16 Socket4 = this->GetMixRecoveryInventory()->GetItem(i)->GetSocket(3);
		uint16 Socket5 = this->GetMixRecoveryInventory()->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = this->GetMixRecoveryInventory()->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = this->GetMixRecoveryInventory()->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint32 dataExtra0 = this->GetMixRecoveryInventory()->GetItem(i)->raw.data_extra[0];
		uint32 dataExtra1 = this->GetMixRecoveryInventory()->GetItem(i)->raw.data_extra[1];
		uint32 dataExtra2 = this->GetMixRecoveryInventory()->GetItem(i)->raw.data_extra[2];
		uint32 PersonalStorePrice = this->GetMixRecoveryInventory()->GetItem(i)->GetPersonalStorePrice();
		uint16 PersonalStoreJoB = this->GetMixRecoveryInventory()->GetItem(i)->GetPersonalStorePriceJoB();
		uint16 PersonalStoreJoS = this->GetMixRecoveryInventory()->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = this->GetMixRecoveryInventory()->GetItem(i)->GetExpireDate();
		uint32 DBFlag = this->GetMixRecoveryInventory()->GetItem(i)->GetDBFlag();

		sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Box, Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
		itemString << buff;
	}

	for (int8 i = 0; i < MUUN_INVENTORY_SIZE; i++) {
		if (!this->GetMuunInventory()->GetItem(i)->IsItem())
			continue;

		char buff[250];

		uint16 Box = ITEM_BOX_MUUN;
		uint32 Slot = this->GetMuunInventory()->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = this->GetMuunInventory()->GetItem(i)->GetItem();
		uint8 SerialServer = this->GetMuunInventory()->GetItem(i)->GetSerialServer();
		uint32 Serial = this->GetMuunInventory()->GetItem(i)->GetSerial();
		uint32 SerialShop = this->GetMuunInventory()->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = this->GetMuunInventory()->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(this->GetMuunInventory()->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(this->GetMuunInventory()->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = this->GetMuunInventory()->GetItem(i)->GetSkill();
		uint8 ItemLuck = this->GetMuunInventory()->GetItem(i)->GetLuck();
		uint8 ItemOption = this->GetMuunInventory()->GetItem(i)->GetOption();
		uint8 ItemExe = this->GetMuunInventory()->GetItem(i)->GetExe();
		uint8 ItemAncient = this->GetMuunInventory()->GetItem(i)->GetAncient();
		uint8 Item380 = this->GetMuunInventory()->GetItem(i)->Get380();
		uint8 ItemHarmony = this->GetMuunInventory()->GetItem(i)->GetHarmony();
		uint16 Socket1 = this->GetMuunInventory()->GetItem(i)->GetSocket(0);
		uint16 Socket2 = this->GetMuunInventory()->GetItem(i)->GetSocket(1);
		uint16 Socket3 = this->GetMuunInventory()->GetItem(i)->GetSocket(2);
		uint16 Socket4 = this->GetMuunInventory()->GetItem(i)->GetSocket(3);
		uint16 Socket5 = this->GetMuunInventory()->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = this->GetMuunInventory()->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = this->GetMuunInventory()->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint32 dataExtra0 = this->GetMuunInventory()->GetItem(i)->raw.data_extra[0];
		uint32 dataExtra1 = this->GetMuunInventory()->GetItem(i)->raw.data_extra[1];
		uint32 dataExtra2 = this->GetMuunInventory()->GetItem(i)->raw.data_extra[2];
		uint32 PersonalStorePrice = this->GetMuunInventory()->GetItem(i)->GetPersonalStorePrice();
		uint16 PersonalStoreJoB = this->GetMuunInventory()->GetItem(i)->GetPersonalStorePriceJoB();
		uint16 PersonalStoreJoS = this->GetMuunInventory()->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = this->GetMuunInventory()->GetItem(i)->GetExpireDate();
		uint32 DBFlag = this->GetMuunInventory()->GetItem(i)->GetDBFlag();

		sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Box, Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
		itemString << buff;
	}

	PentagramJewelInfo const* pPentagramJewelInfo = nullptr;

	for (uint32 i = 0; i < MAX_PENTAGRAM_JEWEL_INFO; i++) {
		char buff[250];

		if (pPentagramJewelInfo = this->GetPentagramJewelInfoInventory(i))
		{
			if (pPentagramJewelInfo->GetIndex() != uint8(-1))
			{
				uint8 type = PENTAGRAM_JEWEL_TYPE_INVENTORY;
				uint8 index = pPentagramJewelInfo->GetIndex();
				uint8 atribute = pPentagramJewelInfo->GetAttribute();
				uint8 itemSection = pPentagramJewelInfo->GetItemSection();
				uint16 itemType = pPentagramJewelInfo->GetItemType();
				uint8 itemLevel = pPentagramJewelInfo->GetItemLevel();

				uint16 rankindex1 = pPentagramJewelInfo->GetRankData(0)->GetIndex();
				uint16 ranklevel1 = pPentagramJewelInfo->GetRankData(0)->GetLevel();
				uint16 rankindex2 = pPentagramJewelInfo->GetRankData(1)->GetIndex();
				uint16 ranklevel2 = pPentagramJewelInfo->GetRankData(1)->GetLevel();
				uint16 rankindex3 = pPentagramJewelInfo->GetRankData(2)->GetIndex();
				uint16 ranklevel3 = pPentagramJewelInfo->GetRankData(2)->GetLevel();
				uint16 rankindex4 = pPentagramJewelInfo->GetRankData(3)->GetIndex();
				uint16 ranklevel4 = pPentagramJewelInfo->GetRankData(3)->GetLevel();
				uint16 rankindex5 = pPentagramJewelInfo->GetRankData(4)->GetIndex();
				uint16 ranklevel5 = pPentagramJewelInfo->GetRankData(4)->GetLevel();

				uint16 serialServer = pPentagramJewelInfo->GetSerialServer();
				uint32 serial = pPentagramJewelInfo->GetSerial();
				uint32 DBFlag = pPentagramJewelInfo->GetDBFlag();
				uint32 rawData1 = pPentagramJewelInfo->GetData(0);
				uint32 rawData2 = pPentagramJewelInfo->GetData(1);
				uint32 rawData3 = pPentagramJewelInfo->GetData(2);

				sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", type, index, atribute, itemSection, itemType, itemLevel, rankindex1, ranklevel1, rankindex2, ranklevel2,
					rankindex3, ranklevel3, rankindex4, ranklevel4, rankindex5, ranklevel5, serialServer, serial, DBFlag, rawData1, rawData2, rawData3);

				itempentagram << buff;
			}
		}

		if (pPentagramJewelInfo = this->GetPentagramJewelInfoWarehouse(i))
		{
			if (pPentagramJewelInfo->GetIndex() != uint8(-1))
			{
				uint8 type = PENTAGRAM_JEWEL_TYPE_WAREHOUSE;
				uint8 index = pPentagramJewelInfo->GetIndex();
				uint8 atribute = pPentagramJewelInfo->GetAttribute();
				uint8 itemSection = pPentagramJewelInfo->GetItemSection();
				uint16 itemType = pPentagramJewelInfo->GetItemType();
				uint8 itemLevel = pPentagramJewelInfo->GetItemLevel();

				uint16 rankindex1 = pPentagramJewelInfo->GetRankData(0)->GetIndex();
				uint16 ranklevel1 = pPentagramJewelInfo->GetRankData(0)->GetLevel();
				uint16 rankindex2 = pPentagramJewelInfo->GetRankData(1)->GetIndex();
				uint16 ranklevel2 = pPentagramJewelInfo->GetRankData(1)->GetLevel();
				uint16 rankindex3 = pPentagramJewelInfo->GetRankData(2)->GetIndex();
				uint16 ranklevel3 = pPentagramJewelInfo->GetRankData(2)->GetLevel();
				uint16 rankindex4 = pPentagramJewelInfo->GetRankData(3)->GetIndex();
				uint16 ranklevel4 = pPentagramJewelInfo->GetRankData(3)->GetLevel();
				uint16 rankindex5 = pPentagramJewelInfo->GetRankData(4)->GetIndex();
				uint16 ranklevel5 = pPentagramJewelInfo->GetRankData(4)->GetLevel();

				uint16 serialServer = pPentagramJewelInfo->GetSerialServer();
				uint32 serial = pPentagramJewelInfo->GetSerial();
				uint32 DBFlag = pPentagramJewelInfo->GetDBFlag();
				uint32 rawData1 = pPentagramJewelInfo->GetData(0);
				uint32 rawData2 = pPentagramJewelInfo->GetData(1);
				uint32 rawData3 = pPentagramJewelInfo->GetData(2);

				sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", type, index, atribute, itemSection, itemType, itemLevel, rankindex1, ranklevel1, rankindex2, ranklevel2,
					rankindex3, ranklevel3, rankindex4, ranklevel4, rankindex5, ranklevel5, serialServer, serial, DBFlag, rawData1, rawData2, rawData3);

				itempentagram << buff;
			}
		}
	}

	//strcpy(buffer, base64::encode((uint8*)itemString.str().c_str(), itemString.str().size()).c_str());
	//strcpy(bufferpentagram, base64::encode((uint8*)itempentagram.str().c_str(), itempentagram.str().size()).c_str());

	//sLog->outDebug(LOG_DEFAULT, "Save Debug");
	//sLog->outDebug(LOG_DEFAULT, "%s", itemString.str().c_str());

	//QueryResult result = MuDatabase.PQuery("REPLACE INTO character_item_inventory(GUID, InventoryData, PentagramInventoryData) VALUES (%d, '%s', '%s')", this->GetGUID(), buffer, bufferpentagram);
	QueryResult saveitem = MuDatabase.PQuery("REPLACE INTO character_item_inventory(GUID, InventoryData) VALUES (%d, '%s')", this->GetGUID(), itemString.str().c_str());
	QueryResult savepentagram = MuDatabase.PQuery("REPLACE INTO character_item_pentagram(GUID, PentagramInventoryData) VALUES (%d, '%s')", this->GetGUID(), itempentagram.str().c_str());
}

void Player::LoadDBItemAllBox() {
	QueryResult loaditem = MuDatabase.PQuery("SELECT InventoryData FROM character_item_inventory where GUID=%d", this->GetGUID());

	if (loaditem) {
		Field* field = loaditem->Fetch();

		Tokenizer tokens1(field[0].GetString().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string box = tokens2[0];
			std::string slot = tokens2[1];
			std::string itemEntry = tokens2[2];
			if (atoi(box.c_str()) == ITEM_BOX_INVENTORY) {
				if (inventory_range(atoi(slot.c_str())) && sItemMgr->IsItem(atoi(itemEntry.c_str())) && !Item::IsEventArchangelWeapon(atoi(itemEntry.c_str())))
				{
					this->GetInventory()->LoadDBItem(buffer.c_str(), this->GetGUID(), ITEM_BOX_INVENTORY);
				}
			}
			else if (atoi(box.c_str()) == ITEM_BOX_MUUN) {
				if (MUUN_INVENTORY_RANGE(atoi(slot.c_str())) && sItemMgr->GetItemInventoryType(atoi(itemEntry.c_str())) == ITEM_INVENTORY_TYPE_MUUN && sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
					this->GetMuunInventory()->LoadDBItem(buffer.c_str(), this->GetGUID(), ITEM_BOX_MUUN);
				}
			}
			else if (atoi(box.c_str()) == ITEM_BOX_EVENT_INVENTORY) {
				if (EVENT_INVENTORY_RANGE(atoi(slot.c_str())) && sItemMgr->GetItemInventoryType(atoi(itemEntry.c_str())) == ITEM_INVENTORY_TYPE_EVENT) {
					this->GetEventInventory()->LoadDBItem(buffer.c_str(), this->GetGUID(), ITEM_BOX_EVENT_INVENTORY);
				}
			}
			else if (atoi(box.c_str()) == ITEM_BOX_SELL_NPC) {
				if (atoi(slot.c_str()) < MAX_ITEM_SOLD && sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
					this->GetNpcSell()->LoadDBItem(buffer.c_str(), this->GetGUID(), ITEM_BOX_SELL_NPC);
				}
			}
			else if (atoi(box.c_str()) == ITEM_BOX_PERSONAL_STORE) {
				if (sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
					this->GetPersonalStore()->LoadDBItem(buffer.c_str(), this->GetGUID(), ITEM_BOX_PERSONAL_STORE);
				}
			}
			else if (atoi(box.c_str()) == ITEM_BOX_MIX_RECOVERY) {
				if (shop_range(atoi(slot.c_str())) && sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
					this->GetMixRecoveryInventory()->LoadDBItem(buffer.c_str(), this->GetGUID(), ITEM_BOX_MIX_RECOVERY);
				}
			}
		}
	}

	QueryResult loadpentagram = MuDatabase.PQuery("SELECT PentagramInventoryData FROM character_item_pentagram where GUID=%d", this->GetGUID());

	if (loadpentagram) {
		Field* field = loadpentagram->Fetch();

		Tokenizer tokens1(field[0].GetString().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');

			std::string type = tokens2[0];
			std::string index = tokens2[1];

			if (atoi(index.c_str()) >= MAX_PENTAGRAM_JEWEL_INFO)
			{
				continue;
			}

			if (atoi(type.c_str()) != PENTAGRAM_JEWEL_TYPE_INVENTORY && atoi(type.c_str()) != PENTAGRAM_JEWEL_TYPE_WAREHOUSE)
			{
				continue;
			}

			PentagramJewelInfo* pPentagramJewelInfoStr = (atoi(type.c_str()) == PENTAGRAM_JEWEL_TYPE_INVENTORY) ? this->GetPentagramJewelInfoInventoryPtr() : this->GetPentagramJewelInfoWarehousePtr();

			if (pPentagramJewelInfoStr[atoi(index.c_str())].GetIndex() != uint8(-1))
			{
				continue;
			}

			std::string atribute = tokens2[2];
			std::string section = tokens2[3];
			std::string itemType = tokens2[4];
			std::string itemLevel = tokens2[5];
			std::string rankindex1 = tokens2[6];
			std::string rankdata1 = tokens2[7];
			std::string rankindex2 = tokens2[8];
			std::string rankdata2 = tokens2[9];
			std::string rankindex3 = tokens2[10];
			std::string rankdata3 = tokens2[11];
			std::string rankindex4 = tokens2[12];
			std::string rankdata4 = tokens2[13];
			std::string rankindex5 = tokens2[14];
			std::string rankdata5 = tokens2[15];
			std::string serialServer = tokens2[16];
			std::string serial = tokens2[17];
			std::string dbflag = tokens2[18];
			std::string rawdata1 = tokens2[19];
			std::string rawdata2 = tokens2[20];
			std::string rawdata3 = tokens2[21];

			pPentagramJewelInfoStr[atoi(index.c_str())].SetIndex(atoi(index.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetType(atoi(type.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetAttribute(atoi(atribute.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetItemSection(atoi(section.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetItemType(atoi(itemType.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetItemLevel(atoi(itemLevel.c_str()));

			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(0)->SetIndex(atoi(rankindex1.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(0)->SetLevel(atoi(rankdata1.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(1)->SetIndex(atoi(rankindex2.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(1)->SetLevel(atoi(rankdata2.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(2)->SetIndex(atoi(rankindex3.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(2)->SetLevel(atoi(rankdata3.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(3)->SetIndex(atoi(rankindex4.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(3)->SetLevel(atoi(rankdata4.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(4)->SetIndex(atoi(rankindex5.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].GetRankData(4)->SetLevel(atoi(rankdata5.c_str()));


			pPentagramJewelInfoStr[atoi(index.c_str())].SetSerialServer(atoi(serialServer.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetSerial(atoi(serial.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetDBFlag(atoi(dbflag.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetData(0, atoi(rawdata1.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetData(1, atoi(rawdata2.c_str()));
			pPentagramJewelInfoStr[atoi(index.c_str())].SetData(2, atoi(rawdata3.c_str()));
		}
	}

}