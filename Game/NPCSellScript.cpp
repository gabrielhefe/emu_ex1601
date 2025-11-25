NpcSellScript::NpcSellScript(Player* pPlayer)
{
        this->m_Player = pPlayer;
        this->Clear();
}

NpcSellScript::~NpcSellScript()
{
	
}

void NpcSellScript::Clear()
{
        for ( int32 i = 0; i < MAX_ITEM_SOLD; ++i )
        {
                this->m_Item[i].clear();
        }

        this->m_Count = MAX_ITEM_SOLD;
}

void NpcSellScript::AssignItem(Item const& add_item, uint8 slot, uint32 price)
{
        this->m_Item[slot].Set(add_item);
        this->m_Item[slot].SetName(add_item.GetName());
        this->m_Item[slot].SetOwner(this->m_Player->GetGUID());
        this->m_Item[slot].SetOwnerSlot(slot);
        this->m_Item[slot].SetState(ITEM_CHANGED);
        this->m_Item[slot].SetBox(ITEM_BOX_SELL_NPC);
        this->m_Item[slot].SetPersonalStorePrice(price);
        this->m_Item[slot].SetPersonalStorePriceJoB(0);
        this->m_Item[slot].SetPersonalStorePriceJoS(0);
        this->m_Item[slot].SetExpireDate(time(nullptr) + DAY);
        this->m_Item[slot].Convert();
}

void NpcSellScript::SaveDB(SQLTransaction & trans, uint8 rules)
{
        /*for (int32 i = 0; i < MAX_ITEM_SOLD; ++i)
        {
                this->GetItem(i)->SaveDB(trans, rules);
        }*/
        this->m_Player->SaveDBAllItemBox();
}

void NpcSellScript::LoadDBItem(const char* ItemArray, uint32 owner, ItemBox box)
{
        Tokenizer tokens(ItemArray, ';');
        std::string slotStr = tokens[1];
        uint8 slot = atoi(slotStr.c_str());

        this->m_Item[slot].LoadDB(ItemArray, owner, slot, box);

        if (this->m_Item[slot].IsExpired())
        {
                this->DeleteItem(slot);
        }
}

void NpcSellScript::LoadDBItemNew(const char* ItemArray, uint32 owner, ItemBox box)
{
        Tokenizer tokens(ItemArray, ';');
        std::string slotStr = tokens[0];
        uint8 slot = atoi(slotStr.c_str());

        this->m_Item[slot].LoadDBNew(ItemArray, owner, slot, box);

        if (this->m_Item[slot].IsExpired())
        {
                this->DeleteItem(slot);
        }
}
	
void NpcSellScript::DeleteItem(uint8 slot)
{
        this->m_Item[slot].clear(false);
        //this->m_Player->SaveSellNpcInventoryBinary();
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
        this->m_Count += 1;

        if ( this->m_Count >= MAX_ITEM_SOLD )
        {
                this->m_Count = 0;
        }

        return this->m_Count;
}