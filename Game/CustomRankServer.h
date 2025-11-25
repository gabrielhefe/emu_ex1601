#ifndef CUSTOM_RANK_SV_H
#define CUSTOM_RANK_SV_H

#include <list>
#include <string>

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
        uint8 m_Rank;
        uint16 m_GUID;
        std::string m_PlayerName;
        uint8 m_Class;
        uint8 m_Reset;
        uint16 m_Level;
        uint16 m_MasterLevel;
        uint16 m_MajesticLevel;
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
        RankData m_SvRankListData;
};

#endif
