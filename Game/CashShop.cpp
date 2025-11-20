/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "CashShop.cpp"
*
*/

CashShopMgr::CashShopMgr()
{

}

CashShopMgr::~CashShopMgr()
{
	MAP_CLEAR(CashShopCategoryMap::iterator, this->m_CategoryMap);
	MAP_CLEAR(CashShopPackageMap::iterator, this->m_PackageMap);
	LIST_CLEAR(CashShopProductList::iterator, this->m_ProductList);
}

void CashShopMgr::LoadItemList()
{
	MAP_CLEAR(CashShopCategoryMap::iterator, this->m_CategoryMap);
	MAP_CLEAR(CashShopPackageMap::iterator, this->m_PackageMap);
	LIST_CLEAR(CashShopProductList::iterator, this->m_ProductList);

	this->LoadCategory("../Data/CashShop/CashShopCategory.xml");
	this->LoadPackageData("../Data/CashShop/CashShopPackage.xml");
	this->LoadProductData("../Data/CashShop/CashShopProduct.xml");
}

void CashShopMgr::LoadCategory(char* pchFileName)
{
	sLog->outLoad(true, "Loading Cash Shop Category...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CashShopCategory");

	uint32 count = 0;

	for (pugi::xml_node Cat = Main.child("Cat"); Cat; Cat = Cat.next_sibling()) {
		CashShopCategory* pData = new CashShopCategory;
		if (!Cat.attribute("Disable").as_bool()) {
			pData->SetID(Cat.attribute("ID").as_int());
			pData->SetName(Cat.attribute("Name").as_string());
			pData->SetParent(Cat.attribute("Parent").as_int());
			pData->SetMain(Cat.attribute("Main").as_int());

			this->m_CategoryMap[pData->GetID()] = pData;
			count++;
		}
	}

	sLog->outLoad(false, ">> Loaded %u cash shop category definitions", count);
}

void CashShopMgr::LoadPackageData(char* pchFileName)
{
	sLog->outLoad(true, "Loading Cash Shop Package List...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CashShopPackage");

	uint32 count = 0;

	for (pugi::xml_node Cat = Main.child("Category"); Cat; Cat = Cat.next_sibling()) {
		for (pugi::xml_node Package = Cat.child("Package"); Package; Package = Package.next_sibling()) {
			CashShopPackage* pData = new CashShopPackage;
			pData->SetPackage(Package.attribute("ID").as_int());
			pData->SetCategory(Cat.attribute("ID").as_int());
			pData->SetPosition(Package.attribute("Position").as_int());
			pData->SetName(Package.attribute("Name").as_string());
			pData->SetDescription(Package.attribute("Description").as_string());
			pData->SetDisplayItem(Package.attribute("DisPlayItem").as_int());
			pData->SetFlags(Package.attribute("Flags").as_int());
			pData->SetPriceType(Package.attribute("PriceType").as_int());
			pData->SetStartDate(Package.attribute("StartDate").as_int());
			pData->SetEndDate(Package.attribute("EndDate").as_int());

			if (Package.attribute("PriceType").as_int() >= CASH_SHOP_COIN_MAX) {
				sLog->outError(LOG_DEFAULT, "CashShopMgr::LoadPackageData() Wrong Price Type: %d - (Package: %u, Name: %s)",
					pData->GetPriceType(), pData->GetPackage(), pData->GetName().c_str());

				delete pData;
				continue;
			}

			this->m_PackageMap[pData->GetPackage()] = pData;
			count++;
		}
	}

	sLog->outLoad(false, ">> Loaded %u cash shop package definitions", count);
}

void CashShopMgr::LoadProductData(char* pchFileName)
{
	sLog->outLoad(true, "Loading Cash Shop Product List...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("CashShopProduct");

	uint32 count = 0;

	for (pugi::xml_node pro = Main.child("Product"); pro; pro = pro.next_sibling()) {
		CashShopProduct* product = new CashShopProduct;

		product->SetProduct(pro.attribute("ProductID").as_int());
		product->SetPackage(pro.attribute("Package").as_int());
		product->SetOption(pro.attribute("Option").as_int());
		product->SetName(pro.attribute("Name").as_string());
		product->SetPrice(pro.attribute("Price").as_int());
		product->SetBuyType(pro.attribute("BuyType").as_int());
		product->SetUseType(pro.attribute("UseType").as_int());
		product->SetFlags(pro.attribute("Flags").as_int());
		product->SetDuration(pro.attribute("Duration").as_int());
		product->SetItemType(pro.attribute("ItemCat").as_int());
		product->SetItemIndex(pro.attribute("ItemIndex").as_int());
		product->SetItemLevel(pro.attribute("ItemLevel").as_int());
		product->SetItemDurability(static_cast<float>(pro.attribute("ItemDur").as_int()));
		product->SetItemSkill(pro.attribute("ItemSkill").as_int());
		product->SetItemLuck(pro.attribute("ItemLuck").as_int());
		product->SetItemOption(pro.attribute("ItemOption").as_int());
		product->SetItemExcellent(pro.attribute("ItemExl").as_int());
		product->SetItemAncient(pro.attribute("ItemAncient").as_int());
		product->SetCount(pro.attribute("Count").as_int());

		if (product->GetCount() <= 0)
		{
			delete product;
			continue;
		}

		if (product->GetBuyType() >= CASH_SHOP_BUY_TYPE_MAX)
		{
			sLog->outError(LOG_DEFAULT, "CashShopMgr::LoadProductData() Wrong Buy Type: %d - (Package: %u, Option: %u, Product: %u, Name: %s)",
				product->GetBuyType(), product->GetPackage(), product->GetOption(), product->GetProduct(), product->GetName().c_str());

			delete product;
			continue;
		}

		if (product->GetUseType() >= CASH_SHOP_USE_TYPE_MAX)
		{
			sLog->outError(LOG_DEFAULT, "CashShopMgr::LoadProductData() Wrong Use Type: %d - (Package: %u, Option: %u, Product: %u, Name: %s)",
				product->GetUseType(), product->GetPackage(), product->GetOption(), product->GetProduct(), product->GetName().c_str());

			delete product;
			continue;
		}

		this->m_ProductList.push_back(product);
		count++;
	}

	sLog->outLoad(false, ">> Loaded %u cash shop product definitions", count);
}

CashShopPackage const* CashShopMgr::GetPackage(uint32 package) const
{
	CashShopPackageMap::const_iterator it = this->m_PackageMap.find(package);

	if (it == this->m_PackageMap.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

CashShopProduct const* CashShopMgr::GetProduct(uint32 product, uint32 option) const
{
	for (auto const& pData : this->m_ProductList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetProduct() == product && pData->GetOption() == option)
		{
			return pData;
		}
	}

	return nullptr;
}

uint32 CashShopMgr::GetPackagePrice(uint32 package, uint32 option) const
{
	uint32 price = 0;

	for (auto const& pData : this->m_ProductList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->IsFlags(CASH_SHOP_FLAG_BUY_DISABLED))
		{
			continue;
		}

		if (pData->GetPackage() == package && pData->GetOption() == option)
		{
			price += pData->GetPrice();
		}
	}

	return price;
}

void CashShopMgr::SaveBuyLog(Player* pPlayer, uint8 price_type, CashShopProduct const* pProduct, bool gift, const char * dest_char)
{
	PreparedStatement* stmt = MuLogDatabase.GetPreparedStatement(MULOG_STATEMENT_CASH_SHOP);
	stmt->setUInt16(sGameServer->GetServerCode());
	stmt->setUInt32(pPlayer->GetAccountData()->GetGUID());
	stmt->setString(pPlayer->GetAccountData()->GetAccount());
	stmt->setUInt32(pPlayer->GetGUID());
	stmt->setString(pPlayer->GetName());
	stmt->setString(pPlayer->BuildLogDB());
	stmt->setString(dest_char);
	stmt->setUInt32(pProduct->GetPackage());
	stmt->setUInt32(pProduct->GetProduct());
	stmt->setUInt32(pProduct->GetOption());
	stmt->setString(pProduct->GetName());
	stmt->setUInt8(price_type);
	stmt->setUInt32(pProduct->GetPrice());
	stmt->setUInt8(gift ? 1 : 0);

	MuLogDatabase.Execute(stmt);
}

void CashShopMgr::CashShopOpenRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		this->CashShopOpenResult(pPlayer, 0);
		return;
	}

	POINTER_PCT_LOG(CASH_SHOP_OPEN_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->action == 0 )
	{
		if ( !sGameServer->IsCashShopEnabled() )
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "CashShop is disabled.");
			this->CashShopOpenResult(pPlayer, 0);
			return;
		}

		if ( !pPlayer->CanInteract() )
		{
			this->CashShopOpenResult(pPlayer, 0);
			return;
		}

		if ( !pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_CASH_SHOP_USE) )
		{
			this->CashShopOpenResult(pPlayer, 0);
			return;
		}

		if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
		{
			sLog->outError(LOG_CASHSHOP, "CashShopOpenRequest() - %s Try to open Cash Shop while using %s interface",
				pPlayer->BuildLog().c_str(), pPlayer->GetInterfaceState()->GetName());

			this->CashShopOpenResult(pPlayer, 0);
			return;
		}

		if ( sGameServer->IsCashShopSafeZone() && !pPlayer->IsInSafeZone() )
		{
			sLog->outError(LOG_CASHSHOP, "CashShopOpenRequest() - %s Try to open Cash Shop while not in Safe Zone :%s [%d / %d]",
				pPlayer->BuildLog().c_str(), pPlayer->GetWorldName(), pPlayer->GetX(), pPlayer->GetY());

			this->CashShopOpenResult(pPlayer, 0);
			return;
		}

		pPlayer->GetInterfaceState()->Set(InterfaceData::CashShop, pPlayer);
		this->CashShopOpenResult(pPlayer, 1);

		sLog->outInfo(LOG_CASHSHOP, "CashShopOpenRequest() - %s Successfully opened Cash Shop", pPlayer->BuildLog().c_str());
	}
	else if ( lpMsg->action == 1 )
	{
		if ( !Player::InterfaceCheck(pPlayer, InterfaceData::CashShop, 0) )
		{
			this->CashShopOpenResult(pPlayer, 0);
			return;
		}

		pPlayer->GetInterfaceState()->Reset();
		this->CashShopOpenResult(pPlayer, 0);

		sLog->outInfo(LOG_CASHSHOP, "CashShopOpenRequest() - %s Successfully closed Cash Shop", pPlayer->BuildLog().c_str());
	}
}
	
void CashShopMgr::CashShopOpenResult(Player* pPlayer, uint8 result)
{
	if ( !pPlayer )
	{
		return;
	}

	CASH_SHOP_OPEN_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CashShopMgr::CashShopCoinRequest(Player* pPlayer)
{
	CASH_SHOP_COIN pMsg(1, pPlayer->GetCredits(), 0, pPlayer->GetGoblinPoints());
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CashShopMgr::CashShopItemListRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::CashShop, 0) )
	{
		return;
	}

	if ( !pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_CASH_SHOP_USE) )
	{
		return;
	}

	POINTER_PCT_LOG(CASH_SHOP_ITEM_LIST_REQUEST, lpMsg, Packet, 0);

	if ( lpMsg->type != CASH_SHOP_ITEM_LIST_GIFT && lpMsg->type != CASH_SHOP_ITEM_LIST_INVENTORY )
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemListRequest() - %s Item List Request Wrong type: %d",
			pPlayer->BuildLog().c_str(), lpMsg->type);

		this->CashShopItemListResult(pPlayer, lpMsg->page, 0);
		return;
	}

	sLog->outInfo(LOG_CASHSHOP, "CashShopItemListRequest() - %s Success. Type: %d - Page: %d",
		pPlayer->BuildLog().c_str(), lpMsg->type, lpMsg->page);

	int32 total_page = (pPlayer->CashShopGetTotalPage(lpMsg->type) / CASH_SHOP_PAGE_SIZE) + 1;

	if ( lpMsg->page > total_page )
		lpMsg->page = total_page;

	this->CashShopItemListResult(pPlayer, lpMsg->page, total_page);
	this->CashShopItemListSend(pPlayer, lpMsg->type, lpMsg->page);
}
	
void CashShopMgr::CashShopItemListRequest(Player* pPlayer, uint16 page, uint8 type)
{
	CASH_SHOP_ITEM_LIST_REQUEST pMsg;
	pMsg.page = page;
	pMsg.type = type;
	this->CashShopItemListRequest(pPlayer, (uint8*)&pMsg);
}
	
void CashShopMgr::CashShopItemListResult(Player* pPlayer, uint16 page_current, uint16 page_total)
{
	CASH_SHOP_ITEM_LIST_RESULT pMsg(page_current, page_total);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}
	
void CashShopMgr::CashShopItemListSend(Player* pPlayer, uint8 type, uint16 page)
{
	CASH_SHOP_ITEM_LIST_DATA pMsg;
	//pMsg.unk2 = type;

	int32 start = (page - 1) * CASH_SHOP_PAGE_SIZE;
	int32 total = page * CASH_SHOP_PAGE_SIZE;
	int32 count = 0;

	if (type == CASH_SHOP_ITEM_LIST_GIFT)
	{
		for (CashShopItemDataList::const_iterator itr = pPlayer->m_CashShopGiftList.begin(); itr != pPlayer->m_CashShopGiftList.end(); ++itr)
		{
			CashShopItemData const* pItemData = *itr;

			if (!pItemData)
			{
				continue;
			}

			if (count >= start && count < total)
			{
				pMsg.serial = pItemData->GetSerial();
				pMsg.serial_cash_shop = pItemData->GetSerialCashShop();
				pMsg.code = pItemData->GetServer();
				pMsg.product = pItemData->GetProduct();
				pMsg.option = pItemData->GetOption();

				pPlayer->sendPacket(MAKE_PCT(pMsg));
			}

			count++;
		}
	}
	else
	{
		for (CashShopItemDataList::const_iterator itr = pPlayer->m_CashShopItemList.begin(); itr != pPlayer->m_CashShopItemList.end(); ++itr)
		{
			CashShopItemData const* pItemData = *itr;

			if (!pItemData)
			{
				continue;
			}

			if (count >= start && count < total)
			{
				pMsg.serial = pItemData->GetSerial();
				pMsg.serial_cash_shop = pItemData->GetSerialCashShop();
				pMsg.code = pItemData->GetServer();
				pMsg.product = pItemData->GetProduct();
				pMsg.option = pItemData->GetOption();

				pPlayer->sendPacket(MAKE_PCT(pMsg));
			}

			count++;
		}
	}
}

void CashShopMgr::CashShopItemBuyRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::CashShop, 0) )
	{
		return;
	}

	if ( !pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_CASH_SHOP_USE) )
	{
		this->CashShopItemBuyResult(pPlayer, 6);
		return;
	}

	if ( !sGameServer->IsCashShopEnabled() )
	{
		this->CashShopItemBuyResult(pPlayer, 6);
		return;
	}

	POINTER_PCT_LOG(CASH_SHOP_ITEM_BUY_REQUEST, lpMsg, Packet, 0);

	sLog->outInfo(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Request. PackageID: %u, Option: %u, Item: %u",
		pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

	CashShopPackage const* package = sCashShopMgr->GetPackage(lpMsg->package);

	if ( !package )
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Data not found. Package: %u, Option: %u, Item: %u",
			pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

		this->CashShopItemBuyResult(pPlayer, 6);
		return;
	}

	for (auto const& pData : this->m_ProductList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetPackage() != lpMsg->package || pData->GetOption() != lpMsg->option)
		{
			continue;
		}

		if (pData->IsFlags(CASH_SHOP_FLAG_BUY_DISABLED))
		{
			continue;
		}

		switch (pData->GetItem())
		{
		case ITEMGET(14, 162):
		{
			if (pPlayer->GetInventory()->GetExpanded() >= sGameServer->GetMaxExpandedInventory())
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Buy not allowed. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 6);
				return;
			}
		} break;

		case ITEMGET(14, 163):
		{
			if (pPlayer->GetWarehouse()->GetExpanded() != 0)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Buy not allowed. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 6);
				return;
			}
		} break;
		}
	}

	uint32 price = this->GetPackagePrice(lpMsg->package, lpMsg->option);

	switch ( package->GetPriceType() )
	{
	case CASH_SHOP_COIN_C:
		{
			if (!package->IsFlags(CASH_SHOP_FLAG_NO_DISCOUNT) && price > 0)
			{
				if (pPlayer->IsDiscountWC())
				{
					price = price * pPlayer->GetCashShopDiscountWC() / 100;
				}
				else if (sGameServer->IsDiscountWC())
				{
					price = price * sGameServer->GetCashShopDiscountWC() / 100;
				}

				if (price <= 0)
				{
					price = 1;
				}
			}

			if (pPlayer->GetCredits() < price)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Low credits. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 1);
				return;
			}

			pPlayer->ReduceCredits(price);
		} break;

	case CASH_SHOP_COIN_P:
		{
			this->CashShopItemBuyResult(pPlayer, 1);
			return;
		} break;

	case CASH_SHOP_COIN_GOBLIN:
		{
			if (!package->IsFlags(CASH_SHOP_FLAG_NO_DISCOUNT) && price > 0)
			{
				if (pPlayer->IsDiscountGP())
				{
					price = price * pPlayer->GetCashShopDiscountGP() / 100;
				}
				else if (sGameServer->IsDiscountGP())
				{
					price = price * sGameServer->GetCashShopDiscountGP() / 100;
				}

				if (price <= 0)
				{
					price = 1;
				}
			}

			if (pPlayer->GetGoblinPoints() < price)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Low goblin points. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 1);
				return;
			}

			pPlayer->ReduceGoblinPoints(price);
		} break;

	case CASH_SHOP_COIN_ZEN:
		{
			if (pPlayer->MoneyGet() < price)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Low zen. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 1);
				return;
			}

			pPlayer->MoneyReduce(price);
		} break;

	case CASH_SHOP_COIN_RUUD:
		{
			if (pPlayer->GetRuudMoney() < price)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Low ruud. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 1);
				return;
			}

			pPlayer->ReduceRuudMoney(price);
			pPlayer->SendRuudMoney();
		} break;

	case CASH_SHOP_COIN_BLESS:
		{
			if (pPlayer->GetInventory()->GetItemCount(JEWEL::BLESS, -1) < price)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Low jewel of bless. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 1);
				return;
			}

			pPlayer->ItemDelete(JEWEL::BLESS, -1, price);
		} break;

	case CASH_SHOP_COIN_SOUL:
		{
			if (pPlayer->GetInventory()->GetItemCount(JEWEL::SOUL, -1) < price)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Low jewel of soul. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 1);
				return;
			}

			pPlayer->ItemDelete(JEWEL::SOUL, -1, price);
		} break;

	case CASH_SHOP_COIN_CHAOS:
		{
			if (pPlayer->GetInventory()->GetItemCount(JEWEL::CHAOS, -1) < price)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Low jewel of chaos. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 1);
				return;
			}

			pPlayer->ItemDelete(JEWEL::CHAOS, -1, price);
		} break;

	case CASH_SHOP_COIN_GUARDIAN:
		{
			if (pPlayer->GetInventory()->GetItemCount(JEWEL::GUARDIAN, -1) < price)
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Low jewel of guardian. Package: %u, Option: %u, Item: %u",
					pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option, lpMsg->item_id);

				this->CashShopItemBuyResult(pPlayer, 1);
				return;
			}

			pPlayer->ItemDelete(JEWEL::GUARDIAN, -1, price);
		} break;
	}

	for (auto const& pData : this->m_ProductList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetPackage() != lpMsg->package || pData->GetOption() != lpMsg->option)
		{
			continue;
		}

		if (pData->IsFlags(CASH_SHOP_FLAG_BUY_DISABLED))
		{
			continue;
		}

		sCashShopMgr->SaveBuyLog(pPlayer, package->GetPriceType(), pData, false, pPlayer->GetCashShopGiftName());

		for (int32 i = 0; i < pData->GetCount(); ++i)
		{
			sItemMgr->ItemSerialCreateCashShop(pPlayer, lpMsg->package, lpMsg->option, pData->GetProduct(), 0);
		}
	}

	this->CashShopItemBuyResult(pPlayer, 0);
	this->CashShopItemListRequest(pPlayer, 1, CASH_SHOP_ITEM_LIST_INVENTORY);
}

void CashShopMgr::CashShopItemBuyResult(Player* pPlayer, uint8 result)
{
	CASH_SHOP_ITEM_BUY_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}
	
void CashShopMgr::CashShopItemBuyCallBack(Player* pPlayer, SerialCreateData* data)
{
	if (data->cash_shop.account_id == 0)
	{
		sLog->outInfo(LOG_CASHSHOP, "CashShopItemBuyCallBack() - %s. Data: Package: %u, Option: %u, Product: %u",
			pPlayer->BuildLog().c_str(), data->cash_shop.package, data->cash_shop.option, data->cash_shop.product);
	}
	else
	{
		sLog->outInfo(LOG_CASHSHOP, "CashShopItemGiftCallBack() - %s. Data: Package: %u, Option: %u, Product: %u",
			pPlayer->BuildLog().c_str(), data->cash_shop.package, data->cash_shop.option, data->cash_shop.product);
	}

	CashShopPackage const* pPackage = sCashShopMgr->GetPackage(data->cash_shop.package);

	if (!pPackage)
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemBuyRequest() - %s. Data not found. Package: %u, Option: %u, Product: %u",
			pPlayer->BuildLog().c_str(), data->cash_shop.package, data->cash_shop.option, data->cash_shop.product);

		this->CashShopItemBuyResult(pPlayer, 6);
		return;
	}

	for (auto const& pData : this->m_ProductList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetPackage() != data->cash_shop.package || pData->GetProduct() != data->cash_shop.product || pData->GetOption() != data->cash_shop.option)
		{
			continue;
		}

		if (pData->IsFlags(CASH_SHOP_FLAG_BUY_DISABLED))
		{
			continue;
		}

		switch (pData->GetBuyType())
		{
		case CASH_SHOP_BUY_TYPE_INVENTORY:
		case CASH_SHOP_BUY_TYPE_LIST:
		{
			if (data->cash_shop.account_id == 0)
			{
				CashShopItemData * pItem = new CashShopItemData;
				pItem->SetProduct(data->cash_shop.product);
				pItem->SetOption(data->cash_shop.option);
				pItem->SetSerial(data->serial);
				pItem->SetSerialCashShop(data->serial_cash_shop);
				pItem->SetServer(data->server);
				pItem->SetDate(time(nullptr));

				pPlayer->CashShopInsertItem(0, pItem, false);
			}
			else
			{
				if (pData->GetItem() == CREDIT_COIN)
				{
					PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_COIN_ADD);
					stmt->setUInt32(pData->GetPrice());
					stmt->setUInt32(data->cash_shop.account_id);
					
					MuDatabase.Execute(stmt);
				}
				else
				{
					PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_CASH_SHOP_ITEM_GIFT_INSERT);
					stmt->setUInt32(data->cash_shop.account_id);
					stmt->setUInt32(data->cash_shop.product);
					stmt->setUInt32(data->cash_shop.option);
					stmt->setUInt32(data->serial);
					stmt->setUInt32(data->serial_cash_shop);
					stmt->setUInt16(data->server);
					stmt->setInt64(time(nullptr));

					MuDatabase.Execute(stmt);
				}
			}
		} break;

		case CASH_SHOP_BUY_TYPE_USE:
		{
			if (data->cash_shop.account_id == 0)
			{
				switch (pData->GetItem())
				{
				case ITEMGET(14, 162):
				{
					pPlayer->GetInventory()->IncreaseExpanded(1);
					EXPANDED_WAREHOUSE_REPORT pMsg(1);
					pPlayer->SEND_PCT(pMsg);
				} break;

				case ITEMGET(14, 163):
				{
					pPlayer->GetWarehouse()->SetExpanded(1);
					EXPANDED_WAREHOUSE_REPORT pMsg(1);
					pPlayer->SEND_PCT(pMsg);
				} break;

				case ITEMGET(13, 97):
				{
					pPlayer->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_MAGIC_GLADIATOR);
				} break;

				case ITEMGET(13, 98):
				{
					pPlayer->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_DARK_LORD);
				} break;

				case ITEMGET(14, 91):
				{
					pPlayer->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_SUMMONER);
				} break;

				case ITEMGET(14, 169):
				{
					pPlayer->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_RAGE_FIGHTER);
				} break;

				case ITEMGET(14, 281):
				{
					pPlayer->GetAccountData()->AddSpecialCharacter(SPECIAL_CHARACTER_GROW_LANCER);
				} break;
				}
			}
		} break;
		}
	}
}

void CashShopMgr::CashShopItemGiftRequest(Player* pPlayer, uint8 * Packet)
{
	if (!Player::IsPlayerBasicState(pPlayer))
	{
		return;
	}

	if (!Player::InterfaceCheck(pPlayer, InterfaceData::CashShop, 0))
	{
		return;
	}

	if (!pPlayer->IsActionAllowed(PlayerAction::PLAYER_ACTION_CASH_SHOP_USE))
	{
		this->CashShopItemGiftResult(pPlayer, 6);
		return;
	}

	if (!sGameServer->IsCashShopEnabled())
	{
		this->CashShopItemGiftResult(pPlayer, 6);
		return;
	}

	POINTER_PCT_LOG(CASH_SHOP_ITEM_GIFT_REQUEST, lpMsg, Packet, 0);

	sLog->outInfo(LOG_CASHSHOP, "CashShopItemGiftRequest() - %s. Request. PackageID: %u, Option: %u",
		pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option);

	CashShopPackage const* pPackage = sCashShopMgr->GetPackage(lpMsg->package);

	if (!pPackage)
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemGiftRequest() - %s. Data not found. Package: %u, Option: %u",
			pPlayer->BuildLog().c_str(), lpMsg->package, lpMsg->option);

		this->CashShopItemGiftResult(pPlayer, 6);
		return;
	}

	if (!pPackage->IsFlags(CASH_SHOP_FLAG_GIFT))
	{
		this->CashShopItemGiftResult(pPlayer, 7);
		return;
	}

	pPlayer->GetInterfaceState()->SetState(1);

	STRING_SAFE_COPY(Name, MAX_CHARACTER_LENGTH + 1, lpMsg->name, MAX_CHARACTER_LENGTH);
	STRING_SAFE_COPY(Message, 200 + 1, lpMsg->message, 200);

	pPlayer->ResetCashShopGiftName();
	pPlayer->ResetCashShopGiftMessage();

	pPlayer->SetCashShopGiftName(Name);
	pPlayer->SetCashShopGiftMessage(Message);
	pPlayer->SetCashShopGiftPackage(lpMsg->package);
	pPlayer->SetCashShopGiftOption(lpMsg->option);

	/*PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_ADD_REQUEST);
	stmt->setString(Name);

	pPlayer->CallBackCashShopGiftSend = MuDatabase.AsyncQuery(stmt);*/

	pPlayer->CashShopGiftSendNew(Name);
}

void CashShopMgr::CashShopItemGiftResult(Player* pPlayer, uint8 result)
{
	CASH_SHOP_ITEM_GIFT_RESULT pMsg(result);
	pPlayer->SEND_PCT(pMsg);
}

void CashShopMgr::CashShopItemUseRequest(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::CashShop, 0) )
	{
		return;
	}

	POINTER_PCT_LOG(CASH_SHOP_ITEM_USE_REQUEST, lpMsg, Packet, 0);

	sLog->outInfo(LOG_CASHSHOP, "CashShopItemUseRequest() - %s. Code: %u, Serial: %u", 
		pPlayer->BuildLog().c_str(), lpMsg->code, lpMsg->serial);

	CashShopItemData const* item_stored = pPlayer->CashShopGetItem(lpMsg->code, lpMsg->serial);

	if ( !item_stored )
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemUseRequest() - %s. Item [%u / %u] not stored", 
			pPlayer->BuildLog().c_str(), lpMsg->code, lpMsg->serial);

		this->CashShopItemUseResult(pPlayer, 1);
		return;
	}

	CashShopProduct const* product = this->GetProduct(item_stored->GetProduct(), item_stored->GetOption());

	if ( !product )
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemUseRequest() - %s. Wrong Cash Shop Item Data. Product: %u, Option: %u",
			pPlayer->BuildLog().c_str(), item_stored->GetProduct(), item_stored->GetOption());

		this->CashShopItemUseResult(pPlayer, 1);
		return;
	}

	if ( product->IsFlags(CASH_SHOP_FLAG_USE_DISABLED) )
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemUseRequest() - %s. Product can't be used. Product: %u, Option: %u",
			pPlayer->BuildLog().c_str(), item_stored->GetProduct(), item_stored->GetOption());

		this->CashShopItemUseResult(pPlayer, 1);
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(product->GetItem());
	buff_template const* item_buff_add = sSkillMgr->GetBuffItem(product->GetItem());

	if ( !item_info )
	{
		sLog->outError(LOG_CASHSHOP, "CashShopItemUseRequest() - %s. Wrong Item Data",
			pPlayer->BuildLog().c_str());

		this->CashShopItemUseResult(pPlayer, 1);
		return;
	}

	switch ( product->GetUseType() )
	{
	case CASH_SHOP_USE_TYPE_BUFF:
		{
			if ( !item_buff_add )
			{
				sLog->outError(LOG_CASHSHOP, "CashShopItemUseRequest() - %s. Wrong Buff Data",
					pPlayer->BuildLog().c_str());

				this->CashShopItemUseResult(pPlayer, 3);
				return;
			}

			if ( !product->IsFlags(CASH_SHOP_FLAG_REPLACE_BUFF) )
			{
				if ( !pPlayer->HasBuff(item_buff_add->GetBuff()) )
				{
					if ( pPlayer->HasBuffGroup(item_buff_add->GetBuff()) )
					{
						this->CashShopItemUseResult(pPlayer, 3);
						return;
					}
				}
			}
		} break;
	}

	uint16 code = item_stored->GetServer();
	uint32 serial = item_stored->GetSerial();
	uint32 serial_cash_shop = item_stored->GetSerialCashShop();

	switch ( product->GetUseType() )
	{
	case CASH_SHOP_USE_TYPE_INVENTORY:
		{
			uint8 existing_slot = -1;

			inventory_loop(i, 0, use_inventory_size)
			{
				if ( !pPlayer->GetInventory()->CanUseExpandedSlot(i) )
				{
					continue;
				}

				Item const* pItem = pPlayer->GetInventory()->GetItem(i);

				if ( !pItem )
				{
					continue;
				}

				if ( !pItem->IsItem() )
				{
					continue;
				}

				if ( !pItem->GetExpireDate() )
				{
					continue;
				}

				if ( pItem->GetItem() != product->GetItem() )
				{
					continue;
				}

				if ( pItem->GetLevel() != product->GetItemLevel() )
				{
					continue;
				}

				existing_slot = i;
			}

			if ( sGameServer->IsCashShopUseJoin() && existing_slot != uint8(-1) )
			{
				pPlayer->GetInventory()->GetItem(existing_slot)->SetExpireDate(pPlayer->GetInventory()->GetItem(existing_slot)->GetExpireDate() + product->GetDuration());
				pPlayer->GetInventory()->GetItem(existing_slot)->SetState(ITEM_CHANGED);
				pPlayer->CashShopSendExpireItem(existing_slot);
			}
			else
			{
				Item item(product->GetItem(), product->GetItemLevel(), product->GetItemDurability(), 
							product->GetItemSkill(), product->GetItemLuck(), product->GetItemOption(),
							product->GetItemExcellent(), product->GetItemAncient());

				item.SetSerialServer(code);
				item.SetSerial(serial);
				item.SetSerialShop(serial_cash_shop);

				if ( product->IsFlags(CASH_SHOP_FLAG_EXPIRABLE) )
				{
					item.SetExpireDate(product->GetDuration() + time(nullptr));
				}

				item.Convert();
				item.CalculateDurability();

				uint8 slot = pPlayer->AddItem(item, -1, ADD_ITEM_SEND_OK);

				if ( slot == uint8(-1) )
				{
					this->CashShopItemUseResult(pPlayer, 1);
					return;
				}
			}
		} break;

	case CASH_SHOP_USE_TYPE_BUFF:
		{
			time_t remain_time = pPlayer->GetBuffGroupTime(item_buff_add->GetBuff());

			pPlayer->RemoveBuff(item_buff_add);

			time_t duration = product->GetDuration();
			uint8 flags = BUFF_FLAG_SHOP | BUFF_FLAG_DB_STORE;

			if ( product->IsFlags(CASH_SHOP_FLAG_EXPIRABLE) )
			{
				duration += time(nullptr);
				flags |= BUFF_FLAG_EXPIRE;
			}

			if ( product->IsFlags(CASH_SHOP_FLAG_ADD_BUFF_TIME) )
			{
				duration += remain_time;
			}

			if ( product->IsFlags(CASH_SHOP_FLAG_ACCOUNT_BOUND) )
			{
				flags |= BUFF_FLAG_ACCOUNT;
			}

			pPlayer->AddBuff(item_buff_add->GetBuff(),
				BuffEffect(BuffOption(item_buff_add->GetEffect(0)), item_buff_add->GetValue(0)),
				BuffEffect(BuffOption(item_buff_add->GetEffect(1)), item_buff_add->GetValue(1)),
				duration, flags);
		} break;
	}

	pPlayer->CashShopDeleteItem(lpMsg->code, lpMsg->serial);
	
	pPlayer->CalculateCharacter();
	this->CashShopItemUseResult(pPlayer, 0);
}
	
void CashShopMgr::CashShopItemUseResult(Player* pPlayer, uint8 result)
{
	CASH_SHOP_ITEM_USE_RESULT pMsg(result);
	pPlayer->sendPacket(MAKE_PCT(pMsg));
}

void CashShopMgr::CashShopItemUseRequestNew(Player* pPlayer, uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(pPlayer) )
	{
		return;
	}

	if ( !Player::InterfaceCheck(pPlayer, InterfaceData::CashShop, 0) )
	{
		return;
	}

	POINTER_PCT_LOG(CASH_SHOP_ITEM_USE_REQUEST_NEW, lpMsg, Packet, 0);

	CASH_SHOP_ITEM_USE_RESULT_NEW pMsg(0x04, lpMsg->item);
	pPlayer->SEND_PCT(pMsg);
}

void CashShopMgr::SendUpdatedData(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	/*this->SendCategoryList(pPlayer);
	this->SendPackageList(pPlayer);
	this->SendProductList(pPlayer);*/
}

void CashShopMgr::SendCategoryList(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	GamePacket data(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_CATEGORY);

	for (auto const& itr : this->m_CategoryMap)
	{
		CashShopCategory const* pData = itr.second;

		if (!pData)
		{
			continue;
		}

		data << pData->GetID() << pData->GetName() << pData->GetParent() << pData->GetMain();
	}

	pPlayer->SendPacket(&data);
}

void CashShopMgr::SendPackageList(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	GamePacket data(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_PACKAGE);

	for (auto const& itr : this->m_PackageMap)
	{
		CashShopPackage const* pData = itr.second;

		if (!pData)
		{
			continue;
		}

		if (pData->IsFlags(CASH_SHOP_FLAG_BUY_DISABLED))
		{
			continue;
		}

		data << (uint16)pData->GetCategory();
		data << pData->GetPosition();
		data << (uint16)pData->GetPackage();
		data << pData->GetName();

		std::map<uint32, uint32> price_map;
		std::map<uint32, uint32> product_map;
		uint8 product_count = 0;

		for (auto const& pProduct : this->m_ProductList)
		{
			if (!pProduct)
			{
				continue;
			}

			if (pProduct->GetPackage() != pData->GetPackage())
			{
				continue;
			}

			if (pProduct->IsFlags(CASH_SHOP_FLAG_BUY_DISABLED))
			{
				continue;
			}

			++product_count;
			price_map[pProduct->GetOption()] += pProduct->GetPrice();
			product_map[pProduct->GetProduct()] = pProduct->GetProduct();
		}

		uint32 price = 0xFFFFFFFF;
		for (auto const& it : price_map)
		{
			if (it.second < price)
			{
				price = it.second;
			}
		}

		data << price;

		data << pData->GetDescription();
		data << pData->GetPriceType();
		
		data << (uint8)product_count;
		for (auto const& it : product_map)
		{
			data << (uint16)it.first;
		}

		data << pData->GetDisplayItem();

		data << (uint8)price_map.size();
		for (auto const& it : price_map)
		{
			data << (uint8)it.first;
		}
	}

	pPlayer->SendPacket(&data);
}

void CashShopMgr::SendProductList(Player* pPlayer)
{
	if (!pPlayer)
	{
		return;
	}

	GamePacket data(HEADCODE_CASH_SHOP, SUBCODE_CASH_SHOP_PRODUCT);

	for (auto const& pData : this->m_ProductList)
	{
		if (!pData)
		{
			continue;
		}

		data << (uint16)pData->GetProduct();
		data << pData->GetName();

		if (pData->IsFlags(CASH_SHOP_FLAG_EXPIRABLE))
		{
			data << (uint8)0;
			data << (uint32)pData->GetDuration();
		}
		else
		{
			data << (uint8)1;

			if (pData->IsFlags(CASH_SHOP_FLAG_DISPLAY_COUNT))
			{
				data << (uint32)pData->GetCount();
			}
			else
			{
				data << (uint32)pData->GetItemDurability();
			}
		}
		
		data << pData->GetPrice();
		data << (uint8)pData->GetOption();
		data << pData->GetItem();
	}

	pPlayer->SendPacket(&data);
}