#ifndef CUSTOM_RANK_TITLE_H
#define CUSTOM_RANK_TITLE_H

struct RankTitle_Head : C2_HEADER_SUB {
	uint8 GUID;
};

struct RankTitle_Body {
	uint8 PosX;
	uint8 PosY;
	uint8 RankLevel;
};

class CCustomRankTitle {
	SingletonInstance(CCustomRankTitle);
public:
	CCustomRankTitle();
	virtual ~CCustomRankTitle();
	void SendRankTitleInfo(Player* pPlayer, Player* ViewPortPlayer);
private:
};

#endif