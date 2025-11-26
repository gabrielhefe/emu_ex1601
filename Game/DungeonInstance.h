#ifndef DUNGEON_INSTANCE_H
#define DUNGEON_INSTANCE_H

typedef std::map<uint32, Player*> DungeonInstancePlayerMap;

typedef std::map<uint8, bool> DungeonInstanceChestMap;

class DungeonInstance
{
	public:
	DungeonInstance();
	virtual ~DungeonInstance();

	void Update();
	void UpdatePlayers();

	void AddPlayer(Player* pPlayer);
	void AddMonster();
	void BossKill(uint8 id);
	void CreateChest();
	void ChestOpen(uint8 type, uint8 id);
	void MovePlayersOut();
	bool IsEmpty() const;
	void SaveInstance();

	int32 GetCountByPC(Player* pPlayer) const;

	void CreateSimpleTrap();
	void CreateTrap(int16 x, int16 y, uint8 type);

	uint8 m_Id = 0;
	uint16 m_LevelId = 0;
	uint16 m_World = 0;
	int32 m_Instance = 0;
	uint16 m_PartyId = 0;
	int32 m_KilledMonsters = 0;
	uint8 m_BossKilled = 0;
	TickTimer m_BossKillTime;
	TickTimer m_Time;
	TickTimer m_SimpleTrapTime;

	int32 m_PlayerCount = 0;

	DungeonInstancePlayerMap m_DungeonInstancePlayerMap;
	DungeonInstanceChestMap m_DungeonInstanceChestMap[2];
};

#endif
