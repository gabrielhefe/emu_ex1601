/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "QuestMgr.cpp"
*
*/

CQuestMgr::CQuestMgr()
{
	this->SetTestingDay(-1);

	this->SetCurrent4thQuestInstance(0);
}

CQuestMgr::~CQuestMgr()
{
	for ( QuestEvolutionMap::iterator itr = this->m_quest_evolution.begin(); itr != this->m_quest_evolution.end(); ++itr )
	{
		LIST_CLEAR(QuestEvolutionDataList::iterator, itr->second->data);
		LIST_CLEAR(QuestEvolutionConditionList::iterator, itr->second->condition);
		delete itr->second;
	}

	this->m_quest_evolution.clear();

	LIST_CLEAR(QuestEvolutionRewardList::iterator, this->m_QuestEvolutionRewardList);

	MAP_CLEAR(QuestGuidedMap::iterator, this->m_quest_guided_map);

	MAP_CLEAR(Quest4thInstanceDataMap::iterator, this->m_quest_4th_instance_data_map);

	MAP_CLEAR(QuestMUMap::iterator, this->m_QuestMUMap);
}

void CQuestMgr::LoadQuestEvolution(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Evolution Template...");

	for ( QuestEvolutionMap::iterator itr = this->m_quest_evolution.begin(); itr != this->m_quest_evolution.end(); ++itr )
	{
		LIST_CLEAR(QuestEvolutionDataList::iterator, itr->second->data);
		LIST_CLEAR(QuestEvolutionConditionList::iterator, itr->second->condition);
		delete itr->second;
	}

	this->m_quest_evolution.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("QuestEvolutionTemplate");

	uint32 count = 0;

	for (pugi::xml_node quest = Main.child("Quest"); quest; quest = quest.next_sibling()) {
		uint8 id = quest.attribute("ID").as_int();

		if (this->GetQuest(id) != nullptr)
		{
			continue;
		}

		QuestEvolution* newQuest = new QuestEvolution;

		newQuest->SetID(id);
		newQuest->SetType(quest.attribute("Type").as_int());
		newQuest->SetNPC(quest.attribute("NpcID").as_int());
		newQuest->SetParty(quest.attribute("Party").as_int());
		newQuest->SetName(quest.attribute("Name").as_string());

		this->m_quest_evolution[id] = newQuest;
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u quest evolution definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CQuestMgr::LoadQuestEvolutionData(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Evolution Data...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("QuestEvolutionData");

	uint32 count = 0;

	for (pugi::xml_node questevo = Main.child("QuestEvo"); questevo; questevo = questevo.next_sibling()) {
		for (pugi::xml_node data = questevo.child("Data"); data; data = data.next_sibling()) {
			uint8 quest = questevo.attribute("ID").as_int();

			QuestEvolution* Quest = this->GetQuest(quest);

			if (!Quest)
			{
				continue;
			}

			QuestEvolutionData* add_data = new QuestEvolutionData;
			add_data->SetQuestID(quest);
			add_data->SetObjectiveType(data.attribute("ObjType").as_int());
			add_data->SetItemType(data.attribute("ItemCat").as_int());
			add_data->SetItemIndex(data.attribute("ItemIndex").as_int());
			add_data->SetItemLevel(data.attribute("ItemLevel").as_int());
			add_data->SetCount(data.attribute("Count").as_int());
			add_data->SetMonsterID(data.attribute("MonsterID").as_int());
			add_data->SetMonsterMinLevel(data.attribute("MonsterMinLevel").as_int());
			add_data->SetMonsterMaxLevel(data.attribute("MonsterMaxLevel").as_int());
			add_data->SetMonsterSlot(data.attribute("MonsterSlot").as_int());
			add_data->SetDropRate(data.attribute("DropRate").as_int());
			add_data->SetRewardType(data.attribute("RewardType").as_int());
			add_data->SetRewardSubType(data.attribute("RewardSubType").as_int());
			add_data->SetRewardCount(data.attribute("RewardCount").as_int());
			add_data->SetConditionIndex(data.attribute("ConditionIndex").as_int());
			add_data->SetContextBeforeReg(data.attribute("ContextBeforeReg").as_int());
			add_data->SetContextAfterReg(data.attribute("ContextAfterReg").as_int());
			add_data->SetContextCompleteQuest(data.attribute("ContextCompleteQuest").as_int());
			add_data->SetContextClearQuest(data.attribute("ContextClearQuest").as_int());

			add_data->SetRequiredClass(0, data.attribute("ReqDW").as_int());
			add_data->SetRequiredClass(1, data.attribute("ReqDK").as_int());
			add_data->SetRequiredClass(2, data.attribute("ReqElf").as_int());
			add_data->SetRequiredClass(3, data.attribute("ReqMG").as_int());
			add_data->SetRequiredClass(4, data.attribute("ReqDL").as_int());
			add_data->SetRequiredClass(5, data.attribute("ReqSUM").as_int());
			add_data->SetRequiredClass(6, data.attribute("ReqRF").as_int());
			add_data->SetRequiredClass(7, data.attribute("ReqGL").as_int());
			add_data->SetRequiredClass(8, data.attribute("ReqRW").as_int());
			add_data->SetRequiredClass(9, data.attribute("ReqSL").as_int());
			add_data->SetRequiredClass(10, data.attribute("ReqGC").as_int());

			if (add_data->GetMonsterSlot() >= QUEST_EVOLUTION_MONSTER_COUNT)
			{
				add_data->SetMonsterSlot(QUEST_EVOLUTION_MONSTER_COUNT - 1);
			}

			Quest->data.push_back(add_data);

			count++;

		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u quest evolution data definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}
	
void CQuestMgr::LoadQuestEvolutionCondition(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Evolution Condition...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("QuestEvolutionCondition");

	uint32 count = 0;

	for (pugi::xml_node questevo = Main.child("QuestEvo"); questevo; questevo = questevo.next_sibling()) {
		for (pugi::xml_node data = questevo.child("Data"); data; data = data.next_sibling()) {
			uint8 quest = questevo.attribute("ID").as_int();
			QuestEvolution* Quest = this->GetQuest(quest);

			if (!Quest)
			{
				continue;
			}

			QuestEvolutionCondition* add_condition = new QuestEvolutionCondition;
			add_condition->SetQuestID(quest);
			add_condition->SetConditionID(data.attribute("ConditionID").as_int());
			add_condition->SetRequiredQuestID(data.attribute("ReqQuestID").as_int());
			add_condition->SetMinLevel(data.attribute("MinLevel").as_int());
			add_condition->SetMaxLevel(data.attribute("MaxLevel").as_int());
			add_condition->SetRequiredZen(data.attribute("ReqZen").as_int());
			add_condition->SetContextStart(data.attribute("ContextStart").as_int());

			Quest->condition.push_back(add_condition);

			count++;
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u quest evolution condition definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CQuestMgr::LoadQuestEvolutionItemReward(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Evolution Item Reward...");

	LIST_CLEAR(QuestEvolutionRewardList::iterator, this->m_QuestEvolutionRewardList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("QuestEvolutionItemReward");

	uint32 count = 0;

	for (pugi::xml_node reward = Main.child("Reward"); reward; reward = reward.next_sibling()) {
		QuestEvolutionReward* pData = new QuestEvolutionReward;
		pData->SetQuestID(reward.attribute("QuestID").as_int());
		pData->SetClassFlag(reward.attribute("ClassFlag").as_int());
		pData->SetType(reward.attribute("ItemCat").as_int());
		pData->SetIndex(reward.attribute("ItemIndex").as_int());
		pData->SetLevel(reward.attribute("ItemLevel").as_int());
		pData->SetDurability(reward.attribute("Dur").as_int());
		pData->SetSkill(reward.attribute("Skill").as_int());
		pData->SetLuck(reward.attribute("Luck").as_int());
		pData->SetOption(reward.attribute("Option").as_int());
		pData->SetExcellent(reward.attribute("Exl").as_int());
		pData->SetAncient(reward.attribute("Ancient").as_int());

		pData->SetSocket(0, reward.attribute("Socket1").as_int());
		pData->SetSocket(1, reward.attribute("Socket2").as_int());
		pData->SetSocket(2, reward.attribute("Socket3").as_int());
		pData->SetSocket(3, reward.attribute("Socket4").as_int());
		pData->SetSocket(4, reward.attribute("Socket5").as_int());


		pData->SetSocketBonus(reward.attribute("SocketBonus").as_int());

		this->m_QuestEvolutionRewardList.push_back(pData);
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u quest evolution item reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CQuestMgr::LoadQuestGuided(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest Guided Setting...");

	MAP_CLEAR(QuestGuidedMap::iterator, this->m_quest_guided_map);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("GuideQuest");

	uint32 count = 0;

	for (pugi::xml_node class_node = Main.child("QuestInfo"); class_node; class_node = class_node.next_sibling())
	{
		QuestGuided * pData = new QuestGuided;

		pData->SetID(class_node.attribute("Index").as_int());
		
		pData->SetName(class_node.attribute("Name").as_string());

		pugi::xml_node Jobss = class_node.child("Job");

		pData->GetCondition()->SetTemplateType(Jobss.attribute("TemplateType").as_int());
		pData->GetCondition()->SetChapter(Jobss.attribute("Chapter").as_int());
		pData->GetCondition()->SetCategory(Jobss.attribute("Category").as_int());
		pData->GetCondition()->SetImportance(Jobss.attribute("Importance").as_int());
		pData->GetCondition()->SetStartType(Jobss.attribute("StartType").as_int());
		pData->GetCondition()->SetStartSubType(Jobss.attribute("StartSubType").as_int());
		pData->GetCondition()->SetLevelMin(Jobss.attribute("MinLevel").as_int());
		pData->GetCondition()->SetQuestRepeatCycle(Jobss.attribute("RepeatCycle").as_int());
		pData->GetCondition()->SetPrecedentQuest(Jobss.attribute("Priority").as_int());
		pData->GetCondition()->SetQuestStartItemType(Jobss.attribute("StartItemCat").as_int());
		pData->GetCondition()->SetQuestStartItemIndex(Jobss.attribute("StartItemIndex").as_int());
		pData->GetCondition()->SetQuestCheckGens(Jobss.attribute("GensCheck").as_int());

		pData->GetCondition()->SetRequiredClass(0, Jobss.attribute("DarkWizard").as_int());
		pData->GetCondition()->SetRequiredClass(1, Jobss.attribute("DarkKnight").as_int());
		pData->GetCondition()->SetRequiredClass(2, Jobss.attribute("FairyElf").as_int());
		pData->GetCondition()->SetRequiredClass(3, Jobss.attribute("MagicGladiator").as_int());
		pData->GetCondition()->SetRequiredClass(4, Jobss.attribute("DarkLord").as_int());
		pData->GetCondition()->SetRequiredClass(5, Jobss.attribute("Summoner").as_int());
		pData->GetCondition()->SetRequiredClass(6, Jobss.attribute("RageFighter").as_int());
		pData->GetCondition()->SetRequiredClass(7, Jobss.attribute("GrowLancer").as_int());
		pData->GetCondition()->SetRequiredClass(8, Jobss.attribute("RuneWizard").as_int());
		pData->GetCondition()->SetRequiredClass(9, Jobss.attribute("Slayer").as_int());
		pData->GetCondition()->SetRequiredClass(10, Jobss.attribute("GunCrusher").as_int());

		pData->GetCondition()->SetStartText(Jobss.attribute("StartText").as_int());
		pData->GetCondition()->SetStartErrorText(Jobss.attribute("StartErrorText").as_int());


		pugi::xml_node Rewards = class_node.child("Reward");

		pData->GetData()->SetTemplateType(Rewards.attribute("TemplateType").as_int());
		pData->GetData()->SetObjectiveType(Rewards.attribute("QuestKind").as_int());
		pData->GetData()->SetQuestMainType(Rewards.attribute("QuestType").as_int());
		pData->GetData()->SetQuestSubType(Rewards.attribute("QuestSubType").as_int());
		pData->GetData()->SetCount(Rewards.attribute("TargetCount").as_int());
		pData->GetData()->SetMonsterMaxLevel(Rewards.attribute("MonsterMaxLevel").as_int());
		pData->GetData()->SetDropRate(Rewards.attribute("DropRate").as_int());
		pData->GetData()->SetGateNumber(Rewards.attribute("GateNumber").as_int());
		pData->GetData()->SetWorld(Rewards.attribute("MapNumber").as_int());
		pData->GetData()->SetX(Rewards.attribute("CoordinateX").as_int());
		pData->GetData()->SetY(Rewards.attribute("CoordinateY").as_int());
		pData->GetData()->SetRewardExp(Rewards.attribute("RewardExp").as_int());
		pData->GetData()->SetRewardZen(Rewards.attribute("RewardZen").as_int());
		pData->GetData()->SetRewardItemType01(Rewards.attribute("RewardItemCat1").as_int());
		pData->GetData()->SetRewardItemIndex01(Rewards.attribute("RewardItemIndex1").as_int());
		pData->GetData()->SetRewardItemCount01(Rewards.attribute("RewardItemCount1").as_int());
		pData->GetData()->SetRewardItemType02(Rewards.attribute("RewardItemCat2").as_int());
		pData->GetData()->SetRewardItemIndex02(Rewards.attribute("RewardItemIndex2").as_int());
		pData->GetData()->SetRewardItemCount02(Rewards.attribute("RewardItemCount2").as_int());
		pData->GetData()->SetRewardItemType03(Rewards.attribute("RewardItemCat3").as_int());
		pData->GetData()->SetRewardItemIndex03(Rewards.attribute("RewardItemIndex3").as_int());
		pData->GetData()->SetRewardItemCount03(Rewards.attribute("RewardItemCount3").as_int());
		pData->GetData()->SetRewardType(Rewards.attribute("RewardCat").as_int());
		pData->GetData()->SetRewardSubType(Rewards.attribute("RewardIndex").as_int());
		pData->GetData()->SetRewardNumber(Rewards.attribute("RewardCount").as_int());
		pData->GetData()->SetProgressText(Rewards.attribute("ProgressText").as_int());
		pData->GetData()->SetCompleteText(Rewards.attribute("CompletetText").as_int());


		if (this->GetQuestGuided(pData->GetID()) != nullptr)
		{
			sLog->outError("root", "Guided Quest [%u / %s] already exist", pData->GetID(), pData->GetName().c_str());
			delete pData;
			continue;
		}

		this->m_quest_guided_map[pData->GetID()] = pData;
		++count;
	}

	sLog->outInfo(LOG_DEFAULT, "Loaded %u quest guided definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CQuestMgr::LoadQuestMU(char *pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Quest MU...");

	MAP_CLEAR(QuestMUMap::iterator, this->m_QuestMUMap);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MuQuest");

	uint32 count = 0;

	for (pugi::xml_node class_node = Main.child("QuestInfo"); class_node; class_node = class_node.next_sibling())
	{
		QuestMU* pData = new QuestMU();

		pData->SetID(class_node.attribute("Index").as_int());
		pData->SetName(class_node.attribute("Name").as_string());
		pData->SetServer(class_node.attribute("Server").as_int());
		pData->SetDay(class_node.attribute("Day").as_int());

		pugi::xml_node Jobss = class_node.child("Job");

		pData->SetTemplateType1(Jobss.attribute("TemplateType").as_int());
		pData->SetChapter(Jobss.attribute("Chapter").as_int());
		pData->SetCategory(Jobss.attribute("Category").as_int());
		pData->SetImportante(Jobss.attribute("Importance").as_int());
		pData->SetStartType(Jobss.attribute("StartType").as_int());
		pData->SetStartSubType(Jobss.attribute("StartSubType").as_int());
		pData->SetLevelMin(Jobss.attribute("MinLevel").as_int());
		pData->SetLevelMax(Jobss.attribute("MaxLevel").as_int());
		pData->SetRepeatCycle(Jobss.attribute("RepeatCycle").as_int());
		pData->SetPrecedenceQuest(Jobss.attribute("Priority").as_int());
		pData->SetStartItemType(Jobss.attribute("StartItemCat").as_int());
		pData->SetStartItemIndex(Jobss.attribute("StartItemIndex").as_int());
		pData->SetCheckGens(Jobss.attribute("GensCheck").as_int());
		pData->SetZen(Jobss.attribute("Zen").as_int());

		pData->SetRequiredClass(0, Jobss.attribute("DarkWizard").as_int());
		pData->SetRequiredClass(1, Jobss.attribute("DarkKnight").as_int());
		pData->SetRequiredClass(2, Jobss.attribute("FairyElf").as_int());
		pData->SetRequiredClass(3, Jobss.attribute("MagicGladiator").as_int());
		pData->SetRequiredClass(4, Jobss.attribute("DarkLord").as_int());
		pData->SetRequiredClass(5, Jobss.attribute("Summoner").as_int());
		pData->SetRequiredClass(6, Jobss.attribute("RageFighter").as_int());
		pData->SetRequiredClass(7, Jobss.attribute("GrowLancer").as_int());
		pData->SetRequiredClass(8, Jobss.attribute("RuneWizard").as_int());
		pData->SetRequiredClass(9, Jobss.attribute("Slayer").as_int());
		pData->SetRequiredClass(10, Jobss.attribute("GunCrusher").as_int());

		pData->SetAddClass(Jobss.attribute("Unk2").as_int());

		pugi::xml_node Rewards = class_node.child("Reward");

		pData->SetTemplateType2(Rewards.attribute("TemplateType").as_int());
		pData->SetObjectiveType(Rewards.attribute("QuestKind").as_int());
		pData->SetObjectiveMainType(Rewards.attribute("QuestType").as_int());
		pData->SetObjectiveMainSubType(Rewards.attribute("QuestSubType").as_int());
		pData->SetTargetNumber(Rewards.attribute("TargetCount").as_int());
		pData->SetTargetMaxLevel(Rewards.attribute("MonsterMaxLevel").as_int());
		pData->SetDropRate(Rewards.attribute("DropRate").as_int());
		pData->SetGateID(Rewards.attribute("GateNumber").as_int());
		pData->SetMapID(Rewards.attribute("MapNumber").as_int());
		pData->SetX(Rewards.attribute("CoordinateX").as_int());
		pData->SetY(Rewards.attribute("CoordinateY").as_int());
		pData->SetRewardExperience(Rewards.attribute("RewardExp").as_int());
		pData->SetRewardZen(Rewards.attribute("RewardZen").as_int());
		pData->SetRewardGensPoints(Rewards.attribute("RewardGensPoint").as_int());
		pData->SetRewardItemType(0, Rewards.attribute("RewardItemCat1").as_int());
		pData->SetRewardItemIndex(0, Rewards.attribute("RewardItemIndex1").as_int());
		pData->SetRewardItemCount(0, Rewards.attribute("RewardItemCount1").as_int());
		pData->SetRewardItemType(1, Rewards.attribute("RewardItemCat2").as_int());
		pData->SetRewardItemIndex(1, Rewards.attribute("RewardItemIndex2").as_int());
		pData->SetRewardItemCount(1, Rewards.attribute("RewardItemCount2").as_int());
		pData->SetRewardItemType(2, Rewards.attribute("RewardItemCat3").as_int());
		pData->SetRewardItemIndex(2, Rewards.attribute("RewardItemIndex3").as_int());
		pData->SetRewardItemCount(2, Rewards.attribute("RewardItemCount3").as_int());
		pData->SetRewardType(Rewards.attribute("RewardCat").as_int());
		pData->SetRewardSubType(Rewards.attribute("RewardIndex").as_int());
		pData->SetRewardNumber(Rewards.attribute("RewardCount").as_int());

		this->m_QuestMUMap[pData->GetID()] = pData;
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, "Loaded %u quest mu definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

QuestEvolution const* CQuestMgr::GetQuest(uint8 quest_id) const
{
	QuestEvolutionMap::const_iterator itr = this->m_quest_evolution.find(quest_id);

	if ( itr != this->m_quest_evolution.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

QuestEvolution * CQuestMgr::GetQuest(uint8 quest_id)
{
	QuestEvolutionMap::iterator itr = this->m_quest_evolution.find(quest_id);

	if ( itr != this->m_quest_evolution.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

QuestGuided * CQuestMgr::GetQuestGuided(uint16 quest)
{
	QuestGuidedMap::const_iterator itr = this->m_quest_guided_map.find(quest);

	if ( itr != this->m_quest_guided_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}
	
QuestGuided const* CQuestMgr::GetQuestGuided(uint16 quest) const
{
	QuestGuidedMap::const_iterator itr = this->m_quest_guided_map.find(quest);

	if ( itr != this->m_quest_guided_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

uint16 CQuestMgr::GetFinalQuestGuided(Player* pPlayer) const
{
	if ( !pPlayer )
	{
		return 0;
	}

	uint16 id = 0;

	for ( QuestGuidedMap::const_iterator itr = sQuestMgr->m_quest_guided_map.begin(); itr != sQuestMgr->m_quest_guided_map.end(); ++itr )
	{
		QuestGuided const* pQuestGuided = itr->second;

		if ( !pQuestGuided )
		{
			continue;
		}

		if ( !pQuestGuided->IsClassAllowed(pPlayer->GetClass(), pPlayer->GetChangeUP(0), pPlayer->GetChangeUP(1), pPlayer->GetChangeUP(2)) )
		{
			continue;
		}

		if ( pQuestGuided->GetID() > id )
		{
			id = pQuestGuided->GetID();
		}
	}

	return id;
}

Quest4thInstanceData * CQuestMgr::Get4thQuestInstance()
{
	int32 count = this->GetCurrent4thQuestInstance();
	int32 total_count = 0;
	
	while ( true )
	{
		if ( !this->GetQuest4thInstanceData(count) )
		{
			Quest4thInstanceData * pData = new Quest4thInstanceData(count);
			this->m_quest_4th_instance_data_map[pData->GetInstance()] = pData;

			this->IncreaseCurrent4thQuestInstance(1);

			if ( this->GetCurrent4thQuestInstance() >= 100000 )
			{
				this->SetCurrent4thQuestInstance(0);
			}

			return pData;
		}

		++count;

		if ( count >= 100000 )
		{
			count = 0;
		}

		++total_count;

		if ( total_count >= 100000 )
		{
			break;
		}
	}

	return nullptr;
}

Quest4thInstanceData const* CQuestMgr::GetQuest4thInstanceData(int32 instance) const
{
	Quest4thInstanceDataMap::const_iterator itr = this->m_quest_4th_instance_data_map.find(instance);

	if ( itr != this->m_quest_4th_instance_data_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}
	
Quest4thInstanceData * CQuestMgr::GetQuest4thInstanceData(int32 instance)
{
	Quest4thInstanceDataMap::const_iterator itr = this->m_quest_4th_instance_data_map.find(instance);

	if ( itr != this->m_quest_4th_instance_data_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

void CQuestMgr::Update4thQuestInstance()
{
	for ( Quest4thInstanceDataMap::iterator itr = this->m_quest_4th_instance_data_map.begin(); itr != this->m_quest_4th_instance_data_map.end(); )
	{
		Quest4thInstanceData * pData = itr->second;

		if ( !pData )
		{
			this->m_quest_4th_instance_data_map.erase(itr++);
			continue;
		}

		if ( sObjectMgr->GetPlayerCountOnInstance(WORLD_PLACE_OF_QUALIFICATION, itr->first) <= 0 )
		{
			delete itr->second;
			this->m_quest_4th_instance_data_map.erase(itr++);
		}
		else
		{
			if ( pData->GetState() == 3 && pData->GetQuestID() == 8 )
			{
				if ( pData->GetTime()->Elapsed(MINUTE * IN_MILLISECONDS) )
				{
					this->Change4thQuestInstanceState(pData->GetInstance(), 0, 0);
				}
			}
			
			++itr;
		}
	}
}

void CQuestMgr::Change4thQuestInstanceState(int32 instance, uint8 state, uint8 quest_id)
{
	Quest4thInstanceData * pInstanceData = this->GetQuest4thInstanceData(instance);

	if ( !pInstanceData )
	{
		return;
	}

	pInstanceData->SetState(state);
	pInstanceData->SetQuestID(quest_id);
	pInstanceData->GetTime()->Start();
	
	switch ( pInstanceData->GetState() )
	{
	case 0: ///- None, recien aparecen en el mapa
		{
			Monster* pMonster = sObjectMgr->MonsterTryAdd(766, WORLD_PLACE_OF_QUALIFICATION);

			if ( pMonster )
			{
				pMonster->SetWorldId(WORLD_PLACE_OF_QUALIFICATION);
				pMonster->SetBasicLocation(147, 29, 147, 29);
				pMonster->SetInstance(pInstanceData->GetInstance());
				pMonster->SetDirection(2);
				pMonster->SetNpcFunction("npc_talk_quest_evolution");
				pMonster->SetScriptName("cent_quest_ai");

				pMonster->AddToWorld();

				sLog->outInfo(LOG_QUEST, "%s -- [%d] Added Quest NPC", __FUNCTION__, pInstanceData->GetInstance());

				this->Change4thQuestInstanceState(pInstanceData->GetInstance(), 1, 0);
			}
		} break;

	case 1: ///- StandBy, no ha clickeado al NPC
		{
		} break;

	case 2: ///- Ready, clickeo al NPC, procesar el pedido en base a la quest
		{
			QuestEvolution const* pQuest = sQuestMgr->GetQuest(pInstanceData->GetQuestID());

			if ( !pQuest )
			{
				return;
			}

			bool success = false;

			for ( QuestEvolutionDataList::const_iterator itr = pQuest->data.begin(); itr != pQuest->data.end(); ++itr )
			{
				QuestEvolutionData const* pData = *itr;

				if ( !pData )
				{
					continue;
				}

				if ( pData->GetObjectiveType() != QUEST_EVOLUTION_OBJECTIVE_MONSTER )
				{
					continue;
				}
					
				int32 loop = 1;

				if ( pInstanceData->GetQuestID() == 8 )
				{
					loop = 3;
				}

				for ( int32 i = 0; i < loop; ++i )
				{
					Monster* pMonster = sObjectMgr->MonsterTryAdd(pData->GetMonsterID(), WORLD_PLACE_OF_QUALIFICATION);

					if ( pMonster )
					{
						pMonster->SetWorldId(WORLD_PLACE_OF_QUALIFICATION);
						pMonster->SetBasicLocation(147, 29, 147, 29);
						pMonster->SetInstance(pInstanceData->GetInstance());
						pMonster->SetDirection(2);
						pMonster->SetScriptName(pInstanceData->GetQuestID() == 8 ? "4th_monster_quest_ai" : "cent_monster_quest_ai");
						pMonster->SetRespawnType(pInstanceData->GetQuestID() == 8 ? GAME_OBJECT_RESPAWN_NORMAL : GAME_OBJECT_RESPAWN_DELETE);
						pMonster->SetMoveDistance(10);

						pMonster->AddToWorld();
						success = true;

						sLog->outInfo(LOG_QUEST, "%s -- [%d] [%u - %s] Added Quest Monster [%u - %s]", __FUNCTION__, pInstanceData->GetInstance(), pQuest->GetID(), pQuest->GetName().c_str(), 
							pMonster->GetClass(), pMonster->GetName());
					}
				}
			}

			if ( success )
			{
				this->Change4thQuestInstanceState(pInstanceData->GetInstance(), 3, pInstanceData->GetQuestID());

				if ( pInstanceData->GetQuestID() == 8 )
				{
					QUEST_SURVIVAL_TIMER pMsg;
					pMsg.time = MINUTE * IN_MILLISECONDS;
					pMsg.type = 5;
					pMsg.increase = 0;

					sObjectMgr->SendPacketToInstance(WORLD_PLACE_OF_QUALIFICATION, pInstanceData->GetInstance(), (uint8*)&pMsg, pMsg.h.get_size());
				}
			}
		} break;

	case 3: ///- Process, procesar el tiempo
		{
			if ( pInstanceData->GetQuestID() == 8 && pInstanceData->GetTime()->Elapsed(MINUTE * IN_MILLISECONDS) )
			{
				this->Change4thQuestInstanceState(pInstanceData->GetInstance(), 0, 0);
			}
		} break;
	}
}

QuestMU const* CQuestMgr::GetQuestMU(uint16 id) const
{
	QuestMUMap::const_iterator itr = this->m_QuestMUMap.find(id);

	if (itr != this->m_QuestMUMap.end())
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

void CQuestMgr::GiveQuestEvolutionItemReward(Player* pPlayer, uint8 quest_id)
{
	for (QuestEvolutionRewardList::const_iterator itr = this->m_QuestEvolutionRewardList.begin(); itr != this->m_QuestEvolutionRewardList.end(); ++itr)
	{
		QuestEvolutionReward const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetQuestID() != quest_id)
		{
			continue;
		}

		if (!pData->IsClassFlag(1 << pPlayer->GetClass()))
		{
			continue;
		}

		INIT_SOCKET_DATA(socket);
		SOCKET_SLOT_LOOP(i)
		{
			socket[i] = pData->GetSocket(i);
		}

		if (pData->GetItem() == MUUN_EVOLUTION_STONE)
		{
			uint16 muun = ITEMGET(socket[0], socket[1]);

			socket[0] = HIBYTE(muun);
			socket[1] = muun % MAX_SUBTYPE_ITEMS;
		}

		sItemMgr->ItemSerialCreateItem(pPlayer, serial_create_inventory, Item(pData->GetItem(), pData->GetLevel(), pData->GetDurability(), pData->GetSkill(), pData->GetLuck(), pData->GetOption(), pData->GetExcellent(), pData->GetAncient(), socket, pData->GetSocketBonus()));
	}
}