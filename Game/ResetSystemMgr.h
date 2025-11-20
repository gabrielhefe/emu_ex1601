#ifndef RESET_SYSTEM_H
#define	RESET_SYSTEM_H

struct reset_data
{
	DECLARE_ENUM(uint32, Id);
	DECLARE_ENUM(uint32, MinLevelRequire);
	DECLARE_ENUM(uint32, MinRSTime);
	DECLARE_ENUM(uint32, MaxRSTime);
	DECLARE_ENUM(uint32, PointsPerReset);
	DECLARE_ENUM(uint32, ZenRequire);
	DECLARE_ENUM(uint32, ChaosRequire);
	DECLARE_ENUM(uint32, CreRequire);
	DECLARE_ENUM(uint32, BlueRequire);
	DECLARE_ENUM(uint32, RewardItemType);
	DECLARE_ENUM(uint32, RewardItemIndex);
	DECLARE_ENUM(uint32, PointsOldIdRS);
};

typedef std::list<reset_data*> ResetDataList;

class CResetMgr {
	SingletonInstance(CResetMgr)

	public:
		CResetMgr();
		virtual ~CResetMgr();

		void LoadResetData(char* pchFileName);
		bool PlayerReset(Player* pPlayer);
		void AutoReset(Player* pPlayer);
	private:
		ResetDataList m_reset_data;
};

#endif
