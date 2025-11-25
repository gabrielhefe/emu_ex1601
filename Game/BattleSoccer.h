/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "BattleSoccer.h"
*/
#ifndef BATTLE_SOCCER_H
#define BATTLE_SOCCER_H

struct BattleSoccerPlayer
{
    Player* m_Player;
    bool m_Master;

    BattleSoccerPlayer(): m_Player(nullptr), m_Master(false) {}

    void Reset()
    {
        this->m_Player = nullptr;
        this->m_Master = false;
    }
};

class BattleSoccerTeam
{
    public:
        BattleSoccerTeam();

        void Reset();

        Guild* m_Guild;
        int32 m_Score;
        uint16 m_PartyID;
        BattleSoccerPlayer m_Player[MAX_PARTY_MEMBERS];
};

class CBattleSoccerMgr
{
    SingletonInstance(CBattleSoccerMgr);

    public:
        CBattleSoccerMgr();
        virtual ~CBattleSoccerMgr();

        void Update();

        void SetState_None();
        void SetState_StandBy();
        void SetState_Playing();
        void SetState_Playend();

        void ProcState_None();
        void ProcState_StandBy();
        void ProcState_Playing();
        void ProcState_Playend();

        void UpdateBall(Monster* Ball);

        bool IsInField(Player const* pPlayer) const;
        bool GetRespawnLocation(int16 & x, int16 & y, Player* pPlayer);
        bool IsAttackAllowed(Player* pPlayer01, Player* pPlayer02);

        void MovePlayersOut();

        void ProcessRequest(Player* pPlayer, const char * guild);
        bool PartySuccess(Party* pParty, Guild* pGuild);
        void ProcessTeams(Player* pPlayer01, Player* pPlayer02, Guild* pGuild01, Guild* pGuild02);
        void SetTeamData(Guild* pGuild01, Guild* pGuild02);

        void Finish(uint8 reason, uint8 winner, uint8 looser, int32 score);

        void SendScore();
    public:
        BattleSoccerTeam m_Team[GUILD_WAR_TEAM_MAX];
        BattleSoccerState m_State;
        TickTimer m_Time;
        TickTimer m_BallMoveTime;
        bool m_BallMove;
};

#endif
