MonsterSoul::MonsterSoul()
{

}

MonsterSoul::~MonsterSoul()
{
	MAP_CLEAR(MonsterSoulConverterMap::iterator, this->m_MonsterSoulConverterMap);
	LIST_CLEAR(MonsterSoulTransformationList::iterator, this->m_MonsterSoulTransformationList);
	LIST_CLEAR(MonsterSoulRewardList::iterator, this->m_MonsterSoulRewardList);
}

void MonsterSoul::LoadConverter(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Soul Converter...");

	MAP_CLEAR(MonsterSoulConverterMap::iterator, this->m_MonsterSoulConverterMap);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterSoulConverter");

	uint32 count = 0;

for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
MonsterSoulConverter* pData = new MonsterSoulConverter;

pData->m_ID = data.attribute("ID").as_int();
pData->m_Item = ITEMGET(data.attribute("Type").as_int(), data.attribute("Index").as_int());
pData->m_Amount = data.attribute("Amount").as_int();

this->m_MonsterSoulConverterMap[pData->m_ID] = pData;
count++;
}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster soul converter definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterSoul::LoadTransformation(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Soul Transformation...");

	LIST_CLEAR(MonsterSoulTransformationList::iterator, this->m_MonsterSoulTransformationList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterSoulTranformation");

	uint32 count = 0;

for (pugi::xml_node type = Main.child("MonsterSoul"); type; type = type.next_sibling()) {
for (pugi::xml_node data = type.child("Data"); data; data = data.next_sibling()) {
MonsterSoulTransformation* pData = new MonsterSoulTransformation;
pData->m_Type = type.attribute("Type").as_int();
pData->m_ID = data.attribute("ID").as_int();
pData->m_World = data.attribute("World").as_int();
pData->m_Monster = data.attribute("Monster").as_int();
pData->m_RequiredSoul = data.attribute("ReqSoul").as_int();
pData->m_DropRate = data.attribute("DropRate").as_int();
pData->m_Location = data.attribute("Location").as_string();

			this->m_MonsterSoulTransformationList.push_back(pData);
			count++;
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster soul transformation definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MonsterSoul::LoadReward(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Monster Soul Reward...");

	LIST_CLEAR(MonsterSoulRewardList::iterator, this->m_MonsterSoulRewardList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MonsterSoulReward");

	uint32 count = 0;

for (pugi::xml_node level = Main.child("MonsterSoul"); level; level = level.next_sibling()) {
for (pugi::xml_node sublevel = level.child("Reward"); sublevel; sublevel = sublevel.next_sibling()) {
MonsterSoulReward* pData = new MonsterSoulReward;

pData->m_ID = level.attribute("Level").as_int();
pData->m_SubID = sublevel.attribute("SubLevel").as_int();
pData->m_ItemBag = sublevel.attribute("ItemBag").as_string();
pData->m_RandomItemBag = sublevel.attribute("RandomItemBag").as_string();

			this->m_MonsterSoulRewardList.push_back(pData);
			count++;
		}

	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM monster_soul_reward");

	if (result)
	{
		do
		{
Field* fields = result->Fetch();

MonsterSoulReward* pData = new MonsterSoulReward;

pData->m_ID = fields[0].GetUInt32();
pData->m_SubID = fields[1].GetUInt8();
pData->m_ItemBag = fields[2].GetString();
pData->m_RandomItemBag = fields[3].GetString();

			this->m_MonsterSoulRewardList.push_back(pData);
			count++;
		} while (result->NextRow());
	}*/

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u monster soul reward definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

MonsterSoulConverter const* MonsterSoul::GetConverter(uint32 id) const
{
	MonsterSoulConverterMap::const_iterator itr = this->m_MonsterSoulConverterMap.find(id);

	if (itr != this->m_MonsterSoulConverterMap.end())
	{
		return itr->second;
	}

	return nullptr;
}

MonsterSoulConverter const* MonsterSoul::GetConverterByItem(uint16 id) const
{
	for (MonsterSoulConverterMap::const_iterator itr = this->m_MonsterSoulConverterMap.begin(); itr != this->m_MonsterSoulConverterMap.end(); ++itr)
	{
		MonsterSoulConverter const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		if (pData->m_Item == id)
		{
			return pData;
		}
	}

	return nullptr;
}

MonsterSoulTransformation const* MonsterSoul::GetTransformation(uint32 type, uint32 id) const
{
	for (MonsterSoulTransformationList::const_iterator itr = this->m_MonsterSoulTransformationList.begin(); itr != this->m_MonsterSoulTransformationList.end(); ++itr)
	{
		MonsterSoulTransformation const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->m_Type == type && pData->m_ID == id)
		{
			return pData;
		}
	}

	return nullptr;
}

MonsterSoulReward const* MonsterSoul::GetReward(uint32 id, uint8 sub_id) const
{
	for (MonsterSoulRewardList::const_iterator itr = this->m_MonsterSoulRewardList.begin(); itr != this->m_MonsterSoulRewardList.end(); ++itr)
	{
		MonsterSoulReward const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->m_ID == id && pData->m_SubID == sub_id)
		{
			return pData;
		}
	}

	return nullptr;
}

bool MonsterSoul::IsSoulConverter(Item const* pItem) const
{
	if (!pItem)
	{
		return false;
	}

	return this->IsSoulConverter(pItem->m_Item);
}

bool MonsterSoul::IsSoulConverter(uint16 item) const
{
	return this->GetConverterByItem(item) != nullptr;
}

bool MonsterSoul::IsPurchaseAllowed(Player* pPlayer, Item const* pItem)
{
	if (!this->IsSoulConverter(pItem))
	{
		return true;
	}

	return !use_inventory_range(pPlayer->ItemFind(pItem->m_Item, 0));
}

void MonsterSoul::ExpireSoulConverter(Player* pPlayer, Item const* pItem)
{
	MonsterSoulConverter const* pConverter = this->GetConverterByItem(pItem->m_Item);

	if (!pConverter)
	{
		return;
	}

	pPlayer->RemoveMonsterSoulData(pConverter->m_ID);

	this->SendStatus(pPlayer);
}

void MonsterSoul::SendItemMove(Player* pPlayer, uint32 source, uint32 target)
{
	MONSTER_SOUL_ITEM_MOVE pMsg;
	pMsg.old_slot = source;
	pMsg.new_slot = target;

	pPlayer->SendPacket(&pMsg);
}

void MonsterSoul::SendPurchaseAvailable(Player* pPlayer)
{
	MONSTER_SOUL_RUUD_NPC pMsg;
	pMsg.result = 1;

	pPlayer->SendPacket(&pMsg);

	uint8 monstersoulpurchase = pPlayer->GetMonsterSoulPurchase();

	MONSTER_SOUL_AVAILABLE_PURCHASE pMsg2;
	pMsg2.amount = sGameServer->GetMonsterSoulPurchaseAmount() - monstersoulpurchase;

	pPlayer->SendPacket(&pMsg2);
}

void MonsterSoul::SendPurchaseList(Player* pPlayer)
{
	uint8 buffer[8192];
	int32 write_size = 0;

	MONSTER_SOUL_NPC_DATA_HEAD* head = (MONSTER_SOUL_NPC_DATA_HEAD*)&buffer[write_size];
	write_size += sizeof(MONSTER_SOUL_NPC_DATA_HEAD);

	head->junk = 0;
	head->count = 0;

	for (MonsterSoulConverterMap::const_iterator itr = this->m_MonsterSoulConverterMap.begin(); itr != this->m_MonsterSoulConverterMap.end(); ++itr)
	{
		MonsterSoulConverter const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		MONSTER_SOUL_NPC_DATA_BODY_1* body = (MONSTER_SOUL_NPC_DATA_BODY_1*)&buffer[write_size];
		write_size += sizeof(MONSTER_SOUL_NPC_DATA_BODY_1);

		body->type = pData->m_ID;
		body->data[0] = 0;
		body->data[1] = 0;
		body->expire_date = time(nullptr) + sGameServer->GetMonsterSoulDurationTime();
		body->junk1 = 0;
		body->amount = 0;
		body->status = 0;
		body->slot = 0;

		uint8 slot = pPlayer->ItemFind(pData->m_Item, 0);
		bool finished = true;

		if (use_inventory_range(slot))
		{
			body->slot = slot;
			body->expire_date = pPlayer->GetInventory()->GetItem(slot)->GetExpireDate();
			
			MonsterSoulData* pPlayerData = pPlayer->GetMonsterSoulData(pData->m_ID);

			if (pPlayerData)
			{
				for (MonsterSoulAmountDataMap::const_iterator it = pPlayerData->m_AmountMap.begin(); it != pPlayerData->m_AmountMap.end(); ++it)
				{
					MonsterSoulTransformation const* pMonsterData = this->GetTransformation(pData->m_ID, it->first);

					if (pMonsterData)
					{
						MONSTER_SOUL_NPC_DATA_BODY_2* data = (MONSTER_SOUL_NPC_DATA_BODY_2*)&buffer[write_size];
						write_size += sizeof(MONSTER_SOUL_NPC_DATA_BODY_2);

						data->id = it->first;
						data->amount = it->second;

						if (data->amount >= pMonsterData->m_RequiredSoul)
						{
							data->amount = pMonsterData->m_RequiredSoul;
						}
						else
						{
							finished = false;
						}

						++body->amount;
					}
				}
			}
		}

		if (body->amount > 0 && finished)
		{
			body->status = 1;
		}

		++head->count;
	}

	head->Set(HEACODE_EVENT_INVENTORY, SUBCODE_EVENT_INVENTORY_MONSTER_SOUL_PURCHASE_LIST, write_size);

	pPlayer->sendPacket(buffer);
}

void MonsterSoul::SoulConverterPurchase(Player* pPlayer, Item * pItem)
{
	MonsterSoulConverter const* pConverter = this->GetConverterByItem(pItem->m_Item);

	if (!pConverter)
	{
		return;
	}

	uint8 monstersoulpurchase = pPlayer->GetMonsterSoulPurchase();

	if (monstersoulpurchase == sGameServer->GetMonsterSoulPurchaseAmount()) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Monster Soul is not available: 0/%d", sGameServer->GetMonsterSoulPurchaseAmount());
		return;
	}

	pPlayer->RemoveMonsterSoulData(pConverter->m_ID);

	MonsterSoulData* pData = new MonsterSoulData;
	pData->SetType(pConverter->m_ID);

	pPlayer->m_MonsterSoulDataMap[pConverter->m_ID] = pData;

	for (int32 i = 0; i < pConverter->m_Amount; ++i)
	{
		RandomValue<uint32> m_RandomValue(0);

		for (MonsterSoulTransformationList::const_iterator itr = this->m_MonsterSoulTransformationList.begin(); itr != this->m_MonsterSoulTransformationList.end(); ++itr)
		{
			MonsterSoulTransformation const* pMonsterData = *itr;

			if (!pMonsterData)
			{
				continue;
			}

			if (pMonsterData->m_Type != pConverter->m_ID)
			{
				continue;
			}

			if (pData->m_AmountMap.find(pMonsterData->m_ID) != pData->m_AmountMap.end())
			{
				continue;
			}

			m_RandomValue.AddValue(pMonsterData->m_ID, 0);
		}

		pData->m_AmountMap[m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM)] = 0;
	}

	this->SendStatus(pPlayer);
	pPlayer->SetMonsterSoulPurchase(monstersoulpurchase + 1);
}

void MonsterSoul::SendStatus(Player* pPlayer)
{
	uint8 buffer[8192];
	int32 write_size = 0;

	MONSTER_SOUL_QUEST_HEAD* head = (MONSTER_SOUL_QUEST_HEAD*)&buffer[write_size];
	write_size += sizeof(MONSTER_SOUL_QUEST_HEAD);

	head->junk = 0;
	head->count = 0;

	for (MonsterSoulConverterMap::const_iterator itr = this->m_MonsterSoulConverterMap.begin(); itr != this->m_MonsterSoulConverterMap.end(); ++itr)
	{
		MonsterSoulConverter const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		uint8 slot = pPlayer->ItemFind(pData->m_Item, 0);

		if (!use_inventory_range(slot))
		{
			pPlayer->RemoveMonsterSoulData(pData->m_ID);
			continue;
		}

		MonsterSoulData* pPlayerData = pPlayer->GetMonsterSoulData(pData->m_ID);

		if (!pPlayerData)
		{
			pPlayer->ItemDelete(pData->m_Item, 0, use_inventory_size);
			continue;
		}

		MONSTER_SOUL_QUEST_BODY_1* body = (MONSTER_SOUL_QUEST_BODY_1*)&buffer[write_size];
		write_size += sizeof(MONSTER_SOUL_QUEST_BODY_1);

		body->type = pData->m_ID;
		body->data[0] = 0;
		body->data[1] = 0;
		body->expire_date = pPlayer->GetInventory()->GetItem(slot)->GetExpireDate();
		body->junk1 = 0;
		body->amount = 0;
		body->status = 0;
		body->slot = slot;

		bool finished = true;

		for (MonsterSoulAmountDataMap::const_iterator it = pPlayerData->m_AmountMap.begin(); it != pPlayerData->m_AmountMap.end(); ++it)
		{
			MonsterSoulTransformation const* pMonsterData = this->GetTransformation(pData->m_ID, it->first);

			if (pMonsterData)
			{
				MONSTER_SOUL_QUEST_BODY_2* data = (MONSTER_SOUL_QUEST_BODY_2*)&buffer[write_size];
				write_size += sizeof(MONSTER_SOUL_QUEST_BODY_2);

				data->id = it->first;
				data->amount = it->second;

				if (data->amount >= pMonsterData->m_RequiredSoul)
				{
					data->amount = pMonsterData->m_RequiredSoul;
				}
				else
				{
					finished = false;
				}

				++body->amount;
			}
		}

		if (finished)
		{
			body->status = 1;
		}

		++head->count;
	}

	head->Set(HEACODE_EVENT_INVENTORY, SUBCODE_EVENT_INVENTORY_MONSTER_SOUL_STATUS, write_size);

	pPlayer->sendPacket(buffer);
}

void MonsterSoul::MonsterKill(Player* pPlayer, Monster* pMonster)
{
	if (!pPlayer || !pMonster)
	{
		return;
	}

	for (MonsterSoulDataMap::iterator itr = pPlayer->m_MonsterSoulDataMap.begin(); itr != pPlayer->m_MonsterSoulDataMap.end(); ++itr)
	{
		MonsterSoulData* pData = itr->second;

		if (!pData)
		{
			continue;
		}

		for (MonsterSoulAmountDataMap::iterator it = pData->m_AmountMap.begin(); it != pData->m_AmountMap.end(); ++it)
		{
			MonsterSoulTransformation const* pMonsterData = this->GetTransformation(pData->m_Type, it->first);

			if (!pMonsterData)
			{
				continue;
			}

			if (it->second >= pMonsterData->m_RequiredSoul)
			{
				continue;
			}

			if ((pMonsterData->m_World == pMonster->GetWorldId()) || (KUBERA_MAP_RANGE(pMonsterData->m_World) && KUBERA_MAP_RANGE(pMonster->GetWorldId())))
			{
				if (pMonsterData->m_Monster != pMonster->GetClass())
				{
					continue;
				}

				if (roll_chance_i(pMonsterData->m_DropRate, 1000000))
				{
					it->second++;

					this->SendUpdateData(pPlayer, pMonsterData->m_Type);

					this->ShareSoulToParty(pPlayer, pMonsterData);
				}
			}
		}
	}
}

void MonsterSoul::ShareSoulToParty(Player* pPlayer, MonsterSoulTransformation const* pMonsterData)
{
	if (!pPlayer || !pMonsterData)
	{
		return;
	}

	Unit* mMember[MAX_PARTY_MEMBERS];
	uint8 count = 0;

	pPlayer->GetPartyMembers(mMember, count, MAX_PARTY_MONSTER_KILL_DISTANCE);

	if (count > MAX_PARTY_MEMBERS)
	{
		count = MAX_PARTY_MEMBERS;
	}

	for (uint8 h = 0; h < count; ++h)
	{
		if (!mMember[h])
		{
			continue;
		}

		Player* pMember = mMember[h]->ToPlayer();

		if (!pMember)
		{
			continue;
		}

		if (pPlayer == pMember)
		{
			continue;
		}

		MonsterSoulData* pData = pMember->GetMonsterSoulData(pMonsterData->m_Type);

		if (!pData)
		{
			continue;
		}

		MonsterSoulAmountDataMap::iterator itr = pData->m_AmountMap.find(pMonsterData->m_ID);

		if (itr != pData->m_AmountMap.end())
		{
			itr->second++;

			this->SendUpdateData(pMember, pMonsterData->m_Type);
		}
	}
}

void MonsterSoul::SendUpdateData(Player* pPlayer, uint32 type)
{
	if (!pPlayer)
	{
		return;
	}

	MonsterSoulConverter const* pConverter = this->GetConverter(type);

	if (!pConverter)
	{
		return;
	}

	MonsterSoulData const* pData = pPlayer->GetMonsterSoulData(type);

	if (!pData)
	{
		return;
	}

	uint8 slot = pPlayer->ItemFind(pConverter->m_Item, 0);

	if (!use_inventory_range(slot))
	{
		return;
	}

	uint8 buffer[1024];
	POINTER_PCT(MONSTER_SOUL_UPDATE_HEAD, head, buffer, 0);
	POINTER_PCT(MONSTER_SOUL_UPDATE_BODY, body, buffer, sizeof(MONSTER_SOUL_UPDATE_HEAD));
	head->type = type;
	head->data[0] = 0;
	head->data[1] = 0;
	head->expire_date = pPlayer->GetInventory()->GetItem(slot)->GetExpireDate();
	head->junk1 = 0;
	head->amount = 0;
	head->status = 0;
	head->slot = slot;

	bool finished = true;

	for (MonsterSoulAmountDataMap::const_iterator it = pData->m_AmountMap.begin(); it != pData->m_AmountMap.end(); ++it)
	{
		MonsterSoulTransformation const* pMonsterData = this->GetTransformation(type, it->first);

		if (pMonsterData)
		{
			body[head->amount].id = it->first;
			body[head->amount].amount = it->second;

			if (body[head->amount].amount >= pMonsterData->m_RequiredSoul)
			{
				body[head->amount].amount = pMonsterData->m_RequiredSoul;
			}
			else
			{
				finished = false;
			}

			++head->amount;
		}
	}

	if (finished)
	{
		head->status = 1;
	}

	head->Set(HEACODE_EVENT_INVENTORY, SUBCODE_EVENT_INVENTORY_MONSTER_SOUL_UPDATE_DATA, sizeof(MONSTER_SOUL_UPDATE_HEAD)+(head->amount * sizeof(MONSTER_SOUL_UPDATE_BODY)));

	pPlayer->sendPacket(buffer);
}

bool MonsterSoul::SoulConverterUse(Player* pPlayer, uint16 item, uint8 slot, uint8 type)
{
	if (!pPlayer)
	{
		return true;
	}

	MonsterSoulConverter const* pConverter = this->GetConverterByItem(item);

	if (!pConverter)
	{
		return false;
	}

	MonsterSoulData const* pData = pPlayer->GetMonsterSoulData(pConverter->m_ID);

	if (!pData)
	{
		return true;
	}

	bool finished = true;

	for (MonsterSoulAmountDataMap::const_iterator itr = pData->m_AmountMap.begin(); itr != pData->m_AmountMap.end(); ++itr)
	{
		MonsterSoulTransformation const* pMonsterInfo = this->GetTransformation(pData->m_Type, itr->first);

		if (!pMonsterInfo)
		{
			continue;
		}

		if (itr->second < pMonsterInfo->m_RequiredSoul)
		{
			finished = false;
			break;
		}
	}

	if (!finished)
	{
		return true;
	}

	MonsterSoulReward const* pReward = this->GetReward(pData->m_Type, type);

	if (!pReward)
	{
		return true;
	}

	pPlayer->ClearItem(slot);
	pPlayer->RemoveMonsterSoulData(pData->m_Type);

	Item new_item;
	sItemBagMgr->RunItemBag(pPlayer, pReward->m_ItemBag, new_item);

	if (sItemBagMgr->RunItemBag(pPlayer, pReward->m_RandomItemBag, new_item))
	{
		MONSTER_SOUL_REWARD pMsg;
		pMsg.id = pConverter->m_ID;
		pMsg.reward_type = type;
		pMsg.item = new_item.GetItem();
		pMsg.amount = 1;

		item_template const* pItemInfo = sItemMgr->GetItem(new_item.GetItem());

		if (pItemInfo && pItemInfo->GetStackData()->GetMaxStack() > 0)
		{
			pMsg.amount = new_item.GetDurability();
		}
				
		pMsg.data = 0;

		pPlayer->SendPacket(&pMsg);
	}

	return true;
}

void MonsterSoul::CheckResetPurchase() {
	Custom::SystemTimer m_timer = Custom::SystemTimer();

	if (m_timer.GetDayOfWeek() == 4) {
		if (!this->IsResetMonsterSoul()) {
			PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();

			for (PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it)
			{
				Player* pPlayer = it->second;

				if (!pPlayer)
					continue;

				pPlayer->SetMonsterSoulPurchase(0);
			}

			QueryResult result = MuDatabase.PQuery("UPDATE character_info SET monster_soul_purchase=0");
			this->SetResetMonsterSoul(true);

			QueryResult result1 = MuDatabase.PQuery("UPDATE server_monster_soul_reset SET monster_soul_reset=1");
		}
	}
	else {
		this->SetResetMonsterSoul(false);
		QueryResult result1 = MuDatabase.PQuery("UPDATE server_monster_soul_reset SET monster_soul_reset=0");
	}
}