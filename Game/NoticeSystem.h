#ifndef NOTICE_SYSTEM_H
#define NOTICE_SYSTEM_H

struct NoticeSystemItemGetNotice {
	DECLARE_ENUM(uint8, ItemCat);
	DECLARE_ENUM(uint8, ItemIndex);
	DECLARE_ENUM(uint8, ItemLevel);
	DECLARE_BOOL(Exl);
	DECLARE_BOOL(Ancient);
};

struct NoticeSystemMonsterKill {
	DECLARE_ENUM(uint16, MonsterID);
};

struct NoticeSystemMonsterSpawn {
	DECLARE_ENUM(uint16, MonsterID);
	DECLARE_ENUM(uint8, WorldID);
};

struct MossMerchantItemNotify {
	DECLARE_ENUM(uint32, ItemCat);
	DECLARE_ENUM(uint32, ItemIndex);
	DECLARE_BOOL(Excl);
	DECLARE_BOOL(Ancient);
};

typedef std::list<NoticeSystemItemGetNotice*> NoticeSystemItemGet;
typedef std::list<NoticeSystemMonsterKill*> NoticeMonsterKill;
typedef std::list<NoticeSystemMonsterSpawn*> NoticeMonsterSpawn;
typedef std::list<MossMerchantItemNotify*> MerchantNotify;

class CNoticeSystem {
	SingletonInstance(CNoticeSystem);
public:
	CNoticeSystem();
	virtual ~CNoticeSystem();

	void Load(char* pchFileName);

	bool CheckItemNotice(Item item);
	bool CheckMonsterKillNotice(Monster* monster);
	bool CheckMonsterRespawnNotice(Monster* monster);
	bool CheckItemMossMerchantNotify(Item item);
	void NoticeGetItem(Player* pPlayer, Item item);
	void NoticeKillPlayer(Player* pPlayerKill, Player* pPlayerDead);
	void NoticeKillMonster(Player* pPlayer, Monster* monster);
	void NoticeRespawnMonster(Monster* monster);
	void NotifyMossMerchant(Player* pPlayer, Item item);
private:
	char textgetitemnotice[255] = "";
	char textkillednotice[255] = "";
	char textspawnmonsternotice[255] = "";

	char texttagitemnormal[15] = "";
	char texttagitemexcl[15] = "";
	char texttagitemancient[15] = "";

	NoticeMonsterKill m_notice_system_monster_kill;
	NoticeSystemItemGet m_notice_system_item_get;
	NoticeMonsterSpawn m_notice_system_monster_spawn;
	MerchantNotify m_moss_merchant_notify;
};

#endif