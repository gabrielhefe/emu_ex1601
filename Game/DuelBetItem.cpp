CDuelBetIem::CDuelBetIem() {

}

CDuelBetIem::~CDuelBetIem() {

}

void CDuelBetIem::DuelBetRequest(Player* pPlayer, Player* playerTarget) {

	if (!Player::IsPlayerBasicState(playerTarget, false))
	{
		return;
	}

	if (!playerTarget->IsAuthorizationEnabled())
	{
		return;
	}

	if (!pPlayer->CanInteract(playerTarget, true))
	{
		return;
	}

	if (!playerTarget->IsFlag(CHARACTER_FLAG_REQUEST))
	{
		return;
	}

	if (pPlayer->GetInterfaceState()->GetID() != InterfaceData::None)
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are busy right now.");
		return;
	}

	if (playerTarget->GetInterfaceState()->GetID() != InterfaceData::None)
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target is busy right now.");
		return;
	}

	if (!pPlayer->IsWorldFlag(WORLD_FLAG_ALLOW_DUEL))
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to send duel request here.");
		return;
	}

	if (pPlayer->GetWorldId() == WORLD_CASTLE_SIEGE && sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START)
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't send duel request in Castle Siege Battle.");
		return;
	}

	if (!pPlayer->SameDimension(playerTarget))
	{
		return;
	}

	if (Util::Distance(pPlayer->GetX(), pPlayer->GetY(), playerTarget->GetX(), playerTarget->GetY()) > 5)
	{
		return;
	}

	if (Guild* pGuild = pPlayer->GuildGet())
	{
		if (pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE)
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't send duel request while in Guild War.");
			return;
		}
	}

	if (Guild* pGuild = playerTarget->GuildGet())
	{
		if (pGuild->GetWarData()->GetState() == GUILD_WAR_STATE_ACTIVE)
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target use is currently in Guild War.");
			return;
		}
	}

	if (pPlayer->GetTotalLevel() < sGameServer->dueling_min_level.get())
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Open only for level %d or higher.", sGameServer->dueling_min_level.get());
		return;
	}

	if (playerTarget->GetTotalLevel() < sGameServer->dueling_min_level.get())
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Open only for level %d or higher.", sGameServer->dueling_min_level.get());
		return;
	}

	if (pPlayer->IsDueling() || playerTarget->IsDueling())
	{
		return;
	}
	
	if (pPlayer->IsInSelfDefense())
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't send duel request while autodefense is active.");
		return;
	}

	if (playerTarget->IsInSelfDefense())
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target user has autodefense activated.");
		return;
	}

	DUEL_REQUEST_SEND pMsg1(pPlayer->GetEntry(), pPlayer->GetName());
	playerTarget->sendPacket(MAKE_PCT(pMsg1));
}

void CDuelBetIem::DuelBetAnswer(Player* pPlayer, uint8* Packet) {

	POINTER_PCT_LOG(DUEL_ANSWER, lpMsg, Packet, 0);

	Player* pPlayerTarget = sObjectMgr->FindPlayer(lpMsg->GetTarget());

	if (!Player::IsPlayerBasicState(pPlayerTarget, false))
	{
		return;
	}

	if (pPlayer->IsDuelClassic() != pPlayerTarget->IsDuelClassic())
	{
		pPlayerTarget->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duel Error.");
		return;
	}

	if (!pPlayer->IsDuelClassic() && pPlayer->GetDuelRoom() != pPlayerTarget->GetDuelRoom())
	{
		pPlayerTarget->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duel room error.");
		return;
	}

	pPlayer->GetInterfaceState()->Reset();
	pPlayerTarget->GetInterfaceState()->Reset();

	if (lpMsg->result == 1)
	{
		pPlayer->GetBetItemData(BET_ITEM_MAX)->SetDuelBetTradeAccept(true);
		pPlayerTarget->GetBetItemData(BET_ITEM_MAX)->SetDuelBetTradeAccept(true);

		Player::TradeBegin(pPlayer, pPlayerTarget);
	}
}


void CDuelBetIem::RewardItem(Player* pPlayerWin, Player* pPlayerLose, uint8 type) {
	if (type == 0) {

		//pPlayerWin->RollBackTransaction();
		pPlayerWin->SendInventory();
		pPlayerLose->SendInventory();

		for (uint8 i = 0; i < 32; ++i) {
			if (!pPlayerLose->GetTradeInventory()->GetItem(i)->IsItem())
			{
				continue;
			}

			sItemMgr->ItemSerialCreateGremoryCase(pPlayerWin, pPlayerLose->GetTradeInventory()->item[i], GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
			this->DeleteItemFinish(pPlayerLose, pPlayerLose->GetTradeInventory()->item[i]);
		}

		for (uint8 i = 0; i < 32; ++i) {
			if (!pPlayerWin->GetTradeInventory()->GetItem(i)->IsItem())
			{
				continue;
			}

			sItemMgr->ItemSerialCreateGremoryCase(pPlayerWin, pPlayerWin->GetTradeInventory()->item[i], GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::EVENT);
			this->DeleteItemFinish(pPlayerWin, pPlayerWin->GetTradeInventory()->item[i]);
		}

		//pPlayerWin->MoneySet(pPlayerWin->MoneyGet() + pPlayerWin->GetBetItemData(BET_ITEM_MAX)->GetBetZen() + pPlayerLose->GetBetItemData(BET_ITEM_MAX)->GetBetZen());
		pPlayerWin->MoneyAdd(pPlayerWin->GetBetItemData(BET_ITEM_MAX)->GetBetZen() + pPlayerLose->GetBetItemData(BET_ITEM_MAX)->GetBetZen());

		sPentagramSystem->ExchangePentagramItem(pPlayerWin, pPlayerLose);
		sPentagramSystem->ExchangePentagramItem(pPlayerLose, pPlayerWin);

		Player::TransactionDBSave(pPlayerWin, pPlayerLose);

		pPlayerWin->CommitTransaction();
		pPlayerLose->CommitTransaction();

		pPlayerWin->GetInterfaceState()->Reset();
		pPlayerLose->GetInterfaceState()->Reset();

		pPlayerWin->GetTradeInventory()->Clear();
		pPlayerLose->GetTradeInventory()->Clear();

		//pPlayerWin->TradeResult(TRADE_OK);
		//pPlayerLose->TradeResult(TRADE_OK);

		pPlayerWin->SendInventory();
		pPlayerLose->SendInventory();

		sEventInventory->GCEventItemListSend(pPlayerWin);
		sEventInventory->GCEventItemListSend(pPlayerLose);

		pPlayerWin->CashShopSendExpireItem();
		pPlayerLose->CashShopSendExpireItem();

		pPlayerWin->InventoryItemSend();
		pPlayerLose->InventoryItemSend();

		pPlayerWin->PreviewSend();
		pPlayerLose->PreviewSend();

		sItemMgr->IsUsingTransformationItem(pPlayerWin);
		sItemMgr->IsUsingTransformationItem(pPlayerLose);

		pPlayerWin->SaveDBAllItemBox();
		pPlayerLose->SaveDBAllItemBox();

		//Player::TransactionDBSave(pPlayerWin, pPlayerLose);

		//pPlayerLose->SendInventory();
		//pPlayerLose->InventoryItemSend();
		//pPlayerLose->PreviewSend();

		Player::TransactionSharedSerialCheck(pPlayerWin, pPlayerLose, "Duel Bet");
	}
	else if (type == 1) {

		pPlayerWin->MoneyAdd(pPlayerWin->GetBetItemData(BET_ITEM_MAX)->GetBetZen());
		pPlayerWin->MoneyAdd(pPlayerWin->GetBetItemData(BET_ITEM_MAX)->GetBetZen());

		pPlayerWin->RollBackTransaction();
		pPlayerLose->RollBackTransaction();

		pPlayerWin->GetInterfaceState()->Reset();
		pPlayerLose->GetInterfaceState()->Reset();

		pPlayerWin->GetTradeInventory()->Clear();
		pPlayerLose->GetTradeInventory()->Clear();

		pPlayerWin->SendInventory();
		pPlayerLose->SendInventory();

		sEventInventory->GCEventItemListSend(pPlayerWin);
		sEventInventory->GCEventItemListSend(pPlayerLose);

		pPlayerWin->CashShopSendExpireItem();
		pPlayerLose->CashShopSendExpireItem();

		pPlayerWin->InventoryItemSend();
		pPlayerLose->InventoryItemSend();

		pPlayerWin->PreviewSend();
		pPlayerLose->PreviewSend();

		sItemMgr->IsUsingTransformationItem(pPlayerWin);
		sItemMgr->IsUsingTransformationItem(pPlayerLose);

		//Player::TransactionSharedSerialCheck(pPlayerWin, pPlayerLose, "Duel Bet");
		//Player::TransactionSharedSerialCheck(pPlayerLose, pPlayerWin, "Duel Bet");
	}
}

void CDuelBetIem::DuelBegin(Player* pPlayer, Player* playerTarget) {

	DuelRoom* room = sDuelMgr->GetFreeRoom();

	if (!room)
	{
		pPlayer->DuelResult(0x10, playerTarget->GetEntry(), playerTarget->GetName());
		return;
	}

	pPlayer->GetInterfaceState()->Set(InterfaceData::Duel, playerTarget, 0, sGameServer->dueling_max_wait_interval.get());
	pPlayer->SetDuelRoom(room->GetRoomID());
	pPlayer->SetDuelClassic(false);

	playerTarget->GetInterfaceState()->Set(InterfaceData::Duel, pPlayer, 0, sGameServer->dueling_max_wait_interval.get());
	playerTarget->SetDuelRoom(room->GetRoomID());
	playerTarget->SetDuelClassic(false);

	room->SetDuelers(pPlayer, playerTarget);
	room->SetState_Requested(); 


	uint16 world_id = 40;

	int16 x = 0;
	int16 y = 0;

	pPlayer->SetDueling(true);
	pPlayer->SetDuelScore(0);

	playerTarget->SetDueling(true);
	playerTarget->SetDuelScore(0);

	pPlayer->DuelResult(0, playerTarget->GetEntry(), playerTarget->GetName());
	playerTarget->DuelResult(0, pPlayer->GetEntry(), pPlayer->GetName());
	sDuelMgr->GetRoom(pPlayer->GetDuelRoom())->SetState_Started();
}

void CDuelBetIem::DeleteItemFinish(Player* pPlayer, Item item) {
	use_inventory_loop(i) {
		if (pPlayer->GetInventory()->GetItem(i)->GetSerial() == item.GetSerial()) {
			pPlayer->ClearItem(i);
		}
	}
}