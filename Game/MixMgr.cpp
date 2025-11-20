MixMgr::MixMgr()
{

}

MixMgr::~MixMgr()
{
	this->Clear();
}

void MixMgr::Clear()
{
	for ( MixDisabledMap::iterator itr = this->mix_disabled.begin(); itr != this->mix_disabled.end(); ++itr )
	{
		itr->second.clear();
	}

	this->mix_disabled.clear();

	LIST_CLEAR(MixSpellStoneList::iterator, this->m_MixSpellStoneList);

	LIST_CLEAR(MixGuardianList::iterator, this->m_MixGuardianList);
}

void MixMgr::LoadDisabled(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mix Disabled...");

	for ( MixDisabledMap::iterator itr = this->mix_disabled.begin(); itr != this->mix_disabled.end(); ++itr )
	{
		itr->second.clear();
	}

	this->mix_disabled.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("MixDisable");

	uint32 count = 0;

	for (pugi::xml_node mix = Main.child("Mix"); mix; mix = mix.next_sibling()) {
		this->mix_disabled[mix.attribute("ItemCat").as_int()][mix.attribute("ItemIndex").as_int()] = 1;
		count++;
	}
		
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mix disabled definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MixMgr::LoadSpellStone(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mix SpellStone...");

	LIST_CLEAR(MixSpellStoneList::iterator, this->m_MixSpellStoneList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	uint32 count = 0;

	pugi::xml_node Main = file.child("MixSpellStone");

	for (pugi::xml_node Mix = Main.child("Mix"); Mix; Mix = Mix.next_sibling()) {
		for (pugi::xml_node data = Mix.child("Data"); data; data = data.next_sibling()) {
			MixSpellStone* pData = new MixSpellStone;
			pData->SetType(Mix.attribute("Type").as_int());
			pData->SetCount(data.attribute("Count").as_int());

			pData->SetRate(0, data.attribute("Rate0").as_int());
			pData->SetRate(1, data.attribute("Rate1").as_int());
			pData->SetRate(2, data.attribute("Rate2").as_int());
			pData->SetRate(3, data.attribute("Rate3").as_int());
			pData->SetRate(4, data.attribute("Rate4").as_int());
			pData->SetRate(5, data.attribute("Rate5").as_int());
			pData->SetRate(6, data.attribute("Rate6").as_int());
			pData->SetRate(7, data.attribute("Rate7").as_int());
			pData->SetRate(8, data.attribute("Rate8").as_int());
			pData->SetRate(9, data.attribute("Rate9").as_int());
			pData->SetRate(10, data.attribute("Rate10").as_int());
			pData->SetRate(11, data.attribute("Rate11").as_int());
			pData->SetRate(12, data.attribute("Rate12").as_int());
			pData->SetRate(13, data.attribute("Rate13").as_int());
			pData->SetRate(14, data.attribute("Rate14").as_int());
			pData->SetRate(15, data.attribute("Rate15").as_int());

			this->m_MixSpellStoneList.push_back(pData);
			count++;
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mix spell stone definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void MixMgr::LoadGuardian(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Mix Guardian...");

	LIST_CLEAR(MixGuardianList::iterator, this->m_MixGuardianList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("GuardianItemMix");

	pugi::xml_node PetCreate = Main.child("PetItemCreation");
	pugi::xml_node GuardianCreate = Main.child("GuardianItemCreation");
	pugi::xml_node GuardianLevelUp = Main.child("GuardianItemLevelUp");
	pugi::xml_node GuardianUpgrade = Main.child("GuardianItemUpgrade");

	sGameServer->SetRareIceDragonRate(GuardianUpgrade.attribute("GuardianRareRate").as_int());
	sGameServer->SetRarePierceRyanRate(GuardianUpgrade.attribute("GuardianRareRate").as_int());

	uint32 count = 0;

	for (pugi::xml_node Mix = PetCreate.child("Mix"); Mix; Mix = Mix.next_sibling()) {
		MixGuardian* pData = new MixGuardian;
		pData->SetType(0);
		pData->SetName(Mix.attribute("Name").as_string());
		pData->SetID(Mix.attribute("ID").as_int());
		pData->SetMainItem(ITEMGET(Mix.attribute("MainItemCat").as_int(), Mix.attribute("MainItemIndex").as_int()));
		pData->SetMainItemLevel(Mix.attribute("MainItemLevel").as_int());
		pData->SetMainItemDurability(Mix.attribute("MainItemDur").as_int());
		pData->SetMaterialItem(0, ITEMGET(Mix.attribute("SubItemCat1").as_int(), Mix.attribute("SubItemIndex1").as_int()));
		pData->SetMaterialAmount(0, Mix.attribute("SubItemDur1").as_int());
		pData->SetMaterialItem(1, ITEMGET(Mix.attribute("SubItemCat2").as_int(), Mix.attribute("SubItemIndex2").as_int()));
		pData->SetMaterialAmount(1, Mix.attribute("SubItemDur2").as_int());
		pData->SetMaterialItem(2, ITEMGET(Mix.attribute("SubItemCat3").as_int(), Mix.attribute("SubItemIndex3").as_int()));
		pData->SetMaterialAmount(2, Mix.attribute("SubItemDur3").as_int());
		pData->SetMaterialItem(3, ITEMGET(Mix.attribute("SubItemCat4").as_int(), Mix.attribute("SubItemIndex4").as_int()));
		pData->SetMaterialAmount(3, Mix.attribute("SubItemDur4").as_int());
		pData->SetMaterialItem(4, ITEMGET(Mix.attribute("SubItemCat5").as_int(), Mix.attribute("SubItemIndex5").as_int()));
		pData->SetMaterialAmount(4, Mix.attribute("SubItemDur5").as_int());
		pData->SetPrice(Mix.attribute("ReqMoney").as_int());
		pData->SetSuccessRate(Mix.attribute("SuccessRate").as_int());
		pData->SetResultItem(ITEMGET(Mix.attribute("ResultItemCat").as_int(), Mix.attribute("ResultItemIndex").as_int()));
		pData->SetResultLevel(Mix.attribute("ResultItemLevel").as_int());

		this->m_MixGuardianList.push_back(pData);
		count++;
	}

	for (pugi::xml_node Mix = GuardianCreate.child("Mix"); Mix; Mix = Mix.next_sibling()) {
		MixGuardian* pData = new MixGuardian;

		pData->SetType(1);
		pData->SetName(Mix.attribute("Name").as_string());
		pData->SetID(Mix.attribute("ID").as_int());
		pData->SetMainItem(ITEMGET(Mix.attribute("MainItemCat").as_int(), Mix.attribute("MainItemIndex").as_int()));
		pData->SetMainItemLevel(Mix.attribute("MainItemLevel").as_int());
		pData->SetMainItemDurability(Mix.attribute("MainItemDur").as_int());
		pData->SetMaterialItem(0, ITEMGET(Mix.attribute("SubItemCat1").as_int(), Mix.attribute("SubItemIndex1").as_int()));
		pData->SetMaterialAmount(0, Mix.attribute("SubItemDur1").as_int());
		pData->SetMaterialItem(1, ITEMGET(Mix.attribute("SubItemCat2").as_int(), Mix.attribute("SubItemIndex2").as_int()));
		pData->SetMaterialAmount(1, Mix.attribute("SubItemDur2").as_int());
		pData->SetMaterialItem(2, ITEMGET(Mix.attribute("SubItemCat3").as_int(), Mix.attribute("SubItemIndex3").as_int()));
		pData->SetMaterialAmount(2, Mix.attribute("SubItemDur3").as_int());
		pData->SetMaterialItem(3, ITEMGET(Mix.attribute("SubItemCat4").as_int(), Mix.attribute("SubItemIndex4").as_int()));
		pData->SetMaterialAmount(3, Mix.attribute("SubItemDur4").as_int());
		pData->SetMaterialItem(4, ITEMGET(Mix.attribute("SubItemCat5").as_int(), Mix.attribute("SubItemIndex5").as_int()));
		pData->SetMaterialAmount(4, Mix.attribute("SubItemDur5").as_int());
		pData->SetPrice(Mix.attribute("ReqMoney").as_int());
		pData->SetSuccessRate(Mix.attribute("SuccessRate").as_int());
		pData->SetResultItem(ITEMGET(Mix.attribute("ResultItemCat").as_int(), Mix.attribute("ResultItemIndex").as_int()));
		pData->SetResultLevel(Mix.attribute("ResultItemLevel").as_int());

		this->m_MixGuardianList.push_back(pData);
		count++;
	}

	for (pugi::xml_node Mix = GuardianLevelUp.child("Mix"); Mix; Mix = Mix.next_sibling()) {
		MixGuardian* pData = new MixGuardian;

		pData->SetType(2);
		pData->SetName(Mix.attribute("Name").as_string());
		pData->SetID(Mix.attribute("ID").as_int());
		pData->SetMainItem(ITEMGET(-1,-1));
		pData->SetMainItemLevel(Mix.attribute("ReqLevel").as_int());
		pData->SetMainItemDurability(-1);
		pData->SetMaterialItem(0, ITEMGET(Mix.attribute("ItemCat1").as_int(), Mix.attribute("ItemIndex1").as_int()));
		pData->SetMaterialAmount(0, Mix.attribute("ItemCount1").as_int());
		pData->SetMaterialItem(1, ITEMGET(Mix.attribute("ItemCat2").as_int(), Mix.attribute("ItemIndex2").as_int()));
		pData->SetMaterialAmount(1, Mix.attribute("ItemCount2").as_int());
		pData->SetMaterialItem(2, ITEMGET(Mix.attribute("ItemCat3").as_int(), Mix.attribute("ItemIndex3").as_int()));
		pData->SetMaterialAmount(2, Mix.attribute("ItemCount3").as_int());
		pData->SetMaterialItem(3, ITEMGET(Mix.attribute("ItemCat4").as_int(), Mix.attribute("ItemIndex4").as_int()));
		pData->SetMaterialAmount(3, Mix.attribute("ItemCount4").as_int());
		pData->SetMaterialItem(4, ITEMGET(Mix.attribute("ItemCat5").as_int(), Mix.attribute("ItemIndex5").as_int()));
		pData->SetMaterialAmount(4, Mix.attribute("ItemCount5").as_int());
		pData->SetPrice(Mix.attribute("ReqMoney").as_int());
		pData->SetSuccessRate(Mix.attribute("SuccessRate").as_int());
		pData->SetResultItem(ITEMGET(-1, -1));
		pData->SetResultLevel(-1);

		this->m_MixGuardianList.push_back(pData);
		count++;
	}

	for (pugi::xml_node Mix = GuardianUpgrade.child("Mix"); Mix; Mix = Mix.next_sibling()) {
		MixGuardian* pData = new MixGuardian;

		pData->SetType(4);
		pData->SetName(Mix.attribute("Name").as_string());
		pData->SetID(Mix.attribute("ID").as_int());
		pData->SetMainItem(ITEMGET(Mix.attribute("MainItemCat").as_int(), Mix.attribute("MainItemIndex").as_int()));
		pData->SetMainItemLevel(Mix.attribute("MainItemLevel").as_int());
		pData->SetMainItemDurability(Mix.attribute("MainItemDur").as_int());
		pData->SetMaterialItem(0, ITEMGET(Mix.attribute("SubItemCat1").as_int(), Mix.attribute("SubItemIndex1").as_int()));
		pData->SetMaterialAmount(0, Mix.attribute("SubItemDur1").as_int());
		pData->SetMaterialItem(1, ITEMGET(Mix.attribute("SubItemCat2").as_int(), Mix.attribute("SubItemIndex2").as_int()));
		pData->SetMaterialAmount(1, Mix.attribute("SubItemDur2").as_int());
		pData->SetMaterialItem(2, ITEMGET(Mix.attribute("SubItemCat3").as_int(), Mix.attribute("SubItemIndex3").as_int()));
		pData->SetMaterialAmount(2, Mix.attribute("SubItemDur3").as_int());
		pData->SetMaterialItem(3, ITEMGET(Mix.attribute("SubItemCat4").as_int(), Mix.attribute("SubItemIndex4").as_int()));
		pData->SetMaterialAmount(3, Mix.attribute("SubItemDur4").as_int());
		pData->SetMaterialItem(4, ITEMGET(Mix.attribute("SubItemCat5").as_int(), Mix.attribute("SubItemIndex5").as_int()));
		pData->SetMaterialAmount(4, Mix.attribute("SubItemDur5").as_int());
		pData->SetPrice(Mix.attribute("ReqMoney").as_int());
		pData->SetSuccessRate(Mix.attribute("SuccessRate").as_int());
		pData->SetResultItem(ITEMGET(Mix.attribute("ResultItemCat").as_int(), Mix.attribute("ResultItemIndex").as_int()));
		pData->SetResultLevel(Mix.attribute("ResultItemLevel").as_int());

		this->m_MixGuardianList.push_back(pData);
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u mix guardian definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

MixGuardian const* MixMgr::GetGuardianMix(uint8 type, uint8 id) const
{
	for (auto & pData : this->m_MixGuardianList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

MixGuardian const* MixMgr::GetGuardianMix2(uint8 type, uint8 id, uint32 MainItem) const
{
	for (auto& pData : this->m_MixGuardianList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetType() == type && pData->GetID() == id)
		{
			pData->SetMainItem(MainItem);
			pData->SetResultItem(MainItem);
			return pData;
		}
	}

	return nullptr;
}

bool MixMgr::IsMixDisabled(uint8 type, uint16 id) const
{
	MixDisabledMap::const_iterator itr = this->mix_disabled.find(type);

	if ( itr != this->mix_disabled.end() )
	{
		MixDisabledList::const_iterator it = itr->second.find(id);

		return (it != itr->second.end());
	}
	else
	{
		return false;
	}
}

uint16 MixMgr::GetSpiritStoneCount(uint8 type, int64 price) const
{
	if (type >= MAX_SPIRIT_STONE)
	{
		return 0;
	}

	int32 division = std::floor(price / sGameServer->GetMixSpellStonePriceDivision(type));
	uint16 count = -1;
	uint16 id = -1;

	for (auto & pData : this->m_MixSpellStoneList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetType() != type)
		{
			continue;
		}

		if (division >= pData->GetCount())
		{
			count = pData->GetCount();
		}
	}

	//sServerLink->NoticeSend(NOTICE_GLOBAL, "Count: %d - Division: %d - Price: %d", count, division, price);

	if (count == uint16(-1))
	{
		return 0;
	}

	RandomValue<uint16> m_RandomValue(0);

	for (auto & pData : this->m_MixSpellStoneList)
	{
		if (!pData)
		{
			continue;
		}

		if (pData->GetType() != type)
		{
			continue;
		}

		if (pData->GetCount() != count)
		{
			continue;
		}

		for (int32 i = 0; i < MAX_SPELL_STONE_COUNT; ++i)
		{
			if (pData->GetRate(i) > 0)
			{
				m_RandomValue.AddValue(i, pData->GetRate(i));
			}
		}
	}

	uint8 mix_result = m_RandomValue.GetRandomValue(RANDOM_POOL_WEIGHT);

	return mix_result;
}