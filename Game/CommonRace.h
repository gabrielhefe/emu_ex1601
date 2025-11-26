#ifndef COMMON_RACE_H
#define COMMON_RACE_H

enum CommonRaceState
{
	COMMON_RACE_STATE_NONE,
	COMMON_RACE_STATE_NOTIFY,
	COMMON_RACE_STATE_PLAYING,
};

struct CommonRaceGate
{
        explicit CommonRaceGate(int32 gate, int32 reward)
        {
                this->m_Gate = gate;
                this->m_Reward = reward;
        }

        int32 m_Gate = 0;
        int32 m_Reward = 0;
};

typedef std::vector<CommonRaceGate*> CommonRaceGateList;

struct CommonRacePlayer
{
        CommonRacePlayer()
        {
                this->m_StartedTime = time(nullptr);
                this->m_Participating = false;
                this->m_GateList.clear();
        }

        std::vector<int32> m_GateList;
        bool m_Participating = false;
        time_t m_StartedTime = 0;
};

typedef std::map<uint32, CommonRacePlayer> CommonRacePlayerMap;

class CommonRace: public EventCommon
{
	public:
		CommonRace();
		virtual ~CommonRace();

		void Clear();
		void Start(int32 duration, int32 notify);

		virtual void Update();

		void SetState_None();
		void SetState_Notify();
		void SetState_Playing();

		void ProcState_None();
		void ProcState_Notify();
                void ProcState_Playing();

                void PlayerInGate(Player* pPlayer, int32 gate);
                void ResetPlayer(Player* pPlayer);

                int32 m_Duration = 0;
                CommonRaceGateList m_GateList;
                CommonRacePlayerMap m_PlayerData;
                size_t m_GateReached = 0;
                std::string m_RaceName;
};

#endif