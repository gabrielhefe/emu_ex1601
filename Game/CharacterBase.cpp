/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "PlayerCharacterBase.cpp"
*
*/

CharacterBaseMgr::CharacterBaseMgr()
{

}

CharacterBaseMgr::~CharacterBaseMgr()
{
	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		this->char_skill_base[i].clear();

		CLEAR_LIST(this->m_character_stat_fruit[i]);
		CLEAR_LIST(this->m_character_item[i]);
		MAP_CLEAR(CharacterHelperMap::iterator, this->m_character_helper[i]);
		CLEAR_LIST(this->m_character_specialization[i]);
	}

	CLEAR_LIST(this->m_CharacterFormulaList);

	CLEAR_LIST(this->m_character_experience_adjust);
	CLEAR_LIST(this->m_character_experience_bonus);
	CLEAR_LIST(this->m_character_bonus_list);
	CLEAR_LIST(this->m_CharacterMaxLevelRewardList);
		
	for ( int32 i = 0; i < LEVEL_DATA_MAX; ++i )
	{
		MAP_CLEAR(CharacterExperienceMap::iterator, this->m_character_experience[i]);
	}

	CLEAR_LIST(this->m_CharacterExperienceTableList);
}

void CharacterBaseMgr::LoadCharacterBase(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Base...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	for (uint8 i = 0; i < Character::MAX_CLASS; ++i)
	{
		LIST_CLEAR(CharacterItemList::iterator, this->m_character_item[i]);
	}


	pugi::xml_node Main = file.child("CharacterBase");
	pugi::xml_node CharInfo = Main.child("CharacterBaseInfo");
	pugi::xml_node CharItem = Main.child("CharacterBaseItem");
	pugi::xml_node CharBuff = Main.child("CharacterBaseBuff");

	uint32 count = 0;

	for (pugi::xml_node info = CharInfo.child("Info"); info; info = info.next_sibling()) {
		this->char_base[info.attribute("Class").as_int()].SetStat(0, info.attribute("Strenght").as_int());
		this->char_base[info.attribute("Class").as_int()].SetStat(1, info.attribute("Agility").as_int());
		this->char_base[info.attribute("Class").as_int()].SetStat(2, info.attribute("Vitality").as_int());
		this->char_base[info.attribute("Class").as_int()].SetStat(3, info.attribute("Energy").as_int());
		this->char_base[info.attribute("Class").as_int()].SetStat(4, info.attribute("Leadership").as_int());
		this->char_base[info.attribute("Class").as_int()].SetLife(info.attribute("Life").as_int());
		this->char_base[info.attribute("Class").as_int()].SetMana(info.attribute("Mana").as_int());
		this->char_base[info.attribute("Class").as_int()].SetLevelToLife(info.attribute("LevelToLife").as_int());
		this->char_base[info.attribute("Class").as_int()].SetLevelToMana(info.attribute("LevelToMana").as_int());
		this->char_base[info.attribute("Class").as_int()].SetVitalityToLife(info.attribute("VitalityToLife").as_int());
		this->char_base[info.attribute("Class").as_int()].SetEnergyToMana(info.attribute("EnergyToMana").as_int());
		this->char_base[info.attribute("Class").as_int()].SetWorld(info.attribute("World").as_int());
		this->char_base[info.attribute("Class").as_int()].SetLevel(info.attribute("Level").as_int());
		this->char_base[info.attribute("Class").as_int()].SetPoints(info.attribute("Points").as_int());
			
		this->char_base[info.attribute("Class").as_int()].SetAutomaticPowerRecovery(0, info.attribute("AutoLifeRecovery").as_int());
		this->char_base[info.attribute("Class").as_int()].SetAutomaticPowerRecovery(1, info.attribute("AutoManaRecovery").as_int());
		this->char_base[info.attribute("Class").as_int()].SetAutomaticPowerRecovery(2, info.attribute("AutoShieldRecovery").as_int());
		this->char_base[info.attribute("Class").as_int()].SetAutomaticPowerRecovery(3, info.attribute("AutoStaminaRecovery").as_int());

		this->char_base[info.attribute("Class").as_int()].SetLevelUpPoints(info.attribute("NormalLevelUpPoints").as_int());
		this->char_base[info.attribute("Class").as_int()].SetPlusLevelUpPoints(info.attribute("PlusLevelUpPoints").as_int());
		this->char_base[info.attribute("Class").as_int()].SetMasterLevelUpPoints(info.attribute("MasterLevelUpPoints").as_int());
		this->char_base[info.attribute("Class").as_int()].SetMajesticLevelUpPoints(info.attribute("MajesticLevelUpPoints").as_int());

		this->char_base[info.attribute("Class").as_int()].SetZen(info.attribute("Zen").as_int());
		this->char_base[info.attribute("Class").as_int()].SetRuud(info.attribute("Ruud").as_int());
		this->char_base[info.attribute("Class").as_int()].SetExpandedInventory(info.attribute("ExpandedInventory").as_int());

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character base definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");

	sLog->outInfo(LOG_DEFAULT, "Loading Character Item...");

	count = 0;

	for (pugi::xml_node info = CharItem.child("Item"); info; info = info.next_sibling()) {
		if (info.attribute("Class").as_int() < Character::MAX_CLASS)
		{
			character_item* add_item = new character_item;

			add_item->SetSlot(info.attribute("Slot").as_int());
			add_item->SetType(info.attribute("ItemCat").as_int());
			add_item->SetIndex(info.attribute("ItemIndex").as_int());

			if (!sItemMgr->IsItem(add_item->GetItem()))
			{
				sLog->outError(LOG_DEFAULT, "LoadCharacterItem() Race: %u, slot: %d, item: %d %d",
					info.attribute("Class").as_int(), add_item->GetSlot(), add_item->GetType(), add_item->GetIndex());

				delete add_item;
				continue;
			}

			add_item->SetLevel(info.attribute("ItemLevel").as_int());
			add_item->SetDurability(info.attribute("ItemDur").as_int());
			add_item->SetSkill(info.attribute("ItemSkill").as_int());
			add_item->SetLuck(info.attribute("ItemLuck").as_int());
			add_item->SetOption(info.attribute("ItemOption").as_int());
			add_item->SetExcellent(info.attribute("ItemExl").as_int());
			add_item->SetAncient(info.attribute("ItemAncient").as_int());
			add_item->SetDuration(info.attribute("Duration").as_int());

			this->m_character_item[info.attribute("Class").as_int()].push_back(add_item);
		}

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character item definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");

	sLog->outInfo(LOG_DEFAULT, "Loading Character Base Buff...");

	count = 0;

	for (pugi::xml_node buff = CharBuff.child("Buff"); buff; buff = buff.next_sibling()) {
		uint8 race = buff.attribute("Class").as_int();
		if (race == 255) {
			for (int i = 0; i < Character::MAX_CLASS; i++) {
				CharacterBaseBuff* add_buff = new CharacterBaseBuff;

				add_buff->SetBuffID(buff.attribute("BuffID").as_int());
				add_buff->SetFlags(buff.attribute("Flags").as_int());
				add_buff->SetDuration(buff.attribute("Duration").as_int());
				count++;
				this->m_character_buff[i].push_back(add_buff);
			}
		}
		else if(race >= Character::MAX_CLASS){
			continue;
		}
		else {
			CharacterBaseBuff* add_buff = new CharacterBaseBuff;

			add_buff->SetBuffID(buff.attribute("BuffID").as_int());
			add_buff->SetFlags(buff.attribute("Flags").as_int());
			add_buff->SetDuration(buff.attribute("Duration").as_int());
			count++;
			this->m_character_buff[race].push_back(add_buff);
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character base buff definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

CharacterBaseData * CharacterBaseMgr::GetCharacterBase(uint8 character_class)
{
	if ( character_class >= Character::MAX_CLASS )
		return nullptr;

	return &this->char_base[character_class];
}

void CharacterBaseMgr::LoadCharacterBaseSkill(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Skill...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		this->char_skill_base[i].clear();
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterSkill");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Option"); option; option = option.next_sibling()) {
		uint8 race = option.attribute("Class").as_int();

		if (race >= Character::MAX_CLASS)
			continue;

		uint16 skill = option.attribute("Skill").as_int();

		if (!sSkillMgr->IsSkill(skill))
			continue;

		this->char_skill_base[race].push_back(skill);

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character skill definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceNormal(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Normal...");

	MAP_CLEAR(CharacterExperienceMap::iterator, this->m_character_experience[LEVEL_DATA_NORMAL]);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("NormalExp");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		CharacterExperience* add_experience = new CharacterExperience;
		add_experience->SetLevel(data.attribute("Level").as_int());
		add_experience->SetExperience(data.attribute("Exp").as_int());
		add_experience->SetAccumulatedExperience(data.attribute("AccumulatedExp").as_int());

		this->m_character_experience[LEVEL_DATA_NORMAL][add_experience->GetLevel()] = add_experience;

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience normal definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceMaster(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Master...");

	MAP_CLEAR(CharacterExperienceMap::iterator, this->m_character_experience[LEVEL_DATA_MASTER]);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MasterExp");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		CharacterExperience* add_experience = new CharacterExperience;
		add_experience->SetLevel(data.attribute("Level").as_int());
		add_experience->SetExperience(data.attribute("Exp").as_int());
		add_experience->SetAccumulatedExperience(data.attribute("AccumulatedExp").as_int());

		this->m_character_experience[LEVEL_DATA_MASTER][add_experience->GetLevel()] = add_experience;

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience master definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceMajestic(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Majestic...");

	MAP_CLEAR(CharacterExperienceMap::iterator, this->m_character_experience[LEVEL_DATA_MAJESTIC]);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MajesticExp");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		CharacterExperience* add_experience = new CharacterExperience;
		add_experience->SetLevel(data.attribute("Level").as_int());
		add_experience->SetExperience(data.attribute("Exp").as_int());
		add_experience->SetAccumulatedExperience(data.attribute("AccumulatedExp").as_int());

		this->m_character_experience[LEVEL_DATA_MAJESTIC][add_experience->GetLevel()] = add_experience;

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience majestic definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

int64 CharacterBaseMgr::GetCharacterExperience(uint8 type, int16 level, bool accumulated) const
{
	int16 level_tmp = level > sGameServer->GetMaxLevel(type) ? sGameServer->GetMaxLevel(type) : level < 0 ? 0 : level;

	CharacterExperienceMap::const_iterator it = this->m_character_experience[type].find(level_tmp);

	if ( it != this->m_character_experience[type].end() )
	{
		if ( accumulated )
		{
			return it->second->GetAccumulatedExperience();
		}
		else
		{
			return it->second->GetExperience();
		}
	}
	else
	{
		return 0;
	}
}

void CharacterBaseMgr::FixCharacterExperience(Player* pPlayer)
{
	uint8 level_type = LEVEL_DATA_NORMAL;

	if ( pPlayer->IsMaster() )
	{
		level_type = LEVEL_DATA_MASTER;
	}

	if ( pPlayer->IsMajestic() )
	{
		level_type = LEVEL_DATA_MAJESTIC;
	}

	LevelData* pLevelData = pPlayer->GetLevelData(level_type);

	//int64 experience_min = sCharacterBase->GetCharacterExperience(level_type, pLevelData->GetLevel());
	int64 experience_min = 0;
	int64 experience_max = sCharacterBase->GetCharacterExperience(level_type, pLevelData->GetLevel() + 1);
	
	if ( pLevelData->GetLevel() >= sGameServer->GetMaxLevel(level_type) )
	{
		pLevelData->SetExperience(experience_max);
	}

	if ( pLevelData->GetExperience() > experience_max )
	{
		pLevelData->SetExperience(experience_max);
	}

	if ( pLevelData->GetExperience() < experience_min )
	{
		pLevelData->SetExperience(experience_min);
	}
}

void CharacterBaseMgr::LoadCharacterHelper(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Helper...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		MAP_CLEAR(CharacterHelperMap::iterator, this->m_character_helper[i]);
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterHelper");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Option"); option; option = option.next_sibling()) {
		uint8 race = option.attribute("Class").as_int();

		if (race < Character::MAX_CLASS)
		{
			character_helper* add_helper = new character_helper;

			add_helper->SetStage(option.attribute("Stage").as_int());
			add_helper->SetDuration(option.attribute("Duration").as_int() * MINUTE * IN_MILLISECONDS);
			add_helper->SetCost(option.attribute("Cost").as_int());
			add_helper->SetCostInterval(option.attribute("CostInterval").as_int() * MINUTE * IN_MILLISECONDS);
			add_helper->SetNextStage(option.attribute("NextStage").as_int());

			this->m_character_helper[race][add_helper->GetStage()] = add_helper;
		}

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character helper stages", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterFormula(char* pchFileName)
{
	sLog->outLoad(true, "Loading Character Formula...");

	CLEAR_LIST(this->m_CharacterFormulaList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterFormula");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Option"); option; option = option.next_sibling()) {
		character_formula* add_formula = new character_formula;

		add_formula->SetType(option.attribute("StatID").as_int());
		add_formula->SetClass(option.attribute("Class").as_int());
		add_formula->SetLevelMul(option.attribute("LevelMul").as_int());
		add_formula->SetLevelDiv(option.attribute("LevelDiv").as_int());

		if (add_formula->GetLevelDiv() == 0)
		{
			add_formula->SetLevelDiv(1);
		}

		add_formula->SetStatMul(0, option.attribute("StrengthMul").as_int());
		add_formula->SetStatDiv(0, option.attribute("StrengthDiv").as_int());
		add_formula->SetStatMul(1, option.attribute("AgilityMul").as_int());
		add_formula->SetStatDiv(1, option.attribute("AgilityDiv").as_int());
		add_formula->SetStatMul(2, option.attribute("VitalityMul").as_int());
		add_formula->SetStatDiv(2, option.attribute("VitalityDiv").as_int());
		add_formula->SetStatMul(3, option.attribute("EnergyMul").as_int());
		add_formula->SetStatDiv(3, option.attribute("EnergyDiv").as_int());
		add_formula->SetStatMul(4, option.attribute("LeaderShipMul").as_int());
		add_formula->SetStatDiv(4, option.attribute("LeaderShipDiv").as_int());

		for (int64 i = 0; i < MAX_STAT_TYPE; ++i)
		{
			if (add_formula->GetStatDiv(i) == 0)
			{
				add_formula->SetStatDiv(i, 1);
			}
		}

		this->m_CharacterFormulaList.push_back(add_formula);
		count++;
	}
	
	sLog->outLoad(false, ">> Loaded %u character formula definitions", count);
}

void CharacterBaseMgr::LoadCharacterStatFruit(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Stat Fruit...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		LIST_CLEAR(CharacterStatFruitList::iterator, this->m_character_stat_fruit[i]);
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterStatFruit");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Option"); option; option = option.next_sibling()) {
		uint8 race = option.attribute("Class").as_int();

		if (race < Character::MAX_CLASS)
		{
			character_stat_fruit* add_stat_fruit = new character_stat_fruit;

			add_stat_fruit->SetMinLevel(option.attribute("MinLevel").as_int());
			add_stat_fruit->SetMaxLevel(option.attribute("MaxLevel").as_int());
			add_stat_fruit->SetTotalAddPoints(option.attribute("TotalAddPoints").as_int());
			add_stat_fruit->SetTotalMinusPoints(option.attribute("TotalMinusPoints").as_int());

			this->m_character_stat_fruit[race].push_back(add_stat_fruit);
		}

		count++;
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character stat fruit definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterSpecialization(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Specialization...");

	for ( uint8 i = 0; i < Character::MAX_CLASS; ++i )
	{
		LIST_CLEAR(CharacterSpecializationList::iterator, this->m_character_specialization[i]);
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterSpecialization");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Option"); option; option = option.next_sibling()) {
		uint8 race = option.attribute("Class").as_int();

		if (race < Character::MAX_CLASS)
		{
			character_specialization* add_data = new character_specialization;

			add_data->SetRace(race);
			add_data->SetStat(option.attribute("Stat").as_int());
			add_data->SetRequiredStat(option.attribute("ReqStat").as_int());
			add_data->SetRequiredStatMin(option.attribute("ReqStatMin").as_int());
			add_data->SetRequiredStatMax(option.attribute("ReqStatMax").as_int());
			add_data->SetPercentMin(option.attribute("PercentMin").as_int());
			add_data->SetPercentMax(option.attribute("PercentMax").as_int());
			add_data->SetRequiredStatLimit(option.attribute("ReqStatLimit").as_int());

			this->m_character_specialization[race].push_back(add_data);
		}

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character specialization definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceAdjust(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Adjust...");

	LIST_CLEAR(CharacterExperienceAdjust::iterator, this->m_character_experience_adjust);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterExpAdjust");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		character_experience_adjust* add_data = new character_experience_adjust;

		add_data->SetLevelMin(data.attribute("MinLevel").as_int());
		add_data->SetLevelMax(data.attribute("MaxLevel").as_int());
		add_data->SetExperienceRate(data.attribute("ExpRate").as_int());

		this->m_character_experience_adjust.push_back(add_data);
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience adjust definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceBonus(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Bonus...");

	LIST_CLEAR(CharacterExperienceBonus::iterator, this->m_character_experience_bonus);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("BonusExp");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Option"); option; option = option.next_sibling()) {
		character_experience_bonus* add_data = new character_experience_bonus;
		add_data->SetRace(0, option.attribute("Race1").as_int());
		add_data->SetRace(1, option.attribute("Race2").as_int());
		add_data->SetRace(2, option.attribute("Race3").as_int());

		if (add_data->GetRace(0) >= Character::MAX_CLASS ||
			add_data->GetRace(1) >= Character::MAX_CLASS ||
			add_data->GetRace(2) >= Character::MAX_CLASS)
		{
			delete add_data;
			continue;
		}

		this->m_character_experience_bonus.push_back(add_data);

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience bonus definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterBonus(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Bonus...");

	LIST_CLEAR(CharacterBonusList::iterator, this->m_character_bonus_list);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterBonus");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		character_bonus* add_data = new character_bonus;

		add_data->SetLevelMin(data.attribute("MinLevel").as_int());
		add_data->SetLevelMax(data.attribute("MaxLevel").as_int());
		add_data->SetExperienceRate(data.attribute("ExpRate").as_int());
		add_data->SetDropRate(data.attribute("DropRate").as_int());

		this->m_character_bonus_list.push_back(add_data);

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character bonus definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterMaxLevelReward(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Max Level Reward...");

	LIST_CLEAR(CharacterMaxLevelRewardList::iterator, this->m_CharacterMaxLevelRewardList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterMaxLevelReward");

	uint32 count = 0;

	for (pugi::xml_node reward = Main.child("Reward"); reward; reward = reward.next_sibling()) {
		CharacterMaxLevelReward* add_data = new CharacterMaxLevelReward;

		add_data->SetClass(reward.attribute("Class").as_int());
		add_data->SetMaxLevel(reward.attribute("MaxLevel").as_int());
		add_data->SetType(reward.attribute("ItemCat").as_int());
		add_data->SetIndex(reward.attribute("ItemIndex").as_int());
		add_data->SetLevel(reward.attribute("ItemLevel").as_int());
		add_data->SetDurability(reward.attribute("Dur").as_int());
		add_data->SetSkill(reward.attribute("Skill").as_int());
		add_data->SetLuck(reward.attribute("Luck").as_int());
		add_data->SetOption(reward.attribute("Option").as_int());
		add_data->SetExcellent(reward.attribute("Exl").as_int());
		add_data->SetAncient(reward.attribute("Ancient").as_int());
		add_data->SetHarmony(reward.attribute("Harmony").as_int());
		add_data->SetOption380(reward.attribute("Option380").as_int());

		add_data->SetSocket(0, reward.attribute("Socket1").as_int());
		add_data->SetSocket(1, reward.attribute("Socket2").as_int());
		add_data->SetSocket(2, reward.attribute("Socket3").as_int());
		add_data->SetSocket(3, reward.attribute("Socket4").as_int());
		add_data->SetSocket(4, reward.attribute("Socket5").as_int());

		add_data->SetSocketBonus(reward.attribute("SocketBonus").as_int());
		add_data->SetFlags(reward.attribute("Flags").as_int());

		this->m_CharacterMaxLevelRewardList.push_back(add_data);

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character max level reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CharacterBaseMgr::LoadCharacterExperienceTable(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Character Experience Table...");

	LIST_CLEAR(CharacterExperienceTableList::iterator, this->m_CharacterExperienceTableList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CharacterExpAdjust");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		CharacterExperienceTable* add_data = new CharacterExperienceTable;

		add_data->SetLevelMin(data.attribute("MinLevel").as_int());
		add_data->SetLevelMax(data.attribute("MaxLevel").as_int());
		add_data->SetExperienceRate(data.attribute("ExpRate").as_int());

		this->m_CharacterExperienceTableList.push_back(add_data);

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u character experience table definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

character_helper * CharacterBaseMgr::GetHelperStage(uint8 race, int32 stage) const
{
	if ( race >= Character::MAX_CLASS )
		return nullptr;

	CharacterHelperMap::const_iterator it = this->m_character_helper[race].find(stage);

	if ( it != this->m_character_helper[race].end() )
	{
		return it->second;
	}

	return nullptr;
}

uint16 CharacterBaseMgr::GetStatFruitMaxAdd(uint8 race, int16 level)
{
	if ( race >= Character::MAX_CLASS )
		return 0;

	uint16 stat_fruit_max = 0;

	for ( CharacterStatFruitList::iterator it = this->m_character_stat_fruit[race].begin(); it != this->m_character_stat_fruit[race].end(); ++it )
	{
		if ( !(*it) )
			continue;

		if ( level < (*it)->GetMinLevel() || level > (*it)->GetMaxLevel() )
			continue;

		stat_fruit_max = (*it)->GetTotalAddPoints();
	}

	return stat_fruit_max;
}
	
uint16 CharacterBaseMgr::GetStatFruitMaxDec(uint8 race, int16 level)
{
	if ( race >= Character::MAX_CLASS )
		return 0;

	uint16 stat_fruit_max = 0;

	for ( CharacterStatFruitList::const_iterator it = this->m_character_stat_fruit[race].begin(); it != this->m_character_stat_fruit[race].end(); ++it )
	{
		if ( !(*it) )
			continue;

		if ( level < (*it)->GetMinLevel() || level > (*it)->GetMaxLevel() )
			continue;

		stat_fruit_max = (*it)->GetTotalMinusPoints();
	}

	return stat_fruit_max;
}

void CharacterBaseMgr::GenerateCharacterItemList(Player* pPlayer)
{
	if ( pPlayer->GetClass() >= Character::MAX_CLASS )
	{
		return;
	}

	CharacterItemList const& item_list = this->m_character_item[pPlayer->GetClass()];

	for ( CharacterItemList::const_iterator it = item_list.begin(); it != item_list.end(); ++it )
	{
		Item item((*it)->GetItem(), (*it)->GetLevel(), (*it)->GetDurability(), (*it)->GetSkill(), (*it)->GetLuck(), (*it)->GetOption(), (*it)->GetExcellent(), (*it)->GetAncient());

		if ( (*it)->GetDuration() > 0 )
		{
			item.SetExpireDate(time(nullptr) + (*it)->GetDuration());
		}

		sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, item, (*it)->GetSlot());
	}
}

void CharacterBaseMgr::GenerateCharacterBuffList(Player* pPlayer)
{
	if (!pPlayer)
		return;

	if (pPlayer->GetClass() >= Character::MAX_CLASS) {
		return;
	}

	CharacterBuff const& bufflist = this->m_character_buff[pPlayer->GetClass()];

	for (CharacterBuff::const_iterator it = bufflist.begin(); it != bufflist.end(); ++it) {
		buff_template const* buff_add = sSkillMgr->GetBuff((*it)->GetBuffID());
		
		pPlayer->AddBuff(buff_add->GetBuff(), BuffEffect(buff_add->GetEffect(0), buff_add->GetValue(0)), BuffEffect(buff_add->GetEffect(1), buff_add->GetValue(1)), (*it)->GetDuration(), (*it)->GetFlags(), pPlayer);
	}
}

void CharacterBaseMgr::GenerateCharacterMoneyAdd(Player* pPlayer)
{
	if (!pPlayer)
		return;

	if (pPlayer->GetClass() >= Character::MAX_CLASS) {
		return;
	}

	CharacterBaseData const* base_data = this->GetCharacterBase(pPlayer->GetClass());

	if (!base_data)
		return;

	pPlayer->MoneyAdd(base_data->GetZen());
	pPlayer->IncreaseRuudMoney(base_data->GetRuud());
	pPlayer->GetInventory()->IncreaseExpanded(base_data->GetExpandedInventory());
}

void CharacterBaseMgr::CalculateStatFormula(Player* pPlayer)
{
	for (auto const& pData : this->m_CharacterFormulaList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() != pPlayer->GetClass())
		{
			continue;
		}


		int32 level = pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel();

		if (pData->GetType() == STAT_ELEMENTAL_ATTACK_SUCCESS_RATE || pData->GetType() == STAT_ELEMENTAL_DEFENSE_SUCCESS_RATE)
		{
			if (pPlayer->IsMaster()) {
				level += pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetLevel();
			}

			if (pPlayer->IsMajestic()) {
				level += pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel();
			}
			//level = pPlayer->GetTotalLevel();
		}

		float value = (level * pData->GetLevelMul()) / pData->GetLevelDiv();

		for (int64 n = 0; n < MAX_STAT_TYPE; ++n)
		{
			value += (pPlayer->GetTotalStat(n) * pData->GetStatMul(n)) / pData->GetStatDiv(n);
		}

		if (value < 0)
		{
			value = 0;
		}

		pPlayer->SetBaseStat(pData->GetType(), value);
	}

	if (pPlayer->GetBaseStat(STAT_ATTACK_RATE) > 0xFFFF)
	{
		pPlayer->SetBaseStat(STAT_ATTACK_RATE, 0xFFFF);
	}

	if (pPlayer->GetClass() == Character::FAIRY_ELF)
	{
		auto pWeapon01 = pPlayer->GetInventory()->GetItem(WEAPON_01);
		auto pWeapon02 = pPlayer->GetInventory()->GetItem(WEAPON_02);

		if (pWeapon01 && pWeapon01->IsValid() && (pWeapon01->GetKind2() == ItemKind::CROSSBOW || pWeapon01->GetKind2() == ItemKind::CROSSBOW))
		{
			pPlayer->SetBaseStat(STAT_ATTACK_DAMAGE_MIN, pPlayer->GetTotalStat(STRENGTH) / 14 + pPlayer->GetTotalStat(AGILITY) / 7);
			pPlayer->SetBaseStat(STAT_ATTACK_DAMAGE_MAX, (pPlayer->GetTotalStat(STRENGTH) >> 3) + (pPlayer->GetTotalStat(AGILITY) >> 2));
		}
	}

	if (pPlayer->GetBuffByGroup(325))
	{
		pPlayer->SetBaseStat(STAT_ATTACK_DAMAGE_MIN, pPlayer->GetTotalStat(STRENGTH) / 9);
		pPlayer->SetBaseStat(STAT_ATTACK_DAMAGE_MAX, pPlayer->GetTotalStat(STRENGTH) / 6);
		pPlayer->SetBaseStat(STAT_DEFENSE, pPlayer->GetTotalStat(AGILITY) / 2);
	}

	pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MAX));
	pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MAX));

	if (pPlayer->GetClass() == Character::FAIRY_ELF)
	{
		auto pWeapon01 = pPlayer->GetInventory()->GetItem(WEAPON_01);
		auto pWeapon02 = pPlayer->GetInventory()->GetItem(WEAPON_02);

		if (pWeapon01 && pWeapon01->IsValid() && (pWeapon01->GetKind2() == ItemKind::CROSSBOW || pWeapon01->GetKind2() == ItemKind::CROSSBOW))
		{
			if (pWeapon02 && pWeapon02->IsValid() && pWeapon02->GetKind2() == ItemKind::QUIVER)
			{
				pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, 0);
				pPlayer->SetIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, 0);
			}
		}
	}

	pPlayer->SetIntData(UNIT_INT_ATTACK_RATE, pPlayer->GetBaseStat(STAT_ATTACK_RATE));
	pPlayer->SetIntData(UNIT_INT_ATTACK_RATE_PVP, pPlayer->GetBaseStat(STAT_ATTACK_RATE_PVP));
	pPlayer->SetIntData(UNIT_INT_ATTACK_SPEED, pPlayer->GetBaseStat(STAT_ATTACK_SPEED));
	pPlayer->SetIntData(UNIT_INT_MAGIC_SPEED, pPlayer->GetBaseStat(STAT_MAGIC_SPEED));
	pPlayer->SetIntData(UNIT_INT_DEFENSE, pPlayer->GetBaseStat(STAT_DEFENSE));
	pPlayer->SetIntData(UNIT_INT_DEFENSE_RATE, pPlayer->GetBaseStat(STAT_DEFENSE_RATE));
	pPlayer->SetIntData(UNIT_INT_DEFENSE_RATE_PVP, pPlayer->GetBaseStat(STAT_DEFENSE_RATE_PVP));
	pPlayer->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MAX));
	pPlayer->SetIntData(UNIT_INT_CURSE_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_CURSE_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MAX));

	if (pPlayer->HasBuff(301) || pPlayer->HasBuff(302) || pPlayer->HasBuff(303))
	{
		pPlayer->SetIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MIN));
		pPlayer->SetIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MAX));
		pPlayer->SetIntData(UNIT_INT_CURSE_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MIN));
		pPlayer->SetIntData(UNIT_INT_CURSE_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MAX));
	}

	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, pPlayer->GetBaseStat(STAT_ELEMENTAL_DEFENSE));
	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, pPlayer->GetBaseStat(STAT_ELEMENTAL_DAMAGE_MIN));
	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, pPlayer->GetBaseStat(STAT_ELEMENTAL_DAMAGE_MAX));
	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, pPlayer->GetBaseStat(STAT_ELEMENTAL_ATTACK_SUCCESS_RATE));
	pPlayer->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, pPlayer->GetBaseStat(STAT_ELEMENTAL_DEFENSE_SUCCESS_RATE));

	pPlayer->SetData(PLAYER_DATA_FENRIR_DAMAGE, pPlayer->GetBaseStat(STAT_FENRIR_DAMAGE));

	pPlayer->SetIntData(UNIT_INT_COMBO_DAMAGE, pPlayer->GetBaseStat(STAT_COMBO_DAMAGE));
}

void CharacterBaseMgr::CalculateSpecialization(Player* pPlayer)
{
	for ( int32 i = 0; i < CHARACTER_SPECIALIZATION_MAX; ++i )
	{
		this->CalculateSpecialization(pPlayer, i);
	}
}
	
void CharacterBaseMgr::CalculateSpecialization(Player* pPlayer, uint8 stat)
{
	if ( pPlayer->GetClass() >= Character::MAX_CLASS )
	{
		return;
	}

	CharacterSpecializationList const& specialization_list = this->m_character_specialization[pPlayer->GetClass()];

	for ( CharacterSpecializationList::const_iterator it = specialization_list.begin(); it != specialization_list.end(); ++it )
	{
		if ( (*it)->GetStat() == stat && (*it)->GetRequiredStat() < MAX_STAT_TYPE )
		{
			if ( pPlayer->GetTotalStat((*it)->GetRequiredStat()) >= (*it)->GetRequiredStatMin() && (pPlayer->GetTotalStat((*it)->GetRequiredStat()) <= (*it)->GetRequiredStatMax() || (*it)->GetRequiredStatMax() == -1) )
			{
				float percent = (*it)->GetPercentMax() - (*it)->GetPercentMin();
				int32 stat_min = (*it)->GetRequiredStatMin() - 1;
				int32 stat_max = (*it)->GetRequiredStatMax();

				if ( stat_max == -1 || stat_max > (*it)->GetRequiredStatLimit() )
				{
					stat_max = (*it)->GetRequiredStatLimit();
				}

				int32 stat_value = pPlayer->GetTotalStat((*it)->GetRequiredStat());

				if ( stat_value > (*it)->GetRequiredStatLimit() )
				{
					stat_value = (*it)->GetRequiredStatLimit();
				}

				stat_value -= stat_min;

				float final_percent = (float)(stat_max - stat_min) / percent;
				
				this->ApplySpecialization(pPlayer, stat, (float)(stat_value / final_percent) + (*it)->GetPercentMin());
				return;
			}
		}
	}
}

void CharacterBaseMgr::ApplySpecialization(Player* pPlayer, uint8 stat, float percent)
{
	switch ( stat )
	{
	case CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MIN:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MIN) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MIN, pPlayer->GetSpecializationStat(stat));
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MIN, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_ATTACK_DAMAGE_MAX:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_ATTACK_DAMAGE_MAX) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_RIGHT_MAX, pPlayer->GetSpecializationStat(stat));
			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_DAMAGE_LEFT_MAX, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_ATTACK_RATE) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_RATE, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_ATTACK_SUCCESS_RATE_PVP:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_ATTACK_RATE_PVP) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_ATTACK_RATE_PVP, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_DEFENSE:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_DEFENSE) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_DEFENSE, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_DEFENSE_RATE) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_DEFENSE_RATE, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_DEFENSE_SUCCESS_RATE_PVP:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_DEFENSE_RATE_PVP) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_DEFENSE_RATE_PVP, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MIN:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MIN) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MIN, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_MAGIC_DAMAGE_MAX:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_MAGIC_DAMAGE_MAX) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_MAGIC_DAMAGE_MAX, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MIN:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MIN) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MIN, pPlayer->GetSpecializationStat(stat));
		} break;

	case CHARACTER_SPECIALIZATION_CURSE_DAMAGE_MAX:
		{
			pPlayer->SetSpecializationStat(stat, pPlayer->GetBaseStat(STAT_CURSE_DAMAGE_MAX) * percent / 100);

			pPlayer->IncreaseIntData(UNIT_INT_CURSE_DAMAGE_MAX, pPlayer->GetSpecializationStat(stat));
		} break;
	}
}

void CharacterBaseMgr::ApplyExperienceAdjust(Player* pPlayer, int64 & experience)
{
	if ( !pPlayer )
	{
		return;
	}

	for ( CharacterExperienceAdjust::const_iterator it = this->m_character_experience_adjust.begin(); it != m_character_experience_adjust.end(); ++it )
	{
		if ( pPlayer->GetTotalLevel() >= (*it)->GetLevelMin() && pPlayer->GetTotalLevel() <= (*it)->GetLevelMax() )
		{
			experience = (experience * (*it)->GetExperienceRate()) / 100;
			break;
		}
	}
}

bool CharacterBaseMgr::IsExperienceBonus(bool const* members) const
{
	if ( !members )
	{
		return false;
	}

	for ( CharacterExperienceBonus::const_iterator it = this->m_character_experience_bonus.begin(); it != m_character_experience_bonus.end(); ++it )
	{
		if ( members[(*it)->GetRace(0)] &&
			 members[(*it)->GetRace(1)] &&
			 members[(*it)->GetRace(2)] )
		{
			return true;
		}
	}

	return false;
}

int32 CharacterBaseMgr::GetBonus(Player* pPlayer, bool drop)
{
	if ( !pPlayer )
	{
		return 0;
	}

	int32 value = 0;

	for ( CharacterBonusList::const_iterator it = this->m_character_bonus_list.begin(); it != this->m_character_bonus_list.end(); ++it )
	{
		if ( pPlayer->GetTotalLevel() >= (*it)->GetLevelMin() && pPlayer->GetTotalLevel() <= (*it)->GetLevelMax() )
		{
			if ( drop )
			{
				value = (*it)->GetDropRate();
			}
			else
			{
				value = (*it)->GetExperienceRate();
			}
			break;
		}
	}

	return value;
}

void CharacterBaseMgr::LevelUpReward(Player* pPlayer, int16 level)
{
	if (!pPlayer)
	{
		return;
	}

	for (CharacterMaxLevelRewardList::const_iterator itr = this->m_CharacterMaxLevelRewardList.begin(); itr != this->m_CharacterMaxLevelRewardList.end(); ++itr)
	{
		CharacterMaxLevelReward const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() != pPlayer->GetClass())
		{
			continue;
		}

		if (pPlayer->GetTotalLevel() < pData->GetMaxLevel())
		{
			continue;
		}

		if (level >= pData->GetMaxLevel())
		{
			continue;
		}

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ITEM_RECOVERY_INSERT);
		stmt->setUInt32(pPlayer->GetGUID());
		stmt->setUInt16(ITEMGET(pData->GetType(), pData->GetIndex()));
		stmt->setUInt8(pData->GetLevel());
		stmt->setUInt8(pData->GetDurability());
		stmt->setUInt8(pData->GetSkill());
		stmt->setUInt8(pData->GetLuck());
		stmt->setUInt8(pData->GetOption());
		stmt->setUInt8(pData->GetExcellent());
		stmt->setUInt8(pData->GetAncient());
		stmt->setUInt8(pData->GetHarmony());
		stmt->setUInt8(pData->GetOption380());
		stmt->setUInt16(pData->GetSocket(0));
		stmt->setUInt16(pData->GetSocket(1));
		stmt->setUInt16(pData->GetSocket(2));
		stmt->setUInt16(pData->GetSocket(3));
		stmt->setUInt16(pData->GetSocket(4));
		stmt->setUInt8(pData->GetSocketBonus());
		stmt->setUInt32(pData->GetFlags());

		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}
}

CharacterExperienceTable const* CharacterBaseMgr::GetCharacterExperienceTable(Player* pPlayer) const
{
	if (!pPlayer)
	{
		return nullptr;
	}

	for (CharacterExperienceTableList::const_iterator itr = this->m_CharacterExperienceTableList.begin(); itr != this->m_CharacterExperienceTableList.end(); ++itr)
	{
		CharacterExperienceTable const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pPlayer->GetTotalLevel() >= pData->GetLevelMin() && pPlayer->GetTotalLevel() <= pData->GetLevelMax())
		{
			return pData;
		}
	}

	return nullptr;
}

void CharacterBaseMgr::LoadCalcCharacterPvPDame() {
	if (boost::filesystem::exists(SERVER_CALCCHARACTER_FILEPATH)) {
		CalcCharacterPVP[0][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[0][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DWToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[1][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[1][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DKToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[2][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[2][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "ELFToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[3][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[3][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "MGToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[4][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[4][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "DLToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[5][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[5][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SUToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[6][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[6][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RFToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[7][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[7][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GLToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[8][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[8][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "RWToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[9][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[9][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "SLAToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);

		CalcCharacterPVP[10][0] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToDW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][1] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToDK_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][2] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToELF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][3] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToMG_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][4] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToDL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][5] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToSU_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][6] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToRF_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][7] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToGL_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][8] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToRW_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][9] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToSLA_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
		CalcCharacterPVP[10][10] = GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_PVPDAMAGE, "GCToGC_PvPDamageRate", 100, SERVER_CALCCHARACTER_FILEPATH);
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load CalcCharacter.ini File");
		MessageBox(NULL, "Error Load CalcCharacter.ini File", "Error", ERROR);
		exit(0);
	}
}

int CharacterBaseMgr::GetCalcPVPDamage(uint8 pPlayerCaster, uint8 pPlayerTarget, uint64 damage) {
	return damage = damage * CalcCharacterPVP[pPlayerCaster][pPlayerTarget] / 100;
}