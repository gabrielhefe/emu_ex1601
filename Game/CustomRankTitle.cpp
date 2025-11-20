CCustomRankTitle::CCustomRankTitle() {

}

CCustomRankTitle::~CCustomRankTitle() {

}

void CCustomRankTitle::SendRankTitleInfo(Player* pPlayer, Player* ViewPortPlayer) {
	uint8 buffer[4092];
	POINTER_PCT(RankTitle_Head, head, buffer, 0);
	POINTER_PCT(RankTitle_Body, body, buffer, sizeof(RankTitle_Head));

	//head->GUID = ViewPortPlayer->GetGUID();
	head->GUID = ViewPortPlayer->GetEntry();
	body->RankLevel = 0;
	body->PosX = ViewPortPlayer->GetX();
	body->PosY = ViewPortPlayer->GetY();
	head->Set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_RANK_TITLE, sizeof(head) + sizeof(body));
	pPlayer->sendPacket(buffer, head->GetSize());
}