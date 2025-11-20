/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ImperialFortress.cpp"
*
*/

ImperialFortressMgr::ImperialFortressMgr()
{
	this->SetInstanceID(0);
}

ImperialFortressMgr::~ImperialFortressMgr()
{
	for (ImperialFortressTrapMap::iterator itr = this->m_ImperialFortressTrapMap.begin(); itr != this->m_ImperialFortressTrapMap.end(); ++itr)
	{
		LIST_CLEAR(ImperialFortressTrapList::iterator, itr->second);
	}

	this->m_ImperialFortressTrapMap.clear();

	for (ImperialFortressLevelMap::iterator it = this->m_level.begin(); it != this->m_level.end(); ++it)
	{
		LIST_CLEAR(ImperialFortressMonsterLevelList::iterator, it->second->monsters);
		delete it->second;
	}

	this->m_level.clear();

	MAP_CLEAR(ImperialFortressMap::iterator, this->m_ImperialFortressMap);
	LIST_CLEAR(ImperialFortressCharacterInstanceList::iterator, this->m_ImperialFortressCharacterInstanceList);
}

void ImperialFortressMgr::LoadIni() {
	if (boost::filesystem::exists(FORTRESS_FILEPATH)) {
		sGameServer->SetImperialFortressEnabled(GetPrivateProfileInt(FORTRESS_SECTION, "Enabled", 0, FORTRESS_FILEPATH));
		sGameServer->SetImperialFortressPKCheck(GetPrivateProfileInt(FORTRESS_SECTION, "PKCheck", 0, FORTRESS_FILEPATH));
		sGameServer->SetImperialFortressPKLevelMax(GetPrivateProfileInt(FORTRESS_SECTION, "PKLevelMax", 0, FORTRESS_FILEPATH));
		sGameServer->SetImperialFortressInParty(GetPrivateProfileInt(FORTRESS_SECTION, "InParty", 0, FORTRESS_FILEPATH));
		sGameServer->SetImperialFortressStandbyTime(GetPrivateProfileInt(FORTRESS_SECTION, "StandbyTime", 0, FORTRESS_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetImperialFortressPlayTime(GetPrivateProfileInt(FORTRESS_SECTION, "PlayTime", 0, FORTRESS_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetImperialFortressEndTime(GetPrivateProfileInt(FORTRESS_SECTION, "EndTime", 0, FORTRESS_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetImperialFortressDay(GetPrivateProfileInt(FORTRESS_SECTION, "Day", -1, FORTRESS_FILEPATH));
		sGameServer->SetImperialFortressTimeZone(0, GetPrivateProfileInt(FORTRESS_SECTION, "TimeZone1", 0, FORTRESS_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetImperialFortressTimeZone(1, GetPrivateProfileInt(FORTRESS_SECTION, "TimeZone2", 0, FORTRESS_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetImperialFortressTimeZone(2, GetPrivateProfileInt(FORTRESS_SECTION, "TimeZone3", 0, FORTRESS_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetImperialFortressTimeZone(3, GetPrivateProfileInt(FORTRESS_SECTION, "TimeZone4", 0, FORTRESS_FILEPATH) * IN_MILLISECONDS);

		sGameServer->SetImperialFortressWalkCheck(GetPrivateProfileInt(FORTRESS_SECTION, "WalkCheck", 0, FORTRESS_FILEPATH));
		sGameServer->SetImperialFortressTeleportCheck(GetPrivateProfileInt(FORTRESS_SECTION, "TeleportCheck", 0, FORTRESS_FILEPATH));
		sGameServer->SetImperialFortressMoveToPrevious(GetPrivateProfileInt(FORTRESS_SECTION, "MoveToPrevious", 0, FORTRESS_FILEPATH));
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load ImperialFortress.ini File");
		MessageBox(NULL, "Error Load ImperialFortress.ini File", "Error", ERROR);
		exit(0);
	}
}

void ImperialFortressMgr::LoadImperialFortressLevel(char* pchFileName)
{
	for (ImperialFortressLevelMap::iterator it = this->m_level.begin(); it != this->m_level.end(); ++it)
	{
		LIST_CLEAR(ImperialFortressMonsterLevelList::iterator, it->second->monsters);
		delete it->second;
	}

	this->m_level.clear();

	if (!sGameServer->IsImperialFortressEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Imperial Fortress Level...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("EventImperialFortressLevel");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		ImperialFortressLevel* level = new ImperialFortressLevel;
		level->guid.set(data.attribute("ID").as_int());
		level->SetLevelMin(data.attribute("LevelMin").as_int());
		level->SetLevelMax(data.attribute("LevelMax").as_int());
		level->experience.set(data.attribute("Exp").as_int());

		this->m_level[level->guid.get()] = level;

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u imperial fortress level definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void ImperialFortressMgr::LoadImperialFortressMonsterSettings(char* pchFileName)
{
	if (!sGameServer->IsImperialFortressEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Imperial Fortress Monster Settings...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("EventImperialFortressMonsterSettings");

	uint32 count = 0;

	for (pugi::xml_node level = Main.child("EventLevel"); level; level = level.next_sibling()) {
		for (pugi::xml_node data = level.child("Data"); data; data = data.next_sibling()) {
			if (ImperialFortressLevel* level_ = this->GetLevel(level.attribute("Level").as_int()))
			{
				ImperialFortressMonsterLevel* monster = new ImperialFortressMonsterLevel;
				monster->monster.set(data.attribute("MonsterID").as_int());
				monster->level.set(data.attribute("Level").as_int());

				monster->power[0].set(data.attribute("Life").as_int());
				monster->power[1].set(data.attribute("Mana").as_int());
				monster->power[2].set(data.attribute("Shield").as_int());
				monster->power[3].set(data.attribute("Stamina").as_int());

				monster->attack_min.set(data.attribute("AttackMin").as_int());
				monster->attack_max.set(data.attribute("AttackMax").as_int());
				monster->attack_rate.set(data.attribute("AttackRate").as_int());
				monster->defense.set(data.attribute("Def").as_int());
				monster->defense_rate.set(data.attribute("DefRate").as_int());
				monster->SetItemBag(data.attribute("ItemBag").as_string());

				level_->monsters.push_back(monster);
			}

			count++;
		}
	}
	/*QueryResult result = GameServerDatabase.PQuery("SELECT * FROM event_imperial_fortress_monster_settings");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			if (ImperialFortressLevel * level_ = this->GetLevel(fields[0].GetUInt32()))
			{
				ImperialFortressMonsterLevel * monster = new ImperialFortressMonsterLevel;
				monster->monster.set(fields[1].GetUInt16());
				monster->level.set(fields[2].GetInt16());

				for (uint8 i = 0; i < POWER_MAX; ++i)
					monster->power[i].set(fields[3 + i].GetInt32());

				monster->attack_min.set(fields[7].GetInt32());
				monster->attack_max.set(fields[8].GetInt32());
				monster->attack_rate.set(fields[9].GetInt32());
				monster->defense.set(fields[10].GetInt32());
				monster->defense_rate.set(fields[11].GetInt32());
				monster->SetItemBag(fields[12].GetString());

				level_->monsters.push_back(monster);
			}

			count++;
		} while (result->NextRow());
	}*/

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u imperial fortress monster settings", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void ImperialFortressMgr::LoadImperialFortressTraps(char* pchFileName)
{
	for (ImperialFortressTrapMap::iterator itr = this->m_ImperialFortressTrapMap.begin(); itr != this->m_ImperialFortressTrapMap.end(); ++itr)
	{
		LIST_CLEAR(ImperialFortressTrapList::iterator, itr->second);
	}

	this->m_ImperialFortressTrapMap.clear();

	if (!sGameServer->IsImperialFortressEnabled())
	{
		return;
	}

	sLog->outInfo(LOG_DEFAULT, "Loading Imperial Fortress Traps...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("EventImperialFortressTrap");

	uint32 count = 0;

	for (pugi::xml_node trap = Main.child("FortressTrap"); trap; trap = trap.next_sibling()) {
		for (pugi::xml_node data = trap.child("Data"); data; data = data.next_sibling()) {
			uint8 day = trap.attribute("ID").as_int();

			ImperialFortressTrap* trap = new ImperialFortressTrap;
			trap->SetX1(data.attribute("X1").as_int());
			trap->SetY1(data.attribute("Y1").as_int());
			trap->SetX2(data.attribute("X2").as_int());
			trap->SetY2(data.attribute("Y2").as_int());
			trap->SetDamage(data.attribute("Damage").as_int());
			trap->SetElement(data.attribute("Element").as_int());

			ImperialFortressTrapList& list = this->m_ImperialFortressTrapMap[day];

			list.push_back(trap);

			count++;
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u imperial fortress traps", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

ImperialFortressLevel * ImperialFortressMgr::GetLevel(uint32 guid) const
{
	ImperialFortressLevelMap::const_iterator it = this->m_level.find(guid);

	if (it != this->m_level.end())
	{
		return it->second;
	}

	return nullptr;
}

void ImperialFortressMgr::EnterRequest(Player* pPlayer)
{
	if (!Player::IsPlayerBasicState(pPlayer))
	{
		return;
	}

	if (!sGameServer->IsImperialFortressEnabled())
	{
		pPlayer->SendMessageBox(1, "Imperial Fortress", "Event is disabled.");
		return;
	}

	if (!Player::IsPlayerTransactionFree(pPlayer))
	{
		pPlayer->SendMessageBox(1, "Imperial Fortress", "Close any transaction or Personal Store before enter.");
		return;
	}
	///- Don't allow entrance if pk check is enabled and pk level is higher than allowed.
	if (sGameServer->IsImperialFortressPKCheck() && pPlayer->GetPKLevel() > sGameServer->GetImperialFortressPKLevelMax())
	{
		this->EnterResult(pPlayer, 0x03);
		return;
	}
	///- Don't allow entrance if party is required and it is not in party
	if (sGameServer->IsImperialFortressInParty() && pPlayer->GetPartyID() == PARTY_NULL)
	{
		this->EnterResult(pPlayer, 0x05);
		return;
	}
	///- Don't allow entrance if it reached maximum entrance per day
	if (!sEventMgr->CanEnterEvent(pPlayer, EVENT_IMPERIAL_FORTRESS))
	{
		this->EnterResult(pPlayer, 0x08);
		return;
	}

	if (this->GetInstanceByCharacter(pPlayer->GetGUID()))
	{
		//this->EnterResult(pPlayer, 0x01);
		pPlayer->SendMessageBox(1, "Imperial Fortress", "Your instance is currently active.");
		return;
	}

	ImperialFortressInstance * pInstance = this->GetInstanceByParty(pPlayer->GetPartyID());

	///- Don't allow entrance if it's party group is inside event
	if (pInstance)
	{
		if (pInstance->GetZone() != 0)
		{
			//this->EnterResult(pPlayer, 0x01);
			pPlayer->SendMessageBox(1, "Imperial Fortress", "Your party instance is currently active and you can't join.");
			return;
		}

		if (pInstance->GetState() != IMPERIAL_FORTRESS_STATE_STANDBY)
		{
			//this->EnterResult(pPlayer, 0x01);
			pPlayer->SendMessageBox(1, "Imperial Fortress", "Your party instance is currently active and you can't join.");
			return;
		}

		if (!pInstance->FindPlayer(pPlayer))
		{
			//this->EnterResult(pPlayer, 0x01);
			pPlayer->SendMessageBox(1, "Imperial Fortress", "You don't belong to your party event instance.");
			return;
		}

		///- Don't allow entrance if it doesn't have ticket
		if (!this->FindTicket(pPlayer, pInstance->GetDay(), false))
		{
			this->EnterResult(pPlayer, 0x02);
			return;
		}

		this->FindTicket(pPlayer, pInstance->GetDay(), true);
	}
	else
	{
		uint32 level = this->EnterPartyLevel(pPlayer);

		if (level == uint32(-1))
		{
			pPlayer->SendMessageBox(1, "Imperial Fortress", "There is no suitable area for your party level.");
			return;
		}

		uint8 day = sGameServer->GetImperialFortressDay() == uint8(-1) ? this->GetDayOfWeek() : sGameServer->GetImperialFortressDay();

		///- Don't allow entrance if it doesn't have ticket
		if (!this->FindTicket(pPlayer, day, false))
		{
			this->EnterResult(pPlayer, 0x02);
			return;
		}

		pInstance = this->CreateInstance();

		pInstance->SetDay(day);
		pInstance->SetWeather(Random(4));
		pInstance->SetZoneLevel(level);
		pInstance->SetPartyID(pPlayer->GetPartyID());

		this->FindTicket(pPlayer, day, true);

		Party* pParty = pPlayer->GetParty();

		if (pParty)
		{
			for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
			{
				pInstance->GetPlayer(i)->SetGUID(pParty->GetMember(i)->GetGUID());
			}
		}
		else
		{
			pInstance->GetPlayer(0)->SetGUID(pPlayer->GetGUID());
		}

		pInstance->SetState_StandBy();
	}

	this->UpdatePlayer(pInstance->GetID(), pPlayer);
	this->EnterResult(pPlayer, 0, pInstance->GetDay() + 1, pInstance->GetZone() + 1, pInstance->GetWeather(), pInstance->GetTime()->GetRemain() / IN_MILLISECONDS);

	if (pPlayer->MoveToGate(g_ImperialFortressGates[pInstance->GetDay()][pInstance->GetZone()].gate))
	{
		pPlayer->SetEventId(EVENT_IMPERIAL_FORTRESS);
		pPlayer->SetEventGround(pInstance->GetDay());
		pPlayer->SetInstance(pInstance->GetID());

		this->AddCharacterInstance(pInstance->GetID(), pPlayer);

		sEventMgr->EventEnter(pPlayer, EVENT_IMPERIAL_FORTRESS);
	}
}

void ImperialFortressMgr::EnterResult(Player* pPlayer, uint8 result, uint8 day, uint8 state, uint8 weather, uint32 time)
{
	IMPERIAL_FORTRESS_ENTER_RESULT pMsg(result, day, state, weather, time);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

uint32 ImperialFortressMgr::EnterPartyLevel(Player* pPlayer)
{
	int16 party_level = pPlayer->GetTotalLevel();

	if (Party const* pParty = pPlayer->GetParty())
	{
		PARTY_LOOP(i)
		{
			if (pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING)
			{
				continue;
			}

			Player* pMember = pParty->GetMember(i)->GetPlayer();

			if (!pMember)
			{
				continue;
			}

			if (party_level < pMember->GetTotalLevel() || party_level == -1)
			{
				party_level = pMember->GetTotalLevel();
			}
		}
	}

	for (ImperialFortressLevelMap::const_iterator it = this->m_level.begin(); it != this->m_level.end(); ++it)
	{
		if (party_level >= it->second->GetLevelMin() && party_level <= it->second->GetLevelMax())
		{
			return it->first;
		}
	}

	return -1;
}

uint8 ImperialFortressMgr::GetDayOfWeek()
{
	Custom::SystemTimer m_time = Custom::SystemTimer();

	return m_time.GetDayOfWeekMod();
}

bool ImperialFortressMgr::FindTicket(Player* pPlayer, uint8 day, bool consume)
{
	uint16 entrance = day == 6 ? ITEMGET(14, 109) : ITEMGET(14, 102);
	uint16 ticket = day == 6 ? ITEMGET(13, 127) : ITEMGET(13, 126);

	use_inventory_loop(i)
	{
		if (!pPlayer->GetInventory()->CanUseExpandedSlot(i))
		{
			continue;
		}

		if (!pPlayer->GetInventory()->GetItem(i)->IsItem())
			continue;

		if (pPlayer->GetInventory()->GetItem(i)->GetItem() == entrance)
		{
			if (consume)
			{
				pPlayer->ClearItem(i);
			}
			return true;
		}

		if (pPlayer->GetInventory()->GetItem(i)->GetItem() == ticket && pPlayer->GetInventory()->GetItem(i)->GetDurability() >= 1.0f)
		{
			if (consume)
			{
				pPlayer->DecreaseItemDurabilityByUse(i, 1.0f);
			}
			return true;
		}
	}

	return false;
}

void ImperialFortressMgr::Update()
{
	for (ImperialFortressMap::const_iterator itr = this->m_ImperialFortressMap.begin(); itr != this->m_ImperialFortressMap.end();)
	{
		ImperialFortressInstance * pInstance = itr->second;

		if (!pInstance)
		{
			this->RemoveInstance(itr->first);

			this->m_ImperialFortressMap.erase(itr++);
			continue;
		}

		if (pInstance->IsReadyToRemove())
		{
			this->RemoveInstance(itr->first);

			delete itr->second;
			this->m_ImperialFortressMap.erase(itr++);
			continue;
		}

		pInstance->Update();
		++itr;
	}
}

bool ImperialFortressMgr::UpdatePlayerGate(Player* pPlayer, uint16 gate)
{
	if (!pPlayer)
	{
		return false;
	}

	ImperialFortressInstance* pInstance = this->GetInstanceByCharacter(pPlayer->GetGUID());

	if (!pInstance)
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Instance ID. Contact Administrators.");

		return false;
	}

	ImperialFortressPlayer* pInstancePlayer = pInstance->FindPlayer(pPlayer);

	if (!pInstancePlayer)
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Instance Player. Contact Administrators.");

		return false;
	}

	uint8 zone = pInstancePlayer->GetZone();
	uint8 next_zone = zone + 1;

	if (!IMPERIAL_FORTRESS_ZONE_RANGE(zone) || !IMPERIAL_FORTRESS_ZONE_RANGE(next_zone))
	{
		sLog->outError("imperial_fortress", "%s(Zone:%u / NextZone:%u) -- Wrong zone %s", __FUNCTION__, zone, next_zone, pPlayer->BuildLog().c_str());

		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong zone ID. Contact Administrators.");
		return false;
	}

	if (!pInstance->IsReadyToMove())
	{
		sLog->outError("imperial_fortress", "%s(Day:%u / Zone:%u) -- Wrong zone state %u / %s", __FUNCTION__, pInstance->GetDay(), pInstance->GetZone(), uint8(pInstance->GetState()), pPlayer->BuildLog().c_str());

		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to move to the next zone.");
		return false;
	}

	/*if ( this->GetZone(next_zone)->GetState() != IMPERIAL_FORTRESS_STATE_NONE && this->GetZone(next_zone)->GetState() != IMPERIAL_FORTRESS_STATE_STANDBY )
	{
	sLog->outError("imperial_fortress", "%s(Day:%u / Zone:%u) -- Wrong next_zone state %u / %s", __FUNCTION__, pInstance->GetDay(), zone, uint8(this->GetZone(next_zone)->GetState()), pPlayer->BuildLog().c_str());

	pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Next zone is not available yet. Please wait until it is released.");
	return false;
	}*/

	if (g_ImperialFortressGates[pInstance->GetDay()][next_zone].gate != gate)
	{
		sLog->outError("imperial_fortress", "%s(Day:%u / Zone:%u) -- Wrong gate %u / %s", __FUNCTION__, pInstance->GetDay(), zone, gate, pPlayer->BuildLog().c_str());

		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong gate ID. Contact Administrators.");
		return false;
	}

	if (pInstance->GetState() == IMPERIAL_FORTRESS_STATE_MOVE)
	{
		pInstance->SetZone(pInstance->GetZone() + 1);
		pInstance->SetState_StandBy();

		pInstance->GetTime()->Start(sGameServer->GetImperialFortressStandbyTime());
	}

	pInstancePlayer->SetZone(pInstance->GetZone());

	this->EnterResult(pPlayer, 0, pInstance->GetDay() + 1, pInstance->GetZone() + 1, pInstance->GetWeather(), pInstance->GetTime()->GetRemain() / IN_MILLISECONDS);

	return true;
}

ImperialFortressPlayer* ImperialFortressMgr::FindPlayer(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return nullptr;
	}

	for (ImperialFortressMap::const_iterator itr = this->m_ImperialFortressMap.begin(); itr != this->m_ImperialFortressMap.end(); ++itr)
	{
		ImperialFortressInstance * pInstance = itr->second;

		if (!pInstance)
		{
			continue;
		}

		for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
		{
			if (pInstance->GetPlayer(i)->GetGUID() == pPlayer->GetGUID())
			{
				return pInstance->GetPlayer(i);
			}
		}
	}

	return nullptr;
}

ImperialFortressPlayer* ImperialFortressMgr::FindPlayer(uint8 zone, Player* pPlayer)
{
	if (!pPlayer)
	{
		return nullptr;
	}

	ImperialFortressInstance * pInstance = this->GetInstanceByCharacter(pPlayer->GetGUID());

	if (!pInstance)
	{
		return nullptr;
	}

	return pInstance->FindPlayer(pPlayer);
}

void ImperialFortressMgr::UpdatePlayer(uint32 id, Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	ImperialFortressInstance* pInstance = this->GetInstance(id);

	if (!pInstance)
	{
		return;
	}

	for (int32 i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (pInstance->GetPlayer(i)->GetGUID() == pPlayer->GetGUID())
		{
			pInstance->GetPlayer(i)->SetPlayer(pPlayer);
			pInstance->GetPlayer(i)->SetZone(pInstance->GetZone());
			pInstance->GetPlayer(i)->SetWorld(pPlayer->GetWorldId());
			pInstance->GetPlayer(i)->SetX(pPlayer->GetX());
			pInstance->GetPlayer(i)->SetY(pPlayer->GetY());
		}
	}
}

ImperialFortressInstance * ImperialFortressMgr::GetInstance(uint32 id) const
{
	ImperialFortressMap::const_iterator itr = this->m_ImperialFortressMap.find(id);

	if (itr != this->m_ImperialFortressMap.end())
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

ImperialFortressInstance * ImperialFortressMgr::GetInstanceByParty(uint16 id) const
{
	if (id == PARTY_NULL)
	{
		return nullptr;
	}

	for (ImperialFortressMap::const_iterator itr = this->m_ImperialFortressMap.begin(); itr != this->m_ImperialFortressMap.end(); ++itr)
	{
		ImperialFortressInstance * pInstance = itr->second;

		if (!pInstance)
		{
			continue;
		}

		if (pInstance->GetPartyID() == id)
		{
			return pInstance;
		}
	}

	return nullptr;
}

ImperialFortressInstance * ImperialFortressMgr::GetInstanceByCharacter(uint32 id) const
{
	for (ImperialFortressCharacterInstanceList::const_iterator itr = this->m_ImperialFortressCharacterInstanceList.begin(); itr != this->m_ImperialFortressCharacterInstanceList.end(); ++itr)
	{
		ImperialFortressCharacterInstance const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetID() == id)
		{
			return this->GetInstance(pData->GetInstance());
		}
	}

	return nullptr;
}

ImperialFortressInstance * ImperialFortressMgr::CreateInstance()
{
	ImperialFortressInstance* pInstance = new ImperialFortressInstance();
	pInstance->SetID(this->GetInstanceID());

	this->m_ImperialFortressMap[pInstance->GetID()] = pInstance;

	this->IncreaseInstanceID(1);

	return pInstance;
}

void ImperialFortressMgr::RemoveInstance(uint32 id)
{
	for (ImperialFortressCharacterInstanceList::const_iterator itr = this->m_ImperialFortressCharacterInstanceList.begin(); itr != this->m_ImperialFortressCharacterInstanceList.end();)
	{
		ImperialFortressCharacterInstance const* pData = *itr;

		if (!pData)
		{
			itr = this->m_ImperialFortressCharacterInstanceList.erase(itr);
			continue;
		}

		if (pData->GetInstance() == id)
		{
			delete *itr;
			itr = this->m_ImperialFortressCharacterInstanceList.erase(itr);
			continue;
		}

		++itr;
	}
}

void ImperialFortressMgr::RemoveInstanceByCharacter(uint32 id)
{
	for (ImperialFortressCharacterInstanceList::const_iterator itr = this->m_ImperialFortressCharacterInstanceList.begin(); itr != this->m_ImperialFortressCharacterInstanceList.end();)
	{
		ImperialFortressCharacterInstance const* pData = *itr;

		if (!pData)
		{
			itr = this->m_ImperialFortressCharacterInstanceList.erase(itr);
			continue;
		}

		if (pData->GetID() == id)
		{
			delete *itr;
			itr = this->m_ImperialFortressCharacterInstanceList.erase(itr);
			continue;
		}

		++itr;
	}
}

void ImperialFortressMgr::AddCharacterInstance(uint32 id, Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	ImperialFortressCharacterInstance* pData = new ImperialFortressCharacterInstance;
	pData->SetID(pPlayer->GetGUID());
	pData->SetInstance(id);

	this->m_ImperialFortressCharacterInstanceList.push_back(pData);
}

ImperialFortressTrapList empty_trap;
ImperialFortressTrapList const& ImperialFortressMgr::GetTrapList(uint8 day) const
{
	ImperialFortressTrapMap::const_iterator itr = this->m_ImperialFortressTrapMap.find(day);
	
	if (itr != this->m_ImperialFortressTrapMap.end())
	{
		return itr->second;
	}

	return empty_trap;
}

void ImperialFortressMgr::Stop()
{
	for (ImperialFortressMap::const_iterator itr = this->m_ImperialFortressMap.begin(); itr != this->m_ImperialFortressMap.end(); ++itr)
	{
		ImperialFortressInstance * pInstance = itr->second;

		if (!pInstance)
		{
			continue;
		}

		pInstance->SetState_End();
	}
}