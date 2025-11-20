/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterMgr.cpp"
*
*/

MonsterManager::MonsterManager()
{
	this->SetLastUpdate(0);
}

MonsterManager::~MonsterManager()
{
	this->ClearItem();

	_monsterTemplate.clear();

	MAP_CLEAR(MonsterAIUnitMap::iterator, this->m_monster_ai_unit);
	MAP_CLEAR(MonsterAIElementMap::iterator, this->m_monster_ai_element);
	MAP_CLEAR(MonsterAIAutomataMap::iterator, this->m_monster_ai_automata);

	for ( MonsterSkillMap::iterator it = this->monster_skill_map.begin(); it != this->monster_skill_map.end(); ++it )
		it->second.clear();

	this->monster_skill_map.clear();

	for ( MonsterSkillSpecialMap::iterator it = this->monster_skill_special_map.begin(); it != this->monster_skill_special_map.end(); ++it )
	{
		LIST_CLEAR(MonsterSkillSpecialList::iterator, it->second);
	}

	MAP_CLEAR(MonsterMap::iterator, this->monster_map);

	for ( MonsterAIGroupMap::iterator it = this->monster_ai_group_map.begin(); it != this->monster_ai_group_map.end(); it++ )
	{
		LIST_CLEAR(MonsterAIGroupList::iterator, it->second);
	}

	for ( MonsterRespawnLocationMap::iterator i_list = this->m_monster_respawn_location_map.begin(); i_list != this->m_monster_respawn_location_map.end(); ++i_list )
	{
		LIST_CLEAR(MonsterRespawnLocationList::iterator, i_list->second);
	}

	this->m_monster_respawn_location_map.clear();

	for ( MonsterEquipmentMap::iterator i_list = this->m_monster_equipment_map.begin(); i_list != this->m_monster_equipment_map.end(); ++i_list )
	{
		LIST_CLEAR(MonsterEquipmentList::iterator, i_list->second);
	}

	this->m_monster_equipment_map.clear();

	
	this->monster_skill_special_map.clear();
	this->monster_ai_group_map.clear();

	CLEAR_MAP(_monsterEvent);
	MAP_CLEAR(MonsterRespawnMap::iterator, this->monster_respawn_map);

	LIST_CLEAR(MonsterExclusiveList::iterator, this->monster_exclusive_list);
}

/*void MonsterManager::LoadNpcHandleList(char* pchFileName)
{
	LIST_CLEAR(NPCHandleData::iterator, this->m_npc_handle);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("NPCHandle");

	for (pugi::xml_node handle = Main.child("Handle"); handle; handle = handle.next_sibling()) {
		NPCHandleList* info = new NPCHandleList;
		info->SetNpcID(handle.attribute("NpcIndex").as_int());
		info->SetScriptName(handle.attribute("ScriptName").as_string());
		m_npc_handle.push_back(info);
	}
}*/

void MonsterManager::LoadMonsterTemplate(char* pchFileName)
{
	sLog->outLoad(true, "Loading Monster Template...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterList");

	_monsterTemplate.rehash((size_t)Main.select_nodes("Monster").size());

	for (pugi::xml_node list = Main.child("Monster"); list; list = list.next_sibling()) {
		auto id = list.attribute("ID").as_int();
		auto & data = _monsterTemplate[id];

		data.Id = id;
		data.Name = list.attribute("Name").as_string();
		data.Model = list.attribute("Model").as_int();
		data.Size = list.attribute("Size").as_int();
		data.Type = list.attribute("Type").as_int();
		data.Level = list.attribute("Level").as_int();

		data.Stat[0] = list.attribute("Life").as_int();
		data.Stat[1] = list.attribute("Mana").as_int();
		data.Stat[2] = list.attribute("Shield").as_int();
		data.Stat[3] = list.attribute("Stamina").as_int();

		data.DamageMin = list.attribute("DamageMin").as_int();
		data.DamageMax = list.attribute("DamageMax").as_int();
		data.CriticalDamageRate = list.attribute("CritDamageRate").as_int();
		data.CriticalDamageAdd = list.attribute("CritDamageAdd").as_int();
		data.ExcellentDamageRate = list.attribute("ExlDamageRate").as_int();
		data.ExcellentDamageAdd = list.attribute("ExlDamageAdd").as_int();
		data.AttackSuccessRate = list.attribute("AttackSuccesRate").as_int();
		data.Defense = list.attribute("Defense").as_int();
		data.DefenseSuccessRate = list.attribute("DefenseSuccesRate").as_int();
		data.MovementRange = list.attribute("MoveRange").as_int();
		data.MovementSpeed = list.attribute("MoveSpeed").as_int();
		data.AttackRange = list.attribute("AttackRange").as_int();
		data.AttackSpeed = list.attribute("AttackSpeed").as_int();
		data.ViewRange = list.attribute("ViewRange").as_int();

		data.Resistance[0] = list.attribute("ResIce").as_int();
		data.Resistance[1] = list.attribute("ResPoison").as_int();
		data.Resistance[2] = list.attribute("ResLightning").as_int();
		data.Resistance[3] = list.attribute("ResFire").as_int();
		data.Resistance[4] = list.attribute("ResEarth").as_int();
		data.Resistance[5] = list.attribute("ResWind").as_int();
		data.Resistance[6] = list.attribute("ResWater").as_int();

		data.RespawnTimeMin = list.attribute("RespawnTimeMin").as_int();
		data.RespawnTimeMax = list.attribute("RespawnTimeMax").as_int();
		data.ItemRate = list.attribute("ItemRate").as_int();
		data.ZenRate = list.attribute("ZenRate").as_int();
		data.ItemMaxLevel = list.attribute("ItemMaxLevel").as_int();

		data.StatRecovery[0] = list.attribute("RegenLife").as_int();
		data.StatRecoveryTime[0] = list.attribute("RegenLifeTime").as_int();
		data.StatRecovery[1] = list.attribute("RegenMana").as_int();
		data.StatRecoveryTime[1] = list.attribute("RegenManaTime").as_int();
		data.StatRecovery[2] = list.attribute("RegenShield").as_int();
		data.StatRecoveryTime[2] = list.attribute("RegenShieldTime").as_int();
		data.StatRecovery[3] = list.attribute("RegenStamina").as_int();
		data.StatRecoveryTime[3] = list.attribute("RegenStaminaTime").as_int();

		data.ScriptName = list.attribute("ScriptName").as_string();

		data.ElementalAttribute = list.attribute("ElementalAtribute").as_int();
		data.ElementalDefense = list.attribute("ElementalDef").as_int();
		data.ElementalDamageMin = list.attribute("ElementalDamageMin").as_int();
		data.ElementalDamageMax = list.attribute("ElementalDamageMax").as_int();
		data.ElementalAttackSuccessRate = list.attribute("ElementalAttackSuccesRate").as_int();
		data.ElementalDefenseSuccessRate = list.attribute("ElementalDefSuccesRate").as_int();
		data.RadianceImmune = list.attribute("RadianceImmune").as_int();
		data.DebuffResistance = list.attribute("DebuffResistance").as_int();
		data.DebuffDefense = list.attribute("DebuffDef").as_int();
		data.CriticalDamageResistance = list.attribute("ResCritDamage").as_int();
		data.ExcellentDamageResistance = list.attribute("ResExlDamage").as_int();
		data.DamageAbsrob = list.attribute("DamageAbsorb").as_int();
		data.IsElite = list.attribute("Elite").as_bool();
		data.IsCustom = list.attribute("Custom").as_bool();
		data.ExpLevel = list.attribute("ExpLevel").as_int();

		if (!sScriptAI->IsScriptAI(data.ScriptName))
		{
			sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", data.ScriptName.c_str());
			data.ScriptName.clear();
		}

		if (data.AttackRange > data.ViewRange)
			data.ViewRange = data.AttackRange;
	}

	sLog->outLoad(false, ">> Loaded %u monsters definitions", _monsterTemplate.size());
}

void MonsterManager::LoadMonsterItems()
{
	sLog->outLoad(true, "Loading Monster Items...");

	ClearItem();

	for (auto itr : _monsterTemplate)
	{
		auto const & monster_template = itr.second;

		CreateItem(monster_template.Level, monster_template.ItemMaxLevel);
	}

	sLog->outLoad(false, ">> Loaded monsters items");
}

MonsterTemplate const* MonsterManager::GetMonsterTemplate(uint16 id) const
{
	auto itr = _monsterTemplate.find(id);
	if (itr != this->_monsterTemplate.end())
		return &itr->second;
	else
		return nullptr;
}

monster * MonsterManager::GetMonsterData(uint16 index) const
{
	MonsterMap::const_iterator it = this->monster_map.find(index);

	if ( it != this->monster_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void MonsterManager::LoadMonsterSkill(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Skill...");

	for ( MonsterSkillMap::iterator it = this->monster_skill_map.begin(); it != this->monster_skill_map.end(); ++it )
		it->second.clear();

	this->monster_skill_map.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterSkill");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {

		uint16 monster = data.attribute("ID").as_int();
		uint16 skill = data.attribute("Skill").as_int();

		if (!sSkillMgr->IsSkill(skill))
			continue;

		this->monster_skill_map[monster].push_back(skill);

		count++;
	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_skill");

	if ( result )
	{
		do
		{

			Field* fields = result->Fetch();

			uint16 monster = fields[0].GetUInt16();
			uint16 skill = fields[1].GetUInt16();

			if ( !sSkillMgr->IsSkill(skill) )
				continue;
		
			this->monster_skill_map[monster].push_back(skill);

			count++;
		}
		while(result->NextRow());
	}*/
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster skill", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::LoadMonsterSkillSpecial(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Skill Special...");

	for ( MonsterSkillSpecialMap::iterator it = this->monster_skill_special_map.begin(); it != this->monster_skill_special_map.end(); it++ )
	{
		for ( MonsterSkillSpecialList::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			delete *it2;

		it->second.clear();
	}

	this->monster_skill_special_map.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterskillSpecial");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		monster_skill_special* add_skill = new monster_skill_special;

		add_skill->SetMonster(data.attribute("Monster").as_int());
		add_skill->SetType(data.attribute("Type").as_int());
		add_skill->SetSkill(data.attribute("ID").as_int());

		if (sSkillMgr->GetSkillSpecial(add_skill->GetSkill()) == nullptr)
		{
			delete add_skill;
			continue;
		}

		this->monster_skill_special_map[add_skill->GetMonster()].push_back(add_skill);

		count++;
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster skill special", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::LoadMonsterAIElement(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster AI Element...");

	MAP_CLEAR(MonsterAIElementMap::iterator, this->m_monster_ai_element);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterAiElement");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		monster_ai_element* ai_data = new monster_ai_element;
		ai_data->SetID(data.attribute("ID").as_int());
		ai_data->SetName(data.attribute("Name").as_string());
		ai_data->SetState(data.attribute("State").as_int());
		ai_data->SetClass(data.attribute("Class").as_int());
		ai_data->SetRate(data.attribute("Rate").as_int());
		ai_data->SetDelay(data.attribute("Delay").as_int());
		ai_data->SetTarget(data.attribute("Target").as_int());
		ai_data->SetX(data.attribute("X").as_int());
		ai_data->SetY(data.attribute("Y").as_int());

		if (ai_data->GetState() >= MONSTER_AI_ELEMENT_MAX)
		{
			delete ai_data;
			continue;
		}

		this->m_monster_ai_element[ai_data->GetID()] = ai_data;
		count++;
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster ai element definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void MonsterManager::LoadMonsterAIAutomata(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster AI Automata...");

	MAP_CLEAR(MonsterAIAutomataMap::iterator, this->m_monster_ai_automata);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterAiAutomata");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		uint32 id = data.attribute("ID").as_int();

		monster_ai_automata* ai_data = this->GetMonsterAIAutomata(id);

		if (!ai_data)
		{
			ai_data = new monster_ai_automata;
			ai_data->SetID(id);
			this->m_monster_ai_automata[id] = ai_data;
		}

		std::string name(data.attribute("Name").as_string());
		uint8 priority = data.attribute("Priority").as_int();
		uint8 current_state = data.attribute("CurrentState").as_int();
		uint8 next_state = data.attribute("NextState").as_int();
		int32 transaction_type = data.attribute("TransitionType").as_int();
		uint8 transaction_rate = data.attribute("TransitionRate").as_int();
		int32 transaction_value_type = data.attribute("TransitionValueType").as_int();
		int32 transition_value = data.attribute("TransitionValue").as_int();
		int32 delay = data.attribute("Delay").as_int();

		if (current_state >= MAX_AI_STATE)
			continue;

		if (next_state >= MAX_AI_STATE)
			continue;

		if (priority >= MAX_AI_PRIORITY)
			continue;

		monster_ai_state* ai_state = &ai_data->m_AIState[current_state][priority];

		ai_state->SetPriority(priority);
		ai_state->SetCurrentState(current_state);
		ai_state->SetNextState(next_state);
		ai_state->SetTransitionType(transaction_type);
		ai_state->SetTransitionRate(transaction_rate);
		ai_state->SetTransitionValueType(transaction_value_type);
		ai_state->SetTransitionValue(transition_value);
		ai_state->SetDelay(delay);
		ai_state->SetName(name);
		ai_data->m_AIStatCount[current_state]++;

		count++;
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster ai automata definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void MonsterManager::LoadMonsterAIUnit(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster AI Unit...");

	MAP_CLEAR(MonsterAIUnitMap::iterator, this->m_monster_ai_unit);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterAiUnit");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		monster_ai_unit* ai_data = new monster_ai_unit;
		ai_data->SetID(data.attribute("ID").as_int());
		ai_data->SetName(data.attribute("ID").as_string());
		ai_data->SetDelay(data.attribute("Delay").as_int());
		ai_data->SetAutomata(this->GetMonsterAIAutomata(data.attribute("Atutomata").as_int()));

		ai_data->SetAIClass(0, this->GetMonsterAIElement(data.attribute("Normal").as_int()));
		ai_data->SetAIClass(1, this->GetMonsterAIElement(data.attribute("Move").as_int()));
		ai_data->SetAIClass(2, this->GetMonsterAIElement(data.attribute("Attack").as_int()));
		ai_data->SetAIClass(3, this->GetMonsterAIElement(data.attribute("Heal").as_int()));
		ai_data->SetAIClass(4, this->GetMonsterAIElement(data.attribute("Avoid").as_int()));
		ai_data->SetAIClass(5, this->GetMonsterAIElement(data.attribute("Help").as_int()));
		ai_data->SetAIClass(6, this->GetMonsterAIElement(data.attribute("Special").as_int()));
		ai_data->SetAIClass(7, this->GetMonsterAIElement(data.attribute("Event").as_int()));

		this->m_monster_ai_unit[ai_data->GetID()] = ai_data;
		count++;

	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_ai_unit");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_ai_unit * ai_data = new monster_ai_unit;
			ai_data->SetID(fields[0].GetUInt32());
			ai_data->SetName(fields[1].GetString());
			ai_data->SetDelay(fields[2].GetUInt32());
			ai_data->SetAutomata(this->GetMonsterAIAutomata(fields[3].GetInt32()));

			ai_data->SetAIClass(0, this->GetMonsterAIElement(fields[4].GetInt32()));
			ai_data->SetAIClass(1, this->GetMonsterAIElement(fields[5].GetInt32()));
			ai_data->SetAIClass(2, this->GetMonsterAIElement(fields[6].GetInt32()));
			ai_data->SetAIClass(3, this->GetMonsterAIElement(fields[7].GetInt32()));
			ai_data->SetAIClass(4, this->GetMonsterAIElement(fields[8].GetInt32()));
			ai_data->SetAIClass(5, this->GetMonsterAIElement(fields[9].GetInt32()));
			ai_data->SetAIClass(6, this->GetMonsterAIElement(fields[10].GetInt32()));
			ai_data->SetAIClass(7, this->GetMonsterAIElement(fields[11].GetInt32()));

			this->m_monster_ai_unit[ai_data->GetID()] = ai_data;
			count++;
		}
		while(result->NextRow());
	}*/
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster ai unit definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::LoadMonsterAIGroup(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster AI Group...");

	for ( MonsterAIGroupMap::iterator it = this->monster_ai_group_map.begin(); it != this->monster_ai_group_map.end(); it++ )
	{
		for ( MonsterAIGroupList::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			delete *it2;

		it->second.clear();
	}

	this->monster_ai_group_map.clear();
	uint32 count = 0;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterAiData");

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		AIData* add_ai_data = new AIData;

		add_ai_data->SetGroup(data.attribute("Group").as_int());
		add_ai_data->SetID(data.attribute("ID").as_int());

		add_ai_data->SetAIOrder(0, data.attribute("AIOrder1").as_int());
		add_ai_data->SetAIOrder(1, data.attribute("AIOrder2").as_int());
		add_ai_data->SetAIOrder(2, data.attribute("AIOrder3").as_int());
		add_ai_data->SetAIOrder(3, data.attribute("AIOrder4").as_int());

		add_ai_data->SetAITime(0, data.attribute("AITime1").as_int());
		add_ai_data->SetAITime(1, data.attribute("AITime2").as_int());
		add_ai_data->SetAITime(2, data.attribute("AITime3").as_int());
		add_ai_data->SetAITime(3, data.attribute("AITime4").as_int());

		this->monster_ai_group_map[add_ai_data->GetGroup()].push_back(add_ai_data);

		count++;
	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_ai_data");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			AIData * add_ai_data = new AIData;

			add_ai_data->SetGroup(fields[0].GetUInt32());
			add_ai_data->SetID(fields[1].GetUInt32());
	
			for ( uint8 i = 0; i < MONSTER_MAX_AI_ORDER; ++i )
			{
				add_ai_data->SetAIOrder(i, fields[2 + i].GetUInt32());
			}

			for ( uint8 i = 0; i < MONSTER_MAX_AI_ORDER; ++i )
			{
				add_ai_data->SetAITime(i, fields[6 + i].GetUInt32());
			}
		
			this->monster_ai_group_map[add_ai_data->GetGroup()].push_back(add_ai_data);

			count++;
		}
		while(result->NextRow());
	}*/
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster ai groups", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::LoadMonsterRespawnLocation(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Respawn Location...");

	for ( MonsterRespawnLocationMap::iterator i_list = this->m_monster_respawn_location_map.begin(); i_list != this->m_monster_respawn_location_map.end(); ++i_list )
	{
		LIST_CLEAR(MonsterRespawnLocationList::iterator, i_list->second);
	}

	this->m_monster_respawn_location_map.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterRespawnLocation");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		monster_respawn_location* add_location = new monster_respawn_location;

		add_location->SetGroup(data.attribute("Group").as_int());
		add_location->SetWorld(data.attribute("WorldID").as_int());
		add_location->SetX1(data.attribute("X1").as_int());
		add_location->SetY1(data.attribute("Y1").as_int());
		add_location->SetX2(data.attribute("X2").as_int());
		add_location->SetY2(data.attribute("Y2").as_int());
		add_location->SetDirection(-1);
		add_location->SetInstance(-1);
		add_location->SetRate(data.attribute("Rate").as_int());

		this->m_monster_respawn_location_map[add_location->GetGroup()].push_back(add_location);
		count++;
	}
	/*PreparedStatement* stmt = GameServerDatabase.GetPreparedStatement(QUERY_SRVCFG_MONSTER_RESPAWN_LOCATION_SELECT);
	stmt->setUInt16(0, sGameServer->GetTransferServer());

	PreparedQueryResult result = GameServerDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_respawn_location * add_location = new monster_respawn_location;

			add_location->SetGroup(fields[1].GetUInt32());
			add_location->SetWorld(fields[2].GetUInt16());
			add_location->SetX1(fields[3].GetInt16());
			add_location->SetY1(fields[4].GetInt16());
			add_location->SetX2(fields[5].GetInt16());
			add_location->SetY2(fields[6].GetInt16());
			add_location->SetDirection(fields[7].GetInt8());
			add_location->SetInstance(fields[8].GetInt32());
			add_location->SetRate(fields[9].GetUInt8());

			this->m_monster_respawn_location_map[add_location->GetGroup()].push_back(add_location);
			count++;
		}
		while(result->NextRow());
	}*/
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster respawn locations", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::LoadMonsterEquipment(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Equipment...");

	for ( MonsterEquipmentMap::iterator i_list = this->m_monster_equipment_map.begin(); i_list != this->m_monster_equipment_map.end(); ++i_list )
	{
		LIST_CLEAR(MonsterEquipmentList::iterator, i_list->second);
	}

	this->m_monster_equipment_map.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterEquipment");

	uint32 count = 0;

	for (pugi::xml_node obj = Main.child("Monster"); obj; obj = obj.next_sibling()) {
		for (pugi::xml_node equip = obj.child("Equip"); equip; equip = equip.next_sibling()) {
			monster_equipment* add_monster = new monster_equipment;

			uint16 monster = obj.attribute("ID").as_int();
			add_monster->slot.set(equip.attribute("Slot").as_int());
			add_monster->type.set(equip.attribute("ItemCat").as_int());
			add_monster->index.set(equip.attribute("ItemIndex").as_int());
			add_monster->level.set(equip.attribute("ItemLevel").as_int());
			add_monster->excellent_ancient.set(equip.attribute("ExlAncient").as_int());

			if (this->GetMonsterTemplate(monster) == nullptr)
			{
				sLog->outError(LOG_DEFAULT, "LoadMonsterEquipment() Wrong Monster ID: %d", monster);
				delete add_monster;
				continue;
			}

			this->m_monster_equipment_map[monster].push_back(add_monster);
			count++;
		}
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster equipment definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::LoadMonster(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster...");

	uint32 guid = 0;

	for (MonsterMap::const_iterator it = this->monster_map.begin(); it != this->monster_map.end(); )
	{
		if (it->first >= guid)
		{
			delete it->second;
			this->monster_map.erase(it++);
		}
		else
		{
			++it;
		}
	}

	uint32 count = 0;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterSpawn");
	for (pugi::xml_node map = Main.child("Map"); map; map = map.next_sibling()) {
		for (pugi::xml_node spot = map.child("Spot"); spot; spot = spot.next_sibling()) {
			for (pugi::xml_node spawn = spot.child("Spawn"); spawn; spawn = spawn.next_sibling()) {

				uint8 WorldID = map.attribute("Number").as_int();
				uint32 monsterid = spawn.attribute("Index").as_int();
				uint16 PosX1 = spawn.attribute("StartX").as_int();
				uint16 PosY1 = spawn.attribute("StartY").as_int();
				uint16 PosX2 = spawn.attribute("EndX").as_int();
				uint16 PosY2 = spawn.attribute("EndY").as_int();
				uint8 Dir = spawn.attribute("Dir").as_int();
				uint16 MoveDistance = spawn.attribute("Distance").as_int();
				uint8 Elemental = spawn.attribute("Elemental").as_int();
				uint16 Count = spawn.attribute("Count").as_int();
				std::string Npcfunction = spawn.attribute("NpcFunction").as_string();

				if (spot.attribute("Type").as_int() == 0) { //OK
					this->AddMonsterSpawn(guid, monsterid, WorldID, PosX1, PosY1, PosX2, PosY2, Dir, MoveDistance, Npcfunction, Elemental);
				}

				else if (spot.attribute("Type").as_int() == 1 || spot.attribute("Type").as_int() == 3) {
					for (int i = 0; i < spawn.attribute("Count").as_int(); i++) {
						this->AddMonsterSpawn(guid, monsterid, WorldID, PosX1, PosY1, PosX2, PosY2, Dir, MoveDistance, Npcfunction, Elemental);						
						guid++;
					}
					guid--;
				}
				else if (spot.attribute("Type").as_int() == 2 || spot.attribute("Type").as_int() == 4) {
					this->AddMonsterSpawn(guid, monsterid, WorldID, PosX1, PosY1, PosX2, PosY2, Dir, MoveDistance, Npcfunction, Elemental);
				}
				else {
					continue;
				}

				count++;
				guid++;
			}
		}
	}
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monsters", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::AddMonsterSpawn(uint32 GUID, uint16 ID, uint16 World, uint16 X1, uint16 Y1, uint16 X2, uint16 Y2, uint8 Dir, uint16 MoveDistance, std::string NpcFunction, uint8 Element) {
	monster* add_monster = new monster;

	add_monster->SetGUID(GUID);
	add_monster->SetID(ID);
	add_monster->SetWorld(World);
	add_monster->SetX1(X1);
	add_monster->SetY1(Y1);
	add_monster->SetX2(X2);
	add_monster->SetY2(Y2);
	add_monster->SetDirection(Dir);
	add_monster->SetSpawnDelay(0);
	add_monster->SetSpawnDistance(0);
	add_monster->SetRespawnTimeMin(0);
	add_monster->SetRespawnTimeMax(0);
	add_monster->SetRespawnID(0);
	add_monster->SetMoveDistance(MoveDistance);
	add_monster->SetNpcFunction(NpcFunction);
	add_monster->SetItemBag("");
	add_monster->SetScriptName("");
	add_monster->SetElementalAttribute(Element);

	if (!sScriptAI->IsScriptAI(add_monster->GetScriptName()))
	{
		sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->GetScriptName().c_str());
		add_monster->ResetScriptName();
	}

	if (add_monster->GetX1() > 255)
		add_monster->SetX1(255);

	if (add_monster->GetX1() < 0)
		add_monster->SetX1(0);

	if (add_monster->GetY1() > 255)
		add_monster->SetY1(255);

	if (add_monster->GetY1() < 0)
		add_monster->SetY1(0);

	if (add_monster->GetX2() > 255)
		add_monster->SetX2(255);

	if (add_monster->GetX2() < 0)
		add_monster->SetX2(0);

	if (add_monster->GetY2() > 255)
		add_monster->SetY2(255);

	if (add_monster->GetY2() < 0)
		add_monster->SetY2(0);

	if (add_monster->GetX2() < add_monster->GetX1())
		add_monster->SetX2(add_monster->GetX1());

	if (add_monster->GetY2() < add_monster->GetY1())
		add_monster->SetY2(add_monster->GetY1());

	if (!sScriptAI->IsScriptAI(add_monster->GetScriptName()))
	{
		sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->GetScriptName().c_str());
		add_monster->ResetScriptName();
	}

	if (add_monster->GetGUID() >= sObjectMgr->GetMaxMonsters())
	{
		sLog->outError(LOG_DEFAULT, "High Monster GUID: %u", add_monster->GetGUID());
		delete add_monster;
	}

	if (this->GetMonsterTemplate(add_monster->GetID()) == nullptr)
	{
		sLog->outError(LOG_DEFAULT, "Wrong Monster GUID: %u / ID: %u", add_monster->GetGUID(), add_monster->GetID());
		delete add_monster;
	}

	if (!this->IsMonsterExclusive(add_monster->GetID()))
	{
		delete add_monster;
	}

	this->monster_map[add_monster->GetGUID()] = add_monster;
}

void MonsterManager::LoadMonsterEvent(char* pchFileName)
{
	sLog->outLoad(true, "Loading Monster Event...");

	CLEAR_MAP(_monsterEvent);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterSpawnEvent");

	uint32 count = 0;

	for (pugi::xml_node events = Main.child("EventInfo"); events; events = events.next_sibling()) {
		for (pugi::xml_node ground = events.child("Ground"); ground; ground = ground.next_sibling()) {
			for (pugi::xml_node monster = ground.child("Monster"); monster; monster = monster.next_sibling()) {
				auto monster_id = monster.attribute("ID").as_int();
				auto map_id = ground.attribute("WorldID").as_int();
				auto x1 = monster.attribute("StartX").as_int();
				auto y1 = monster.attribute("StartY").as_int();
				auto x2 = monster.attribute("EndX").as_int();
				auto y2 = monster.attribute("EndY").as_int();
				auto direction = monster.attribute("Direction").as_int();
				auto spawn_delay = monster.attribute("SpawnDelay").as_int();
				auto spawn_distance = 0;
				auto respawn_time = 0;
				auto respawn_id = 0;
				auto movement_distance = monster.attribute("MoveDistance").as_int();
				auto event_id = events.attribute("EventID").as_int();
				auto raw_data_1 = monster.attribute("Data1").as_int();
				auto raw_data_2 = monster.attribute("Data2").as_int();
				auto raw_data_3 = monster.attribute("Data3").as_int();
				auto raw_data_4 = monster.attribute("Data4").as_int();
				auto raw_data_5 = monster.attribute("Data5").as_int();
				auto npc_function = monster.attribute("NpcFunction").as_string();
				auto item_bag = monster.attribute("ItemBag").as_string();
				auto script_name = monster.attribute("ScriptName").as_string();
				auto ai_group = monster.attribute("AIGroup").as_int();
				auto ai_group_member = monster.attribute("AIGroupMember").as_int();
				auto add_count = monster.attribute("Count").as_int();

				for (int32 i = 0; i < add_count; ++i)
				{
					MonsterEvent* add_monster = new MonsterEvent;

					add_monster->MonsterId = monster_id;
					add_monster->MapId = map_id;
					add_monster->X1 = x1;
					add_monster->Y1 = y1;
					add_monster->X2 = x2;
					add_monster->Y2 = y2;
					add_monster->Direction = direction;
					add_monster->SpawnDelay = spawn_delay;
					add_monster->SpawnDistance = spawn_distance;
					add_monster->RespawnTime = respawn_time;
					add_monster->RespawnId = respawn_id;
					add_monster->MovementDistance = movement_distance;
					add_monster->EventId = event_id;
					add_monster->raw.data[0] = raw_data_1;
					add_monster->raw.data[1] = raw_data_2;
					add_monster->raw.data[2] = raw_data_3;
					add_monster->raw.data[3] = raw_data_4;
					add_monster->raw.data[4] = raw_data_5;
					add_monster->NpcFunction = npc_function;
					add_monster->ItemBag = item_bag;
					add_monster->ScriptName = script_name;
					add_monster->AIGroup = ai_group;
					add_monster->AIGroupMember = ai_group_member;

					if (!sScriptAI->IsScriptAI(add_monster->ScriptName))
					{
						sLog->outError(LOG_DEFAULT, "Wrong AI Script: %s", add_monster->ScriptName.c_str());
						add_monster->ScriptName.clear();
					}

					if (add_monster->X1 > 255)
						add_monster->X1 = 255;

					if (add_monster->X1 < 0)
						add_monster->X1 = 0;

					if (add_monster->Y1 > 255)
						add_monster->Y1 = 255;

					if (add_monster->Y1 < 0)
						add_monster->Y1 = 0;

					if (add_monster->X2 > 255)
						add_monster->X2 = 255;

					if (add_monster->X2 < 0)
						add_monster->X2 = 0;

					if (add_monster->Y2 > 255)
						add_monster->Y2 = 255;

					if (add_monster->Y2 < 0)
						add_monster->Y2 = 0;

					if (add_monster->X2 < add_monster->X1)
						add_monster->X2 = add_monster->X1;

					if (add_monster->Y2 < add_monster->Y1)
						add_monster->Y2 = add_monster->Y1;

					if (!GetMonsterTemplate(add_monster->MonsterId))
					{
						sLog->outError(LOG_DEFAULT, "Wrong Monster Event ID: %u", add_monster->MonsterId);
						delete add_monster;
						continue;
					}

					_monsterEvent.emplace(add_monster->EventId, add_monster);
				}
			}
		}
	}

	sLog->outLoad(false, ">> Loaded %u monster event definitions", _monsterEvent.size());
}

void MonsterManager::LoadMonsterRespawn()
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Respawn...");

	MAP_CLEAR(MonsterRespawnMap::iterator, this->monster_respawn_map);

	uint32 count = 0;

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_MONSTER_RESPAWN_SELECT);
	stmt->setUInt16(0, sGameServer->GetServerCode());

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			monster_respawn * add_monster = new monster_respawn;

			add_monster->SetGUID(fields[0].GetUInt16());
			add_monster->SetDate(fields[1].GetInt64());

			this->monster_respawn_map[add_monster->GetGUID()] = add_monster;
			count++;
		}
		while(result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster respawn definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::LoadMonsterExclusive(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Exclusive...");

	LIST_CLEAR(MonsterExclusiveList::iterator, this->monster_exclusive_list);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterExclusive");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		monster_exclusive* add_monster = new monster_exclusive;

		add_monster->SetServer(data.attribute("ServerCode").as_int());
		add_monster->SetGUID(data.attribute("MonsterID").as_int());
		add_monster->SetAvailableServer(data.attribute("ServerCode").as_int());

		this->monster_exclusive_list.push_back(add_monster);
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster exclusive definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterManager::SpawnMonsters()
{
	for (MonsterMap::const_iterator it = this->monster_map.begin(); it != this->monster_map.end(); ++it)
	{


		Monster* mMonster = MONSTER_ADD_CONTINUE(it->second);

		if (!mMonster)
		{
			continue;
		}

		if (!mMonster->GetWorld())
		{
			mMonster->SetConnectStatus(CONNECT_STATUS_NONE);
			continue;
		}

		if (sNoticeSystem->CheckMonsterRespawnNotice(mMonster)) {
			sNoticeSystem->NoticeRespawnMonster(mMonster);
		}

		MonsterRespawnMap::const_iterator itr = this->monster_respawn_map.find(mMonster->GetEntry());


		if (itr != this->monster_respawn_map.end())
		{
			time_t current_time = time(nullptr);

			if (itr->second->GetDate() > current_time)
			{
				mMonster->SetSpawnTime((itr->second->GetDate() - current_time) * IN_MILLISECONDS);
			}
		}

		mMonster->AddToWorld();

	}
}

bool MonsterManager::IsMonsterExclusive(uint16 guid) const
{
	bool success = true;
	bool founded = false;

	for ( MonsterExclusiveList::const_iterator it = this->monster_exclusive_list.begin(); it != this->monster_exclusive_list.end(); ++it )
	{
		if ( (*it) && (*it)->GetServer() == sGameServer->GetTransferServer() && (*it)->GetGUID() == guid )
		{
			success = false;

			if ( (*it)->GetAvailableServer() == sGameServer->GetServerCode() )
			{
				founded = true;
			}
		}
	}

	if ( founded )
	{
		success = true;
	}

	return success;
}

void MonsterManager::CreateItem(int32 level, int32 max_level)
{
	if ( level < 1 )
	{
		return;
	}

	ItemTemplateMap const& item_template_map = sItemMgr->GetItemMap();
	for ( ItemTemplateMap::const_iterator it = item_template_map.begin(); it != item_template_map.end(); ++it )
	{
		item_template const* item_info = it->second;

		if ( !item_info )
		{
			continue;
		}

		if ( !item_info->IsFlag(ITEM_FLAG_MONSTER_DROP) )
		{
			continue;
		}

		if ( !item_info->GetDropRate() )
		{
			continue;
		}
		
		uint8 type = item_info->GetType();
		uint16 index = item_info->GetIndex();

		int32 level_tmp = this->GenerateItemLevel(item_info, level);

		if ( level_tmp < 0 )
		{
			continue;
		}

		if ( type < 12 )
		{
			level_tmp = 0;
		}

		if ( level_tmp > max_level )
		{
			level_tmp = max_level;
		}

		this->InsertItem(level, ITEMGET(type, index), level_tmp);
	}
}

int32 MonsterManager::GenerateItemLevel(item_template const* item_info, int32 level)
{
	if ( item_info->GetLevel() == 0 )
	{
		return -1;
	}

	if ( item_info->GetLevel() >= (level - 18) && item_info->GetLevel() <= level ) // Rango de niveles
	{
		int32 ilevel = (level - item_info->GetLevel()) / 3;

		if ( item_info->GetType() == 13 )
		{
			// Rings y Pendants
			if ( item_info->GetIndex() == 8 || item_info->GetIndex() == 9 || item_info->GetIndex() == 12 || item_info->GetIndex() == 13 || item_info->GetIndex() == 20 || 
				 item_info->GetIndex() == 21 || item_info->GetIndex() == 22 || item_info->GetIndex() == 23 || item_info->GetIndex() == 24 || item_info->GetIndex() == 25 || 
				 item_info->GetIndex() == 26 || item_info->GetIndex() == 27 || item_info->GetIndex() == 28 )
			{
				if ( ilevel > 4 )
				{
					ilevel = 4;
				}
			}
		}

		return ilevel;
	}

	return -1;
}

bool MonsterManager::InsertItem(int32 level, uint16 item, uint8 item_level)
{
	MonsterItemList & item_list = this->m_monster_item_map[level];

	for ( MonsterItemList::const_iterator it = item_list.begin(); it != item_list.end(); ++it )
	{
		if ( (*it)->GetItem() == item && (*it)->GetLevel() == item_level )
		{
			return false;
		}
	}

	MonsterItem* pNewItem = new MonsterItem;
	pNewItem->SetItem(item);
	pNewItem->SetLevel(item_level);

	item_list.push_back(pNewItem);
	return true;
}

bool MonsterManager::GenerateItem(Item & item, int32 level, bool exe)
{
	if ( level < 1 )
		return false;

	RandomValue<MonsterItem const*> m_randomizer(nullptr);
	m_randomizer.SetTotalWeight(10000);
	
	MonsterItemMap::iterator i_list = this->m_monster_item_map.find(level);

	if ( i_list != this->m_monster_item_map.end() )
	{
		for ( MonsterItemList::iterator it = i_list->second.begin(); it != i_list->second.end(); ++it )
		{
			MonsterItem const* pItem = (*it);

			if ( !pItem )
			{
				continue;
			}

			item_template const* item_info = sItemMgr->GetItem(pItem->GetItem());

			if ( !item_info )
			{
				continue;
			}

			if ( !item_info->GetDropRate() )
			{
				continue;
			}

			if ( exe && !item_info->IsFlag(ITEM_FLAG_MONSTER_DROP_EXE) )
			{
				continue;
			}

			m_randomizer.AddValue(pItem, item_info->GetDropRate());
		}
	}

	MonsterItem const* pItem = m_randomizer.GetRandomValue(RANDOM_POOL_RATE);

	if ( pItem )
	{
		if (exe) {
			item.SetItem(pItem->GetItem());
			item.SetLevel(pItem->GetLevel());
			item.SetExe(GetRandomExcOption(sGameServer->GetItemDropExeOptionMax()));
			return true;
		}

		item.SetItem(pItem->GetItem());
		item.SetLevel(pItem->GetLevel());
		return true;
	}

	return false;
}
	
void MonsterManager::ClearItem()
{
	for ( MonsterItemMap::iterator i_list = this->m_monster_item_map.begin(); i_list != this->m_monster_item_map.end(); ++i_list )
	{
		for ( MonsterItemList::iterator it = i_list->second.begin(); it != i_list->second.end(); ++it )
		{
			delete *it;
		}

		i_list->second.clear();
	}

	this->m_monster_item_map.clear();
}

void MonsterManager::GenerateRespawnLocation(Monster* pMonster)
{
	monster_respawn_location const* pData = this->GetRandomRespawnLocation(pMonster);

	if ( !pData )
	{
		return;
	}

	pMonster->SetWorldId(pData->GetWorld());
	pMonster->SetDirection(pData->GetDirection());
	pMonster->SetInstance(pData->GetInstance());
	pMonster->SetBasicLocation(pData->GetX1(), pData->GetY1(), pData->GetX2(), pData->GetY2());
}

monster_respawn_location const* MonsterManager::GetRandomRespawnLocation(Monster* pMonster) const
{
	if ( !pMonster->GetRespawnID() )
		return nullptr;

	MonsterRespawnLocationMap::const_iterator it = this->m_monster_respawn_location_map.find(pMonster->GetRespawnID());

	if ( it == this->m_monster_respawn_location_map.end() )
		return nullptr;

	if ( it->second.empty() )
		return nullptr;

	RandomValue<monster_respawn_location const*> m_randomizer(nullptr);
	m_randomizer.SetTotalWeight(100);

	for ( MonsterRespawnLocationList::const_iterator data = it->second.begin(); data != it->second.end(); ++data )
	{
		m_randomizer.AddValue(*data, (*data)->GetRate());
	}

	return m_randomizer.GetRandomValue(RANDOM_POOL_RATE);
}

bool MonsterManager::TeleportToRandomLocation(Monster* pMonster, int32 attempts)
{
	do
	{
		monster_respawn_location const* pLocation = this->GetRandomRespawnLocation(pMonster);

		if ( pLocation )
		{
			if ( pLocation->GetWorld() != pMonster->GetWorldId() )
			{
				pMonster->UpdateLastLocation();

				pMonster->ClearPathData();
				pMonster->SetBasicLocation(pLocation->GetX1(), pLocation->GetY1(), pLocation->GetX2(), pLocation->GetY2());
				pMonster->SetWorldId(pLocation->GetWorld());

				pMonster->MakeRespawnLocation(false);

				pMonster->MagicAttackSend(ENTRY(pMonster), SKILL_TELEPORT);
				pMonster->ViewportClose();

				//pMonster->TeleportToLocation(pLocation->GetWorld(), pMonster->GetX(), pMonster->GetY(), pLocation->GetDirection(), pLocation->GetInstance());
				return true;
			}
		}
	}
	while ( attempts-- > 0 );

	return false;
}

void MonsterManager::BuildMonsterCustomData(Monster* pMonster, Player* pPlayer)
{
	if (!pMonster->GetMonsterTemplate()->IsCustom)
		return;

	VIEWPORT_MONSTER_CUSTOM_DATA pMsg;
	pMsg.index = pMonster->GetEntry();
	pMsg.type = pMonster->GetSendType();
	memcpy(pMsg.name, pMonster->GetName(), MAX_MONSTER_NAME_LENGTH);
	pMsg.size = pMonster->GetSize();
	
	MonsterEquipmentMap::const_iterator it = this->m_monster_equipment_map.find(pMonster->GetClass());

	if ( it != this->m_monster_equipment_map.end() )
	{
		for ( MonsterEquipmentList::const_iterator equipment = it->second.begin(); equipment != it->second.end(); ++equipment )
		{
			if ( (*equipment)->slot.get() < 12 )
			{
				pMsg.equipment[(*equipment)->slot.get()].item = ITEMGET((*equipment)->type.get(), (*equipment)->index.get());
				pMsg.equipment[(*equipment)->slot.get()].level = (*equipment)->level.get();
				pMsg.equipment[(*equipment)->slot.get()].excellent_ancient = (*equipment)->excellent_ancient.get();
			}
		}
	}

	if ( pPlayer )
	{
		pPlayer->sendPacket((uint8*)&pMsg, pMsg.h.get_size());
	}
	else
	{
		pMonster->sendPacket_viewport((uint8*)&pMsg, pMsg.h.get_size());
	}
}

uint16 MonsterManager::GetRandomSkillSpecial(Monster* pMonster, int16 type)
{
	MonsterSkillSpecialMap::const_iterator it = this->monster_skill_special_map.find(pMonster->GetClass());

	RandomValue<uint16> m_randomizer(0);

	if ( it != this->monster_skill_special_map.end() )
	{
		for ( MonsterSkillSpecialList::const_iterator itr = it->second.begin(); itr != it->second.end(); ++itr )
		{
			if ( (*itr)->GetType() == type )
			{
				m_randomizer.AddValue((*itr)->GetSkill(), 0);
			}
		}
	}

	return m_randomizer.GetRandomValue(RANDOM_POOL_RANDOM);
}

bool AllowItemDropInWorld(uint16 world, Item const& item)
{
	if ( item.GetItem() == JEWEL::BLESS ||
		 item.GetItem() == JEWEL::SOUL ||
		 item.GetItem() == JEWEL::CHAOS ||
		 item.GetItem() == JEWEL::LIFE ||
		 item.GetItem() == JEWEL::CREATION ||
		 item.GetItem() == JEWEL::DEATH ||
		 item.GetItem() == JEWEL::GUARDIAN )
	{
		if ( sCrywolf->GetOccupationState() == CRYWOLF_OCCUPATION_FAIL && sGameServer->IsCrywolfPenaltyApply() )
		{
			if ( Random(100) < sGameServer->GetCrywolfPenaltyJewelDrop() )
			{
				return false;
			}
		}
	}

	if (item.IsSocket())
	{
		if (world != WORLD_RAKLION && 
			world != WORLD_RAKLION_BOSS && 
			world != WORLD_ACHERON && 
			world != WORLD_DEVENTER && 
			world != WORLD_URUK_MOUNTAIN &&
			world != WORLD_NARS &&
			world != WORLD_FEREA &&
			world != WORLD_NIXIES_LAKE &&
			world != WORLD_DEEP_DUNGEON_1 &&
			world != WORLD_DEEP_DUNGEON_2 &&
			world != WORLD_DEEP_DUNGEON_3 &&
			world != WORLD_DEEP_DUNGEON_4 &&
			world != WORLD_DEEP_DUNGEON_5 &&
			world != WORLD_SWAMP_OF_DARKNESS &&
			world != WORLD_KUBERA_MINE_1 &&
			world != WORLD_KUBERA_MINE_2 &&
			world != WORLD_KUBERA_MINE_3 &&
			world != WORLD_KUBERA_MINE_4 &&
			world != WORLD_KUBERA_MINE_5 &&
			world != WORLD_ABYSS_ATLANS_1 &&
			world != WORLD_ABYSS_ATLANS_2 &&
			world != WORLD_ABYSS_ATLANS_3 &&
			world != WORLD_SCORCHED_CANYON &&
			world != WORLD_SCARLET_ICARUS)
		{
			return false;
		}
	}

	bool fail = false;

	if ( DS_MAP_RANGE(world) )
	{
		if ( item.GetItemType() >= 7 && item.GetItemType() <= 11 )
		{
			fail = item.GetItemIndex() == 17 || item.GetItemIndex() == 18 || item.GetItemIndex() == 19;
		}
		else if ( item.GetItemType() == 0 )
		{
			fail = item.GetItemIndex() == 17 || item.GetItemIndex() == 18;
		}
		else if ( item.GetItemType() == 12 )
		{
			fail = item.GetItemIndex() == 12 || item.GetItemIndex() == 13 || item.GetItemIndex() == 14 ||
				   item.GetItemIndex() == 16 || item.GetItemIndex() == 17 || item.GetItemIndex() == 18 ||
				   item.GetItemIndex() == 19;
		}
		else
		{
			fail = item.GetItem() == ITEMGET(4, 17) || item.GetItem() == ITEMGET(5, 9);
		}
	}
	else if ( DG_MAP_RANGE(world) )
	{
		if ( item.GetItem() == ITEMGET(14, 101) || item.GetItem() == ITEMGET(14, 110) )
		{
			fail = true;
		}
	}

	return fail ? RANDOM(10) == 0: true;
}