MossMerchant::MossMerchant()
{

}

MossMerchant::~MossMerchant()
{
	MAP_CLEAR(MossMerchantSectionMap::iterator, this->section_map);
}

void MossMerchant::Load(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Moss Merchant Section...");

	MAP_CLEAR(MossMerchantSectionMap::iterator, this->section_map);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MossMerchantSection");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		MossMerchantSection* pData = new MossMerchantSection;

		pData->SetID(data.attribute("ID").as_int());
		pData->SetName(data.attribute("Name").as_string());

		pData->SetRequiredItemType(0, data.attribute("ReqItem1").as_int());
		pData->SetRequiredItemCount(0, data.attribute("ReqCount1").as_int());
		pData->SetRequiredItemType(1, data.attribute("ReqItem2").as_int());
		pData->SetRequiredItemCount(1, data.attribute("ReqCount2").as_int());
		pData->SetRequiredItemType(2, data.attribute("ReqItem3").as_int());
		pData->SetRequiredItemCount(2, data.attribute("ReqCount3").as_int());

		pData->SetRequiredZen(data.attribute("ReqZen").as_int());
		pData->SetItemBag(data.attribute("ItemBag").as_string());
		pData->SetEnabled(data.attribute("Enable").as_bool());

		this->section_map[pData->GetID()] = pData;
		count++;
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u moss merchant section definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MossMerchant::Open(Player* pPlayer)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::MossMerchant, 0) )
	{
		return;
	}

	uint8 buffer[1024];
	POINTER_PCT(MOSS_MERCHANT_OPEN, head, buffer, 0);
	POINTER_PCT(uint8, body, buffer, sizeof(MOSS_MERCHANT_OPEN));
	head->count = 0;

	for ( MossMerchantSectionMap::const_iterator itr = this->section_map.begin(); itr != this->section_map.end(); ++itr )
	{
		if ( !itr->second )
		{
			continue;
		}

		body[head->count] = itr->second->GetEnabled();
		++head->count;
	}

	head->h.set(HEADCODE_MERCHANT_DATA, SUBCODE_MERCHANT_DATA_OPEN, sizeof(MOSS_MERCHANT_OPEN) + head->count);

	pPlayer->sendPacket(buffer, head->h.get_size());
	pPlayer->MoneySend();
}

void MossMerchant::OpenBox(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::MossMerchant, 0) )
	{
		return;
	}

	if ( !pPlayer->IsAuthorizationEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	POINTER_PCT_LOG(MOSS_MERCHANT_OPEN_BOX, lpMsg, Packet, 0);

	MossMerchantSection const* pSection = this->GetMossMerchantSection(lpMsg->type);

	if ( !pSection )
	{
		sLog->outError("moss_merchant", "%s -- Wrong type: %u -- %s", __FUNCTION__, lpMsg->type, pPlayer->BuildLog().c_str());
		return;
	}

	if ( !pSection->GetEnabled() )
	{
		sLog->outError("moss_merchant", "%s -- Not enabled: %u -- %s", __FUNCTION__, lpMsg->type, pPlayer->BuildLog().c_str());
		return;
	}

	if (pSection->GetRequiredZen())
	{
		if (!pPlayer->MoneyHave(pSection->GetRequiredZen()))
		{
			sLog->outError("moss_merchant", "%s -- Lack money %u [%u - %u]-- %s", __FUNCTION__, lpMsg->type, pPlayer->MoneyGet(), pSection->GetRequiredZen(), pPlayer->BuildLog().c_str());
			return;
		}
	}

	else
	{
		for (uint8 n = 0; n < MOSS_MERCHANT_REQUIRED_ITEM_MAX; ++n)
		{
			if (pSection->GetRequiredItemType(n) == uint16(-1))
				continue;

			int32 count = pPlayer->GetInventory()->GetItemCount(pSection->GetRequiredItemType(n), 0);

			if (count < pSection->GetRequiredItemCount(n))
			{
				sLog->outError("moss_merchant", "%s -- Lack items %u [%u - %u]-- %s", __FUNCTION__, lpMsg->type, count, pSection->GetRequiredItemCount(n), pPlayer->BuildLog().c_str());
				return;
			}
		}
	}

	Item item;
	if ( !sItemBagMgr->RunItemBag(pPlayer, pSection->GetItemBag(), item) )
	{
		sLog->outError("moss_merchant", "%s -- Can run itembag: %u -- %s", __FUNCTION__, lpMsg->type, pPlayer->BuildLog().c_str());
		return;
	}

	sLog->outInfo("moss_merchant", "%s -- %u succeed -- %s -- %s", __FUNCTION__, lpMsg->type, pPlayer->BuildLog().c_str(), item.BuildLog(0).c_str());

	if (pSection->GetRequiredZen())
	{
		pPlayer->MoneyReduce(pSection->GetRequiredZen());
	}

	else
	{
		for (uint8 n = 0; n < MOSS_MERCHANT_REQUIRED_ITEM_MAX; ++n)
		{
			if (pSection->GetRequiredItemType(n) == uint16(-1))
				continue;

			for (uint8 i = 0; i < pSection->GetRequiredItemCount(n); ++i)
			{
				pPlayer->ItemFind(pSection->GetRequiredItemType(n), 0, true);
			}
		}
	}

	if (sNoticeSystem->CheckItemMossMerchantNotify(item)) {
		sNoticeSystem->NotifyMossMerchant(pPlayer, item);
	}

	MOSS_MERCHANT_REWARD pMsg;
	item.ConvertToBuffer(pMsg.item_info);
	pPlayer->SEND_PCT(pMsg);
}

void MossMerchant::OpenBoxResult(Player* pPlayer, uint8 result)
{
	if ( !pPlayer )
	{
		return;
	}
}

MossMerchantSection const* MossMerchant::GetMossMerchantSection(uint8 id) const
{
	MossMerchantSectionMap::const_iterator itr = this->section_map.find(id);

	if ( itr != this->section_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}