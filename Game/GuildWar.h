#ifndef GUILD_WAR_H
#define GUILD_WAR_H

class GuildWarData
{
        public:
                explicit GuildWarData(uint32 Guild01, uint32 Guild02)
                {
                        this->m_Guild01 = Guild01;
                        this->m_Guild02 = Guild02;
                        this->m_Count = 0;
                        this->m_WarTime.Reset();
                        this->m_ResetTime.Reset();
                        this->m_WarTime.Start();
                        this->m_ResetTime.Start();
                }

        private:
                uint32 m_Guild01;
                uint32 m_Guild02;
                int32 m_Count;
                TickTimer m_WarTime;
                TickTimer m_ResetTime;
};

typedef std::list<GuildWarData*> GuildWarDataList;

class GuildWarMgr
{
	SingletonInstance(GuildWarMgr);

	public:
		GuildWarMgr();
		virtual ~GuildWarMgr();

		void Update();

		void StartWarRequest(Player* pPlayer, const char * guild);
		void StartWarRequestResult(Player* pPlayer, uint8 result);

		void StartWarRequestSend(Player* pPlayer, const char * guild, GuildWarType type);
		void StartWarResult(Player* pPlayer, uint8 * Packet);

		void SendStart(Player* pPlayer, const char * guild, uint8 type, uint8 team);
		void SendScore(Player* pPlayer, int32 score01, int32 score02);
		void SendEnd(Player* pPlayer, const char * guild, uint8 result);

		bool IsWarAllowed(Guild* pGuild01, Guild* pGuild02);
		void StartWar(Guild* pGuild01, Guild* pGuild02);
		void EndWar(Guild* pLooser, Guild* pWinner, uint8 reason);
		
		void IncreaseScore(Player* pPlayer, Player* pVictim);
	private:
		GuildWarDataList guild_war_list;
};

#endif
