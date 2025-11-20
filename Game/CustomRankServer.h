#ifndef CUSTOM_RANK_SV_H
#define CUSTOM_RANK_SV_H

#define MAX_VIEW_RANK_PLAYER 100

struct RankServer_Head : C2_HEADER_SUB {
	uint16 count;
};

struct RankServer_Body {
	char pPlayerName[MAX_CHARACTER_LENGTH + 1];
	uint8 Class;
	uint8 Reset;
	uint16 Level;
	uint16 MasterLevel;
	uint16 MajesticLevel;
};

struct RankServerData {
	DECLARE_ENUM(uint8, Rank);
	DECLARE_ENUM(uint16, GUID);
	DECLARE_PROPERTY_STRING(pPlayerName);
	DECLARE_ENUM(uint8, Class);
	DECLARE_ENUM(uint8, Reset);
	DECLARE_ENUM(uint16, Level);
	DECLARE_ENUM(uint16, MasterLevel);
	DECLARE_ENUM(uint16, MajesticLevel);
};

typedef std::list<RankServerData*> RankData;

class CCustomRankServer {
	SingletonInstance(CCustomRankServer);
public:
	CCustomRankServer();
	virtual ~CCustomRankServer();

	void LoadRankServer();
	void SendRankServerData(Player* pPlayer);
private:
	RankData sv_rank_list_data;
};

#endif