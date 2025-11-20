#ifndef DYNAMIC_EXPERIENCE_H
#define DYNAMIC_EXPERIENCE_H

struct dynamic_exp_data {
	DECLARE_ENUM(uint32, Id);
	DECLARE_ENUM(uint32, MinTotalLevel);
	DECLARE_ENUM(uint32, MaxTotalLevel);
	DECLARE_ENUM(uint32, MinResetTime);
	DECLARE_ENUM(uint32, MaxResetTime);
	DECLARE_ENUM(uint32, NormalExpRate);
	DECLARE_ENUM(uint32, MasterExpRate);
	DECLARE_ENUM(uint32, MajesticExpRate);
};

typedef std::list<dynamic_exp_data*> DynamicExpData;

class CDynamicExpMgr {
	SingletonInstance(CDynamicExpMgr);
public:
	CDynamicExpMgr();
	virtual ~CDynamicExpMgr();
	void LoadDynamicExperienceData(char* pchFileName);
	int ExpToPlayer(Player* pPlayer);
private:
	DynamicExpData m_dynamic_exp_data;
};

#endif