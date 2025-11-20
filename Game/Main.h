#ifndef MAIN_H
#define MAIN_H

//COMMON
constexpr char* SERVER_COMMON_FILEPATH = "../Data/Common.ini";
constexpr char* SERVER_COMMON_SECTION_ACCOUNT = "Account";
constexpr char* SERVER_COMMON_SECTION_CLASS_CREATE = "ClassCreate";
constexpr char* SERVER_COMMON_SECTION_CUSTOM_BOSS = "CustomBoss";
constexpr char* SERVER_COMMON_SECTION_COMMON = "Common";
constexpr char* SERVER_COMMON_SECTION_GERENAL = "Gerenal";
constexpr char* SERVER_COMMON_SECTION_WARE_HOUSE = "WareHouse";
constexpr char* SERVER_COMMON_SECTION_SWITCH_CHANNEL = "SwitchChannel";
constexpr char* SERVER_COMMON_SECTION_DUEL = "Duel";
constexpr char* SERVER_COMMON_SECTION_PARTY = "Party";
constexpr char* SERVER_COMMON_SECTION_CUSTOM = "Custom";
constexpr char* SERVER_COMMON_SECTION_PERSONAL_STORE = "PersonalStore";
constexpr char* SERVER_COMMON_SECTION_KUNDUN = "Kundun";
constexpr char* SERVER_COMMON_SECTION_FEREABOSS = "FereaBoss";
constexpr char* SERVER_COMMON_SECTION_GUILD = "Guild";

//JEWELS MANAGER
constexpr char* SERVER_JEWELSMANAGER_FILEPATH = "../Data/JewelsManager.ini";
constexpr char* SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_LIFE = "JewelOfLife";
constexpr char* SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_SOUL = "JewelOfSoul";
constexpr char* SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_HARMONY = "JewelOfHarmony";
constexpr char* SERVER_JEWELSMANAGER_SECTION_RADIANCE = "Radiance";
constexpr char* SERVER_JEWELSMANAGER_SECTION_JEWEL_PRICE = "Price";

//CASHSHOP
constexpr char* SERVER_CASHSHOP_FILEPATH = "../Data/CashShop.ini";
constexpr char* SERVER_CASHSHOP_SECTION_CASHSHOP = "CashShop";

//CALC CHARACTER
constexpr char* SERVER_CALCCHARACTER_FILEPATH = "../Data/CalcCharacter.ini";
constexpr char* SERVER_CALCCHARACTER_SECTION_GERENAL = "Gerenal";
constexpr char* SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED = "AttackSpeed";
constexpr char* SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD = "CalcDameMod";
constexpr char* SERVER_CALCCHARACTER_SECTION_DAMAGE_RATE = "DamageRate";
constexpr char* SERVER_CALCCHARACTER_SECTION_PVPDAMAGE = "PVPDamage";


//ANTIHACK
constexpr char* SERVER_ANTIHACK_FILEPATH = "../Data/AntiHack.ini";
constexpr char* SERVER_ANTIHACK_SECTION_ANTIHACK = "AntiHack";
constexpr char* SERVER_ANTIHACK_SECTION_ANTIMACRO = "AntiMacro";
constexpr char* SERVER_ANTIHACK_SECTION_CHEATSCAN= "CheatScan";
constexpr char* SERVER_ANTIHACK_SECTION_COMBOCHECK = "ComboCheck";
constexpr char* SERVER_ANTIHACK_SECTION_GSCHECK = "GameServerCheck";

//COMMAND
constexpr char* SERVER_COMMAND_FILEPATH = "../Data/Command.ini";
constexpr char* SERVER_COMMAND_SECTION_ADDSTAT = "AddStat";
constexpr char* SERVER_COMMAND_SECTION_POST = "Post";
constexpr char* SERVER_COMMAND_SECTION = "Command";

//ITEM
constexpr char* SERVER_ITEM_FILEPATH = "../Data/Item/Item.ini";
constexpr char* SERVER_ITEM_SECTION_DROP = "ItemDrop";
constexpr char* SERVER_ITEM_SECTION_ZEN_DROP = "ZenDrop";
constexpr char* SERVER_ITEM_SECTION_380_ITEM = "380Item";
constexpr char* SERVER_ITEM_SECTION_RUUD_WEAPON_SHOP = "RuudWeaponShop";
constexpr char* SERVER_ITEM_SECTION_MONSTER_SOUL = "MonsterSoul";


//CHARACTER
constexpr char* SERVER_CHAR_FILEPATH = "../Data/Character/Character.ini";
constexpr char* SERVER_CHAR_SECTION_GERENAL = "Gerenal";
constexpr char* SERVER_CHAR_SECTION_CHARACTER = "Character";
constexpr char* SERVER_CHAR_SECTION_CHARACTER_PK = "CharacterPK";
constexpr char* SERVER_CHAR_SECTION_PK_DROP = "PKDrop";
constexpr char* SERVER_CHAR_SECTION_INVENTORY = "Inventory";

//CHAOS MIX
constexpr char* SERVER_CHAOSMIX_FILEPATH = "../Data/ChaosMix.ini";
constexpr char* SERVER_CHAOSMIX_SECTION_MIX_RECOVERY = "MixRecovery";
constexpr char* SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE = "ItemUpgrade";
constexpr char* SERVER_CHAOSMIX_SECTION_MIX = "Mix";
constexpr char* SERVER_CHAOSMIX_SECTION_SPELL_STONE = "SpellStone";
constexpr char* SERVER_CHAOSMIX_SECTION_SEALED_MASTERY = "SealedMastery";


//EVENT
constexpr char* SERVER_EVENT_FILEPATH = "../Data/Event/Event.ini";
constexpr char* SERVER_EVENT_SECTION_WORLD_BOSS = "WorldBoss";
constexpr char* SERVER_EVENT_SECTION_SOCCER = "BattleSoccer";
constexpr char* SERVER_EVENT_SECTION_HAPPY_HOUR = "HappyHour";
constexpr char* SERVER_EVENT_SECTION_SWAMP_DARKNESS = "SwampOfDarkness";
constexpr char* SERVER_EVENT_SECTION_NIXIESLAKE = "NixiesLake";
constexpr char* SERVER_EVENT_SECTION_LOST_RACE = "LosttowerRace";
constexpr char* SERVER_EVENT_SECTION_PK_BOSS = "PKBoss";
constexpr char* SERVER_EVENT_SECTION_LAST_MAN = "LastManStanding";
constexpr char* SERVER_EVENT_SECTION_SANTA = "SantaVillage";
constexpr char* SERVER_EVENT_SECTION_OTHER = "Other";
constexpr char* SERVER_EVENT_SECTION_LODE = "LoDe";


//SKILL
constexpr char* SERVER_SKILL_FILEPATH = "../Data/Skill/Skill.ini";
constexpr char* SERVER_SKILL_SECTION = "Skill";

extern int MaxPlayersSet;
extern int ReservedPlayers;

struct AccTest {
	char UserName[20];
};

typedef std::list<AccTest*> AccountTest;

/// Timers for different object refresh rates
enum WorldTimers
{
    WUPDATE_WEATHERS,
	WUPDATE_VIEWPORT,
    WUPDATE_PINGDB,
	WUPDATE_SHUTDOWN,
	WUPDATE_EVENT_ANIMATION,
	WUPDATE_TOP_RANKING,
	WUPDATE_MONSTER_SOUL_RESET,
    WUPDATE_COUNT
};

class MainApplication: public AppConsole
{
	SingletonInstance(MainApplication);

	public:
		MainApplication();
		virtual ~MainApplication();

		void Load();
		void CreateSetting(std::string const& key, std::string const& default);

		void LoadCommonFile();
		void LoadAntiHackFile();
		void LoadCalcCharacter();
		void LoadJewelManagerFile();
		void LoadCommandIni();
		void LoadItemIni();
		void LoadCharacterIni();
		void LoadSkillIni();
		void LoadChaosMixIni();
		void LoadEventIni();
		bool LoadServerMap(char* pchFileName);
		void LoadAccountTestGS(char* pchFileName);
		bool CheckAccountTestLoginSV(const char* UserName);

		void End();
		bool Run();
		void Update(uint32 diff);
		
		IntervalTimer m_timers[WUPDATE_COUNT];

private:
	AccountTest m_acc_test;
};

#define sMain SingNull(MainApplication)

#endif