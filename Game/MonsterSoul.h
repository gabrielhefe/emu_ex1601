#ifndef MONSTER_SOUL_H
#define MONSTER_SOUL_H

#include <string>

#pragma pack(1)
struct MONSTER_SOUL_NPC_DATA_BODY_2
{
	uint32 id; ///- segunda columa de monster_soul_transformation
	int32 amount;
};

struct MONSTER_SOUL_NPC_DATA_BODY_1
{
	uint32 type; ///- primera columa de monster_soul_transformation
	uint32 data[2];
	uint32 expire_date;
	uint32 junk1;
	uint32 amount; ///- cantidad de monsters diferentes para buscar
	uint32 status; ///- 0 = normal, 1 = complete
	uint32 slot;
};

struct MONSTER_SOUL_NPC_DATA_HEAD : C1_HEADER_SUB
{
	uint32 junk;
	uint32 count;
};

struct MONSTER_SOUL_QUEST_BODY_2
{
	uint32 id;
	uint32 amount;
};

struct MONSTER_SOUL_QUEST_BODY_1
{
	uint32 type; ///- primera columa de monster_soul_transformation
	uint32 data[2];
	uint32 expire_date;
	uint32 junk1;
	uint32 amount; ///- cantidad de monsters diferentes para buscar
	uint32 status; ///- 0 = normal, 1 = complete
	uint32 slot; ///- inventory slot
};

struct MONSTER_SOUL_QUEST_HEAD : C1_HEADER_SUB
{
	uint32 junk;
	uint32 count;
};

struct MONSTER_SOUL_RUUD_NPC : C1_HEADER_SUB
{
	MONSTER_SOUL_RUUD_NPC()
	{
		this->Set(HEACODE_EVENT_INVENTORY, SUBCODE_EVENT_INVENTORY_MONSTER_SOUL_RUUD_NPC, sizeof(MONSTER_SOUL_RUUD_NPC));
		this->result = 0;
	}

	uint8 result;
};

struct MS_PACKET_43_HEAD : C1_HEADER_SUB
{
	MS_PACKET_43_HEAD()
	{
		this->Set(HEACODE_EVENT_INVENTORY, SUBCODE_EVENT_INVENTORY_MONSTER_SOUL_PACKET_43_HEAD, sizeof(MS_PACKET_43_HEAD));
		this->data1 = 0;
		this->data2 = 0;
		this->data3 = 0;
		this->data4 = 0;
	}

	uint32 data1;
	uint32 data2;
	uint32 data3;
	uint32 data4;
};

struct MONSTER_SOUL_ITEM_MOVE : C1_HEADER_SUB
{
	MONSTER_SOUL_ITEM_MOVE()
	{
		this->Set(HEACODE_EVENT_INVENTORY, SUBCODE_EVENT_INVENTORY_MONSTER_SOUL_MONSTER_SOUL_ITEM_MOVE, sizeof(MONSTER_SOUL_ITEM_MOVE));
		this->old_slot = 0;
		this->new_slot = 0;
	}

	uint32 old_slot;
	uint32 new_slot;
};

struct MONSTER_SOUL_UPDATE_BODY
{
	uint32 id;
	uint32 amount;
};

struct MONSTER_SOUL_UPDATE_HEAD : C1_HEADER_SUB
{
	uint32 type; ///- primera columa de monster_soul_transformation
	uint32 data[2];
	uint32 expire_date;
	uint32 junk1;
	uint32 amount; ///- cantidad de monsters diferentes para buscar
	uint32 status; ///- 0 = normal, 1 = complete
	uint32 slot;

	/*
	Este packet puede actualizar individualmente, puede agregar o sacar objetivos.
	*/
};

struct MONSTER_SOUL_AVAILABLE_PURCHASE : C1_HEADER_SUB
{
	MONSTER_SOUL_AVAILABLE_PURCHASE()
	{
		this->Set(HEACODE_EVENT_INVENTORY, SUBCODE_EVENT_INVENTORY_MONSTER_SOUL_AVAILABLE_PURCHASE, sizeof(MONSTER_SOUL_AVAILABLE_PURCHASE));
		this->amount = 0;
	}

	uint32 amount;
};

struct MONSTER_SOUL_REWARD : C1_HEADER_SUB
{
	MONSTER_SOUL_REWARD()
	{
		this->Set(HEACODE_EVENT_INVENTORY, SUBCODE_EVENT_INVENTORY_MONSTER_SOUL_MONSTER_SOUL_REWARD, sizeof(MONSTER_SOUL_REWARD));
	}

	uint32 id; /*1, 2, 3, 4, 5*/
	uint32 reward_type; /* 2 = Ruud, 3 = Jewel, 4 = Elemental, 5 = Accessories */
	uint32 item;
	uint32 amount;
	uint32 data;
};
#pragma pack()

struct MonsterSoulConverter
{
	uint32 m_ID;
	uint16 m_Item;
	int32 m_Amount;
};

struct MonsterSoulTransformation
{
	uint32 m_Type;
	uint32 m_ID;
	uint16 m_World;
	uint16 m_Monster;
	int32 m_RequiredSoul;
	int32 m_DropRate;
	std::string m_Location;
};

struct MonsterSoulReward
{
	uint32 m_ID;
	uint8 m_SubID;
	std::string m_ItemBag;
	std::string m_RandomItemBag;
};

typedef std::map<uint32, MonsterSoulConverter*> MonsterSoulConverterMap;
typedef std::vector<MonsterSoulTransformation*> MonsterSoulTransformationList;
typedef std::vector<MonsterSoulReward*> MonsterSoulRewardList;

class MonsterSoul
{
	SingletonInstance(MonsterSoul);

	public:
		MonsterSoul();
		virtual ~MonsterSoul();

		void LoadConverter(char* pchFileName);
		void LoadTransformation(char* pchFileName);
		void LoadReward(char* pchFileName);

		MonsterSoulConverter const* GetConverter(uint32 id) const;
		MonsterSoulConverter const* GetConverterByItem(uint16 id) const;
		MonsterSoulTransformation const* GetTransformation(uint32 type, uint32 id) const;
		MonsterSoulReward const* GetReward(uint32 id, uint8 sub_id) const;

		bool IsSoulConverter(Item const* pItem) const;
		bool IsSoulConverter(uint16 item) const;
		bool IsPurchaseAllowed(Player* pPlayer, Item const* pItem);
		void ExpireSoulConverter(Player* pPlayer, Item const* pItem);

		void SendItemMove(Player* pPlayer, uint32 source, uint32 target);
		void SendPurchaseAvailable(Player* pPlayer);
		void SendPurchaseList(Player* pPlayer);
		void SoulConverterPurchase(Player* pPlayer, Item * pItem);

		void SendStatus(Player* pPlayer);
		void MonsterKill(Player* pPlayer, Monster* pMonster);
		void ShareSoulToParty(Player* pPlayer, MonsterSoulTransformation const* pMonsterData);
		void SendUpdateData(Player* pPlayer, uint32 type);

		bool SoulConverterUse(Player* pPlayer, uint16 item, uint8 slot, uint8 type);
		void CheckResetPurchase();

		MonsterSoulConverterMap m_MonsterSoulConverterMap;
		MonsterSoulTransformationList m_MonsterSoulTransformationList;
		MonsterSoulRewardList m_MonsterSoulRewardList;

		DECLARE_BOOL(ResetMonsterSoul);
};

#endif