void WarehouseScript::LoadDBData(PreparedQueryResult result)
{
	StoreScript::Clear();
	this->SetZen(0);
	this->SetPassword(0);

	if ( result )
	{
		Field* fields = result->Fetch();

		this->SetZen(fields[0].GetUInt32());
		this->SetPassword(fields[1].GetUInt16());

		if ( this->GetPassword() )
		{
			this->SetLocked(true);
		}
	}
}

void WarehouseScript::LoadDBDataNew()
{
	StoreScript::Clear();
	this->SetZen(0);
	this->SetPassword(0);

	QueryResult result = MuDatabase.PQuery("SELECT a.money, a.password FROM account_warehouse a WHERE a.account_id = %d", this->GetPlayer()->GetAccountData()->GetGUID());

	if (result)
	{
		Field* fields = result->Fetch();

		this->SetZen(fields[0].GetUInt32());
		this->SetPassword(fields[1].GetUInt16());

		if (this->GetPassword())
		{
			this->SetLocked(true);
		}
	}
}

void WarehouseScript::LoadDBItemListNew()
{
	QueryResult load = MuDatabase.PQuery("SELECT inventory FROM account_warehouse where account_id=%d", this->GetPlayer()->GetAccountData()->GetGUID());

	if (load) {
		Field* field = load->Fetch();

		Tokenizer tokens1(field[0].GetBase64String().c_str(), ',');

		for (int i = 0; i < tokens1.size(); i++) {
			std::string buffer = tokens1[i];
			buffer.erase(buffer.begin());
			buffer.erase(buffer.end() - 1);

			Tokenizer tokens2(buffer.c_str(), ';');
			std::string slot = tokens2[0];
			std::string itemEntry = tokens2[1];

			if (warehouse_range(atoi(slot.c_str())) && sItemMgr->IsItem(atoi(itemEntry.c_str()))) {
				this->LoadDBItemNew(buffer.c_str(), this->GetPlayer()->GetAccountData()->GetGUID(), ITEM_BOX_WAREHOUSE);
			}
		}
	}
}

void WarehouseScript::Open()
{
	EXPANDED_WAREHOUSE_STATUS pMsg1(this->GetExpanded());
	this->GetPlayer()->sendPacket(MAKE_PCT(pMsg1));

	TALK_TO_NPC_RESULT pMsg2(0x02);
	this->GetPlayer()->sendPacket(MAKE_PCT(pMsg2));

	uint8 buffer[8192];
	SHOP_ITEM_LIST_HEAD * head = (SHOP_ITEM_LIST_HEAD*)buffer;
	SHOP_ITEM_LIST_BODY * body = (SHOP_ITEM_LIST_BODY*)&buffer[sizeof(SHOP_ITEM_LIST_HEAD)];
	head->count = 0;
	head->type = 0;

	for ( uint8 i = 0; i < warehouse_size; ++i )
	{
		if ( !this->GetItem(i)->IsItem() )
			continue;

		body[head->count].slot = i;
		this->GetItem(i)->ConvertToBuffer(body[head->count].item_info);
		head->count++;
	}

	head->h.set(HEADCODE_CLOSE_INTERFACE, sizeof(SHOP_ITEM_LIST_HEAD) + (sizeof(SHOP_ITEM_LIST_BODY) * head->count));
	
	this->GetPlayer()->sendPacket(buffer, head->h.get_size());

	this->GetPlayer()->WarehouseMoneyInOutResult(1, this->GetZen(), this->GetPlayer()->MoneyGet());

	if ( !this->GetPassword() )
	{
		this->GetPlayer()->WarehouseStateSend(0x00);
	}
	else if ( !this->IsLocked() )
	{
		this->GetPlayer()->WarehouseStateSend(0x0C);
	}
	else
	{
		this->GetPlayer()->WarehouseStateSend(0x01);
	}
}

void WarehouseScript::SaveDBData(SQLTransaction & trans)
{	
	QueryResult checkwarehouse = MuDatabase.PQuery("SELECT * FROM account_warehouse WHERE account_id=%d", this->GetPlayer()->GetAccountData()->GetGUID());

	if (!checkwarehouse) {
		QueryResult checkwarehouse = MuDatabase.PQuery("INSERT INTO account_warehouse (account_id, money, password) VALUES (%d,0,0)", this->GetPlayer()->GetAccountData()->GetGUID());
	}
	else {
		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(ACCOUNT_WAREHOUSE_DATA_REPLACE);
		stmt->setUInt32(0, this->GetPlayer()->GetAccountData()->GetGUID());
		stmt->setUInt32(1, this->GetZen());
		stmt->setUInt16(2, this->GetPassword());
		trans->Append(stmt);

		this->WarehouseSaveItemDbBinary();
	}
}

void WarehouseScript::Clear()
{
	StoreScript::Clear();
	this->SetZen(0);
	this->SetPassword(0);
	this->SetExpanded(0);
	this->SetExpandedTime(0);
	this->SetLocked(false);
}

uint8 WarehouseScript::RectCheck(uint8 x, uint8 y, uint8 width, uint8 height) const
{
	int32 slot = ((y * 8) + x);

	if( WAREHOUSE_MAIN_RANGE(slot) && ((x+width) > 8 || (y+height) > 15))
	{
		return 0xFF;
	}

	if( WAREHOUSE_EXT1_RANGE(slot) && ((x+width) > 8 || (y+height) > 30))
	{
		return 0xFF;
	}

	for( int32 sy = 0; sy < height; sy++)
	{
		for( int32 sx = 0; sx < width; sx++ )
		{
			if( this->item_map[(((sy+y)*8)+(sx+x))] != 0 )
			{
				return 0xFF;
			}
		}
	}

	return slot;
}

void WarehouseScript::ItemSet(uint8 slot, uint8 set)
{
	if( !warehouse_range(slot) )
	{
		return;
	}

	item_template const* item_info = sItemMgr->GetItem(this->GetItem(slot)->GetItem());

	if( !item_info )
	{
		return;
	}

	int32 x = slot % 8;
	int32 y = slot / 8;

	if( WAREHOUSE_MAIN_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 15) )
	{
		return;
	}

	if( WAREHOUSE_EXT1_RANGE(slot) && ((x + item_info->GetX()) > 8 || (y + item_info->GetY()) > 30) )
	{
		return;
	}

	for( int32 sy = 0; sy < item_info->GetY(); sy++ )
	{
		for( int32 sx = 0; sx < item_info->GetX(); sx++ )
		{
			this->item_map[(((sy+y)*8)+(sx+x))] = set;
		}
	}
}

void WarehouseScript::DeleteItem(uint8 slot)
{
	this->ItemSet(slot, 0);
	this->item[slot].clear(false);
	//this->WarehouseSaveItemDbBinary();
	//this->GetPlayer()->SaveItemDbBinary();
}

bool WarehouseScript::MoneyReachMaximum(uint32 ammount) const
{
	return (this->GetZen() + ammount) > sGameServer->GetWarehouseMaxMoney();
}

void WarehouseScript::MoneyAdd(uint32 ammount)
{
	if ( this->MoneyReachMaximum(ammount) )
	{
		this->SetZen(sGameServer->GetWarehouseMaxMoney());
	}
	else
	{
		this->SetZen(this->GetZen() + ammount);
	}
}

void WarehouseScript::MoneyReduce(uint32 ammount)
{
	if ( this->GetZen() < ammount )
	{
		this->SetZen(0);
	}
	else
	{
		this->SetZen(this->GetZen() - ammount);
	}
}

bool WarehouseScript::MoneyHave(uint32 ammount)
{
	return this->GetZen() >= ammount;
}

void Player::WarehouseClose()
{
	if ( !this->IsPlaying() )
		return;

	if ( this->GetInterfaceState()->GetID() != InterfaceData::Warehouse )
		return;

	this->WarehouseCloseResult();

	Player::TransactionSerialCheck(this, "WAREHOUSE");
	
	this->GetInterfaceState()->Reset();
}

void Player::WarehouseCloseResult()
{
	WAREHOUSE_CLOSE_RESULT pMsg;
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::WarehousePasswordRequest(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this, false) )
	{
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	WAREHOUSE_PASSWORD * lpMsg = (WAREHOUSE_PASSWORD*)Packet;

	STRING_SAFE_COPY(secure_code, MAX_SECURE_CODE_LENGTH + 1, lpMsg->secure_code, MAX_SECURE_CODE_LENGTH);

	switch ( lpMsg->type )
	{
	case 0: // Warehouse Open
		{
			if ( this->GetWarehouse()->IsLocked() )
			{
				if ( this->GetWarehouse()->GetPassword() == lpMsg->password )
				{
					this->GetWarehouse()->SetLocked(false);
					this->WarehouseStateSend(0x0C);
				}
				else
				{
					this->WarehouseStateSend(0x0A);
				}
			}
		} break;

	case 1: // Password Set
		{
			if ( !this->GetWarehouse()->IsLocked() )
			{
				if ( !this->SecureCodeCheck(secure_code) )
				{
					this->WarehouseStateSend(0x0D);
					return;
				}

				this->GetWarehouse()->SetPassword(lpMsg->password);
				this->GetWarehouse()->SetLocked(false);
				this->WarehouseStateSend(0x0C);
			}
			else
			{
				this->WarehouseStateSend(0x0B);
			}
		} break;

	case 2: // Password Delete
		{
			if ( !this->SecureCodeCheck(secure_code) )
			{
				this->WarehouseStateSend(0x0D);
				return;
			}

			this->GetWarehouse()->SetPassword(0);
			this->GetWarehouse()->SetLocked(false);
			this->WarehouseStateSend(0x00);
		} break;
	}
}

void Player::WarehouseStateSend(uint8 state)
{
	WAREHOUSE_STATE pMsg(state);
	this->sendPacket(MAKE_PCT(pMsg));
}

void Player::WarehouseMoneyInOut(uint8 * Packet)
{
	if ( !Player::IsPlayerBasicState(this) )
	{
		this->WarehouseMoneyInOutResult(0,0,0);
		return;
	}

	if ( this->GetInterfaceState()->GetID() != InterfaceData::Warehouse )
	{
		this->WarehouseMoneyInOutResult(0, 0, 0);
		return;
	}

	if ( !this->IsAuthorizationEnabled() )
	{
		this->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		this->WarehouseMoneyInOutResult(0, 0, 0);
		return;
	}

	POINTER_PCT(WAREHOUSE_MONEY_IN_OUT const, lpMsg, Packet, 0);

	switch ( lpMsg->type )
	{
	case 0x00:
		{
			if ( lpMsg->money == 0 || lpMsg->money > sGameServer->GetWarehouseMaxMoney() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			if ( !sGameServer->IsWarehouseSaveMoney() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			if ( this->GetWarehouse()->MoneyReachMaximum(lpMsg->money) )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}


			if ( !this->MoneyHave(lpMsg->money) )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			this->MoneyReduce(lpMsg->money, false);
			this->GetWarehouse()->MoneyAdd(lpMsg->money);
		} break;

	case 0x01:
		{
			if ( sGameServer->IsWarehouseLock() && this->GetWarehouse()->IsLocked() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				this->SendNotice(NOTICE_NORMAL_BLUE,"The vault is locked");
				return;
			}

			if ( lpMsg->money == 0 || lpMsg->money > sGameServer->GetWarehouseMaxMoney() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			if ( !sGameServer->IsWarehouseGetMoney() )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			if ( !this->GetWarehouse()->MoneyHave(lpMsg->money) )
			{
				this->WarehouseMoneyInOutResult(0, 0, 0);
				return;
			}

			this->GetWarehouse()->MoneyReduce(lpMsg->money);
			this->MoneyAdd(lpMsg->money, false);

			uint32 used_money = this->WarehouseGetUsedHowMuch();

			if ( this->MoneyHave(used_money) )
			{
				this->MoneyReduce(used_money, false);
			}
			else if ( this->GetWarehouse()->MoneyHave(used_money) )
			{
				this->GetWarehouse()->MoneyReduce(used_money);
			}
			else
			{
				this->GetWarehouse()->MoneyAdd(lpMsg->money);
				this->MoneyReduce(lpMsg->money, false);
				this->WarehouseMoneyInOutResult(0, 0, 0);
				this->SendNotice(NOTICE_NORMAL_BLUE, "You need %u Zen to use your vault", used_money);
				return;
			}
		} break;

	default:
		{
			this->WarehouseMoneyInOutResult(0, 0, 0);
			return;
		} break;
	}

	this->WarehouseMoneyInOutResult(1, this->GetWarehouse()->GetZen(), this->MoneyGet());
}

void Player::WarehouseMoneyInOutResult(uint8 result, uint32 w_money, uint32 i_money)
{
	WAREHOUSE_MONEY_IN_OUT_RESULT pMsg(result, w_money, i_money);
	this->sendPacket(MAKE_PCT(pMsg));
}

uint32 Player::WarehouseGetUsedHowMuch()
{
	uint32 money = floor(double((this->GetTotalLevel() * this->GetTotalLevel()) * 0.1f * 0.4f));

	if ( sGameServer->IsWarehouseLock() && this->GetWarehouse()->IsLocked() )
	{
		money += this->GetLevel() * 2;
	}

	if ( money >= 1000 )
	{
		money = (money / 100) * 100;
	}
	else if ( money >= 100 )
	{
		money = (money / 10) * 10;
	}

	return money;
}

void WarehouseScript::AssignItem(Item const& add_item, uint8 slot)
{
	StoreScript::AssignItem(add_item, slot);
	this->item[slot].SetOwner(this->GetPlayer()->GetAccountData()->GetGUID());
	this->item[slot].SetBox(ITEM_BOX_WAREHOUSE);
	this->item[slot].SetState(ITEM_MOVE_WAREHOUSE);
}

uint8 WarehouseScript::AddItem(Item const& item, uint8 slot)
{
	if( !warehouse_range(slot) )
	{
		return 0xFF;
	}

	if( this->GetItem(slot)->IsItem() || !item.IsItem() )
	{
		return 0xFF;
	}

	item_template const* item_info = sItemMgr->GetItem(item.GetItem());

	if( !item_info )
	{
		return 0xFF;
	}

	int32 x = slot % 8;
	int32 y = slot / 8;

	if( this->RectCheck(x, y, item_info->GetX(), item_info->GetY()) == 0xFF)
	{
		return 0xFF;
	}
	
	this->AssignItem(item, slot);
	this->ItemSet(slot, 1);
	this->WarehouseSaveItemDbBinary();
	this->GetPlayer()->SaveDBAllItemBox();
	return slot;
}

void WarehouseScript::WarehouseSaveItemDbBinary() {
	Player* pPlayer = this->GetPlayer();

	if (!pPlayer)
		return;

	std::ostringstream itemString;

	warehouse_loop(i) {
		if (!pPlayer->GetWarehouse()->GetItem(i)->IsItem())
			continue;

		char buff[200];

		uint32 Slot = pPlayer->GetWarehouse()->GetItem(i)->GetOwnerSlot();
		uint32 ItemId = pPlayer->GetWarehouse()->GetItem(i)->GetItem();
		uint16 SerialServer = pPlayer->GetWarehouse()->GetItem(i)->GetSerialServer();
		uint32 Serial = pPlayer->GetWarehouse()->GetItem(i)->GetSerial();
		uint32 SerialShop = pPlayer->GetWarehouse()->GetItem(i)->GetSerialShop();
		uint8 ItemLevel = pPlayer->GetWarehouse()->GetItem(i)->GetLevel();
		uint8 ItemDurability = static_cast<uint8>(pPlayer->GetWarehouse()->GetItem(i)->GetDurability());
		uint32 ItemDurabilityState = static_cast<int32>(pPlayer->GetWarehouse()->GetItem(i)->GetDurabilityState());
		uint8 ItemSkill = pPlayer->GetWarehouse()->GetItem(i)->GetSkill();
		uint8 ItemLuck = pPlayer->GetWarehouse()->GetItem(i)->GetLuck();
		uint8 ItemOption = pPlayer->GetWarehouse()->GetItem(i)->GetOption();
		uint8 ItemExe = pPlayer->GetWarehouse()->GetItem(i)->GetExe();
		uint8 ItemAncient = pPlayer->GetWarehouse()->GetItem(i)->GetAncient();
		uint8 Item380 = pPlayer->GetWarehouse()->GetItem(i)->Get380();
		uint8 ItemHarmony = pPlayer->GetWarehouse()->GetItem(i)->GetHarmony();
		uint16 Socket1 = pPlayer->GetWarehouse()->GetItem(i)->GetSocket(0);
		uint16 Socket2 = pPlayer->GetWarehouse()->GetItem(i)->GetSocket(1);
		uint16 Socket3 = pPlayer->GetWarehouse()->GetItem(i)->GetSocket(2);
		uint16 Socket4 = pPlayer->GetWarehouse()->GetItem(i)->GetSocket(3);
		uint16 Socket5 = pPlayer->GetWarehouse()->GetItem(i)->GetSocket(4);
		uint16 SocketBonus = pPlayer->GetWarehouse()->GetItem(i)->GetSocketBonus();
		uint8 IsInventoryActive = pPlayer->GetWarehouse()->GetItem(i)->IsInventoryActive() ? 1 : 0;
		uint16 dataExtra0 = pPlayer->GetWarehouse()->GetItem(i)->raw.data_extra[0];
		uint16 dataExtra1 = pPlayer->GetWarehouse()->GetItem(i)->raw.data_extra[1];
		uint16 dataExtra2 = pPlayer->GetWarehouse()->GetItem(i)->raw.data_extra[2];
		uint64 PersonalStorePrice = pPlayer->GetWarehouse()->GetItem(i)->GetPersonalStorePrice();
		uint64 PersonalStoreJoB = pPlayer->GetWarehouse()->GetItem(i)->GetPersonalStorePriceJoB();
		uint64 PersonalStoreJoS = pPlayer->GetWarehouse()->GetItem(i)->GetPersonalStorePriceJoS();
		uint64 ExpireDate = pPlayer->GetWarehouse()->GetItem(i)->GetExpireDate();
		uint32 DBFlag = pPlayer->GetWarehouse()->GetItem(i)->GetDBFlag();

		sprintf(buff, "{%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d},", Slot, ItemId, SerialServer, Serial, SerialShop, ItemLevel, ItemDurability, ItemDurabilityState, ItemSkill,
			ItemLuck, ItemOption, ItemExe, ItemAncient, Item380, ItemHarmony, Socket1, Socket2, Socket3, Socket4, Socket5, SocketBonus, IsInventoryActive, dataExtra0, dataExtra1, dataExtra2, PersonalStorePrice,
			PersonalStoreJoB, PersonalStoreJoS, ExpireDate, DBFlag);
		itemString << buff;
	}

	char buffer[55000];
	strcpy(buffer, base64::encode((uint8*)itemString.str().c_str(), itemString.str().size()).c_str());

	QueryResult result = MuDatabase.PQuery("UPDATE account_warehouse SET inventory='%s' WHERE account_id=%d", buffer, pPlayer->GetAccountData()->GetGUID());
}
