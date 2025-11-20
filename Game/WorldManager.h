#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

constexpr char* MOVEMAP_FILEPATH = "../Data/World/MoveMap.ini";
constexpr char* MOVEMAP_SECTION_BARRACK = "Barrack";
constexpr char* MOVEMAP_SECTION_REFUGE = "Refuge";
constexpr char* MOVEMAP_SECTION_SPECIAL_MAP = "SpecialMap";

typedef std::set<uint16> WorldServerList;
typedef std::map<uint16, WorldServerList> WorldServerMap;

struct world_experience
{
	DECLARE_ENUM(uint16, World);
	DECLARE_PROPERTY_ARRAY(int32, Rate, MAX_PARTY_MEMBERS);
};

typedef std::map<uint16, world_experience*> WorldExperienceMap;

typedef std::map<uint16, World*> WorldMap;

struct WorldFreePK
{
	DECLARE_ENUM(uint16, World);
	DECLARE_ENUM(int16, X1);
	DECLARE_ENUM(int16, Y1);
	DECLARE_ENUM(int16, X2);
	DECLARE_ENUM(int16, Y2);
};

typedef std::vector<WorldFreePK*> WorldFreePKList;

class WorldMgr
{
	SingletonInstance(WorldMgr);

	private:
		
		WorldServerMap world_server_map;
		WorldFreePKList m_WorldFreePKList;

	public:
		WorldMgr();
		virtual ~WorldMgr();

		void LoadWorldList(char* pchFileName);
		void LoadWorldData(char* pchFileName);
		void LoadWorldAIPath(char* pchFileName);
		void LoadWorldBuff(char* pchFileName);
		void LoadWorldAreaRestriction(char* pchFileName);
		void LoadWorldServer(char* pchFileName);
		void LoadWorldAttribute(char* pchFileName);
		void LoadWorldExperience(char* pchFileName);
		void LoadWorldAreaAttribute(char* pchFileName);
		void LoadWorldFreePK(char* pchFileName);
		void LoadBarrackIni();

		void Update();
		void UpdateWeather();
		bool IsWorld(uint16 world);
		World* GetWorld(uint16 world);

		void statusChange(uint16 entry, uint8 status);
		void MakeItemVisible(Player* pPlayer);

		uint16 AllowMoveToWorld(Player* pPlayer, uint16 world);
		bool AllowMoveToWorld(Player* pPlayer, uint16 world, uint16 server);

		bool IsWorldAllowed(uint16 world);

		void UpdateUrukMountainZone(bool apply);
		void SendUrukMountainZone();
		void SendUrukMountainZone(Player* pPlayer);

		void UpdateFereaZone(bool apply);
		void SendFereaZone();
		void SendFereaZone(Player* pPlayer);
		
		WorldMap world_map;
		WorldExperienceMap world_experience_map;

		int32 GetWorldExperienceRate(uint16 world, uint8 count);

		void SetPKBoss(uint16 world, bool enabled);
		bool IsPKBoss(uint16 world);

		bool IsItemDropAllowed(Player* pPlayer, World* pWorld, int16 x, int16 y);
		bool IsFreePK(uint16 world, int16 x, int16 y) const;
};

#endif