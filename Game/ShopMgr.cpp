/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ShopMgr.cpp"
*
*/

Shop::Shop()
{
	for (int32 i = 0; i < max_shop_item; ++i)
	{
		GetItem(i)->clear();
		GetItem(i)->position.set(-1);
		_shopMap[i] = 0;
	}

	SetMaxBuyCount(0);
	SetMaxBuyType(SHOP_COUNT_NONE);
}

void Shop::AddItem(ItemShop item)
{
	auto const item_info = sItemMgr->GetItem(item.GetItem());
	if (!item_info)
		return;

	if (GetType() != SHOP_TYPE_RUUD)
	{
		uint8 blank = 0xFF;

		for (uint8 Y = 0; Y < 15; ++Y)
		{
			for (uint8 X = 0; X < 8; ++X)
			{
				if (_shopMap[X + Y * 8] == 0)
				{
					blank = MapCheck(X, Y, item_info->GetX(), item_info->GetY());
					if (blank != 0xFF)
					{
						goto SkipLoop;
					}
				}
			}
		}

		if (blank == 0xFF)
			return;

	SkipLoop:
		item.CalculateDurability();
		item.Convert();

		item.position.set(blank);
		this->SetItem(blank, item);
		this->IncreaseItemCount(1);
	}
	else
	{
		item.CalculateDurability();
		item.Convert();

		item.position.set(this->GetItemCount());
		this->SetItem(this->GetItemCount(), item);
		this->IncreaseItemCount(1);
	}
}

uint8 Shop::MapCheck(uint8 X, uint8 Y, uint8 W, uint8 H)
{
	if ((X + W) > 8)
		return 0xFF;

	if ((Y + H) > 15)
		return 0xFF;

	for (uint8 YY = 0; YY < H; ++YY)
	{
		for (uint8 XX = 0; XX < W; ++XX)
		{
			if (_shopMap[(Y + YY) * 8 + (X + XX)] == 1)
				return 0xFF;
		}
	}

	for (uint8 YY = 0; YY < H; ++YY)
	{
		for (uint8 XX = 0; XX < W; ++XX)
		{
			_shopMap[(Y + YY) * 8 + (X + XX)] = 1;
		}
	}

	return (X + Y * 8);
}

ShopMgr::ShopMgr() : _updateDay(-1)
{
	
}

ShopMgr::~ShopMgr()
{
	CLEAR_MAP(_shops);
	_characterPurchases.clear();
	_accountPurchases.clear();
	_pcPurchases.clear();
	_serverPurchases.clear();
}

void ShopMgr::LoadShopTemplate(char* pchFileName)
{
	sLog->outLoad(true, "Loading Shop Template...");

	CLEAR_MAP(_shops);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("ShopList");


	for (pugi::xml_node shop = Main.child("Shop"); shop; shop = shop.next_sibling()) {
		Shop* add_shop = new Shop;

		add_shop->SetID(shop.attribute("ID").as_int());
		add_shop->SetNpcIndex(shop.attribute("NPCIndex").as_int());
		add_shop->SetName(shop.attribute("Name").as_string());
		add_shop->SetMaxPKLevel(shop.attribute("MaxPkLevel").as_int());
		add_shop->SetPKText(shop.attribute("Message").as_string());
		add_shop->SetFlag(shop.attribute("Flags").as_int());
		add_shop->SetType(shop.attribute("Type").as_int());
		add_shop->SetMaxBuyCount(shop.attribute("MaxBuyCount").as_int());
		add_shop->SetMaxBuyType(shop.attribute("MaxBuyType").as_int());
		add_shop->SetItemCount(0);
		add_shop->SetFileScriptsName(shop.attribute("FileScriptsName").as_string());

		_shops[add_shop->GetID()] = add_shop;
	}

	sLog->outLoad(false, ">> Loaded %u shop definitions", _shops.size());
}

void ShopMgr::LoadShopScripts() {
	sLog->outLoad(true, "Loading Shop Items...");

	uint32 count = 0;

	for (auto& itr : _shops)
	{
		auto shop = itr.second;
		std::string pathfile = "../Data/Shops/Scripts/";
		pathfile = pathfile + shop->GetFileScriptsName().c_str();
		LoadShopItems(pathfile.c_str(), shop->GetID());

		count++;
	}

	sLog->outLoad(false, ">> Loaded %u shop items", count);
}

void ShopMgr::LoadShopItems(const char* pchFileName, uint8 ShopID)
{

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("ShopItem");


	for (pugi::xml_node item = Main.child("Item"); item; item = item.next_sibling()) {
		auto shop = GetShop(ShopID);
		if (!shop)
			continue;

		ItemShop additem;
		additem.clear();
		additem.SetItem(ITEMGET(item.attribute("ItemCat").as_int(), item.attribute("ItemIndex").as_int()));
		additem.SetLevel(item.attribute("ItemLevel").as_int());
		additem.SetDurability(static_cast<float>(item.attribute("ItemDur").as_int()));
		additem.SetSkill(item.attribute("ItemSkill").as_int());
		additem.SetLuck(item.attribute("ItemLuck").as_int());
		additem.SetOption(item.attribute("ItemOption").as_int());
		additem.SetExe(item.attribute("ItemExel").as_int());
		additem.SetAncient(item.attribute("ItemAncient").as_int());

		additem.SetSocket(0, item.attribute("Socket1").as_int());
		additem.SetSocket(1, item.attribute("Socket2").as_int());
		additem.SetSocket(2, item.attribute("Socket3").as_int());
		additem.SetSocket(3, item.attribute("Socket4").as_int());
		additem.SetSocket(4, item.attribute("Socket5").as_int());

		additem.SetPrice(item.attribute("Price").as_int());

		if (!sItemMgr->IsItem(additem.GetItem()))
		{
			sLog->outError("root", "%s :: Shop: %u, item: %d", __FUNCTION__, ShopID, additem.GetItem());
			continue;
		}

		shop->AddItem(additem);
	}
}

Shop * ShopMgr::GetShop(uint8 id)
{
	auto itr = _shops.find(id);
	if (itr != _shops.end())
		return itr->second;
	else
		return nullptr;
}

Shop const* ShopMgr::GetShop(uint8 id) const
{
	auto itr = _shops.find(id);
	if (itr != _shops.end())
		return itr->second;
	else
		return nullptr;
}

Shop const* ShopMgr::GetShop(std::string const& name) const
{
	for (auto & itr : _shops)
	{
		auto shop = itr.second;
		if (shop->GetName() == name)
			return shop;
	}

	return nullptr;
}

bool ShopMgr::IsShop(std::string const& name) const
{
	return this->GetShop(name) != nullptr;
}

bool ShopMgr::EnableToBuy(Player* player, Shop const* shop)
{
	if (!player || !shop)
		return false;

	int32 count = 0;

	switch (shop->GetMaxBuyType())
	{
	case SHOP_COUNT_CHARACTER:
	{
								 auto & data_map = _characterPurchases[shop->GetID()];

								 auto it = data_map.find(player->GetGUID());
								 if (it != data_map.end())
									 count = it->second;
	} break;

	case SHOP_COUNT_ACCOUNT:
	{
							   auto & data_map = _accountPurchases[shop->GetID()];

							   auto it = data_map.find(player->GetAccountData()->GetGUID());
							   if (it != data_map.end())
								   count = it->second;
	} break;

	case SHOP_COUNT_PC:
	{
						  auto & data_map = _pcPurchases[shop->GetID()];

						  auto it = data_map.find(player->GetAccountData()->GetDiskSerial());
						  if (it != data_map.end())
							  count = it->second;
	} break;

	case SHOP_COUNT_SERVER:
	{
							  auto & data_map = _serverPurchases[shop->GetID()];

							  auto it = data_map.find(sGameServer->GetServerCode());
							  if (it != data_map.end())
								  count = it->second;
	} break;

	default:
		return true;
		break;
	}

	if (count >= shop->GetMaxBuyCount())
	{
		player->SendMessageBox(0, "Error", "Reached maximum day buy count.");
		return false;
	}

	++count;

	switch (shop->GetMaxBuyType())
	{
	case SHOP_COUNT_CHARACTER:
		_characterPurchases[shop->GetID()][player->GetGUID()] = count;
		break;

	case SHOP_COUNT_ACCOUNT:
		_accountPurchases[shop->GetID()][player->GetAccountData()->GetGUID()] = count;
		break;

	case SHOP_COUNT_PC:
		_pcPurchases[shop->GetID()][player->GetAccountData()->GetDiskSerial()] = count;
		break;

	case SHOP_COUNT_SERVER:
		_serverPurchases[shop->GetID()][sGameServer->GetServerCode()] = count;
		break;
	}

	return true;
}

void ShopMgr::Update()
{
	auto time = Custom::SystemTimer();
	if (time.GetDay() != _updateDay)
	{
		_characterPurchases.clear();
		_accountPurchases.clear();
		_pcPurchases.clear();
		_serverPurchases.clear();

		_updateDay = time.GetDay();
	}
}
