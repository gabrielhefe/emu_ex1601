Dungeon::Dungeon()
{
	this->SetInstanceID(0);
}

Dungeon::~Dungeon()
{
	MAP_CLEAR(DungeonInstanceMap::iterator, this->m_DungeonInstanceMap);
	MAP_CLEAR(DungeonInstanceDataMap::iterator, this->m_DungeonInstanceDataMap);
	MAP_CLEAR(DungeonInstanceLevelMap::iterator, this->m_DungeonInstanceLevelMap);
	MAP_CLEAR(DungeonInstanceSavedMap::iterator, this->m_DungeonInstanceSavedMap);
}

void Dungeon::LoadSavedInstance()
{
	if (!sGameServer->IsDungeonEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Dungeon Saved Instances...");

	uint32 count = 0;

	MAP_CLEAR(DungeonInstanceSavedMap::iterator, this->m_DungeonInstanceSavedMap);


	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_DUNGEON_INSTANCE_SELECT);

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			DungeonInstanceSaved * pData = new DungeonInstanceSaved;
			pData->SetCharID(fields[loop++].GetUInt32());
			pData->SetID(fields[loop++].GetUInt8());
			pData->SetTime(fields[loop++].GetInt64());
			
			this->m_DungeonInstanceSavedMap[pData->GetCharID()] = pData;
			count++;
		} while (result->NextRow());
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u dungeon saved instances definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void Dungeon::LoadIni() {
	if (boost::filesystem::exists(DUNGEON_FILEPATH)) {
		char dungeonrace[10];
		char dungeonracereward[255];
		sGameServer->SetDungeonEnabled(GetPrivateProfileInt(DUNGEON_SECTION, "Enabled", 0, DUNGEON_FILEPATH));
		sGameServer->SetDungeonDuration(GetPrivateProfileInt(DUNGEON_SECTION, "Duration", 0, DUNGEON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetDungeonEndGate(GetPrivateProfileInt(DUNGEON_SECTION, "EndGate", 0, DUNGEON_FILEPATH));
		sGameServer->SetDungeonGAP(GetPrivateProfileInt(DUNGEON_SECTION, "GAP", 0, DUNGEON_FILEPATH));
		sGameServer->SetDungeonMaxPerPC(GetPrivateProfileInt(DUNGEON_SECTION, "MaxPerPC", 0, DUNGEON_FILEPATH));
		sGameServer->SetDungeonMinParty(GetPrivateProfileInt(DUNGEON_SECTION, "MinParty", 0, DUNGEON_FILEPATH));
		sGameServer->SetDungeonSilverChestMonster(GetPrivateProfileInt(DUNGEON_SECTION, "SilverChestMonster", 0, DUNGEON_FILEPATH));
		sGameServer->SetDungeonSimpleTrapDamage(GetPrivateProfileInt(DUNGEON_SECTION, "SimpleTrapDamage", 0, DUNGEON_FILEPATH));
		sGameServer->SetDungeonSimpleTrapTime(GetPrivateProfileInt(DUNGEON_SECTION, "SimpleTrapTime", 0, DUNGEON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetDungeonWaitTime(GetPrivateProfileInt(DUNGEON_SECTION, "WaitTime", 0, DUNGEON_FILEPATH));

		GetPrivateProfileString(DUNGEON_RACE_SECTION, "Enabled", "", dungeonrace, sizeof(dungeonrace), DUNGEON_FILEPATH);
		GetPrivateProfileString(DUNGEON_RACE_SECTION, "Reward", "", dungeonracereward, sizeof(dungeonracereward), DUNGEON_FILEPATH);

		sGameServer->SetDungeonRaceEnabled(IsServerIN(dungeonrace));
		sGameServer->SetDungeonRaceReward(dungeonracereward);

	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Dungeon.ini File");
		MessageBox(NULL, "Error Load Dungeon.ini File", "Error", ERROR);
		exit(0);
	}
}

void Dungeon::LoadInstance(char* pchFileName)
{
	if (!sGameServer->IsDungeonEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Dungeon Instances...");

	MAP_CLEAR(DungeonInstanceDataMap::iterator, this->m_DungeonInstanceDataMap);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("EventDungeon");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Event"); data; data = data.next_sibling()) {
		DungeonInstanceData* pData = new DungeonInstanceData();
		pData->SetID(data.attribute("ID").as_int());
		pData->SetWorld(data.attribute("WorldID").as_int());
		pData->SetFlags(data.attribute("Flags").as_int());
		pData->SetGate(data.attribute("Gate").as_int());
		pData->SetMainChestX(data.attribute("MainChestX").as_int());
		pData->SetMainChestY(data.attribute("MainChestY").as_int());

		pData->SetChestX(0, data.attribute("ChestX1").as_int());
		pData->SetChestY(0, data.attribute("ChestY1").as_int());
		pData->SetChestX(1, data.attribute("ChestX2").as_int());
		pData->SetChestY(1, data.attribute("ChestY2").as_int());
		pData->SetChestX(2, data.attribute("ChestX3").as_int());
		pData->SetChestY(2, data.attribute("ChestY3").as_int());
		pData->SetChestX(3, data.attribute("ChestX4").as_int());
		pData->SetChestY(3, data.attribute("ChestY4").as_int());

		this->m_DungeonInstanceDataMap[pData->GetID()] = pData;
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u dungeon instances definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void Dungeon::LoadLevel(char* pchFileName)
{
	if (!sGameServer->IsDungeonEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Dungeon Level...");

	MAP_CLEAR(DungeonInstanceLevelMap::iterator, this->m_DungeonInstanceLevelMap);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("EventDungeonLevel");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		DungeonInstanceLevel* pData = new DungeonInstanceLevel();
		pData->SetID(data.attribute("ID").as_int());
		pData->SetMinLevel(data.attribute("LevelMin").as_int());
		pData->SetMaxLevel(data.attribute("LevelMax").as_int());
		pData->SetLevel(data.attribute("Level").as_int());
		pData->SetHP(data.attribute("HP").as_int());
		pData->SetDamage(data.attribute("Damage").as_int());
		pData->SetDefense(data.attribute("Def").as_int());
		pData->SetAttackSuccess(data.attribute("AttackSucces").as_int());
		pData->SetDefenseSuccess(data.attribute("DefSucces").as_int());
		pData->SetElementalDamage(data.attribute("ElementalDamage").as_int());
		pData->SetElementalDefense(data.attribute("ElementalDef").as_int());
		pData->SetElementalAttackSuccess(data.attribute("ElementalAttackSucces").as_int());
		pData->SetElementalDefenseSuccess(data.attribute("ElementalDefSucces").as_int());
		pData->SetItemBag(data.attribute("ItemBag").as_string());
		pData->SetItemBagBoss(data.attribute("BossItemBag").as_string());

		this->m_DungeonInstanceLevelMap[pData->GetID()] = pData;
		count++;
	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM event_dungeon_level");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			DungeonInstanceLevel * pData = new DungeonInstanceLevel();
			pData->SetID(fields[loop++].GetUInt16());
			pData->SetMinLevel(fields[loop++].GetInt16());
			pData->SetMaxLevel(fields[loop++].GetInt16());
			pData->SetLevel(fields[loop++].GetFloat());
			pData->SetHP(fields[loop++].GetFloat());
			pData->SetDamage(fields[loop++].GetFloat());
			pData->SetDefense(fields[loop++].GetFloat());
			pData->SetAttackSuccess(fields[loop++].GetFloat());
			pData->SetDefenseSuccess(fields[loop++].GetFloat());
			pData->SetElementalDamage(fields[loop++].GetFloat());
			pData->SetElementalDefense(fields[loop++].GetFloat());
			pData->SetElementalAttackSuccess(fields[loop++].GetFloat());
			pData->SetElementalDefenseSuccess(fields[loop++].GetFloat());
			pData->SetItemBag(fields[loop++].GetString());
			pData->SetItemBagBoss(fields[loop++].GetString());
			
			this->m_DungeonInstanceLevelMap[pData->GetID()] = pData;
			count++;
		} while (result->NextRow());
	}*/

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u dungeon level definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

DungeonInstanceData const* Dungeon::GetRandomDungeon() const
{
	RandomValue<DungeonInstanceData const*> m_RandomValue(nullptr);

	for (DungeonInstanceDataMap::const_iterator itr = this->m_DungeonInstanceDataMap.begin(); itr != this->m_DungeonInstanceDataMap.end(); ++itr)
	{
		DungeonInstanceData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		m_RandomValue.AddValue(pData, 0);
	}

	return m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);
}

DungeonInstanceData const* Dungeon::GetInstanceData(uint8 id) const
{
	DungeonInstanceDataMap::const_iterator itr = this->m_DungeonInstanceDataMap.find(id);

	if (itr != this->m_DungeonInstanceDataMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

DungeonInstanceLevel const* Dungeon::GetInstanceLevel(uint16 level) const
{
	DungeonInstanceLevelMap::const_iterator itr = this->m_DungeonInstanceLevelMap.find(level);

	if (itr != this->m_DungeonInstanceLevelMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

DungeonInstanceLevel const* Dungeon::GetInstanceByLevel(int16 level) const
{
	for (DungeonInstanceLevelMap::const_iterator itr = this->m_DungeonInstanceLevelMap.begin(); itr != this->m_DungeonInstanceLevelMap.end(); ++itr)
	{
		DungeonInstanceLevel const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (level >= pData->GetMinLevel() && level <= pData->GetMaxLevel())
		{
			return pData;
		}
	}

	return nullptr;
}

bool Dungeon::IsDungeon(uint16 world) const
{
	for (DungeonInstanceDataMap::const_iterator itr = this->m_DungeonInstanceDataMap.begin(); itr != this->m_DungeonInstanceDataMap.end(); ++itr)
	{
		DungeonInstanceData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetWorld() == world)
		{
			return true;
		}
	}

	return false;
}

void Dungeon::GetDungeonRespawn(Player* pPlayer, uint16 & world, int16 & x, int16 & y)
{
	for (DungeonInstanceDataMap::const_iterator itr = this->m_DungeonInstanceDataMap.begin(); itr != this->m_DungeonInstanceDataMap.end(); ++itr)
	{
		DungeonInstanceData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetWorld() == world)
		{
			pPlayer->GetValidCoordinates(pData->GetGate(), world, x, y);
			return;
		}
	}
}

DungeonInstance * Dungeon::FindInstance(int32 id) const
{
	DungeonInstanceMap::const_iterator itr = this->m_DungeonInstanceMap.find(id);

	if (itr != this->m_DungeonInstanceMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

DungeonInstance * Dungeon::FindInstanceByParty(uint16 id) const
{
	for (DungeonInstanceMap::const_iterator itr = this->m_DungeonInstanceMap.begin(); itr != this->m_DungeonInstanceMap.end(); ++itr)
	{
		DungeonInstance * pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->GetPartyID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

DungeonInstanceSaved * Dungeon::FindPlayerInstance(uint32 id) const
{
	DungeonInstanceSavedMap::const_iterator itr = this->m_DungeonInstanceSavedMap.find(id);

	if (itr != this->m_DungeonInstanceSavedMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

void Dungeon::Update()
{
	for (DungeonInstanceMap::const_iterator itr = this->m_DungeonInstanceMap.begin(); itr != this->m_DungeonInstanceMap.end();)
	{
		DungeonInstance * pData = itr->second;

		if (!pData)
		{
			this->m_DungeonInstanceMap.erase(itr++);
			continue;
		}

		if (pData->IsEmpty())
		{
			delete itr->second;
			this->m_DungeonInstanceMap.erase(itr++);
			continue;
		}

		pData->Update();
		++itr;
	}
}

void Dungeon::EnterRequest(Player* pPlayer)
{
	if (!Player::IsPlayerBasicState(pPlayer, false))
	{
		return;
	}

	if (!sGameServer->IsDungeonEnabled())
	{
		pPlayer->SendMessageBox(0, "Instanced Dungeon", "Instanced Dungeon system is disabled.");
		return;
	}

	time_t current_time = time(nullptr);

	DungeonInstanceSaved * pSavedData = this->FindPlayerInstance(pPlayer->GetGUID());

	if (pSavedData)
	{
		if ((pSavedData->GetTime() + sGameServer->GetDungeonWaitTime()) > current_time)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "You will be able to enter again in %s", secsToTimeString((pSavedData->GetTime() + sGameServer->GetDungeonWaitTime()) - current_time).c_str());
			return;
		}
	}

	Party* pParty = pPlayer->GetParty();

	if (!pParty)
	{
		pPlayer->SendMessageBox(0, "Instanced Dungeon", "You need to be in Party.");
		return;
	}

	if (sGameServer->GetDungeonMaxPerPC() > 0)
	{
		if (this->GetCountByPC(pPlayer) >= sGameServer->GetDungeonMaxPerPC())
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "You can only participate with %d character at the same time.", sGameServer->GetDungeonMaxPerPC());
			return;
		}
	}

	DungeonInstance* pInstance = this->FindInstanceByParty(pParty->GetID());

	if (!pInstance)
	{
		DungeonInstanceLevel const* pLevelData = this->GetInstanceByLevel(pPlayer->GetTotalLevel());

		if (!pLevelData)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "There is not suitable Dungeon level for you.");
			return;
		}

		DungeonInstanceData const* pData = this->GetRandomDungeon();

		if (!pData)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "Can't find Dungeon database information.");
			return;
		}

		this->IncreaseInstanceID(1);

		pInstance = new DungeonInstance();
		pInstance->SetInstance(this->GetInstanceID());
		pInstance->SetID(pData->GetID());
		pInstance->SetLevelID(pLevelData->GetID());
		pInstance->SetPartyID(pParty->GetID());
		pInstance->SetWorld(pData->GetWorld());
		pInstance->GetTime()->Start(sGameServer->GetDungeonDuration());

		pInstance->AddMonster();

		this->m_DungeonInstanceMap[pInstance->GetInstance()] = pInstance;
	}
	else
	{
		if (pInstance->GetBossKilled() != 0)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "Your party instance has already finished.");
			return;
		}

		DungeonInstanceLevel const* pLevelData = this->GetInstanceLevel(pInstance->GetLevelID());

		if (!pLevelData)
		{
			pPlayer->SendMessageBox(0, "Instanced Dungeon", "Instance level error.");
			return;
		}

		if (pPlayer->GetTotalLevel() < pLevelData->GetMinLevel())
		{
			int32 diff = pLevelData->GetMinLevel() - pPlayer->GetTotalLevel();

			if (diff > sGameServer->GetDungeonGAP())
			{
				pPlayer->SendMessageBox(0, "Instanced Dungeon", "Your level is too low to enter this instance.");
				return;
			}
		}

		if (pPlayer->GetTotalLevel() > pLevelData->GetMaxLevel())
		{
			int32 diff = pPlayer->GetTotalLevel() - pLevelData->GetMaxLevel();

			if (diff > sGameServer->GetDungeonGAP())
			{
				pPlayer->SendMessageBox(0, "Instanced Dungeon", "Your level is too high to enter this instance.");
				return;
			}
		}
	}

	pInstance->AddPlayer(pPlayer);
}

void Dungeon::SaveInstance(uint32 char_id, uint8 id, int64 time)
{
	DungeonInstanceSaved* pData = this->FindPlayerInstance(char_id);

	if (!pData)
	{
		pData = new DungeonInstanceSaved;
		pData->SetCharID(char_id);

		this->m_DungeonInstanceSavedMap[pData->GetCharID()] = pData;
	}

	pData->SetID(id);
	pData->SetTime(time);
}

int32 Dungeon::GetCountByPC(Player* pPlayer) const
{
	int32 count = 0;

	for (DungeonInstanceMap::const_iterator itr = this->m_DungeonInstanceMap.begin(); itr != this->m_DungeonInstanceMap.end(); ++itr)
	{
		DungeonInstance * pData = itr->second;

		if (!pData)
		{
			continue;
		}

		count += pData->GetCountByPC(pPlayer);
	}

	return count;
}