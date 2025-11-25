CNoticeSystem::CNoticeSystem() {

}

CNoticeSystem::~CNoticeSystem() {

}

void CNoticeSystem::Load(char* pchFileName) {
	LIST_CLEAR(NoticeMonsterKill::iterator, m_notice_system_monster_kill);
	LIST_CLEAR(NoticeSystemItemGet::iterator, m_notice_system_item_get);
	LIST_CLEAR(NoticeMonsterSpawn::iterator, m_notice_system_monster_spawn);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("NoticeSystem");
	pugi::xml_node PVPNoticeSetting = Main.child("PvPNotice");
	pugi::xml_node Tags = Main.child("Tags");
	pugi::xml_node Messages = Main.child("Messages");
	pugi::xml_node ItemGetNotice = Main.child("ItemGetNotice");
	pugi::xml_node MonsterKillNotice = Main.child("MonsterKillNotice");
	pugi::xml_node MonsterSpawnNotice = Main.child("MonsterSpawnNotice");
	pugi::xml_node MossMerchant = Main.child("MossMerchant");

	sGameServer->SetNoticeSystemEnabled(Main.attribute("Enabled").as_bool());
	sGameServer->SetPVPNoticeEnabled(PVPNoticeSetting.attribute("Enable").as_bool());
	sGameServer->SetItemGetNoticeEnabled(ItemGetNotice.attribute("Enabled").as_bool());
	sGameServer->SetMonsterKillNoticeEnabled(MonsterKillNotice.attribute("Enabled").as_bool());
	sGameServer->SetMonsterSpawnNoticeEnabled(MonsterSpawnNotice.attribute("Enabled").as_bool());
	sGameServer->SetMossMerchantNotifyEnbaled(MossMerchant.attribute("Enabled").as_bool());

	for (pugi::xml_node tag = Tags.child("Tag"); tag; tag = tag.next_sibling()) {
		if (tag.attribute("ID").as_int() == 0) {
			strcpy(texttagitemnormal, tag.attribute("Text").as_string());
		}
		else if (tag.attribute("ID").as_int() == 1) {
			strcpy(texttagitemexcl, tag.attribute("Text").as_string());
		}
		else if (tag.attribute("ID").as_int() == 2) {
			strcpy(texttagitemancient, tag.attribute("Text").as_string());
		}
	}

	for (pugi::xml_node mess = Messages.child("Msg"); mess; mess = mess.next_sibling()) {
		if (mess.attribute("ID").as_int() == 0) {
			strcpy(textgetitemnotice, mess.attribute("Text").as_string());
		}
		else if (mess.attribute("ID").as_int() == 1) {
			strcpy(textkillednotice, mess.attribute("Text").as_string());
		}
		else if (mess.attribute("ID").as_int() == 2) {
			strcpy(textspawnmonsternotice, mess.attribute("Text").as_string());
		}
	}

	for (pugi::xml_node item = ItemGetNotice.child("Item"); item; item = item.next_sibling()) {
		NoticeSystemItemGetNotice* data = new NoticeSystemItemGetNotice;
		data->SetItemCat(item.attribute("Cat").as_int());
		data->SetItemIndex(item.attribute("Index").as_int());
		data->SetItemLevel(item.attribute("Level").as_int());
		data->SetExl(item.attribute("IsExc").as_bool());
		data->SetAncient(item.attribute("IsSet").as_bool());

		m_notice_system_item_get.push_back(data);
	}

	for (pugi::xml_node monster = MonsterKillNotice.child("Monster"); monster; monster = monster.next_sibling()) {
		NoticeSystemMonsterKill* data = new NoticeSystemMonsterKill;

		data->SetMonsterID(monster.attribute("ID").as_int());

		m_notice_system_monster_kill.push_back(data);
	}

	for (pugi::xml_node monster = MonsterSpawnNotice.child("Monster"); monster; monster = monster.next_sibling()) {
		NoticeSystemMonsterSpawn* data = new NoticeSystemMonsterSpawn;

		data->SetMonsterID(monster.attribute("ID").as_int());
		data->SetWorldID(monster.attribute("MapNum").as_int());

		m_notice_system_monster_spawn.push_back(data);
	}

	for (pugi::xml_node item = MossMerchant.child("Item"); item; item = item.next_sibling()) {
		MossMerchantItemNotify* data = new MossMerchantItemNotify;
		data->SetItemCat(item.attribute("Cat").as_int());
		data->SetItemIndex(item.attribute("Index").as_int());
		data->SetExcl(item.attribute("IsExc").as_bool());
		data->SetAncient(item.attribute("IsSet").as_bool());

		m_moss_merchant_notify.push_back(data);
	}
	sLog->outInfo(LOG_DEFAULT, ">> Loaded Notice System Succes");
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool CNoticeSystem::CheckItemNotice(Item item) {
	if (!sGameServer->IsItemGetNoticeEnabled())
		return false;

	for (NoticeSystemItemGet::const_iterator it = m_notice_system_item_get.begin(); it != m_notice_system_item_get.end(); it++) {
		if (ITEMGET((*it)->GetItemCat(), (*it)->GetItemIndex()) == item.GetItem()) {
			if ((*it)->GetItemLevel() >= item.GetLevel()) {
				if ((*it)->IsAncient() == item.IsAncient()) {
					return true;
				}

				if ((*it)->IsExl() == item.IsExcellent()) {
					return true;
				}

				if (!(*it)->IsAncient() && !(*it)->IsExl()) {
					return true;
				}
			}
		}
	}

	return false;
}

bool CNoticeSystem::CheckMonsterKillNotice(Monster* monster) {
	if (!sGameServer->IsMonsterKillNoticeEnabled())
		return false;

	for (NoticeMonsterKill::const_iterator it = m_notice_system_monster_kill.begin(); it != m_notice_system_monster_kill.end(); it++) {
		if ((*it)->GetMonsterID() == monster->GetMonsterTemplate()->Id) {
			return true;
		}
	}

	return false;
}

bool CNoticeSystem::CheckMonsterRespawnNotice(Monster* monster)
{
	if (!sGameServer->IsMonsterSpawnNoticeEnabled())
		return false;
	
	for (NoticeMonsterSpawn::iterator it = m_notice_system_monster_spawn.begin(); it != m_notice_system_monster_spawn.end(); it++) {
		if ((*it)->GetMonsterID() == monster->GetMonsterTemplate()->Id && (*it)->GetWorldID() == monster->GetWorldId()) {
			return true;
		}
	}

	return false;

}

bool CNoticeSystem::CheckItemMossMerchantNotify(Item item) {
	if (!sGameServer->IsMossMerchantNotifyEnbaled())
		return false;

	for (MerchantNotify::iterator it = m_moss_merchant_notify.begin(); it != m_moss_merchant_notify.end(); it++) {
		if (ITEMGET((*it)->GetItemCat(), (*it)->GetItemIndex()) == item.GetItem()) {
			if ((*it)->IsExcl() == item.IsExcellent()) {
				return true;
			}

			if ((*it)->IsAncient() == item.IsAncient()) {
				return true;
			}

			if (!(*it)->IsAncient() && !(*it)->IsExcl()) {
				return true;
			}
		}
	}

	return false;
}

void CNoticeSystem::NoticeGetItem(Player* pPlayer, Item item) {
	if (!pPlayer)
		return;

	char tagtext[15];

	if (item.IsAncient()) {
		sprintf(tagtext, texttagitemancient);

	}
	else if (item.IsExcellent()) {
		sprintf(tagtext, texttagitemexcl);

	}
	else {
		sprintf(tagtext, texttagitemnormal);
	}

	sObjectMgr->SendNoticeToAll(CUSTOM_MESSAGE_ID_NONE, textgetitemnotice, pPlayer->GetName(), pPlayer->GetWorldName(), tagtext, item.GetName());
}

void CNoticeSystem::NoticeKillPlayer(Player* pPlayerKill, Player* pPlayerDead) {
	if (!sGameServer->IsPVPNoticeEnabled())
		return;

	if (!pPlayerKill || !pPlayerDead)
		return;

	if (pPlayerKill->IsDueling() || pPlayerDead->IsDueling())
		return;

	sObjectMgr->SendNoticeToAll(CUSTOM_MESSAGE_ID_NONE, textkillednotice, pPlayerKill->GetName(), pPlayerDead->GetName(), pPlayerKill->GetWorldName(), pPlayerKill->GetX(), pPlayerKill->GetY());
}

void CNoticeSystem::NoticeKillMonster(Player* pPlayer, Monster* monster) {
	if (!pPlayer || !monster)
		return;

	sObjectMgr->SendNoticeToAll(CUSTOM_MESSAGE_ID_NONE, textkillednotice, pPlayer->GetName(), monster->GetName(), pPlayer->GetWorldName(), pPlayer->GetX(), pPlayer->GetY());
}

void CNoticeSystem::NoticeRespawnMonster(Monster* monster) {
	sObjectMgr->SendNoticeToAll(CUSTOM_MESSAGE_ID_NONE, textspawnmonsternotice, monster->GetName(), monster->GetWorldName(), monster->GetX(), monster->GetY());
}

void CNoticeSystem::NotifyMossMerchant(Player* pPlayer, Item item) {
	if (!pPlayer)
		return;

	char tagtext[15];
	uint32 getitem = item.GetItemType() * 512 + item.GetItemIndex();
	item_template const* itemnotify = sItemMgr->GetItem(item.GetItem());

	if (item.IsAncient()) {
		sprintf(tagtext, texttagitemancient);
		sDataServer->NoticeSend(0, "[%s] Open Box Moss Merchant received [%s %s]", pPlayer->GetName(), tagtext, itemnotify->GetName());
	}
	else if (item.IsExcellent()) {
		sprintf(tagtext, texttagitemexcl);
		sDataServer->NoticeSend(0, "[%s] Open Box Moss Merchant received [%s %s]", pPlayer->GetName(), tagtext, itemnotify->GetName());
	}
	else {
		sDataServer->NoticeSend(0, "[%s] Open Box Moss Merchant received [%s]", pPlayer->GetName(), itemnotify->GetName());
	}

}