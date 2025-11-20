NpcSellScript::NpcSellScript(Player* pPlayer)
{
	this->SetPlayer(pPlayer);
	this->Clear();
}

NpcSellScript::~NpcSellScript()
{
	
}

void NpcSellScript::Clear()
{
	for ( int32 i = 0; i < MAX_ITEM_SOLD; ++i )
	{
		this->GetItem(i)->clear();
	}

	this->SetCount(MAX_ITEM_SOLD);
}

void NpcSellScript::AssignItem(Item const& add_item, uint8 slot, uint32 price)
{
	//this->SetItem(slot, add_item);
	this->GetItem(slot)->Set(add_item);
	this->GetItem(slot)->SetName(add_item.GetName());
	this->GetItem(slot)->SetOwner(this->GetPlayer()->GetGUID());
	this->GetItem(slot)->SetOwnerSlot(slot);
	this->GetItem(slot)->SetState(ITEM_CHANGED);
	this->GetItem(slot)->SetBox(ITEM_BOX_SELL_NPC);
	this->GetItem(slot)->SetPersonalStorePrice(price);
	this->GetItem(slot)->SetPersonalStorePriceJoB(0);
	this->GetItem(slot)->SetPersonalStorePriceJoS(0);
	this->GetItem(slot)->SetExpireDate(time(nullptr) + DAY);
	this->GetItem(slot)->Convert();
}

void NpcSellScript::SaveDB(SQLTransaction & trans, uint8 rules)
{
	/*for (int32 i = 0; i < MAX_ITEM_SOLD; ++i)
	{
		this->GetItem(i)->SaveDB(trans, rules);
	}*/
	this->GetPlayer()->SaveDBAllItemBox();
}

void NpcSellScript::LoadDBItem(const char* ItemArray, uint32 owner, ItemBox box)
{
	Tokenizer tokens(ItemArray, ';');
	std::string slotStr = tokens[1];
	uint8 slot = atoi(slotStr.c_str());

	this->GetItem(slot)->LoadDB(ItemArray, owner, slot, box);

	if (this->GetItem(slot)->IsExpired())
	{
		this->DeleteItem(slot);
	}
}

void NpcSellScript::LoadDBItemNew(const char* ItemArray, uint32 owner, ItemBox box)
{
	Tokenizer tokens(ItemArray, ';');
	std::string slotStr = tokens[0];
	uint8 slot = atoi(slotStr.c_str());

	this->GetItem(slot)->LoadDBNew(ItemArray, owner, slot, box);

	if (this->GetItem(slot)->IsExpired())
	{
		this->DeleteItem(slot);
	}
}
	
void NpcSellScript::DeleteItem(uint8 slot)
{
	this->GetItem(slot)->clear(false);
	//this->GetPlayer()->SaveSellNpcInventoryBinary();
}

uint8 NpcSellScript::AddItem(Item const& item, uint32 price)
{
	uint8 slot = this->GetEmptySlot();

	if ( slot != 0xFF )
	{
		this->AssignItem(item, slot, price);
	}

	return slot;
}

uint8 NpcSellScript::GetEmptySlot()
{
	this->IncreaseCount(1);

	if ( this->GetCount() >= MAX_ITEM_SOLD )
	{
		this->SetCount(0);
	}

	return this->GetCount();
}