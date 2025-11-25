/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "ChatHandler.cpp"
*
*/

#include <cstring>

static size_t GetCommandTableSize(const ChatCommand* commands)
{
	if (!commands)
        return 0;
    size_t count = 0;
    while (commands[count].name != nullptr)
        count++;
    return count;
}

namespace CommandNormal
{
	static ChatCommand chat_command[] =
	{
		{	"addstr",		&ChatHandler::CommandAddStrength,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/addstr #points",				nullptr},
		{	"addagi",		&ChatHandler::CommandAddAgility,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/addagi #points",				nullptr},
		{	"addvit",		&ChatHandler::CommandAddVitality,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/addvit #points",				nullptr},
		{	"addene",		&ChatHandler::CommandAddEnergy,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/addene #points",				nullptr},
		{	"addcmd",		&ChatHandler::CommandAddLeadership,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/addcmd #points",				nullptr},
		{	"hostilcancel",	&ChatHandler::CommandHostilCancel,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"",								nullptr},
		{	"request",		&ChatHandler::CommandRequest,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/request <status>",			nullptr},
		{	"postview",		&ChatHandler::CommandPostRequest,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/postview #status",			nullptr},
		{	"refresh",		&ChatHandler::CommandRefresh,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"",								nullptr},
		//{	"addkick",		&ChatHandler::CommandFriendAddKick,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/addkick <name>",				nullptr},
		//{	"removekick",	&ChatHandler::CommandFriendRemoveKick,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,	"/removekick <name>",			nullptr},
		//{	"kick",			&ChatHandler::CommandFriendKick,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/kick <name>",					nullptr},
		{	"guildwar",		&ChatHandler::CommandGuildWar,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/guildwar <guild>",			nullptr},
		{	"war",			&ChatHandler::CommandGuildWar,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/war <guild>",					nullptr},
		{	"battlesoccer",	&ChatHandler::CommandBattleSoccer,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/battlesoccer <guild>",		nullptr},
		{	"soccer",		&ChatHandler::CommandBattleSoccer,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/soccer <guild>",				nullptr},
		{	"post",			&ChatHandler::CommandNormalPost,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/post <message>",				nullptr},
		//{	"csrefresh",	&ChatHandler::CommandSiegeRefresh,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"",	nullptr},
		//{	"master",		&ChatHandler::CommandMasterUP,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"",	nullptr},
		{	"offstore",		&ChatHandler::CommandOffTrade,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"",	nullptr},
		{	"auth",			&ChatHandler::CommandAuthorize,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"",	nullptr},
		{	"offattack",	&ChatHandler::CommandOffAttack,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		{	"pkpoints",		&ChatHandler::CommandPKPoints,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		//{	"gold",			&ChatHandler::CommandGold,			AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		{	"reset",		&ChatHandler::CommanResetCharacter,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		{	"resetauto",	&ChatHandler::CommanAutoResetCharacter,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/resetauto #type", nullptr},
		{	"danhlo",		&ChatHandler::CommandDanhLo,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/danhlo #number <bettype> #betvalue", nullptr},
		{	"danhde",		&ChatHandler::CommandDanhDe,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"/danhde #number <bettype> #betvalue", nullptr},
		{	"thuonglode",	&ChatHandler::CommandNhanThuongLoDe,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		//{	"cmdlist",		&ChatHandler::CommandCmdList,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		{	"clearinventory",		&ChatHandler::CommandClearInventory,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		{	"cleargremorycase",		&ChatHandler::CommandClearGremoryCase,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		{	"addexpanded",		&ChatHandler::CommandAddExpandedInventoryWareHouse,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_NONE,		"", nullptr},
		//{	"errtel",		&ChatHandler::CommandErrtel,		AUTHORITY_CODE_NORMAL, ADMIN_FLAG_NONE,					"/errtel #type #element #r1 #lv1 #r2 #lv2 #r3 #lv3", nullptr },

		{	nullptr,		nullptr,							0,								ADMIN_FLAG_NONE,		"",				nullptr}
	};
}

namespace CommandAdmin
{
	static ChatCommand reload_command[] =
	{
		{	"shop",			&ChatHandler::ReloadShop,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"quest",		&ChatHandler::ReloadQuest,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"event",		&ChatHandler::ReloadEvent,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"#eventid",		nullptr},
		{	"monster",		&ChatHandler::ReloadMonster,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"#type",		nullptr},
		{	"config",		&ChatHandler::ReloadConfig,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"item",			&ChatHandler::ReloadItem,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"character",	&ChatHandler::ReloadCharacter,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"skill",		&ChatHandler::ReloadSkill,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"teleport",		&ChatHandler::ReloadTeleport,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"cashshop",		&ChatHandler::ReloadCashShop,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"mix",			&ChatHandler::ReloadMix,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"world",		&ChatHandler::ReloadWorld,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	"pentagram",	&ChatHandler::ReloadPentagram,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RELOAD,		"",				nullptr},
		{	nullptr,		nullptr,		0,	ADMIN_FLAG_NONE,		"",				nullptr}
	};

	static ChatCommand item_command[] = 
	{
		{	"normal",		&ChatHandler::CommandItemNormal,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM,		".item normal #count #type #index #lvl #dur #skill #luck #opt #exe #anc",			nullptr},
		{	"add",			&ChatHandler::CommandItemAdd,				AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM,		".item add #count #type #index #lvl #dur #skill #luck #opt #exe #anc",				nullptr},
		{	"show",			&ChatHandler::CommandItemShow,				AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM_EVENT,	"",																					nullptr},
		{	"hide",			&ChatHandler::CommandItemHide,				AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM_EVENT,	".item hide #count #type #index #lvl #dur #skill #luck #opt #duration #looting",	nullptr},
		{	"remove",		&ChatHandler::CommandItemRemove,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM_EVENT,	".item remove #owner",																nullptr},
		{	"pentagram",	&ChatHandler::CommandAdminItemPentagram,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM,		".item pentagram #type #index #level #sock1 #sock2 #sock3 #sock4 #sock5 #sockbonus",																nullptr},
		{	"gremory",		&ChatHandler::CommandItemGremory,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM,		".item gremory #count #type #index #lvl #dur #skill #luck #opt #exe #anc #duration",				nullptr},
		{	"wing",			&ChatHandler::CommandItemWing,				AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM,		".item wing #index #lvl #skc1 #skc2 #skc3 #skc4 #skc5 #skcb", nullptr },
		{	"player",		&ChatHandler::CommandItemAddPlayer,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ITEM,		".item player #PID #count #type #index #lvl #dur #skill #luck #opt #exe #anc #socketbonus", nullptr },
		{	nullptr,		nullptr,		0,	ADMIN_FLAG_NONE,		"",				nullptr}
	};

	static ChatCommand buff_command[] = 
	{
		{	"up",			&ChatHandler::CommandGMBuff,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_BUFF,		".buff up #id #duration(min) #value",	nullptr},
		{	"all",			&ChatHandler::CommandGMBuffAll,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_BUFF,		".buff all #id #duration(min) #value",	nullptr},
		{	"to",			&ChatHandler::CommandGMBuffTo,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_BUFF,		".buff to #id #duration(min) #value",	nullptr},
		{	nullptr,		nullptr,								0,							ADMIN_FLAG_NONE,		"",				nullptr}
	};

	static ChatCommand monster_command[] = 
	{
		{	"temp",			&ChatHandler::CommandMonsterAddTemp,AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MONSTER,		".monster temp #monster #move_distance #despawn_time #despawn_die",	nullptr},
		{	"alter",		&ChatHandler::CommandMonsterAlter,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MONSTER,		".monster alter #type #data",	nullptr},
		{	nullptr,		nullptr,								0,						ADMIN_FLAG_NONE,		"",				nullptr}
	};

	static ChatCommand ban_command[] = 
	{
		{	"char",			&ChatHandler::CommandBanChar,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_BAN,			".ban char <name>",					nullptr},
		{	"acc",			&ChatHandler::CommandBanAcc,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_BAN,			".ban acc <account>"},
		{	nullptr,		nullptr,								0,						ADMIN_FLAG_NONE,		"",				nullptr}
	};

	static ChatCommand talk_command[] = 
	{
		{	"guild",	&ChatHandler::CommandGuildTalk,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_GUILD_TALK,		".talk guild <guild>",	nullptr},
		{	"alliance",	&ChatHandler::CommandAllianceTalk,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_GUILD_TALK, 		".talk alliance <guild>",	nullptr},
		{	nullptr,			nullptr,								0,						ADMIN_FLAG_NONE,			"",				nullptr}
	};

	static ChatCommand zen_command[] = 
	{
		{	"add",			&ChatHandler::CommandZenAdd,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ZEN,				".zen add #ammount #type",	nullptr},
		{	"set",			&ChatHandler::CommandZenSet,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_ZEN,				".zen set #ammount #type",	nullptr},
		{	nullptr,			nullptr,								0,						ADMIN_FLAG_NONE,			"", nullptr}
	};

	static ChatCommand misc_command[] = 
	{
		{	"kill",			&ChatHandler::CommandMiscKill,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			".misc kill <name>",	nullptr},
		{	nullptr,			nullptr,								0,						ADMIN_FLAG_NONE,			"", nullptr}
	};

	static ChatCommand change_command[] =
	{
		{ "guild",			&ChatHandler::CommandChangeGuild,		AUTHORITY_CODE_ADMINISTRATOR, ADMIN_FLAG_EVENTS, ".change guild <name> #guild", nullptr },
		{ "alliance",		&ChatHandler::CommandChangeAlliance,	AUTHORITY_CODE_ADMINISTRATOR, ADMIN_FLAG_EVENTS, ".change alliance #guild #alliance", nullptr },
		{ "hostil",			&ChatHandler::CommandChangeHostil,		AUTHORITY_CODE_ADMINISTRATOR, ADMIN_FLAG_EVENTS, ".change hostil #guild #hostil", nullptr },
		{ "csowner",		&ChatHandler::CommandChangeCSOwner,		AUTHORITY_CODE_ADMINISTRATOR, ADMIN_FLAG_EVENTS, ".change csowner #owner", nullptr },
		{ "crywolf",		&ChatHandler::CommandAdminCrywolf,		AUTHORITY_CODE_ADMINISTRATOR, ADMIN_FLAG_EVENTS, ".change crywolf #state", nullptr },
		{ nullptr, nullptr, 0, ADMIN_FLAG_NONE, "", nullptr }
	};

	static ChatCommand skill_command[] =
	{
		{	"add",			&ChatHandler::CommandSkillAdd,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,		".skill add #skill_id",		nullptr},
		{	"remove",		&ChatHandler::CommandSkillRemove,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,		".skill remove #skill_id",	nullptr},
		{ nullptr, nullptr, 0, ADMIN_FLAG_NONE, "", nullptr }
	};

	static ChatCommand castle_siege_command[] =
	{
		{	"reg",			&ChatHandler::CommandCastleSiegeRegister,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".siege reg <guild>",	nullptr},
		{	"unreg",		&ChatHandler::CommandCastleSiegeUnRegister,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".siege unreg <guild>",	nullptr},
		{	"incmark",		&ChatHandler::CommandCastleSiegeIncreaseMarks,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".siege incmark <guild> #count",	nullptr},
		{	"decmark",		&ChatHandler::CommandCastleSiegeReduceMarks,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".siege decmark <guild> #count",	nullptr},
		{	"kill",			&ChatHandler::CommandMiscSiege,					AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".siege kill <name> #ammount",	nullptr},
		{	"npcload",		&ChatHandler::CommandCastleSiegeNPCLoad,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	"",	nullptr},
		{	"fix",			&ChatHandler::CommandCastleSiegeFix,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	"",	nullptr},
		{	"fixgate",		&ChatHandler::CommandCastleSiegeFixGate,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".siege fixgate #id",	nullptr},
		{	nullptr,		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	"",	nullptr }
	};

	static ChatCommand stat_command[] =
	{
		{	"add",			&ChatHandler::CommandAdminStatAdd,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,		".stat add <name> #stat_id #value",		nullptr},
		{	"set",			&ChatHandler::CommandAdminStatSet,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,		".stat set <name> #stat_id #value",		nullptr},
		{	"remove",		&ChatHandler::CommandAdminStatRemove,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,		".stat remove <name> #stat_id #value",	nullptr},
		{ nullptr, nullptr, 0, ADMIN_FLAG_NONE, "", nullptr }
	};

	static ChatCommand arca_war_command[] =
	{
		{	"regmaster",	&ChatHandler::CommandArcaWarRegisterMaster,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".arcawar regmaster <guild>",	nullptr},
		{	"regmember",	&ChatHandler::CommandArcaWarRegisterMember,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".arcawar regmember <member>",	nullptr},
		{	"regsign",		&ChatHandler::CommandArcaWarIncreaseMarks,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".arcawar regsign <guild> #count",	nullptr},
		{	"reload",		&ChatHandler::CommandArcaWarReload,				AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	"",	nullptr},
		{	nullptr,		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	"",	nullptr }
	};

	static ChatCommand quest_command[] =
	{
		{	"state",		&ChatHandler::CommandAdminChangeQuestState,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".quest state <name> #id #state",	nullptr},
		{	"killcount",	&ChatHandler::CommandAdminChangeQuestKillCount,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	".quest killcount <name> #id #slot #count",	nullptr},
		{	nullptr,		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,	"",	nullptr }
	};

	static ChatCommand chat_command_admin[] =
	{
		{	"kick",			&ChatHandler::CommandKick,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_KICK,			".kick <name> #time(min)", nullptr},
		{	"event",		&ChatHandler::CommandEvents,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			".event #id #invasion #time #notify_time", nullptr},
		{	"stopevent",	&ChatHandler::CommandStopEvents,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			".stopevent #id #invasion", nullptr},
		{	"item",			nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"", item_command},
		{	"ban",			nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"",	ban_command},
		{	"reload",		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"",	reload_command},
		{	"monster",		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"",	monster_command},
		{	"zen",			nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"",	zen_command},
		{	"stat",			nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"",	stat_command},
		{	"talk",			nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"",	talk_command},
		{	"buff",			nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"", buff_command},
		{	"misc",			nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"", misc_command},
		{	"skill",		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"", skill_command},
		{	"siege",		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"", castle_siege_command},
		{	"arcawar",		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"", arca_war_command},
		{	"quest",		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"", quest_command},
		{	"mute",			&ChatHandler::CommandMute,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MUTE,			".mute <name> #time", nullptr},
		{	"restrict",		&ChatHandler::CommandRestriction,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_RESTRICTION,		".restrict <player> #restriction #time", nullptr},
		{	"online",		&ChatHandler::CommandOnline,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"", nullptr},
		{	"shutdown",		&ChatHandler::CommandShutdown,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_SHUTDOWN,		".shutdown #time(sec)", nullptr},
		{	"action",		&ChatHandler::CommandAction,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			".action #id", nullptr},
		{	"firework",		&ChatHandler::CommandFirework,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_NONE,			"",	nullptr},
		{	"challenge",	&ChatHandler::CommandAdminChallenge,AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			".challenge #size",	nullptr},
		{	"change",		nullptr,							AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			"",	change_command},
		{	"setpk",		&ChatHandler::CommandAdminSetPK,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			".setpk <name> #pk_level", nullptr},
		{	"clear",		&ChatHandler::CommandTestPacket,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MASIVE,			"",								nullptr},
		{	"summon",		&ChatHandler::CommandAdminSummon,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			".summon #id #move #time #type",	nullptr},
		{	"season",		&ChatHandler::CommandAdminSeason,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			".season #id",	nullptr},
		{	"duelreset",	&ChatHandler::CommandAdminResetDuel,AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_EVENTS,			".duelreset #room",	nullptr},
		{	"transfer",		&ChatHandler::CommandAdminTransfer,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MASIVE,			".transfer #server_id",	nullptr},
		{	"closeps",		&ChatHandler::CommandAdminPersonalStore,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_KICK,			".closeps <name> #operation",	nullptr},
		{	"postreset",	&ChatHandler::CommandAdminPostReset,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MASIVE,			"",	nullptr},
		{	"globalmute",	&ChatHandler::CommandAdminGlobalMute,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_MASIVE,		"",	nullptr},
		{	"macmute",		&ChatHandler::CommandAdminMuteMAC,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_MASIVE,		".macmute <name> #operation",	nullptr},
		{	"macrestrict",	&ChatHandler::CommandAdminMACRestrict,	AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_MASIVE,		".macrestrict <name> #operation #restriction #time",	nullptr},
		{	"iteminfo",		&ChatHandler::CommandAdminItemInfo,		AUTHORITY_CODE_NORMAL,			ADMIN_FLAG_MASIVE,		".iteminfo #type #slot/#id>",	nullptr},
		//{	"acckick",		&ChatHandler::CommandAdminAccountKick,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_KICK,		".acckick #account_id",	nullptr},
		//{	"ipget",		&ChatHandler::CommandAdminAccountKick,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_KICK,		".ipget #account_id",	nullptr},
		//{	"ipkick",		&ChatHandler::CommandAdminAccountKick,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_KICK,		".ipkick #account_id",	nullptr},

		{	"trace",		&ChatHandler::AdminCommandTrace,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_TRACE,	".trace <name>", nullptr},
		{	"trans",		&ChatHandler::AdminCommandTrans,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	".trans <name>", nullptr},
		{	"gate",			&ChatHandler::AdminCommandGate,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	".gate <name> #gate", nullptr},
		{	"move",			&ChatHandler::AdminCommandMove,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	".move <name> #world #x #y", nullptr},
		{	"follow",		&ChatHandler::AdminCommandFollow,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	".follow #id", nullptr},
		{	"updatestat",	&ChatHandler::AdminCommandUpdateStat,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	".updatestat <name>", nullptr},

		{	"active",		&ChatHandler::AdminCommandAcitve,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MASIVE,	".active 1/0", nullptr},

		{	"admincmd",		&ChatHandler::CommandAdminInvi,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rshop",		&ChatHandler::ReloadShop,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rquest",		&ChatHandler::ReloadQuest,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"ritem",		&ChatHandler::ReloadItem,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rchara",		&ChatHandler::ReloadCharacter,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rskill",		&ChatHandler::ReloadSkill,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rtele",		&ChatHandler::ReloadTeleport,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rcashshop",	&ChatHandler::ReloadCashShop,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rmix",			&ChatHandler::ReloadMix,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rpentagram",	&ChatHandler::ReloadPentagram,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"revent",		&ChatHandler::ReloadEvent,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	".revent #id #sub_id", nullptr},
		{	"rconfig",		&ChatHandler::ReloadConfig,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rworld",		&ChatHandler::ReloadWorld,			AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"rmonster",		&ChatHandler::ReloadMonster,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	".rmonster #guid", nullptr},
		{	"testfunc",		&ChatHandler::CommandTestFunc,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	"", nullptr},
		{	"notify",		&ChatHandler::CommandAdminNotify,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	".notify <text>", nullptr},
		{	"delitem",		&ChatHandler::CommandAdminDeleteItemServer,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	".delitem #id", nullptr},
		{	"delitemsr",	&ChatHandler::CommandAdminDeleteItemServerSerial,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	".delitemsr #serial", nullptr},
		{	"delitemerrorp",	&ChatHandler::CommandAdminDeleteItemErrorPersonal,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_PANEL,	"", nullptr},
		{	"kickacc",		&ChatHandler::CommandKickAccount,		AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	".kickacc #accid", nullptr},
		//{	"track",		&ChatHandler::CommandAdminTrack,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MASIVE,	".track <name>", nullptr},
		{	"track",		&ChatHandler::CommandAdminTrackCharacter,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MOVE,	"", nullptr},

		{	"spot",			&ChatHandler::CommandAdminSpotCreate,	AUTHORITY_CODE_ADMINISTRATOR,	ADMIN_FLAG_MASIVE,	".spot #count", nullptr},
		{ "attrchange", &ChatHandler::CommandAdminAttrChange, AUTHORITY_CODE_ADMINISTRATOR, ADMIN_FLAG_MASIVE, ".attrchange #x #y #attr #apply", nullptr },

		{	nullptr,		nullptr,							0,								ADMIN_FLAG_NONE,			"", nullptr}
	};
}

ChatHandler::ChatHandler(Player * pPlayer, const char * whisper_name)
{
	this->m_Player = pPlayer;
	std::memset(this->m_WhisperName, 0, sizeof(this->m_WhisperName));

        if ( whisper_name )
        {
                std::strncpy(this->m_WhisperName, whisper_name, sizeof(this->m_WhisperName) - 1);
        }
}

ChatHandler::~ChatHandler()
{

}

bool ChatHandler::IsAvailable(ChatCommand const& table)
{
	if ( table.Authority == AUTHORITY_CODE_ADMINISTRATOR )
	{
		if ( !this->m_Player->IsAdministrator() )
		{
			return false;
		}

		if ( !this->m_Player->IsAuthorizationEnabled() )
		{
			return false;
		}
	}

	if ( table.flag != ADMIN_FLAG_NONE && !this->m_Player->IsAdministratorFlag(table.flag) )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You don't have permission to use this command");
		return false;
	}

	return true;
}

void ChatHandler::ExecuteCommand(const char * text)
{
	SafeAssert(text, "text == nullptr");
	SafeAssert(*text, "*text == nullptr");

	if (strlen(text) < 2)
		return;

	std::string fullcmd = text;
	std::string const_cmd = text;

	if ( (text[0] == '/' && text[1] == '/') )
		return;

    if ( text[0] == '/' )
        ++text;

	if ( this->ExecuteCommandInTable(CommandNormal::chat_command, text, fullcmd) )
	{
		sLog->outInfo(LOG_CHAT, "[ COMMAND ] %s Used Command: %s",
			this->m_Player->BuildLog().c_str(), const_cmd.c_str());
	}
}

void ChatHandler::ExecuteCommandAdmin(const char * text)
{
	SafeAssert(text, "text == nullptr");
	SafeAssert(*text, "*text == nullptr");

	if (strlen(text) < 2)
		return;

	std::string fullcmd = text;
	std::string const_cmd = text;

	if ( (text[0] == '.' && text[1] == '.') )
		return;

    if ( text[0] == '.' )
        ++text;

	if ( this->ExecuteCommandInTable(CommandAdmin::chat_command_admin, text, fullcmd) )
	{
		sLog->outInfo(LOG_CHAT, "[ COMMAND ADMIN ] %s Used Command: %s",
			this->m_Player->BuildLog().c_str(), const_cmd.c_str());
	}
	else
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "There is no such command.");
	}
}

bool ChatHandler::ExecuteCommandInTable(ChatCommand * table, const char * text, std::string & fullcmd)
{
	char const* oldtext = text;
	std::string cmd = "";
	
	while (*text != ' ' && *text != '\0')
    {
        cmd += *text;
        ++text;
    }

	while (*text == ' ') ++text;

	size_t table_size = GetCommandTableSize(table);

	for ( size_t i = 0; i < table_size; ++i )
    {
		if ( strlen(table[i].name) != cmd.length() )
			continue;

		if ( memcmp(table[i].name, cmd.c_str(), strlen(table[i].name)) )
			continue;

		// select subcommand from child commands list
		if (table[i].SubTable != nullptr)
		{
			if (!ExecuteCommandInTable(table[i].SubTable, text, fullcmd))
			{
				this->ShowHelpForCommand(table[i].SubTable, text);
			}

			return true;
		}

		// must be available and have handler
		if (!table[i].handler || !this->IsAvailable(table[i]))
			continue;

		std::string newtext = table[i].name[0] != '\0' ? text : oldtext;

		if ( !table[i].sintax.empty() && !strlen(newtext.c_str()) )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Syntax");
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, " %s", table[i].sintax.c_str());
			return true;
		}

		(this->*table[i].handler)(newtext.c_str());
        return true;
	}

    return false;
}

bool ChatHandler::ShowHelpForCommand(ChatCommand* table, const char* cmd)
{
	if (*cmd)
    {
        for (uint32 i = 0; table[i].name != nullptr; ++i)
        {
            // must be available (ignore handler existence for show command with possible available subcommands)
            if (!this->IsAvailable(table[i]))
                continue;

            if (!hasStringAbbr(table[i].name, cmd))
                continue;

            // have subcommand
            char const* subcmd = (*cmd) ? strtok(nullptr, " ") : "";

            if (table[i].SubTable && subcmd && *subcmd)
            {
                if (ShowHelpForCommand(table[i].SubTable, subcmd))
                    return true;
            }

            if (!table[i].sintax.empty())
				this->m_Player->SendNotice(NOTICE_NORMAL_BLUE, table[i].sintax.c_str());

            if (table[i].SubTable)
                if (ShowHelpForSubCommands(table[i].SubTable, table[i].name, subcmd ? subcmd : ""))
                    return true;

            return !table[i].sintax.empty();
        }
    }
    else
    {
        for (uint32 i = 0; table[i].name != nullptr; ++i)
        {
            // must be available (ignore handler existence for show command with possible available subcommands)
            if (!this->IsAvailable(table[i]))
                continue;

            if (strlen(table[i].name))
                continue;

            if (!table[i].sintax.empty())
				this->m_Player->SendNotice(NOTICE_NORMAL_BLUE, table[i].sintax.c_str());

            if (table[i].SubTable)
                if (this->ShowHelpForSubCommands(table[i].SubTable, "", ""))
                    return true;

            return !table[i].sintax.empty();
        }
    }

    return this->ShowHelpForSubCommands(table, "", cmd);
}

bool ChatHandler::ShowHelpForSubCommands(ChatCommand* table, char const* cmd, char const* subcmd)
{
    std::string list;
    for (uint32 i = 0; table[i].name != nullptr; ++i)
    {
        // must be available (ignore handler existence for show command with possible available subcommands)
        if (!this->IsAvailable(table[i]))
            continue;

        // for empty subcmd show all available
        if (*subcmd && !hasStringAbbr(table[i].name, subcmd))
            continue;

		this->m_Player->SendNotice(NOTICE_NORMAL_BLUE, table[i].name);
		list += "\n    ";
        list += table[i].name;

        if (table[i].SubTable)
            list += " ...";
    }

    if (list.empty())
        return false;

    return true;
}

bool ChatHandler::CommandAddValid()
{
	if ( !sGameServer->IsCommandAddStatEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Add Stat command disabled.");
		return false;
	}

	if ( this->m_Player->GetTotalLevel() < sGameServer->GetCommandAddStatMinLevel() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required level %d to use add stat command.", sGameServer->GetCommandAddStatMinLevel());
		return false;
	}

	if ( !this->m_Player->MoneyHave(sGameServer->GetCommandAddStatCost()) )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Required %u zen to use add stat command.", sGameServer->GetCommandAddStatCost());
		return false;
	}

	this->m_Player->MoneyReduce(sGameServer->GetCommandAddStatCost());
	
	return true;
}

void ChatHandler::CommandAddStrength(const char * msg)
{
	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !this->CommandAddValid() )
		return;

	std::stringstream conversor(msg);
	int32 points = 0;
	conversor >> points;

	this->m_Player->AddNormalPoints(points, STRENGTH);
}
	
void ChatHandler::CommandAddAgility(const char * msg)
{
	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !this->CommandAddValid() )
		return;

	std::stringstream conversor(msg);
	int32 points = 0;
	conversor >> points;

	this->m_Player->AddNormalPoints(points, AGILITY);
}
	
void ChatHandler::CommandAddVitality(const char * msg)
{
	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !this->CommandAddValid() )
		return;

	std::stringstream conversor(msg);
	int32 points = 0;
	conversor >> points;

	this->m_Player->AddNormalPoints(points, VITALITY);
}
	
void ChatHandler::CommandAddEnergy(const char * msg)
{
	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !this->CommandAddValid() )
		return;

	std::stringstream conversor(msg);
	int32 points = 0;
	conversor >> points;

	this->m_Player->AddNormalPoints(points, ENERGY);
}

void ChatHandler::CommandAddLeadership(const char * msg)
{
	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !this->CommandAddValid() )
		return;

	std::stringstream conversor(msg);
	int32 points = 0;
	conversor >> points;

	this->m_Player->AddNormalPoints(points, LEADERSHIP);
}

void ChatHandler::CommandHostilCancel(const char * msg)
{
	if ( !sGameServer->IsHostilCancelCommand() )
		return;

	Guild* pGuild = this->m_Player->GuildGet();

	if ( !pGuild )
		return;

	if ( this->m_Player->GuildGetRanking() != GUILD_RANK_MASTER )
		return;

	if ( !pGuild->GetHostil() )
		return;

	sDataServer->GuildRelationshipRequest(pGuild->GetID(), pGuild->GetHostil(), GUILD_RELATIONSHIP_RIVAL, false);
}

void ChatHandler::CommandRequest(const char * msg)
{
	std::stringstream conversor(msg);
	std::string status;
	conversor >> status;
	strToLower(status);

	if ( status == "on" )
	{
		this->m_Player->AddFlag(CHARACTER_FLAG_REQUEST);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Request ON");
	}
	else if ( status == "off" )
	{
		this->m_Player->RemoveFlag(CHARACTER_FLAG_REQUEST);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Request OFF");
	}
}

void ChatHandler::CommandPostRequest(const char * msg)
{
	std::stringstream conversor(msg);
	int32 status = 0;
	conversor >> status;
	
	if ( status == 1 )
	{
		this->m_Player->AddFlag(CHARACTER_FLAG_POST);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Post ON");
	}
	else if ( status == 0 )
	{
		this->m_Player->RemoveFlag(CHARACTER_FLAG_POST);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Post OFF");
	}
}

void ChatHandler::CommandRefresh(const char * msg)
{
	if ( this->m_Player->IsBusy() )
		return;

	if ( !sGameServer->IsRefreshEnabled() )
		return;

	if ( !this->m_Player->GetTimer(PLAYER_TIMER_REFRESH)->Elapsed(sGameServer->GetRefreshTime()) )
	{
		return;
	}

	this->m_Player->TeleportToLocation(this->m_Player->GetWorldId(), this->m_Player->GetX(), this->m_Player->GetY(), this->m_Player->GetDirection(), this->m_Player->GetInstance());
}

bool ChatHandler::CommandFriendKickVerify(const char * name)
{
	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_KICK_VERIFY);
	stmt->setUInt32(0, this->m_Player->GetGUID());
	stmt->setString(1, name);

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( !result )
		return false;

	return true;
}

void ChatHandler::CommandFriendAddKick(const char * msg)
{
	if ( !sGameServer->IsKickEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Kick system is disabled.");
		return;
	}

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( sGameServer->GetKickType() != 1 )
		return;

	std::stringstream conversor(msg);
	std::string name_safe;
	conversor >> name_safe;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), name_safe.c_str(), name_safe.size() > MAX_CHARACTER_LENGTH ? MAX_CHARACTER_LENGTH: name_safe.size());

	std::string lower_name = name;
	std::string lower_my_name = this->m_Player->GetName();

	strToLower(lower_name);
	strToLower(lower_my_name);

	if ( lower_name == lower_my_name )
		return;

	if ( this->CommandFriendKickVerify(name) )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Character %s is already in kick list.", name);
		return;
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_KICK_ADD);
	stmt->setUInt32(0, this->m_Player->GetGUID());
	stmt->setString(1, name);
	trans->Append(stmt);
	
	MuDatabase.CommitTransaction(trans);

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Character %s added to kick list.", name);
}
	
void ChatHandler::CommandFriendRemoveKick(const char * msg)
{
	if ( !sGameServer->IsKickEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Kick system is disabled.");
		return;
	}

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( sGameServer->GetKickType() != 1 )
		return;

	std::stringstream conversor(msg);
	std::string name_safe;
	conversor >> name_safe;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), name_safe.c_str(), name_safe.size() > MAX_CHARACTER_LENGTH ? MAX_CHARACTER_LENGTH: name_safe.size());

	std::string lower_name = name;
	std::string lower_my_name = this->m_Player->GetName();

	strToLower(lower_name);
	strToLower(lower_my_name);

	if ( lower_name == lower_my_name )
		return;

	if ( !this->CommandFriendKickVerify(name) )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Character %s is not in kick list.", name);
		return;
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_KICK_REMOVE);
	stmt->setUInt32(0, this->m_Player->GetGUID());
	stmt->setString(1, name);
	trans->Append(stmt);
	
	MuDatabase.CommitTransaction(trans);

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Character %s removed from kick list.", name);
}

void ChatHandler::CommandFriendKick(const char * msg)
{
	if ( !sGameServer->IsKickEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Kick system is disabled.");
		return;
	}

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	std::stringstream conversor(msg);
	std::string name_safe;
	conversor >> name_safe;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), name_safe.c_str(), name_safe.size() > MAX_CHARACTER_LENGTH ? MAX_CHARACTER_LENGTH: name_safe.size());

	std::string lower_name = name;
	std::string lower_my_name = this->m_Player->GetName();

	strToLower(lower_name);
	strToLower(lower_my_name);

	if ( lower_name == lower_my_name )
	{
		return;
	}

	if ( sGameServer->GetKickType() == 1 )
	{
		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_KICK_SELECT);
		stmt->setString(0, name);
		stmt->setString(1, this->m_Player->GetName());

		PreparedQueryResult result = MuDatabase.Query(stmt);

		if ( !result )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not in %s kick list.", name);
			return;
		}

		Player* pPlayer = sObjectMgr->FindPlayerByName(name);

		if ( !pPlayer )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s is offline.", name);
			return;
		}

		KICK_PLAYER(pPlayer, "Friend Kick DB");
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s kicked.", name);
	}
}

void ChatHandler::CommandGuildWar(const char * msg)
{
	std::stringstream conversor(msg);
	std::string guild_name;
	conversor >> guild_name;
	
	STRING_SAFE_COPY(guild, (MAX_GUILD_NAME_LENGTH + 1), guild_name.c_str(), guild_name.size() > MAX_GUILD_NAME_LENGTH ? MAX_GUILD_NAME_LENGTH: guild_name.size());

	sGuildWarMgr->StartWarRequest(this->m_Player, guild);
}
	
void ChatHandler::CommandBattleSoccer(const char * msg)
{
	std::stringstream conversor(msg);
	std::string guild_name;
	conversor >> guild_name;

	STRING_SAFE_COPY(guild, (MAX_GUILD_NAME_LENGTH + 1), guild_name.c_str(), guild_name.size() > MAX_GUILD_NAME_LENGTH ? MAX_GUILD_NAME_LENGTH: guild_name.size());

	sBattleSoccerMgr->ProcessRequest(this->m_Player, guild);
}

void ChatHandler::CommandNormalPost(const char * msg)
{
	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	char new_message[MAX_CHAT_LENGTH];
	memset(new_message, 0, MAX_CHAT_LENGTH);
	std::string converted_string = TrimBlankSpaces(msg);

	sprintf_s(new_message, ";%s", converted_string.c_str());
	
	this->m_Player->ChatProcess(CHAT_POST, new_message, this->m_WhisperName, false);
}

void ChatHandler::CommandOffTrade(const char * msg)
{
	std::stringstream conversor(msg);
	int32 credits = 0;

	conversor >> credits;

	if ( !sGameServer->IsPersonalStoreOff() )
	{
		this->m_Player->SendMessageBox(1, "Personal Store", "Off option is disabled.");
		return;
	}

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( !this->m_Player->IsInSafeZone() )
	{
		this->m_Player->SendMessageBox(1, "Personal Store", "Off option can be used only in safe zone.");
		return;
	}

	if ( !this->m_Player->IsWorldFlag(WORLD_FLAG_ALLOW_OFFLINE_TRADE) )
	{
		this->m_Player->SendMessageBox(1, "Personal Store", "Off option can't be used on this map.");
		return;
	}

	World* pWorld = this->m_Player->GetWorld();

	if ( !pWorld )
	{
		this->m_Player->SendMessageBox(1, "Personal Store", "ERROR. Contact Administrators");
		return;
	}

	if ( pWorld->IsAreaRestriction(this->m_Player->GetX(), this->m_Player->GetY(), WORLD_AREA_FLAG_OFF_STORE) )
	{
		this->m_Player->SendMessageBox(1, "Personal Store", "You can't use off store on this map location.");
		return;
	}

	if ( !this->m_Player->GetPersonalStore()->IsOpen() )
	{
		this->m_Player->SendMessageBox(1, "Personal Store", "Need to open Personal Store.");
		return;
	}

	if ( sGameServer->GetPersonalStoreOffCount() > 0 )
	{
		int32 count = 0;
		PlayerSessionMap const& player_map = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
			{
				continue;
			}

			if ( !pPlayer->GetPersonalStore()->IsOpen() )
			{
				continue;
			}

			if ( !pPlayer->GetPersonalStore()->IsOff() )
			{
				continue;
			}

			if ( !memcmp(pPlayer->GetAccountData()->GetMac(), this->m_Player->GetAccountData()->GetMac(), MAX_ACCOUNT_MAC_LENGTH) &&
				 pPlayer->GetAccountData()->GetDiskSerial() == this->m_Player->GetAccountData()->GetDiskSerial() )
			{
				++count;
			}
		}

		if ( count >= sGameServer->GetPersonalStoreOffCount() )
		{
			this->m_Player->SendMessageBox(1, "Personal Store", "You have exceeded the maximum offstore ammount.");
			return;
		}
	}

	if ( sGameServer->GetPersonalStoreOffTotalCount() > 0 )
	{
		int32 count = 0;
		PlayerSessionMap const& player_map = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
			{
				continue;
			}

			if ( !pPlayer->GetPersonalStore()->IsOpen() )
			{
				continue;
			}

			if ( !pPlayer->GetPersonalStore()->IsOff() )
			{
				continue;
			}

			++count;
		}

		if ( count >= sGameServer->GetPersonalStoreOffTotalCount() )
		{
			this->m_Player->SendMessageBox(1, "Personal Store", "Can't open more offstores on this server.");
			return;
		}
	}

	this->m_Player->GetPersonalStore()->SetOff(true);
	this->m_Player->GetPersonalStore()->SetOffTime(MyGetTickCount());
	//this->m_Player->GetAccountData()->SetMoveToSelectChar(true);
	this->m_Player->UpdateAccountStatusType(1);
	this->m_Player->SaveCharacter();
	this->m_Player->Close(1, CLOSE_FLAG_EXIT_GAME);

	this->m_Player->SetAutoLoginInfo(credits);
}

void ChatHandler::CommandAuthorize(const char * msg)
{
	if ( this->m_Player->IsAuthorizationEnabled() )
	{
		return;
	}

	/*if ( !this->m_Player->IsAdministrator() )
	{
		return;
	}*/
	
	std::stringstream conversor(msg);
	std::string security_code;

	conversor >> security_code;

	if ( security_code.size() > MAX_SECURE_CODE_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(code, (MAX_SECURE_CODE_LENGTH + 1), security_code.c_str(), security_code.size());

	if ( !memcmp(code, this->m_Player->GetAccountData()->secure_code, MAX_SECURE_CODE_LENGTH) || strcmp(this->m_Player->GetAccountData()->secure_code, code) == 0)
	{
		this->m_Player->SetAuthorizationEnabled(true);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Account Authorized.");

		if ( !this->m_Player->IsAdministrator() )
		{
			sAuthServer->AccountAuthorization(this->m_Player->GetAccountData()->GetGUID(), false);
		}
	}
	else
	{
		this->m_Player->SendMessageBox(1, "AUTH", "Wrong Authorization Code.");

		sAuthServer->AccountAuthorization(this->m_Player->GetAccountData()->GetGUID(), true);
	}
}

void ChatHandler::CommandOffAttack(const char * msg)
{
	if ( !sGameServer->IsOfflineAttack() )
	{
		return;
	}

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( sGameServer->GetOfflineAttackMinLevel() > 0 && this->m_Player->GetTotalLevel() < sGameServer->GetOfflineAttackMinLevel() )
	{
		this->m_Player->SendMessageBox(1, "Offline Attack", "Min level required %d.", sGameServer->GetOfflineAttackMinLevel());
		return;
	}

	if ( sGameServer->GetOfflineAttackMaxLevel() > 0 && this->m_Player->GetTotalLevel() > sGameServer->GetOfflineAttackMaxLevel() )
	{
		this->m_Player->SendMessageBox(1, "Offline Attack", "Max level allowed %d.", sGameServer->GetOfflineAttackMaxLevel());
		return;
	}

	if ( !sGameServer->IsOfflineAttackWorld(this->m_Player, this->m_Player->GetWorldId()) )
	{
		return;
	}

	if (!sGameServer->IsOfflineAttackZone(this->m_Player, this->m_Player->GetWorldId(), this->m_Player->GetX(), this->m_Player->GetY()))
	{
		this->m_Player->SendMessageBox(1, "Offline Attack", "You are not allowed to use on this zone.");
		return;
	}

	if ( sGameServer->GetOfflineAttackCount() > 0 )
	{
		int32 count = 0;
		PlayerSessionMap const& player_map = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
			{
				continue;
			}

			if ( !pPlayer->GetHelper()->IsOffline() )
			{
				continue;
			}

			if (sGameServer->GetOfflineAttackFlag() == 1)
			{
				if (!memcmp(pPlayer->GetAccountData()->GetMac(), this->m_Player->GetAccountData()->GetMac(), MAX_ACCOUNT_MAC_LENGTH))
				{
					++count;
				}
			}
			else if (sGameServer->GetOfflineAttackFlag() == 2)
			{
				if (pPlayer->GetAccountData()->GetDiskSerial() == this->m_Player->GetAccountData()->GetDiskSerial())
				{
					++count;
				}
			}
			else
			{
				if (!memcmp(pPlayer->GetAccountData()->GetMac(), this->m_Player->GetAccountData()->GetMac(), MAX_ACCOUNT_MAC_LENGTH) &&
					pPlayer->GetAccountData()->GetDiskSerial() == this->m_Player->GetAccountData()->GetDiskSerial())
				{
					++count;
				}
			}
		}

		if ( count >= sGameServer->GetOfflineAttackCount() )
		{
			this->m_Player->SendMessageBox(1, "Offline Attack", "You have exceeded the maximum count.");
			return;
		}
	}

	if (sGameServer->GetOfflineAttackIPCount() > 0)
	{
		int32 count = 0;
		PlayerSessionMap const& player_map = sObjectMgr->GetAllCharacters();
		for (PlayerSessionMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it)
		{
			Player* pPlayer = it->second;

			if (!pPlayer)
			{
				continue;
			}

			if (!pPlayer->GetHelper()->IsOffline())
			{
				continue;
			}

			if (!memcmp(pPlayer->GetAccountData()->GetIP(), this->m_Player->GetAccountData()->GetIP(), MAX_ACCOUNT_IP_LENGTH))
			{
				++count;
			}
		}

		if (count >= sGameServer->GetOfflineAttackIPCount())
		{
			this->m_Player->SendMessageBox(1, "Offline Attack", "You have exceeded the maximum count.");
			return;
		}
	}

	if ( !this->m_Player->GetHelper()->IsStarted() )
	{
		this->m_Player->SendMessageBox(1, "Offline Attack", "Helper must be active.");
		return;
	}

	if ( this->m_Player->GetPersonalStore()->IsBusy() )
	{
		this->m_Player->SendMessageBox(1, "Offline Attack", "Close Personal Store before start.");
		return;
	}

	if (this->m_Player->IsBusy() || this->m_Player->GetTransaction() == TRANSACTION_TYPE_BEGIN)
	{
		this->m_Player->SendMessageBox(1, "Offline Attack", "Close Interface before start.");
		return;
	}

	if ( !this->m_Player->IsWorldFlag(WORLD_FLAG_ALLOW_OFFLINE_ATTACK) )
	{
		this->m_Player->SendMessageBox(1, "Offline Attack", "Not allowed to use on this map.");
		return;
	}

	this->m_Player->UpdateAccountStatusType(2);

	this->m_Player->RemoveFlag(CHARACTER_FLAG_REQUEST);	
	this->m_Player->GetHelper()->SetStartedTime(MyGetTickCount());
	this->m_Player->GetHelper()->SetOffline(true);
	this->m_Player->SaveCharacter();
	this->m_Player->Close(1, CLOSE_FLAG_EXIT_GAME);

	//uint8 guidacc = this->m_Player->GetAccountData()->GetGUID();
	//QueryResult result = LoginDatabase.PQuery("UPDATE accounts_status SET online = 0 where account_id=%d",guidacc);

	this->m_Player->SetAutoLoginInfo(3);
}

void ChatHandler::CommandPKPoints(const char * msg)
{
	if ( this->m_Player->GetPKLevel() <= PK_STATUS_COMMONER )
	{
		return;
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "You have to earn %d points to get out of Outlaw status..", this->m_Player->GetPKPoints());
}

void ChatHandler::CommandGold(const char * msg)
{
	if ( !sGameServer->IsCommandGoldEnabled() )
	{
		return;
	}

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GOLDEN_CHANNEL_SELECT);
	stmt->setUInt32(0, this->m_Player->GetAccountData()->GetGUID());

	PreparedQueryResult result = LoginDatabase.Query(stmt);

	if ( !result )
	{
		return;
	}

	Field* fields = result->Fetch();
	time_t golden_channel = fields[0].GetInt64();

	std::stringstream conversor(msg);
	int32 days = 0;

	conversor >> days;

	if ( days < 0 )
	{
		days = 0;
	}

	if ( days > sGameServer->GetCommandGoldMaxDays() )
	{
		days = sGameServer->GetCommandGoldMaxDays();
	}

	int32 price = days * sGameServer->GetCommandGoldPrice();

	if ( days == 0 )
	{
		this->m_Player->SendMessageBox(0, "Golden Channel", "You need to set how many days you are going to purchase. \n "
			"Each day costs %d wcoin. \n "
			"Example: /gold 10. That means 10 days.", sGameServer->GetCommandGoldPrice());
	}
	else
	{
		if ( this->m_Player->GetCredits() < price )
		{
			this->m_Player->SendMessageBox(0, "Golden Channel", "You need %d wcoin to buy %d days.", price, days);
			return;
		}

		this->m_Player->ReduceCredits(price);

		time_t purchase_days = days * DAY;
		time_t current_time = time(nullptr);

		if ( !golden_channel || golden_channel < current_time )
		{
			golden_channel = time(nullptr);
		}

		golden_channel += purchase_days;

		this->m_Player->GetAccountData()->SetGoldenChannel(golden_channel);

		SQLTransaction trans = LoginDatabase.BeginTransaction();

		stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GOLDEN_CHANNEL_UPDATE);
		stmt->setInt64(0, golden_channel);
		stmt->setUInt32(1, this->m_Player->GetAccountData()->GetGUID());
		trans->Append(stmt);

		LoginDatabase.CommitTransaction(trans);

		SQLTransaction strans = MuDatabase.BeginTransaction();

		this->m_Player->SaveDBAccount(strans);

		MuDatabase.CommitTransaction(strans);

		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "You have been subscribed to gold server for %d days.", (golden_channel - current_time) / DAY);
	}
}

void ChatHandler::CommanResetCharacter(const char* msg)
{
	if (!sGameServer->IsResetSystemEnbale())
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Reset System Disable");
		return;
	}

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	Player* pPlayer = sObjectMgr->FindPlayerByName(this->m_Player->GetName());

	if (sResetSystemMgr->PlayerReset(pPlayer)) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Reset Succes!");
	}
	else {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Reset Failed!");
	}
}

void ChatHandler::CommanAutoResetCharacter(const char* msg) {

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	Player* pPlayer = sObjectMgr->FindPlayerByName(this->m_Player->GetName());
	std::stringstream conversor(msg);
	int32 type = 0;

	conversor >> type;

	if (type == 1) {
		pPlayer->GetResetSystem(RESET_AUTO)->SetReset(1);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Reset Auto Enanle");
	}
	else if (type == 0) {
		pPlayer->GetResetSystem(RESET_AUTO)->SetReset(0);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Reset Auto Disable");
	}
	else {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Command");
	}
}
/*
void ChatHandler::CommandDuelBetItem(const char* msg) {
	if (!sGameServer->IsDuelBetItem()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Duel Bet System Is Disabled.");
		return;
	}

	std::stringstream conversor(msg);
	std::string safe_name;
	std::string safe_type;
	uint32 count = 0;
	uint8 type = 0;
	conversor >> safe_name >> safe_type >> count;

	if (safe_name.size() > MAX_CHARACTER_LENGTH)
	{
		return;
	}

	std::transform(safe_type.begin(), safe_type.end(), safe_type.begin(), [](unsigned char c) {return std::toupper(c); });
	const char* type_name = safe_type.c_str();

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	type = sDuelBetItem->GetTypeBet(type_name);

	Player* pPlayer = this->m_Player;
	Player* pPlayerTarget = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if (type > 7) {
		pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "Not Type");
		return;
	}

	if (!sDuelBetItem->ItemBetData(pPlayer, type, count)) {
		return;
	}

	sDuelBetItem->DuelBetRequest(pPlayer, pPlayerTarget);
}
*/

void ChatHandler::CommandDanhLo(const char* msg) {
	Player* pPlayer = this->m_Player;

	if (!sGameServer->IsLoDeEnabled()) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Chuc nang lo de hien khong hoat dong");
		return;
	}

	Custom::SystemTimer m_timer = Custom::SystemTimer();

	if (m_timer.GetHour() >= 18) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Da qua thoi gian ghi lo de");
		return;
	}

	std::stringstream conversor(msg);
	uint16 bet_number = 0;
	std::string bet_type_string;
	uint32 bet_value = 0;
	uint32 betitem = 0;

	conversor >> bet_number >> bet_type_string >> bet_value;

	if (bet_number >= 100) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "So ghi khong hop le");
		return;
	}

	std::transform(bet_type_string.begin(), bet_type_string.end(), bet_type_string.begin(), [](unsigned char c) {return std::toupper(c); });
	const char* type_name = bet_type_string.c_str();

	if (strcmp(type_name, "ZEN") == 0) {
		if (bet_value > 100000000) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong zen dat cuoc khong lon hon 100 trieu Zen");
			return;
		}

		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Zen: %d", bet_value);
		if (pPlayer->MoneyGet() < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Zen dat cuoc");
			return;
		}

		pPlayer->MoneyReduce(bet_value);
	}
	else if(strcmp(type_name, "CHAOS") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Chaos: %d", bet_value);
		if (bet_value > 200) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 200");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(12, 15), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Chaos", bet_value);
			return;
		}

		betitem = ITEMGET(12, 15);
		pPlayer->ItemDelete(ITEMGET(12, 15), 0, bet_value);
	}
	else if(strcmp(type_name, "BLESS") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Bless: %d", bet_value);
		if (bet_value > 200) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 200");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(14, 13), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Bless", bet_value);
			return;
		}

		betitem = ITEMGET(14, 13);
		pPlayer->ItemDelete(betitem, 0, bet_value);
	}
	else if(strcmp(type_name, "SOUL") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Soul: %d", bet_value);
		if (bet_value > 200) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 200");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(14, 14), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Soul", bet_value);
			return;
		}

		betitem = ITEMGET(14, 14);
		pPlayer->ItemDelete(ITEMGET(14, 14), 0, bet_value);
	}
	else if(strcmp(type_name, "LIFE") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Life: %d", bet_value);
		if (bet_value > 200) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 200");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(14, 16), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Life", bet_value);
			return;
		}

		betitem = ITEMGET(14, 16);
		pPlayer->ItemDelete(ITEMGET(14, 16), 0, bet_value);
	}
	else if(strcmp(type_name, "CREA") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Crea: %d", bet_value);
		if (bet_value > 200) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 200");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(14, 22), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Creation", bet_value);
			return;
		}

		betitem = ITEMGET(14, 22);
		pPlayer->ItemDelete(ITEMGET(14, 22), 0, bet_value);
	}
	else {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Sai hinh thuc dat cuoc", bet_value);
		return;
	}

	QueryResult query = MuDatabase.PQuery("INSERT INTO lo_de(account_id, char_id, bet_type, bet_number, bet_item, bet_value, bet_time, reward_value_x, reward_status) VALUES(%d, %d, 0, %d, %d, %d, NOW(), 0, 0)", pPlayer->GetAccountData()->GetGUID(),
		pPlayer->GetGUID(), bet_number, betitem, bet_value);

	pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Ghi lo thanh cong. Chuc ban phat tai sau 18h30");
}

void ChatHandler::CommandDanhDe(const char* msg) {
	Player* pPlayer = this->m_Player;

	if (!sGameServer->IsLoDeEnabled()) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Chuc nang lo de hien khong hoat dong");
		return;
	}

	Custom::SystemTimer m_timer = Custom::SystemTimer();

	if (m_timer.GetHour() >= 18) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Da qua thoi gian ghi lo de");
		return;
	}

	std::stringstream conversor(msg);
	uint16 bet_number = 0;
	std::string bet_type_string;
	uint32 bet_value = 0;
	uint32 betitem = 0;

	conversor >> bet_number >> bet_type_string >> bet_value;

	if (bet_number >= 100) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "So ghi khong hop le");
		return;
	}

	std::transform(bet_type_string.begin(), bet_type_string.end(), bet_type_string.begin(), [](unsigned char c) {return std::toupper(c); });
	const char* type_name = bet_type_string.c_str();

	if (strcmp(type_name, "ZEN") == 0) {
		if (bet_value > 20000000) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong zen dat cuoc khong lon hon 20 trieu Zen");
			return;
		}

		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Zen: %d", bet_value);
		if (pPlayer->MoneyGet() < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Zen dat cuoc");
			return;
		}

		pPlayer->MoneyReduce(bet_value);
	}
	else if (strcmp(type_name, "CHAOS") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Chaos: %d", bet_value);

		if (bet_value > 50) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 50");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(12, 15), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Chaos", bet_value);
			return;
		}

		betitem = ITEMGET(12, 15);
		pPlayer->ItemDelete(ITEMGET(12, 15), 0, bet_value);
	}
	else if (strcmp(type_name, "BLESS") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Bless: %d", bet_value);
		if (bet_value > 50) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 50");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(14, 13), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Bless", bet_value);
			return;
		}

		betitem = ITEMGET(14, 13);
		pPlayer->ItemDelete(betitem, 0, bet_value);
	}
	else if (strcmp(type_name, "SOUL") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Soul: %d", bet_value);
		if (bet_value > 50) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 50");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(14, 14), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Soul", bet_value);
			return;
		}

		betitem = ITEMGET(14, 14);
		pPlayer->ItemDelete(ITEMGET(14, 14), 0, bet_value);
	}
	else if (strcmp(type_name, "LIFE") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Life: %d", bet_value);
		if (bet_value > 50) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 50");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(14, 16), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Life", bet_value);
			return;
		}

		betitem = ITEMGET(14, 16);
		pPlayer->ItemDelete(ITEMGET(14, 16), 0, bet_value);
	}
	else if (strcmp(type_name, "CREA") == 0) {
		//pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Bet Crea: %d", bet_value);
		if (bet_value > 50) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong vat pham dat cuoc khong duoc nhieu hon 50");
			return;
		}

		if (pPlayer->GetInventory()->GetItemCount(ITEMGET(14, 22), 0) < bet_value) {
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong du Creation", bet_value);
			return;
		}

		betitem = ITEMGET(14, 22);
		pPlayer->ItemDelete(ITEMGET(14, 22), 0, bet_value);
	}
	else {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Sai hinh thuc dat cuoc", bet_value);
		return;
	}

	QueryResult query = MuDatabase.PQuery("INSERT INTO lo_de(account_id, char_id, bet_type, bet_number, bet_item, bet_value, bet_time, reward_value_x, reward_status) VALUES(%d, %d, 1, %d, %d, %d, NOW(), 0, 0)", pPlayer->GetAccountData()->GetGUID(),
		pPlayer->GetGUID(), bet_number, betitem, bet_value);

	pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Ghi de thanh cong. Chuc ban phat tai sau 18h30");
}

void ChatHandler::CommandNhanThuongLoDe(const char* msg) {
	Player* pPlayer = this->m_Player;

	if (!pPlayer)
		return;

	if (!sGameServer->IsLoDeEnabled()) {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Chuc nang lo de hien khong hoat dong");
		return;
	}

	//QueryResult result = MuDatabase.PQuery("SELECT id, bet_type, bet_item, bet_value, reward_value_x FROM lo_de where char_id=%d AND reward_status=2", pPlayer->GetGUID());
	QueryResult result = MuDatabase.PQuery("SELECT id, bet_type, bet_item, bet_value, reward_value_x FROM lo_de where char_id=%d AND reward_status=2 LIMIT 1", pPlayer->GetGUID());

	if (result) {
		/*do {
			FieldReader reader(result->Fetch());

			uint32 id = reader.GetUInt32();
			uint8 bettype = reader.GetUInt8();
			uint32 betitem = reader.GetUInt32();
			uint32 betvalue = reader.GetUInt32();
			uint8 rewardvaluex = reader.GetUInt32();
			uint32 reward_value = betvalue * rewardvaluex;

			if (betitem == 0) {

				if (pPlayer->MoneyGet() + reward_value > 2000000000) {
					pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong Zen khi nhan qua nhieu.");
					continue;
				}

				pPlayer->MoneyAdd(reward_value);
				sDataServer->NoticeSend(NOTICE_GLOBAL, "[%s] choi lo de nhan thuong %d Zen", pPlayer->GetName(), reward_value);
			}
			else {
				//Item item_new(ITEMGET(type, index), level, durability, skill, luck, option, exe, ancient, 0, 255, 86400);
				item_template* item_temp = sItemMgr->GetItem(betitem);

				sDataServer->NoticeSend(NOTICE_GLOBAL, "[%s] choi lo de nhan thuong %d [%s]", pPlayer->GetName(), reward_value, item_temp->GetName());

				if (reward_value > 50) {
					while (reward_value > 0) {
						if (reward_value < 50) {
							Item item_reward(betitem, 0, reward_value);

							if (pPlayer->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(49)->IsItem()) {
								if (pPlayer->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(49)->IsItem()) {
									pPlayer->GetInventory()->AddItem(item_reward);
								}
								else {
									sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item_reward, GREMORY_CASE_TYPE_ACCOUNT, GremoryCaseReward::EVENT);
								}
							}
							else {
								sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item_reward, GREMORY_CASE_TYPE_MOBILE, GremoryCaseReward::EVENT);
							}
							
							reward_value -= reward_value;
						}
						else {
							Item item_reward(betitem, 0, 50);

							if (pPlayer->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(49)->IsItem()) {
								if (pPlayer->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(49)->IsItem()) {
									pPlayer->GetInventory()->AddItem(item_reward);
								}
								else {
									sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item_reward, GREMORY_CASE_TYPE_ACCOUNT, GremoryCaseReward::EVENT);
								}
							}
							else {
								sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item_reward, GREMORY_CASE_TYPE_MOBILE, GremoryCaseReward::EVENT);
							}

							reward_value -= 50;
						}
					}
				}
			}

			QueryResult update_status = MuDatabase.PQuery("UPDATE lo_de set reward_status=3 where id=%d", id);

		}while (result->NextRow());*/

		Field* field = result->Fetch();
		uint32 id = field[0].GetUInt32();
		uint8 bettype = field[1].GetUInt8();
		uint32 betitem = field[2].GetUInt32();
		uint32 betvalue = field[3].GetUInt32();
		uint8 rewardvaluex = field[4].GetUInt32();
		uint32 reward_value = betvalue * rewardvaluex;

		if (betitem == 0) {

			if (pPlayer->MoneyGet() + reward_value > 2000000000) {
				pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "So luong Zen khi nhan qua nhieu.");
				return;
			}

			pPlayer->MoneyAdd(reward_value);
			sDataServer->NoticeSend(NOTICE_GLOBAL, "[%s] choi lo de nhan thuong %d Zen", pPlayer->GetName(), reward_value);
		}
		else {
			//Item item_new(ITEMGET(type, index), level, durability, skill, luck, option, exe, ancient, 0, 255, 86400);
			item_template* item_temp = sItemMgr->GetItem(betitem);

			sDataServer->NoticeSend(NOTICE_GLOBAL, "[%s] choi lo de nhan thuong %d [%s]", pPlayer->GetName(), reward_value, item_temp->GetName());

			while (reward_value > 0) {
				if (reward_value < 50) {
					Item item_reward(betitem, 0, reward_value);

					if (pPlayer->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(49)->IsItem()) {
						if (pPlayer->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(49)->IsItem()) {
							pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong gian Gremory Case da day khong the nhan du phan thuong");
							break;
						}
						else {
							sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item_reward, GREMORY_CASE_TYPE_ACCOUNT, GremoryCaseReward::EVENT);
						}
					}
					else {
						sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item_reward, GREMORY_CASE_TYPE_MOBILE, GremoryCaseReward::EVENT);
					}

					reward_value -= reward_value;
				}
				else {
					Item item_reward(betitem, 0, 50);

					if (pPlayer->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->GetItem(49)->IsItem()) {
						if (pPlayer->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->GetItem(49)->IsItem()) {
							pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Khong gian Gremory Case da day khong the nhan du phan thuong");
							break;
						}
						else {
							sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item_reward, GREMORY_CASE_TYPE_ACCOUNT, GremoryCaseReward::EVENT);
						}
					}
					else {
						sItemMgr->ItemSerialCreateGremoryCase(pPlayer, item_reward, GREMORY_CASE_TYPE_MOBILE, GremoryCaseReward::EVENT);
					}

					reward_value -= 50;
				}
			}
		}

		QueryResult update_status = MuDatabase.PQuery("UPDATE lo_de set reward_status=3 where id=%d", id);
	}
	else {
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Ban khong co phan thuong de nhan");
	}
}

void ChatHandler::CommandCmdList(const char* msg) {
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "==========================================");
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Command Info List");
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Add Stats Command:");
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "/str /agi /vit /ene /cmd");
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Reset Command: /reset");
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "/resetauto 0/1(disable/enable)");
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Offline Attack: /offattack");
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "==========================================");
}

void ChatHandler::CommandErrtel(const char * msg)
{
	if (!sGameServer->IsTestEnabled())
	{
		return;
	}

	std::stringstream conversor(msg);
	int32 type = 0;
	int32 element = 0;
	int32 rank1 = 0;
	int32 level1 = 0;
	int32 rank2 = 0;
	int32 level2 = 0;
	int32 rank3 = 0;
	int32 level3 = 0;

	conversor >> type >> element >> rank1 >> level1 >> rank2 >> level2 >> rank3 >> level3;

	uint16 errtel_item = 0;

	switch (type)
	{
	case 0:
		{
			errtel_item = ITEMGET(12, 221);
		} break;

	case 1:
		{
			errtel_item = ITEMGET(12, 231);
		} break;

	case 2:
		{
			errtel_item = ITEMGET(12, 241);
		} break;

	case 3:
		{
			errtel_item = ITEMGET(12, 251);
		} break;

	case 4:
		{
			errtel_item = ITEMGET(12, 261);
		} break;
	}

	if (errtel_item <= 0)
	{
		this->m_Player->SendMessageBox(0, "Errtel Create", "Wrong errtel id");
		return;
	}

	if (element <= 0 || element >= ELEMENTAL_ATTRIBUTE_MAX)
	{
		this->m_Player->SendMessageBox(0, "Errtel Create", "Wrong errtel element");
		return;
	}

	if (!this->m_Player->GetInventory()->IsEmptySpace(errtel_item))
	{
		this->m_Player->SendMessageBox(0, "Errtel Create", "Not enough space in inventory.");
		return;
	}

	if (level1 < 0)
	{
		level1 = 0;
	}

	if (level1 > 10)
	{
		level1 = 10;
	}

	if (level2 < 0)
	{
		level2 = 0;
	}

	if (level2 > 10)
	{
		level2 = 10;
	}

	if (level3 < 0)
	{
		level3 = 0;
	}

	if (level3 > 10)
	{
		level3 = 10;
	}

	Item item(errtel_item);
	uint8 rank_level = 0;

	ErrtelOption const* pPentagramJewelOptionInfo01 = sPentagramSystem->GetErrtelOption(errtel_item, 1, rank1);

	if (pPentagramJewelOptionInfo01)
	{
		rank_level++;
		item.SetSocket(0, rank1 | (level1 * 16));

		ErrtelOption const* pPentagramJewelOptionInfo02 = sPentagramSystem->GetErrtelOption(errtel_item, 2, rank2);

		if (pPentagramJewelOptionInfo02)
		{
			rank_level++;
			item.SetSocket(1, rank2 | (level2 * 16));

			ErrtelOption const* pPentagramJewelOptionInfo03 = sPentagramSystem->GetErrtelOption(errtel_item, 3, rank3);

			if (pPentagramJewelOptionInfo03)
			{
				rank_level++;
				item.SetSocket(2, rank3 | (level3 * 16));
			}
		}
	}

	if (rank_level <= 0)
	{
		this->m_Player->SendMessageBox(0, "Errtel Create", "Wrong errtel options");
		return;
	}

	item.SetSocketBonus(element | rank_level * 16);
	
	sItemMgr->ItemSerialCreateItem(this->m_Player, serial_create_inventory, item);
}

void ChatHandler::CommandSiegeRefresh(const char * msg)
{
	this->m_Player->ViewportCreate(VIEWPORT_CREATE_FLAG_SIEGE);
}

void ChatHandler::CommandTestPacket(const char * msg)
{
	if ( !this->m_Player->IsAdministrator() )
		return;

	std::stringstream conversor(msg);
	int32 result = 0;
	int32 data1 = 0;
	int32 data2 = 0;
	int32 data3 = 0;
	int32 data4 = 0;
	int32 data5 = 0;
	int32 data6 = 0;
	int32 data7 = 0;
	int32 data8 = 0;
	int32 data9 = 0;
	int32 data10 = 0;
	int32 data11 = 0;

	/*
	EnterResult(pPlayer, 0x06);
	*/

	conversor >> result >> data1 >> data2 >> data3 >> data4 >> data5 >> data6 >> data7 >> data8 >> data9 >> data10 >> data11;

	if ( result < 0 )
	{
		result = 0;
	}

	if ( result > 255 )
	{
		result = 255;
	}

	this->m_Player->SetPacketResult(result);
	this->m_Player->SetPacketData1(data1);
	this->m_Player->SetPacketData2(data2);
	this->m_Player->SetPacketData3(data3);
	this->m_Player->SendNotice(NOTICE_NORMAL_BLUE, "Packet Result: %02X", uint8(result));

	//this->m_Player->SetSendBuff(result != 0);

	switch (result)
	{
	case 0:
		{
			 /* MU_HELPER_PLUS_81 pMsg;
			  pMsg.result = data1;
			  pMsg.data1 = data2;
			  pMsg.data2 = data3;
			  pMsg.data3 = data4;

			  this->m_Player->SendPacket(&pMsg);*/

			  this->m_Player->StateInfoSend(data1, data2);

			  /*this->m_Player->SetIntData(UNIT_INT_ATTACK_SPEED, data1);
			  this->m_Player->SetIntData(UNIT_INT_MAGIC_SPEED, data1);

			  CHARACTER_ATTACK_SPEED_SEND pMsg(this->m_Player->GetIntData(UNIT_INT_ATTACK_SPEED), this->m_Player->GetIntData(UNIT_INT_MAGIC_SPEED));
			  this->m_Player->sendPacket(MAKE_PCT(pMsg));

			  this->m_Player->AttackSpeedSend();*/
			  /*ITEM_ENHANCEMENT_RESULT pMsg;
			  pMsg.result = data1;

			  this->m_Player->sendPacket(&pMsg);*/

			  //sPentagramSystem->DelAllPentagramJewelInfo(this->m_Player, this->m_Player->GetInventory()->GetItem(PENTAGRAM_SLOT), 0);

			  /*TEST_PACKET_94 pMsg;
			  pMsg.result = data1;
			  pMsg.index = this->m_Player->GetEntry();

			  this->m_Player->sendPacket(&pMsg);*/
		} break;

	case 1:
		{
			  /*MU_HELPER_PLUS_82 pMsg;
			  pMsg.result = data1;
			  pMsg.data1 = data2;
			  pMsg.data2 = data3;
			  pMsg.data3 = data4;

			  this->m_Player->SendPacket(&pMsg);*/


			  //this->m_Player->StateInfoSend(data1, data2);

			  /*TEST_PACKET_95 pMsg;
			  pMsg.result = data1;
			  pMsg.index = this->m_Player->GetEntry();

			  this->m_Player->sendPacket(&pMsg);*/
			  /*uint8 buffer[8192];
			  int32 write_size = 0;

			  MONSTER_SOUL_QUEST_HEAD* head = (MONSTER_SOUL_QUEST_HEAD*)&buffer[write_size];
			  head->junk = 0;
			  head->count = data1;

			  write_size += sizeof(MONSTER_SOUL_QUEST_HEAD);

			  for (int32 i = 0; i < data1; ++i)
			  {
				  MONSTER_SOUL_QUEST_BODY_1* body = (MONSTER_SOUL_QUEST_BODY_1*)&buffer[write_size];
				  body->type = data2;
				  body->data[0] = data3;
				  body->data[1] = data4;
				  body->expire_date = data5;
				  body->junk1 = data6;
				  body->amount = data7;
				  body->status = data8;
				  body->junk2 = data9;

				  write_size += sizeof(MONSTER_SOUL_QUEST_BODY_1);

				  for (int32 n = 0; n < body->amount; ++n)
				  {
					  MONSTER_SOUL_QUEST_BODY_2* data = (MONSTER_SOUL_QUEST_BODY_2*)&buffer[write_size];
					  data->id = n;
					  data->amount = data10;

					  write_size += sizeof(MONSTER_SOUL_QUEST_BODY_2);
				  }
			  }

			  head->set(HEACODE_EVENT_INVENTORY, 0x41, write_size);

			  this->m_Player->sendPacket(buffer);*/
		} break;

	case 2:
		{
			  MU_HELPER_PLUS_83 pMsg;
			  pMsg.result = data1;

			  this->m_Player->SendPacket(&pMsg);
		} break;

	case 3:
		{
			  MU_HELPER_PLUS_RUN_SEND pMsg;
			  pMsg.result = data1;

			  this->m_Player->SendPacket(&pMsg);
		} break;

	case 4:
		{
			  MU_HELPER_PLUS_86 pMsg;

			  this->m_Player->SendPacket(&pMsg);
		} break;

	case 5:
		{
			  MU_HELPER_PLUS_88 pMsg;
			  pMsg.result = data1;

			  this->m_Player->SendPacket(&pMsg);
		} break;

	case 6:
		{
			  MU_HELPER_PLUS_90 pMsg;
			  pMsg.result = data1;

			  this->m_Player->SendPacket(&pMsg);
		} break;

	case 7:
		{
			  if (!inventory_range(data1))
			  {
				  full_inventory_loop(i)
				  {
					  this->m_Player->PrintItemData(this->m_Player->GetInventory()->GetItem(i));
				  }
			  }
			  else
			  {
				  this->m_Player->PrintItemData(this->m_Player->GetInventory()->GetItem(data1));
			  }
		} break;

	case 8:
		{
			  INIT_SOCKET_DATA(socket);
			  for (int32 i = 0; i < data3; ++i)
			  {
				  socket[i] = sItemMgr->GenerateWingGradedOption(socket);
			  }

			  sItemMgr->ItemSerialCreateItem(this->m_Player, serial_create_inventory, Item(ITEMGET(data1, data2), 0, 0, 0, 0, 0, 0, 0, socket));
		} break;

	case 9:
		{

		} break;

	case 10:
		{
			/*PACKET_0xEC_0x60 pMsg;
			pMsg.data = data1;

			this->m_Player->SEND_PCT(pMsg);*/
		} break;

	case 11:
		{
			/*uint8 buffer[2048];
			POINTER_PCT(PACKET_CHANNEL_HEAD, head, buffer, 0);
			POINTER_PCT(PACKET_CHANNEL_BODY, body, buffer, sizeof(PACKET_CHANNEL_HEAD));

			body[0].server = data1;
			body[0].data1 = data2;
			body[0].data2 = data3;
			body[0].type = data4;
			body[0].gold = data5;

			body[1].server = data6;
			body[1].data1 = data7;
			body[1].data2 = data8;
			body[1].type = data9;
			body[1].gold = data10;

			head->count = 2;
			head->data = data11;
			
			head->Set(0xEC, 0x57, sizeof(PACKET_CHANNEL_HEAD) + (head->count * sizeof(PACKET_CHANNEL_BODY)));

			this->m_Player->sendPacket(buffer, head->GetSize());*/
		} break;

	case 12:
		{
			/*PACKET_0x59_0x03 pMsg;
			pMsg.data = data1;

			this->m_Player->SEND_PCT(pMsg);*/
		} break;

	case 13:
		{
			/*LABYRINTH_OF_DIMENSIONS_ENTRANCE_HEAD pMsg;
			pMsg.rank = data1;
			pMsg.position = data2;
			pMsg.max_floor = data3;
			pMsg.top_position = data4;
			pMsg.remain_time = data5;
			
			this->m_Player->SEND_PCT(pMsg);*/
		} break;

	case 14:
		{

		} break;

	case 15:
		{

		} break;

	case 16:
		{

		} break;
	}

	/*switch (result)
	{
	case 0:
		{
			this->m_Player->GetTimer(PLAYER_TIMER_CASTLE_SIEGE_TIME)->Start();
		} break;

	case 1:
		{
			sEventMgr->CastleSiegeTime(this->m_Player, data1);
		} break;
	}*/

	/*for ( int32 i = 0; i < data1; ++i )
	{
		switch ( result )
		{
		case 0:
			{
				sItemBagMgr->RunItemBag(this->m_Player, sGameServer->GetLastManStandingReward(), Item());
			} break;

		case 1:
			{
				sItemBagMgr->RunItemBag(this->m_Player, sGameServer->GetScrambleReward(), Item());
			} break;

		case 2:
			{
				sItemBagMgr->RunItemBag(this->m_Player, sGameServer->GetDungeonRaceReward(), Item());
			} break;

		case 3:
			{
				sItemBagMgr->RunItemBag(this->m_Player, sGameServer->GetLosttowerRaceReward(), Item());
			} break;

		case 4:
			{
				sItemBagMgr->RunItemBag(this->m_Player, "Muun Egg", Item());
			} break;

		case 5:
			{
				sItemBagMgr->RunItemBag(this->m_Player, "Seed Capsule(Fire)", Item());
			} break;
		}
	}*/

	/*int32 damage_min = 0;
	int32 damage_max = 0;

	this->m_Player->ApplyWeaponDamage(damage_min, damage_max);

	this->m_Player->SendNotice(NOTICE_NORMAL_BLUE, "damage_min: %d / damage_max: %d", damage_min, damage_max);*/

	/*int32 monster = 247;
	int32 move_distance = 30;
	uint32 despawn_time = 86400;

	static const uint16 world_list[29] = { 0, 1, 2, 3, 4, 7, 8, 10, 24, 25, 26, 27, 28, 29, 33, 36, 37, 38, 51, 56, 57, 63, 80, 81, 91, 95, 101, 110, 112 };

	Monster * mMonster = nullptr;

	uint32 oldMStime = getMSTime();
	sLog->outInfo("root", "Adding Monster Kill BOT...");
	int32 added_count = 0;

	for ( int32 world_id = 0; world_id < 29; ++world_id )
	{
		for ( int32 i = 0; i < result; ++i )
		{
			mMonster = sObjectMgr->MonsterTryAdd(monster);

			if ( !mMonster )
				break;
	
			mMonster->SetWorldId(world_list[world_id]);
			mMonster->SetInstance(-1);
			mMonster->SetBasicLocation(0, 0, 255, 255);
			mMonster->SetDespawnTime(despawn_time * IN_MILLISECONDS);
			mMonster->SetDespawnTick(GetTickCount());
			mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			mMonster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
			mMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
			mMonster->SetMoveDistance(move_distance);
			mMonster->SetSummoner(this->m_Player);
			mMonster->SetScriptName("monster_kill_bot_ai");
			mMonster->SetElementalAttribute(ELEMENTAL_ATTRIBUTE_MAX);
			mMonster->SetScriptElementalAttribute(ELEMENTAL_ATTRIBUTE_MAX);
			mMonster->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MIN, 1000);
			mMonster->SetIntData(UNIT_INT_ELEMENTAL_DAMAGE_MAX, 2000);
			mMonster->SetIntData(UNIT_INT_ELEMENTAL_ATTACK_SUCCESS_RATE, 1000);
			mMonster->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE, 2000);
			mMonster->SetIntData(UNIT_INT_ELEMENTAL_DEFENSE_SUCCESS_RATE, 1000);
			mMonster->SetSendType(MAIN_OBJECT_PLAYER);
	
			if ( !mMonster->GetWorld() )
			{
				mMonster->SetConnectStatus(CONNECT_STATUS_NONE);
				continue;
			}

			mMonster->AddToWorld();
			++added_count;
		}
	}

	sLog->outInfo("root", "Added %d Monster Kill BOT in %u ms", added_count, GetMSTimeDiffToNow(oldMStime));*/
	
	/*std::stringstream conversor(msg);
	int32 day = -1;

	conversor >> day;
	sQuestMgr->SetTestingDay(day);*/
}

void ChatHandler::CommandKick(const char * msg)
{
	std::stringstream conversor(msg);

	std::string name_safe;
	int32 time = 0;
	conversor >> name_safe >> time;

	time *= MINUTE;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), name_safe.c_str(), name_safe.size() > MAX_CHARACTER_LENGTH ? MAX_CHARACTER_LENGTH : name_safe.size());

	sDataServer->AdminCommandRequest(this->m_Player, 0, 0, name, nullptr, time);
}

void ChatHandler::CommandEvents(const char * msg)
{
	std::stringstream conversor(msg);

	int32 event_id = -1;
	int32 invasion = 0;
	int32 event_time = 0;
	int32 notify_time = 0;
	conversor >> event_id >> invasion >> event_time >> notify_time;

	if ( event_time <= 0 )
		event_time = 1;

	if ( notify_time < 0 )
		notify_time = 0;

	switch ( event_id )
	{
	case EVENT_BLOOD_CASTLE:
		{
			sBloodCastleMgr->Start(event_time, notify_time);
		} break;

	case EVENT_DEVIL_SQUARE:
		{
			sDevilSquareMgr->Start(event_time, notify_time);
		} break;

	case EVENT_CHAOS_CASTLE:
		{
			sChaosCastleMgr->Start(event_time, notify_time);
		} break;

	case EVENT_ILLUSION_TEMPLE:
		{
			sIllusionTempleMgr->Start(event_time, notify_time);
		} break;

	case EVENT_CRYWOLF:
		{
			if ( sCrywolf->GetState() == CRYWOLF_STATE_NONE )
				sCrywolf->SetState_Notify_1();
		} break;
	case EVENT_RAKLION:
		{
			if ( sRaklion->GetState() == RAKLION_STATE_CLOSED )
				sRaklion->ChangeState(RAKLION_STATE_OPEN);
		} break;

	case EVENT_INVASION:
		{
			sInvasionMgr->LaunchInvasion(invasion, event_time, notify_time);
		} break;

	case EVENT_HAPPY_HOUR:
		{
			sHappyHour->Start(event_time, notify_time);
		} break;

	case EVENT_CASTLE_SIEGE:
		{
			sCastleSiege->ChangeState(invasion, event_time == 1 ? true: false, notify_time);
		} break;

	case EVENT_SCRAMBLE:
		{
			sScramble->Start(event_time, notify_time);
		} break;

	case EVENT_DUNGEON_RACE:
		{
			sDungeonRace->Start(event_time, notify_time);
		} break;

	case EVENT_LOSTTOWER_RACE:
		{
			sLosttowerRace->Start(event_time, notify_time);
		} break;

	case EVENT_CHAOS_CASTLE_SURVIVAL:
		{
			sChaosCastleSurvivalMgr->Start(event_time, notify_time);
		} break;

	case EVENT_PROTECTOR_OF_ACHERON:
		{
			sProtectorOfAcheron->Start();
		} break;

	case EVENT_TORMENTED_SQUARE:
		{
			sTormentedSquare->Start(notify_time);
		} break;

	case EVENT_ARKA_WAR:
		{
			sArkaWar->ChangeState(invasion, !sGameServer->IsArkaWarEnabled());
		} break;

	case EVENT_DOPPELGANGER:
		{
			sDoppelganger->Start();
		} break;

	case EVENT_LAST_MAN_STANDING:
		{
			sLastManStanding->Start(notify_time);
		} break;

	case EVENT_NIXIES_LAKE:
		{
			if ( sNixiesLake->GetState() == NIXIES_LAKE_STATE_NONE )
			{
				sNixiesLake->ChangeState(NIXIES_LAKE_STATE_OPEN);
			}
		} break;

	case EVENT_LABYRINTH_OF_DIMENSIONS:
		{
		} break;

	case EVENT_TORMENTED_SQUARE_SURVIVAL:
		{
			sTormentedSquareSurvival->Start(notify_time);
		} break;

	case EVENT_CASTLE_DEEP:
		{
			sCastleDeep->Start(notify_time);
		} break;

	case EVENT_WORLD_BOSS:
		{
			sWorldBoss->Start();
		} break;

	case EVENT_SWAMP_OF_DARKNESS:
		{
			if (sSwampOfDarkness->GetState() == SWAMP_OF_DARKNESS_STATE_NONE)
			{
				sSwampOfDarkness->ChangeState(SWAMP_OF_DARKNESS_STATE_OPEN);
			}
		} break;
	}
}

void ChatHandler::CommandStopEvents(const char * msg)
{
	std::stringstream conversor(msg);

	int32 event_id = -1;
	int32 invasion = 0;
	conversor >> event_id >> invasion;

	switch ( event_id )
	{
	case EVENT_BLOOD_CASTLE:
		{
			if ( sBloodCastleMgr->GetState() == EVENT_MGR_STATE_PLAYING )
			{
				for ( int32 i = 0; i < MAX_BLOOD_CASTLE_GROUND; ++i )
				{
					sBloodCastleMgr->GetGround(i)->SetState_Playend();
				}
			}
		} break;

	case EVENT_DEVIL_SQUARE:
		{
			if ( sDevilSquareMgr->GetState() == EVENT_MGR_STATE_PLAYING )
			{
				for ( int32 i = 0; i < MAX_DEVIL_SQUARE_GROUND; ++i )
				{
					sDevilSquareMgr->GetGround(i)->SetState_Playend();
				}
			}
		} break;

	case EVENT_CHAOS_CASTLE:
		{
			if ( sChaosCastleMgr->GetState() == EVENT_MGR_STATE_PLAYING )
			{
				for ( int32 i = 0; i < MAX_CHAOS_CASTLE_GROUND; ++i )
				{
					sChaosCastleMgr->GetGround(i)->SetState_Playend();
				}
			}
		} break;

	case EVENT_ILLUSION_TEMPLE:
		{
			if ( sIllusionTempleMgr->GetState() == EVENT_MGR_STATE_PLAYING )
			{
				for ( int32 i = 0; i < MAX_ILLUSION_TEMPLE_GROUND; ++i )
				{
					sIllusionTempleMgr->GetGround(i)->SetState_Playend();
				}
			}
		} break;

	case EVENT_CRYWOLF:
		{
			sCrywolf->SetState_None();
		} break;

	case EVENT_IMPERIAL_FORTRESS:
		{
			sImperialFortressMgr->Stop();
		} break;

	case EVENT_RAKLION:
		{
			sRaklion->ChangeState(RAKLION_STATE_END);
		} break;

	case EVENT_KANTURU:
		{
			sKanturuMgr->ChangeState(KANTURU_STATE_NONE);
		} break;

	case EVENT_INVASION:
		{
			sInvasionMgr->StopInvasion(invasion);
		} break;

	case EVENT_HAPPY_HOUR:
		{
			sHappyHour->SetState_None();
		} break;

	case EVENT_CASTLE_SIEGE:
		{
			sCastleSiege->ChangeState(invasion, true);
		} break;

	case EVENT_SCRAMBLE:
		{
			sScramble->SetState_None();
		} break;

	case EVENT_DUNGEON_RACE:
		{
			sDungeonRace->SetState_None();
		} break;

	case EVENT_LOSTTOWER_RACE:
		{
			sLosttowerRace->SetState_None();
		} break;

	case EVENT_DOPPELGANGER:
		{
			if ( sDoppelganger->GetState() == DOPPELGANGER_STATE_PLAYING )
			{
				for ( int32 i = 0; i < MAX_DOPPELGANGER_GROUND; ++i )
				{
					sDoppelganger->GetGround(i)->SetState_End();
				}
			}
		} break;

	case EVENT_CHAOS_CASTLE_SURVIVAL:
		{
			if ( sChaosCastleSurvivalMgr->GetState() == EVENT_MGR_STATE_PLAYING )
			{
				sChaosCastleSurvivalMgr->GetGround()->SetState_Playend();
			}

		} break;

	case EVENT_PROTECTOR_OF_ACHERON:
		{
			if ( sProtectorOfAcheron->GetState() == PROTECTOR_OF_ACHERON_STATE_PLAYING )
			{
				sProtectorOfAcheron->SetState_End();
			}
		} break;

	case EVENT_TORMENTED_SQUARE: 
		{
			if ( sTormentedSquare->GetState() == TORMENTED_SQUARE_STATE_PLAYING )
			{
				for ( int32 i = 0; i < MAX_TORMENTED_SQUARE_GROUND; ++i )
				{
					sTormentedSquare->GetGround(i)->SetState_End();
				}
			}
		} break;

	case EVENT_LAST_MAN_STANDING:
		{
			sLastManStanding->SetState_End();
		} break;

	case EVENT_NIXIES_LAKE:
		{
			sNixiesLake->ChangeState(NIXIES_LAKE_STATE_NONE);
		} break;

	case EVENT_LABYRINTH_OF_DIMENSIONS:
		{
		} break;

	case EVENT_TORMENTED_SQUARE_SURVIVAL:
		{
			if ( sTormentedSquareSurvival->GetState() == TORMENTED_SQUARE_STATE_PLAYING )
			{
				sTormentedSquareSurvival->GetGround()->SetState_End();
			}
		} break;

	case EVENT_CASTLE_DEEP:
		{
			if ( sCastleDeep->GetState() == CASTLE_DEEP_STATE_PLAYING || sCastleDeep->GetState() == CASTLE_DEEP_STATE_NOTIFY )
			{
				sCastleDeep->SetState_None();
			}
		} break;

	case EVENT_WORLD_BOSS:
		{
			if ( sWorldBoss->GetState() == WORLD_BOSS_STATE_PLAYING )
			{
				sWorldBoss->SetState_None();
			}
		} break;

	case EVENT_SWAMP_OF_DARKNESS:
		{
			sSwampOfDarkness->ChangeState(SWAMP_OF_DARKNESS_STATE_NONE);
		} break;
	}
}

void ChatHandler::CommandMute(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 mute_time = 0;

	conversor >> safe_name >> mute_time;

	if ( safe_name.size() >= (MAX_CHARACTER_LENGTH + 1) || mute_time <= 0 )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size() > MAX_CHARACTER_LENGTH ? MAX_CHARACTER_LENGTH: safe_name.size());

	sDataServer->AdminCommandRequest(this->m_Player, 2, 0, name, nullptr, mute_time);
}

void ChatHandler::CommandItemNormal(const char * msg)
{
	std::stringstream conversor(msg);
	int32 count = 1;
	int32 type = 0;
	int32 index = 0;
	int32 level = 0;
	int32 durability = 0;
	int32 skill = 0;
	int32 luck = 0;
	int32 option = 0;
	int32 exe = 0;
	int32 ancient = 0;
	int32 harmony = 0;
	int32 opt380 = 0;
	int32 duration = 0;

	conversor >> count >> type >> index >> level >> durability >> skill >> luck >> option >> exe >> ancient >> harmony >> opt380 >> duration;

	item_template const* item = sItemMgr->GetItem(ITEMGET(type, index));

	if ( !item )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Item");
		return;
	}

	for ( int32 i = 0; i < count; i++ )
	{
		Item item_new(ITEMGET(type,index), level, durability, skill, luck, option, exe, ancient, nullptr, 0xFF, duration);
		item_new.SetHarmony(harmony);
		item_new.Set380(opt380);

		sItemMgr->ItemSerialCreate(this->m_Player,
			this->m_Player->GetWorldId(), this->m_Player->GetX(), this->m_Player->GetY(), item_new, false);
	}
}
	
void ChatHandler::CommandItemAdd(const char * msg)
{
	std::stringstream conversor(msg);
	int32 count = 1;
	int32 type = 0;
	int32 index = 0;
	int32 level = 0;
	int32 durability = 0;
	int32 skill = 0;
	int32 luck = 0;
	int32 option = 0;
	int32 exe = 0;
	int32 ancient = 0;
	int32 harmony = 0;
	int32 opt380 = 0;
	int32 duration = 0;

	conversor >> count >> type >> index >> level >> durability >> skill >> luck >> option >> exe >> ancient >> harmony >> opt380 >> duration;

	Player* pOwner = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if ( !pOwner )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	if ( pOwner->GetInterfaceState()->GetID() != InterfaceData::None || pOwner->GetPersonalStore()->IsBusy() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s is busy", this->m_WhisperName);
		return;
	}
	
	item_template const* item = sItemMgr->GetItem(ITEMGET(type, index));

	if ( !item )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Item");
		return;
	}

	for ( int32 i = 0; i < count; ++i )
	{
		if ( !pOwner->GetInventory()->IsEmptySpace(item->GetX(), item->GetY()) )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s has full inventory", pOwner->GetName());
			return;
		}

		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Created item %s on %s", item->GetName(level), pOwner->GetName());
		pOwner->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s created item %s", this->m_Player->GetName(), item->GetName(level));

		Item item_new(ITEMGET(type,index), level, durability, skill, luck, option, exe, ancient);
		item_new.SetHarmony(harmony);
		item_new.Set380(opt380);
		if ( duration > 0 )
		{
			item_new.SetExpireDate(duration + time(nullptr));
		}

		sItemMgr->ItemSerialCreateItem(pOwner, serial_create_inventory, item_new);
	}
}

void ChatHandler::CommandItemAddPlayer(const char* msg)
{
	std::stringstream conversor(msg);
	int32 playerID = 0;
	int32 count = 1;
	int32 type = 0;
	int32 index = 0;
	int32 level = 0;
	int32 durability = 0;
	int32 skill = 0;
	int32 luck = 0;
	int32 option = 0;
	int32 exe = 0;
	int32 ancient = 0;
	int32 harmony = 0;
	int32 opt380 = 0;
	int32 duration = 0;
	int32 socketbonus = 0;

	conversor >> playerID >> count >> type >> index >> level >> durability >> skill >> luck >> option >> exe >> ancient >> socketbonus >> harmony >> opt380 >> duration;

	Player* pOwner = sObjectMgr->FindPlayerByGUID(playerID);

	if (!pOwner)
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player");
		return;
	}

	item_template const* item = sItemMgr->GetItem(ITEMGET(type, index));

	if (!item)
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Item");
		return;
	}

	for (int32 i = 0; i < count; ++i)
	{
		if (!pOwner->GetInventory()->IsEmptySpace(item->GetX(), item->GetY()))
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s has full inventory", pOwner->GetName());
			return;
		}

		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Created item %s on %s", item->GetName(level), pOwner->GetName());
		pOwner->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s created item %s", this->m_Player->GetName(), item->GetName(level));

		Item item_new(ITEMGET(type, index), level, durability, skill, luck, option, exe, ancient);
		item_new.SetHarmony(harmony);
		item_new.Set380(opt380);
		if (socketbonus > 0) {
			if (type < 6) {
				item_new.SetSocketBonus(sItemMgr->GetRandomMasteryBonus(2));
			}
			else if (type < 12) {
				//item_new.SetSocketBonus(sItemMgr->GetRandomMasteryBonus(1));
				item_new.SetSocketBonus(socketbonus);
			}
		}

		if (duration > 0)
		{
			item_new.SetExpireDate(duration + time(nullptr));
		}

		sItemMgr->ItemSerialCreateItem(pOwner, serial_create_inventory, item_new);
	}
}

	
void ChatHandler::CommandItemShow(const char * msg)
{
	sWorldMgr->MakeItemVisible(this->m_Player);
}
	
void ChatHandler::CommandItemHide(const char * msg)
{
	std::stringstream conversor(msg);
	int32 count = 1;
	int32 type = 0;
	int32 index = 0;
	int32 level = 0;
	int32 durability = 0;
	int32 skill = 0;
	int32 luck = 0;
	int32 option = 0;
	int32 duration = 0;
	int32 looting = 0;

	conversor >> count >> type >> index >> level >> durability >> skill >> luck >> option >> duration >> looting;

	item_template const* item = sItemMgr->GetItem(ITEMGET(type, index));

	if ( !item )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Item");
		return;
	}

	Item add_item(ITEMGET(type,index), level, durability, skill, luck, option);
	add_item.SetDurationTime(duration * IN_MILLISECONDS);
	add_item.SetLootingTime(looting * IN_MILLISECONDS);
	
	for ( int32 i = 0; i < count; i++ )
	{
		sItemMgr->ItemSerialCreate(this->m_Player,
			this->m_Player->GetWorldId(), this->m_Player->GetX(), this->m_Player->GetY(), 
			add_item, false, false);
	}
}

void ChatHandler::CommandItemRemove(const char * msg)
{

}

void ChatHandler::CommandAdminItemPentagram(const char * msg)
{
	std::stringstream conversor(msg);
	int32 type = 0;
	int32 index = 0;
	int32 level = 0;
	int32 socket_1 = 0;
	int32 socket_2 = 0;
	int32 socket_3 = 0;
	int32 socket_4 = 0;
	int32 socket_5 = 0;
	int32 socket_bonus;

	conversor >> type >> index >> level >> socket_1 >> socket_2 >> socket_3 >> socket_4 >> socket_5 >> socket_bonus;

	if ( socket_1 < 0 ) { socket_1 = 0; }
	if ( socket_2 < 0 ) { socket_2 = 0; }
	if ( socket_3 < 0 ) { socket_3 = 0; }
	if ( socket_4 < 0 ) { socket_4 = 0; }
	if ( socket_5 < 0 ) { socket_5 = 0; }
	if ( socket_bonus < 0 ) { socket_bonus = 0; }

	if ( socket_1 > SOCKET_SLOT_NONE ) { socket_1 = SOCKET_SLOT_NONE; }
	if ( socket_2 > SOCKET_SLOT_NONE ) { socket_2 = SOCKET_SLOT_NONE; }
	if ( socket_3 > SOCKET_SLOT_NONE ) { socket_3 = SOCKET_SLOT_NONE; }
	if ( socket_4 > SOCKET_SLOT_NONE ) { socket_4 = SOCKET_SLOT_NONE; }
	if ( socket_5 > SOCKET_SLOT_NONE ) { socket_5 = SOCKET_SLOT_NONE; }
	if ( socket_bonus > 255 ) { socket_bonus = 255; }

	if ( socket_1 == 0xFF ) { socket_1 = SOCKET_SLOT_NONE; }
	if ( socket_2 == 0xFF ) { socket_2 = SOCKET_SLOT_NONE; }
	if ( socket_3 == 0xFF ) { socket_3 = SOCKET_SLOT_NONE; }
	if ( socket_4 == 0xFF ) { socket_4 = SOCKET_SLOT_NONE; }
	if ( socket_5 == 0xFF ) { socket_5 = SOCKET_SLOT_NONE; }
	if ( socket_1 == 0xFE ) { socket_1 = SOCKET_SLOT_EMPTY; }
	if ( socket_2 == 0xFE ) { socket_2 = SOCKET_SLOT_EMPTY; }
	if ( socket_3 == 0xFE ) { socket_3 = SOCKET_SLOT_EMPTY; }
	if ( socket_4 == 0xFE ) { socket_4 = SOCKET_SLOT_EMPTY; }
	if ( socket_5 == 0xFE ) { socket_5 = SOCKET_SLOT_EMPTY; }

	SocketDataType socket[MAX_SOCKET_SLOT] = { (SocketDataType)socket_1, (SocketDataType)socket_2, (SocketDataType)socket_3, (SocketDataType)socket_4, (SocketDataType)socket_5 };
	
	Player* pOwner = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if ( !pOwner )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	if ( pOwner->GetInterfaceState()->GetID() != InterfaceData::None || pOwner->GetPersonalStore()->IsBusy() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s is busy", this->m_WhisperName);
		return;
	}
	
	item_template const* item = sItemMgr->GetItem(ITEMGET(type, index));

	if ( !item )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Item");
		return;
	}

	if ( !pOwner->GetInventory()->IsEmptySpace(item->GetX(), item->GetY()) )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s has full inventory", pOwner->GetName());
		return;
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Created item %s on %s", item->GetName(level), pOwner->GetName());
	pOwner->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s created item %s", this->m_Player->GetName(), item->GetName(level));

	Item item_new(ITEMGET(type, index), level, 5, 0, 0, 0, 0, 0, socket, socket_bonus);

	sItemMgr->ItemSerialCreateItem(pOwner, serial_create_inventory, item_new);
}

void ChatHandler::CommandItemGremory(const char * msg)
{
	std::stringstream conversor(msg);
	int32 inventory = 1;
	int32 type = 0;
	int32 index = 0;
	int32 level = 0;
	int32 durability = 0;
	int32 skill = 0;
	int32 luck = 0;
	int32 option = 0;
	int32 exe = 0;
	int32 ancient = 0;
	int32 duration = 0;

	conversor >> inventory >> type >> index >> level >> durability >> skill >> luck >> option >> exe >> ancient >> duration;

	Player* pOwner = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if ( !pOwner )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	if ( pOwner->GetInterfaceState()->GetID() != InterfaceData::None || pOwner->GetPersonalStore()->IsBusy() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s is busy", this->m_WhisperName);
		return;
	}
	
	item_template const* item = sItemMgr->GetItem(ITEMGET(type, index));

	if ( !item )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Item");
		return;
	}

	if ( inventory < 0 )
	{
		inventory = 0;
	}

	if ( inventory >= 1 )
	{
		inventory = 1;
	}

	if ( !pOwner->GetInventory()->IsEmptySpace(item->GetX(), item->GetY()) )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s has full inventory", pOwner->GetName());
		return;
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Created item %s on %s", item->GetName(level), pOwner->GetName());
	pOwner->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s created item %s", this->m_Player->GetName(), item->GetName(level));

	Item item_new(ITEMGET(type,index), level, durability, skill, luck, option, exe, ancient,0,255,86400);

	sItemMgr->ItemSerialCreateGremoryCase(pOwner, item_new, inventory, GremoryCaseReward::GM_REWARD);
}

void ChatHandler::CommandItemWing(const char * msg)
{
	std::stringstream conversor(msg);
	int32 type = 12;
	int32 index = 0;
	int32 level = 0;
	int32 socket_1 = 0;
	int32 socket_2 = 0;
	int32 socket_3 = 0;
	int32 socket_4 = 0;
	int32 socket_5 = 0;
	int32 socket_bonus = 0;

	conversor >> index >> level >> socket_1 >> socket_2 >> socket_3 >> socket_4 >> socket_5 >> socket_bonus;

	Player* pOwner = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if (!pOwner)
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	if (pOwner->GetInterfaceState()->GetID() != InterfaceData::None || pOwner->GetPersonalStore()->IsBusy())
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s is busy", this->m_WhisperName);
		return;
	}

	item_template const* item = sItemMgr->GetItem(ITEMGET(type, index));

	if (!item)
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Item");
		return;
	}

	if (!pOwner->GetInventory()->IsEmptySpace(item->GetX(), item->GetY()))
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s has full inventory", pOwner->GetName());
		return;
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Created item %s on %s", item->GetName(level), pOwner->GetName());
	pOwner->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s created item %s", this->m_Player->GetName(), item->GetName(level));

	Item item_new(ITEMGET(type, index), level);
	item_new.SetSocket(0, socket_1);
	item_new.SetSocket(1, socket_2);
	item_new.SetSocket(2, socket_3);
	item_new.SetSocket(3, socket_4);
	item_new.SetSocket(4, socket_5);
	item_new.SetSocketBonus(socket_bonus);

	sItemMgr->ItemSerialCreateItem(pOwner, serial_create_inventory, item_new);
}

void ChatHandler::CommandBanChar(const char * msg)
{
	std::stringstream conversor(msg);
	std::string name_safe;
	conversor >> name_safe;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), name_safe.c_str(), name_safe.size() > MAX_CHARACTER_LENGTH ? MAX_CHARACTER_LENGTH: name_safe.size());

	sDataServer->AdminCommandRequest(this->m_Player, 3, 0, name, nullptr, 0);
}

void ChatHandler::CommandBanAcc(const char * msg)
{
	
}

void ChatHandler::CommandZenAdd(const char * msg)
{
	std::stringstream conversor(msg);
	uint32 zen = 0;
	int32 type = 0;
	conversor >> zen >> type;

	limitmax(zen, sGameServer->GetMaxCharacterZen());
	
	Player* mTarget = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if ( !mTarget )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	if ( type == 0 )
	{
		mTarget->MoneyAdd(zen);
		mTarget->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Added %u zen",zen);
	}
	else
	{
		mTarget->IncreaseRuudMoney(zen);
		mTarget->SendRuudMoney(zen);
		mTarget->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Added %u ruud",zen);
	}
}

void ChatHandler::CommandZenSet(const char * msg)
{
	std::stringstream conversor(msg);
	uint32 zen = 0;
	int32 type = 0;
	conversor >> zen >> type;

	limitmax(zen, sGameServer->GetMaxCharacterZen());

	Player* mTarget = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if ( !mTarget )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	if ( type == 0 )
	{
		mTarget->MoneySet(zen, true);
		mTarget->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Set %u zen", zen);
	}
	else
	{
		mTarget->SetRuudMoney(zen);
		mTarget->SendRuudMoney();
		mTarget->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Set %u ruud",zen);
	}
}

void ChatHandler::CommandGuildTalk(const char * msg)
{
	std::stringstream conversor(msg);

	if ( strlen(msg) > MAX_GUILD_NAME_LENGTH )
		return;

	STRING_SAFE(guild, (MAX_GUILD_NAME_LENGTH + 1));
	conversor >> guild;

	Guild * pGuild = sGuildMgr->GetGuild(guild);

	if ( !pGuild )
	{
		this->m_Player->TalkingGuild.set(0);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s doesn't exist",guild);
		return;
	}

	this->m_Player->TalkingGuild.set(pGuild->GetID());
}
	
void ChatHandler::CommandAllianceTalk(const char * msg)
{
	std::stringstream conversor(msg);

	if ( strlen(msg) > MAX_GUILD_NAME_LENGTH )
		return;

	STRING_SAFE(guild, (MAX_GUILD_NAME_LENGTH + 1));
	conversor >> guild;

	Guild * pGuild = sGuildMgr->GetGuild(guild);

	if ( !pGuild )
	{
		this->m_Player->TalkingAlliance.set(0);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s doesn't exist",guild);
		return;
	}

	this->m_Player->TalkingAlliance.set(pGuild->GetID());
}

void ChatHandler::CommandFirework(const char * msg)
{
	this->m_Player->FireworksSend();
}

void ChatHandler::CommandMonsterAddTemp(const char * msg)
{
	std::stringstream conversor(msg);
	int32 monster = 0;
	int32 move_distance = 0;
	uint32 despawn_time = 0;
	int32 level = 0;
	int32 count = 0;
	conversor >> monster >> move_distance >> despawn_time >> level >> count;

	if ( count <= 0 )
	{
		count = 1;
	}

	for ( int32 i = 0; i < count; ++i )
	{
		Monster * mMonster = sObjectMgr->MonsterTryAdd(monster, this->m_Player->GetWorldId());

		if ( !mMonster )
			continue;
	
		mMonster->SetWorldId(this->m_Player->GetWorldId());
		mMonster->SetInstance(this->m_Player->GetInstance());
		mMonster->SetBasicLocation(this->m_Player->GetX(),this->m_Player->GetY(),this->m_Player->GetX(),this->m_Player->GetY());
		mMonster->SetDirection(this->m_Player->GetDirection());
		mMonster->SetDespawnTime(despawn_time * IN_MILLISECONDS);
		mMonster->SetDespawnTick(MyGetTickCount());
		mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
		mMonster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
		mMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
		mMonster->SetMoveDistance(move_distance);

		if ( level > 0 )
		{
			mMonster->SetLevel(level);
		}
	
		if ( !mMonster->GetWorld() )
		{
			mMonster->SetConnectStatus(CONNECT_STATUS_NONE);
			continue;
		}

		mMonster->AddToWorld();
	}
}

enum
{
	COMMAND_GM_BUFF_GROUP_GM,
	COMMAND_GM_BUFF_GROUP_JACK,
	COMMAND_GM_BUFF_GROUP_BLOSSOM,
	COMMAND_GM_BUFF_GROUP_SANTA,
	COMMAND_GM_BUFF_GROUP_SOLDIER,
	COMMAND_GM_BUFF_GROUP_MISC,
	COMMAND_GM_BUFF_GROUP_MAX
};

void ChatHandler::CommandGMBuff(const char * msg)
{
	std::stringstream conversor(msg);
	int32 id = 0;
	int32 duration = 0;
	int32 value = 0;
	conversor >> id >> duration >> value;

	duration *= MINUTE;
	
	if ( id >= COMMAND_GM_BUFF_GROUP_MAX )
		return;

	Object* pUnit = nullptr;

	VIEWPORT_LOOP_OBJECT(this->m_Player, pUnit)

		if ( !pUnit->IsPlayer() || !pUnit->IsPlaying() || !pUnit->ToUnit()->IsLive() )
			continue;

		this->CommandGMBuffApply(pUnit->ToUnit(), id, duration, value);
	}
}

void ChatHandler::CommandGMBuffAll(const char * msg)
{
	std::stringstream conversor(msg);
	int32 id = 0;
	int32 duration = 0;
	int32 value = 0;
	conversor >> id >> duration >> value;

	duration *= MINUTE;
	
	if ( id >= COMMAND_GM_BUFF_GROUP_MAX )
		return;

	Player* pPlayer = nullptr;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsLive() )
			continue;

		this->CommandGMBuffApply(pPlayer, id, duration, value);
	}
}

void ChatHandler::CommandGMBuffTo(const char * msg)
{
	std::stringstream conversor(msg);
	int32 id = 0;
	int32 duration = 0;
	int32 value = 0;
	conversor >> id >> duration >> value;

	duration *= MINUTE;

	if (id >= COMMAND_GM_BUFF_GROUP_MAX)
		return;

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if (!pPlayer)
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	if (!pPlayer->IsPlaying() || !pPlayer->IsLive())
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Player %s can't receive buff.", this->m_WhisperName);
		return;
	}

	this->CommandGMBuffApply(pPlayer, id, duration, value);
}

void ChatHandler::CommandGMBuffApply(Unit* pUnit, int32 id, int32 duration, int32 value)
{
	switch ( id )
	{
	case COMMAND_GM_BUFF_GROUP_GM:
		{
			pUnit->AddBuff(BUFF_GM_BUFF_STAT, 
				BuffEffect(BUFF_OPTION_INCREASE_MAXIMUM_LIFE, 300),
				BuffEffect(BUFF_OPTION_INCREASE_MAXIMUM_MANA, 300),
				duration, BUFF_FLAG_COMBAT | BUFF_FLAG_DB_STORE, this->m_Player, false);

			pUnit->AddBuff(BUFF_GM_BUFF_DEFENSE_ATTACK, 
				BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, 50),
				BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, 50),
				BuffEffect(BUFF_OPTION_INCREASE_DEFENSE_SUCCESS_RATE, 100),
				duration, BUFF_FLAG_COMBAT | BUFF_FLAG_DB_STORE, this->m_Player, false);

			pUnit->AddBuff(BUFF_GM_BUFF_EXPERIENCE,
				BuffEffect(BUFF_OPTION_INCREASE_EXPERIENCE, 5),
				duration, BUFF_FLAG_COMBAT | BUFF_FLAG_DB_STORE, this->m_Player, false);
		} break;

	case COMMAND_GM_BUFF_GROUP_JACK:
		{
			for ( uint8 h = 0; h < 3; h++ )
			{
				pUnit->AddItemBuff(ITEMGET(14, 46) + h, duration, BUFF_FLAG_DB_STORE, pUnit, false);
			}
		} break;

	case COMMAND_GM_BUFF_GROUP_BLOSSOM:
		{
			for ( uint8 h = 0; h < 3; h++ )
			{
				pUnit->AddItemBuff(ITEMGET(14, 85) + h, duration, BUFF_FLAG_DB_STORE, pUnit, false);
			}
		} break;

	case COMMAND_GM_BUFF_GROUP_SANTA:
		{
pUnit->AddBuff(BUFF_SANTA_BLESSING_CHRISTMAS, BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, 100), BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, 100), duration, 0, this->m_Player, false);
pUnit->AddBuff(BUFF_SANTA_HEALING, BuffEffect(BUFF_OPTION_INCREASE_MAXIMUM_LIFE, 500), duration, 0, this->m_Player, false);
pUnit->AddBuff(BUFF_SANTA_PROTECTION, BuffEffect(BUFF_OPTION_INCREASE_MAXIMUM_MANA, 500), duration, 0, this->m_Player, false);
pUnit->AddBuff(BUFF_SANTA_STRENGTHENER, BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, 30), duration, 0, this->m_Player, false);
pUnit->AddBuff(BUFF_SANTA_DEFENSE, BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, 100), duration, 0, this->m_Player, false);
pUnit->AddBuff(BUFF_SANTA_QUICKNESS, BuffEffect(BUFF_OPTION_INCREASE_ATTACK_SPEED, 15), duration, 0, this->m_Player, false);
pUnit->AddBuff(BUFF_SANTA_FORTUNE, BuffEffect(BUFF_OPTION_INCREASE_AG_RECOVERY_ADD, 10), duration, 0, this->m_Player, false);
		} break;

	case COMMAND_GM_BUFF_GROUP_SOLDIER:
		{
			pUnit->AddBuff(BUFF_ELF_SOLDIER, 
				BuffEffect(BUFF_OPTION_INCREASE_ATTACK_DAMAGE, pUnit->ToPlayer()->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() / 3 + 45),
				BuffEffect(BUFF_OPTION_INCREASE_DEFENSE, pUnit->ToPlayer()->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() / 5 + 50), 
				60 * ((pUnit->ToPlayer()->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() / 6) + 30), 0, this->m_Player, false);
		} break;

	case COMMAND_GM_BUFF_GROUP_MISC:
		{
pUnit->AddBuff(duration / MINUTE, value, 0, this->m_Player);
		} break;
	}
}

void ChatHandler::CommandRestriction(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 restriction = 0;
	int32 duration = 0;

	conversor >> safe_name >> restriction >> duration;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());

	if ( restriction < 0 || restriction >= PlayerAction::PLAYER_ACTION_MAX )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Restriction ID.");
		return;
	}

	if ( duration <= 0 )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong Restriction Time.");
		return;
	}
	
	sDataServer->AdminCommandRequest(this->m_Player, 1, restriction, name, nullptr, duration);
}

void ChatHandler::CommandOnline(const char * msg)
{
	std::string safe_name;
	std::stringstream conversor(msg);
	conversor >> safe_name;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());

	StringToLower(name);

	Player* pPlayer = nullptr;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		STRING_SAFE_COPY(name_tmp, (MAX_CHARACTER_LENGTH + 1), pPlayer->GetName(), (MAX_CHARACTER_LENGTH + 1));
		StringToLower(name_tmp);

		if ( strlen(name) <= 0 || !memcmp(name_tmp, name, strlen(name)) )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Player %s is in %s at %d / %d",
				pPlayer->GetName(), pPlayer->GetWorldName(), pPlayer->GetX(), pPlayer->GetY());
		}
	}
}

void ChatHandler::CommandShutdown(const char * msg)
{
	std::stringstream conversor(msg);
	int32 time = 0;
	conversor >> time;

	if ( time < 0 )
		return;

	sGameServer->SetShutdownTime(time);
	sGameServer->SetShutdown(!sGameServer->IsShutdown());

	if ( !sGameServer->IsShutdown() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Server Shutdown aborted.");
	}
	else
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Server Shutdown started. Time : %d", time);
	}
}

void ChatHandler::CommandAction(const char * msg)
{
	std::stringstream conversor(msg);
	int32 id = 0;
	int32 monster = 0;
	conversor >> id >> monster;

	if ( id < 0 )
		return;

	Object* pUnit = nullptr;

	VIEWPORT_LOOP_OBJECT(this->m_Player, pUnit)

		if ( !pUnit->IsUnit() || !pUnit->IsPlaying() )
			continue;

		if ( pUnit->IsPlayer() )
		{
			if ( pUnit->ToPlayer()->IsBusy() )
				continue;
		}
		else if ( monster != 1 )
		{
			continue;
		}
		
		pUnit->ToUnit()->ActionSend(ENTRY(pUnit), id, pUnit->GetDirection(), true);

	VIEWPORT_CLOSE
}

void ChatHandler::CommandMonsterAlter(const char * msg)
{
	std::stringstream conversor(msg);
	int32 id = 0;
	int32 data = 0;
	conversor >> id >> data;

	if ( id != 0 && !this->m_Player->GetCurrentTarget() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You don't have a target.");
		return;
	}

	switch ( id )
	{
	case 0:
		{
			Unit* pUnit = sObjectMgr->FindMonster(data);

			if ( !pUnit )
			{
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Invalid Monster ID.");
			}
			else
			{
				this->m_Player->SetCurrentTarget(pUnit->ToCreature());
			}
		} break;

	case 1:
		{
			this->m_Player->GetCurrentTarget()->SetLastUpdate(0);
		} break;

	case 2:
		{
			this->m_Player->GetCurrentTarget()->SetLastUpdate(0);
			this->m_Player->GetCurrentTarget()->Kill();
		} break;

	case 3:
		{
			if ( !this->m_Player->GetCurrentTarget()->SetTemplateInfo() )
			{
				this->m_Player->GetCurrentTarget()->Remove();
			}
			else
			{
				this->m_Player->GetCurrentTarget()->SetDBData();
			}
		} break;
	}
}

void ChatHandler::CommandMiscSiege(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 count = 0;

	conversor >> safe_name >> count;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());
	
	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s.", name);
		return;
	}

	if ( count < 0 || count > 255 )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Invalid Count.");
		return;
	}

	pPlayer->SetKillCount(count);
	pPlayer->KillCountSend();
}

void ChatHandler::CommandMiscKill(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;

	conversor >> safe_name;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());
	
	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s.", name);
		return;
	}

	pPlayer->Kill();
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s killed.", name);
}

void ChatHandler::CommandChangeGuild(const char * msg)
{
	std::stringstream conversor(msg);
	uint32 guild = 0;

	conversor >> guild;

	sDataServer->GuildDeleteRequest(guild);
}
	
void ChatHandler::CommandChangeAlliance(const char * msg)
{
	std::stringstream conversor(msg);
	uint32 guild01 = 0;
	uint32 guild02 = 0;
	int32 add = 0;

	conversor >> guild01 >> guild02 >> add;

	sDataServer->GuildRelationshipRequest(guild01, guild02, GUILD_RELATIONSHIP_UNION, add == 0 ? false : true);

	//35200 40598 1
}
	
void ChatHandler::CommandChangeHostil(const char * msg)
{

}
	
void ChatHandler::CommandChangeCSOwner(const char * msg)
{
	std::stringstream conversor(msg);
	uint32 guild = 0;
	int32 occupied = 0;

	conversor >> guild >> occupied;

	sCastleSiege->SetCastleOwner(guild);
	sCastleSiege->SetOccupied(guild == 0 ? false: occupied == 1);
	sCastleSiege->SaveDBCastleOwner();
}

void ChatHandler::CommandSkillAdd(const char * msg)
{
	std::stringstream conversor(msg);
	uint16 skill_id = 0;

	conversor >> skill_id;

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	int32 result = pPlayer->MagicAdd(skill_id, 0);

	if ( result != -1 )
	{
		pPlayer->SendSkill(result, pPlayer->GetSkill(result)->GetSkill(), pPlayer->GetSkill(result)->GetLevel());
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Added Skill %d", skill_id);
	}
}
	
void ChatHandler::CommandSkillRemove(const char * msg)
{
	std::stringstream conversor(msg);
	uint16 skill_id = 0;

	conversor >> skill_id;

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(this->m_WhisperName);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", this->m_WhisperName);
		return;
	}

	int32 result = pPlayer->MagicRemove(skill_id);

	if ( result != -1 )
	{
		pPlayer->SendSkill(result, skill_id, 0, false);
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Removed Skill %d", skill_id);
	}
}

void ChatHandler::CommandCastleSiegeRegister(const char * msg)
{
	std::stringstream conversor(msg);
	std::string guild_name;

	conversor >> guild_name;

	if ( guild_name.empty() || guild_name.size() > MAX_GUILD_NAME_LENGTH )
		return;

	Guild * pGuild = sGuildMgr->GetGuild(guild_name);

	if ( !pGuild )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s doesn't exist.", guild_name.c_str());
		return;
	}

	if ( !pGuild->IsAllianceMaster() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s is not in alliance or is not alliance master.", guild_name.c_str());
		return;
	}

	if ( pGuild->IsRegisteredInCaslteSiege() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s is already registered in siege.", guild_name.c_str());
		return;
	}

	DS_CASTLE_SIEGE_INSERT_REGISTERED_GUILD pMsg;
	pMsg.guild = pGuild->GetID();
	sDataServer->SendPacket(MAKE_PCT(pMsg));
	
	sDataServer->CastleSiegeRegisterGuild(pGuild->GetID(), true, 0);
	pGuild->SetRegisteredInCaslteSiege(true);
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Guild %s registered successfully.", guild_name.c_str());
}
	
void ChatHandler::CommandCastleSiegeUnRegister(const char * msg)
{
	std::stringstream conversor(msg);
	std::string guild_name;

	conversor >> guild_name;

	if ( guild_name.empty() || guild_name.size() > MAX_GUILD_NAME_LENGTH )
		return;

	Guild * pGuild = sGuildMgr->GetGuild(guild_name);

	if ( !pGuild )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s doesn't exist.", guild_name.c_str());
		return;
	}

	if ( !pGuild->IsRegisteredInCaslteSiege() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s is not registered in siege.", guild_name.c_str());
		return;
	}

	DS_CASTLE_SIEGE_DELETE_REGISTERED_GUILD pMsg;
	pMsg.guild = pGuild->GetID();
	sDataServer->SendPacket(MAKE_PCT(pMsg));

	sDataServer->CastleSiegeRegisterGuild(pGuild->GetID(), false, 0);
	pGuild->SetRegisteredInCaslteSiege(false);
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Guild %s unregistered successfully.", guild_name.c_str());
}
	
void ChatHandler::CommandCastleSiegeIncreaseMarks(const char * msg)
{
	std::stringstream conversor(msg);
	std::string guild_name;
	int32 count = 0;

	conversor >> guild_name >> count;

	if ( guild_name.empty() || guild_name.size() > MAX_GUILD_NAME_LENGTH )
		return;

	if ( count <= 0 )
		count = 1;

	Guild * pGuild = sGuildMgr->GetGuild(guild_name);

	if ( !pGuild )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s doesn't exist.", guild_name.c_str());
		return;
	}

	if ( !pGuild->IsRegisteredInCaslteSiege() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s is not registered in siege.", guild_name.c_str());
		return;
	}

	pGuild->SetCastleSiegeMarks(pGuild->GetCastleSiegeMarks() + count);

	DS_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD pMsg(pGuild->GetID(), pGuild->GetCastleSiegeMarks());
	sDataServer->SendPacket(MAKE_PCT(pMsg));

	sDataServer->CastleSiegeRegisterGuild(pGuild->GetID(), true, pGuild->GetCastleSiegeMarks());
	
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Guild %s registered mark successfully. Count %u.", guild_name.c_str(), pGuild->GetCastleSiegeMarks());
}
	
void ChatHandler::CommandCastleSiegeReduceMarks(const char * msg)
{
	std::stringstream conversor(msg);
	std::string guild_name;
	int32 count = 0;

	conversor >> guild_name >> count;

	if ( guild_name.empty() || guild_name.size() > MAX_GUILD_NAME_LENGTH )
		return;

	if ( count <= 0 )
		count = 1;

	Guild * pGuild = sGuildMgr->GetGuild(guild_name);

	if ( !pGuild )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s doesn't exist.", guild_name.c_str());
		return;
	}

	if ( !pGuild->IsRegisteredInCaslteSiege() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s is not registered in siege.", guild_name.c_str());
		return;
	}

	if ( pGuild->GetCastleSiegeMarks() < uint32(count) )
	{
		pGuild->SetCastleSiegeMarks(0);
	}
	else
	{
		pGuild->SetCastleSiegeMarks(pGuild->GetCastleSiegeMarks() - count);
	}

	DS_CASTLE_SIEGE_UPDATE_REGISTERED_GUILD pMsg(pGuild->GetID(), pGuild->GetCastleSiegeMarks());
	sDataServer->SendPacket(MAKE_PCT(pMsg));

	sDataServer->CastleSiegeRegisterGuild(pGuild->GetID(), true, pGuild->GetCastleSiegeMarks());
	
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Guild %s unregistered mark successfully. Count %u.", guild_name.c_str(), pGuild->GetCastleSiegeMarks());
}

void ChatHandler::CommandCastleSiegeNPCLoad(const char * msg)
{
	sDataServer->CastleSiegeLoadNpcRequest();
}

void ChatHandler::CommandCastleSiegeFix(const char * msg)
{
	sCastleSiege->CalculateCurrentState();
}

void ChatHandler::CommandCastleSiegeFixGate(const char * msg)
{
	std::stringstream conversor(msg);
	int32 id = 0;

	conversor >> id;

	CastleSiegeNpcData * pNpcData = sCastleSiege->GetNpcData(277, id);

	if ( !pNpcData )
		return;

	World* pWorld = sWorldMgr->GetWorld(WORLD_CASTLE_SIEGE);

	if ( !pWorld )
	{
		return;
	}

	int16 data[4] = { pNpcData->x.get() - 2, pNpcData->y.get(), pNpcData->x.get() + 3, pNpcData->y.get() + 1 };

	pWorld->ApplyAttribute(data, 16, false);

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Gate %d unlocked", id);
}

void ChatHandler::CommandArcaWarRegisterMaster(const char * msg)
{
	std::stringstream conversor(msg);
	std::string guild_name;

	conversor >> guild_name;

	if ( guild_name.empty() || guild_name.size() > MAX_GUILD_NAME_LENGTH )
		return;

	Guild * pGuild = sGuildMgr->GetGuild(guild_name);

	if ( !pGuild )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s doesn't exist.", guild_name.c_str());
		return;
	}

	DS_ARKA_WAR_MASTER_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.player.guid = pGuild->GetID();
	memcpy(pMsg.player.name, pGuild->GetMember(0)->GetName(), MAX_CHARACTER_LENGTH);
	pMsg.guild = pGuild->GetID();

	sDataServer->SendPacket(MAKE_PCT(pMsg));

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Guild %s registered.", pGuild->GetName());
}
	
void ChatHandler::CommandArcaWarRegisterMember(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;

	conversor >> safe_name;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	DS_ARKA_WAR_MEMBER_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.guild = pPlayer->GetGuildID();

	sDataServer->SendPacket(MAKE_PCT(pMsg));

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Player %s registered.", pPlayer->GetName());
}

void ChatHandler::CommandArcaWarIncreaseMarks(const char * msg)
{
	std::stringstream conversor(msg);
	std::string guild_name;

	conversor >> guild_name;

	if ( guild_name.empty() || guild_name.size() > MAX_GUILD_NAME_LENGTH )
		return;

	Guild * pGuild = sGuildMgr->GetGuild(guild_name);

	if ( !pGuild )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild %s doesn't exist.", guild_name.c_str());
		return;
	}

	DS_ARKA_WAR_SIGN_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	this->m_Player->BuildCustomPacketData(pMsg.player);
	pMsg.guild = pGuild->GetID();

	sDataServer->SendPacket(MAKE_PCT(pMsg));

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Guild %s sign registered.", pGuild->GetName());
}

void ChatHandler::CommandArcaWarReload(const char * msg)
{
	std::stringstream conversor(msg);
	int32 command = 0;

	conversor >> command;

	if ( command == 0 )
	{
		sDataServer->ArkaWarGuildRequest();
	}
	else
	{
		sDataServer->ArkaWarGuildSave();
	}
}

void ChatHandler::CommandAdminStatAdd(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 stat_id = 0;
	int32 points = 0;

	conversor >> safe_name >> stat_id >> points;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	this->CommandAdminStatModify(pPlayer, stat_id, points, 0);
}
	
void ChatHandler::CommandAdminStatSet(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 stat_id = 0;
	int32 points = 0;

	conversor >> safe_name >> stat_id >> points;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	this->CommandAdminStatModify(pPlayer, stat_id, points, 1);
}
	
void ChatHandler::CommandAdminStatRemove(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 stat_id = 0;
	int32 points = 0;

	conversor >> safe_name >> stat_id >> points;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	this->CommandAdminStatModify(pPlayer, stat_id, points, 2);
}

void ChatHandler::CommandAdminStatModify(Player* pPlayer, int32 stat_id, int32 points, uint8 type)
{
	CharacterBaseData* pCharacterData = sCharacterBase->GetCharacterBase(pPlayer->GetClass());

	if ( !pCharacterData )
		return;

	switch ( stat_id )
	{
	case STRENGTH:
	case AGILITY:
	case VITALITY:
	case ENERGY:
	case LEADERSHIP:
		{
			if ( type == 0 ) // Add
			{
				points = pPlayer->GetStat(stat_id) + points;
			}
			else if ( type == 2 ) // Remove
			{
				points = pPlayer->GetStat(stat_id) - points;
			}

			if ( points < pCharacterData->GetStat(stat_id) )
				points = pCharacterData->GetStat(stat_id);

			if ( points > sGameServer->GetMaxStat(stat_id) )
				points = sGameServer->GetMaxStat(stat_id);

			pPlayer->SetStat(stat_id, points);
			pPlayer->SendStats();
			pPlayer->CalculateCharacter();
		} break;

	case 5:
		{
			if ( type == 0 ) // Add
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints() + points;
			}
			else if ( type == 2 ) // Remove
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetPoints() - points;
			}

			if ( points < 0 )
				points = 0;

			pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetPoints(points);
			pPlayer->SendStats();
		} break;

	case 6:
		{
			if ( type == 0 ) // Add
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetPoints() + points;
			}
			else if ( type == 2 ) // Remove
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetPoints() - points;
			}

			if ( points < 0 )
				points = 0;

			pPlayer->GetLevelData(LEVEL_DATA_MASTER)->SetPoints(points);
			pPlayer->MasterSendStatus();
		} break;

	case 7:
		{
			if ( type == 0 ) // Add
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() + points;
			}
			else if ( type == 2 ) // Remove
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->GetLevel() - points;
			}

			if ( points < 1 )
				points = 1;

			if ( points > sGameServer->GetMaxLevel(LEVEL_DATA_NORMAL) )
				points = sGameServer->GetMaxLevel(LEVEL_DATA_NORMAL);

			pPlayer->GetLevelData(LEVEL_DATA_NORMAL)->SetLevel(points);
			pPlayer->Close(6, CLOSE_FLAG_SELECT_CHAR);
		} break;

	case 8:
		{
			if ( type == 0 ) // Add
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() + points;
			}
			else if ( type == 2 ) // Remove
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_MASTER)->GetLevel() - points;
			}

			if ( points < 0 )
				points = 0;

			if ( points > sGameServer->GetMaxLevel(LEVEL_DATA_MASTER) )
				points = sGameServer->GetMaxLevel(LEVEL_DATA_MASTER);

			pPlayer->GetLevelData(LEVEL_DATA_MASTER)->SetLevel(points);
			pPlayer->Close(6, CLOSE_FLAG_SELECT_CHAR);
		} break;

	case 9:
		{
			if ( type == 0 ) // Add
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints() + points;
			}
			else if ( type == 2 ) // Remove
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->GetPoints() - points;
			}

			if ( points < 0 )
				points = 0;

			pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->SetPoints(points);
			pPlayer->MajesticSkillTreeSendPoints();
		} break;

	case 10:
		{
			if ( type == 0 ) // Add
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel() + points;
			}
			else if ( type == 2 ) // Remove
			{
				points = pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->GetLevel() - points;
			}

			if ( points < 0 )
				points = 0;

			if ( points > sGameServer->GetMaxLevel(LEVEL_DATA_MAJESTIC) )
				points = sGameServer->GetMaxLevel(LEVEL_DATA_MAJESTIC);

			pPlayer->GetLevelData(LEVEL_DATA_MAJESTIC)->SetLevel(points);
			pPlayer->MasterSendStatus();
		} break;
	}
}

void ChatHandler::CommandAdminChallenge(const char * msg)
{
	/*std::stringstream conversor(msg);
	int32 size = 4;

	conversor >> size;

	if ( size <= 0 )
		size = 1;

	if ( this->m_Player->IsInChallenge() )
	{
		this->m_Player->SetSize(1.0f);
		this->m_Player->SendCustomObjectData();
		this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_ATTACK);
		this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED);
		this->m_Player->SetInChallenge(false);
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Challenge Mode Disabled.");
	}
	else
	{
		if ( this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE) )
		{
			this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_ATTACK);
			this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED);
			this->m_Player->SetSize(size);
			this->m_Player->SendCustomObjectData();
			this->m_Player->SetInChallenge(true);
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Challenge Mode Enabled.");
		}
	}*/
}

void ChatHandler::CommandAdminSetPK(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 pk_level = 0;
	int32 pk_count = 0;
	int32 pk_points = 0;

	conversor >> safe_name >> pk_level;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, (MAX_CHARACTER_LENGTH + 1), safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	if ( pk_level < 0 )
		pk_level = PK_STATUS_COMMONER;

	if ( pk_level >= PK_STATUS_MAX )
		pk_level = PK_STATUS_MURDER_LVL_3;

	switch ( pk_level )
	{
	case PK_STATUS_HERO:
		{
			pk_count = sGameServer->GetPlayerHeroCount();
			pk_points = 43200 + (3600 * pk_count);
		} break;

	case PK_STATUS_HERO_LVL_2:
		{
			pk_count = 2;
			pk_points = 43200;
		} break;

	case PK_STATUS_HERO_LVL_1:
		{
			pk_count = 1;
			pk_points = 21600;
		} break;

	case PK_STATUS_COMMONER:
		{
			pk_count = 0;
			pk_points = 0;
		} break;

	case PK_STATUS_MURDER_LVL_1:
		{
			pk_count = 1;
			pk_points = 21600;
		} break;

	case PK_STATUS_MURDER_LVL_2:
		{
			pk_count = 2;
			pk_points = 43200;
		} break;

	case PK_STATUS_MURDER_LVL_3:
		{
			pk_count = 3;
			pk_points = 86400;
		} break;
	}

	pPlayer->SetPKCount(pk_count);
	pPlayer->SetPKPoints(pk_points);
	pPlayer->SetPKLevel(pk_level);
	pPlayer->PKLevelSend();

	pPlayer->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Your pk status has been changed.");
}

void ChatHandler::CommandAdminCrywolf(const char * msg)
{
	std::stringstream conversor(msg);
	int32 state = CRYWOLF_OCCUPATION_SUCCESS;

	conversor >> state;

	sCrywolf->SetOccupationState(state);
	sDataServer->CrywolfDataSave();
}

void ChatHandler::CommandAdminSummon(const char * msg)
{
	std::stringstream conversor(msg);
	int32 monster = 0;
	int32 move_distance = 0;
	uint32 despawn_time = 0;
	int32 type = 0;
	conversor >> monster >> move_distance >> despawn_time >> type;

	Monster * mMonster = sObjectMgr->MonsterTryAdd(monster, this->m_Player->GetWorldId());

	if ( !mMonster )
		return;
	
	mMonster->SetWorldId(this->m_Player->GetWorldId());
	mMonster->SetInstance(this->m_Player->GetInstance());
	mMonster->SetBasicLocation(this->m_Player->GetX(),this->m_Player->GetY(),this->m_Player->GetX(),this->m_Player->GetY());
	mMonster->SetDirection(this->m_Player->GetDirection());
	mMonster->SetDespawnTime(despawn_time * IN_MILLISECONDS);
	mMonster->SetDespawnTick(MyGetTickCount());
	mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
	mMonster->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
	mMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
	mMonster->SetMoveDistance(move_distance);
	mMonster->SetSummoner(this->m_Player);
	mMonster->SetSummonType(type);

	if ( !mMonster->GetWorld() )
	{
		mMonster->SetConnectStatus(CONNECT_STATUS_NONE);
		return;
	}

	mMonster->AddToWorld();
}

void ChatHandler::CommandAdminSeason(const char * msg)
{
	std::stringstream conversor(msg);
	int32 season = -1;

	conversor >> season;

	EventSeasonData * pEventSeasonData = sEventMgr->GetSeasonData(season);

	if ( !pEventSeasonData )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "No season data for id: %d.", season);
		return;
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Season [%d][%s] is %s.", season, pEventSeasonData->GetName().c_str(), pEventSeasonData->IsStarted() ? "active": "not active");
}

void ChatHandler::CommandAdminResetDuel(const char * msg)
{
	std::stringstream conversor(msg);
	int32 room = -1;

	conversor >> room;

	if ( room < -1 )
		room = -1;

	if ( room >= MAX_DUEL_ROOM )
		room = MAX_DUEL_ROOM - 1;

	if ( room == -1 )
	{
		for ( uint8 i = 0; i < MAX_DUEL_ROOM; ++i )
		{
			DuelRoom* pRoom = sDuelMgr->GetRoom(i);

			if ( !pRoom )
				continue;

			if ( pRoom->GetStatus() == DUEL_ROOM_STARTED )
				pRoom->FinishedOutOfTime();
			else
				pRoom->SetState_Empty();
		}
	}
	else
	{
		DuelRoom* pRoom = sDuelMgr->GetRoom(room);

		if ( !pRoom )
			return;

		if ( pRoom->GetStatus() == DUEL_ROOM_STARTED )
			pRoom->FinishedOutOfTime();
		else
			pRoom->SetState_Empty();
	}
}

void ChatHandler::CommandAdminTransfer(const char * msg)
{
	/*if ( !sGameServer->IsTransferEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Server transfer is disabled.");
		return;
	}

	std::stringstream conversor(msg);
	int32 server_id = -1;

	conversor >> server_id;

	if ( server_id < 0 )
		return;

	if ( server_id == sGameServer->GetServerCode() )
		return;

	if ( (server_id / MAX_SERVER_PER_GROUP) != sGameServer->GetServerGroup() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Server %d does not belong to same group.", server_id);
		return;
	}

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GAMESERVER_SELECT);
	stmt->setUInt16(0, server_id);

	PreparedQueryResult result = LoginDatabase.Query(stmt);

	if ( !result )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Server %d does not exist in database.", server_id);
		return;
	}

	Field* fields = result->Fetch();

	if ( !fields[5].GetBool() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Server %d is offline.", server_id);
		return;
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Starting Transfer.");
	
	int32 count = 0;

	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
			continue;

		if ( !pPlayer->IsPlaying() || !pPlayer->IsLive() || pPlayer->IsServerQuit() )
			continue;

		if ( pPlayer->GetInterfaceState()->GetID() != InterfaceData::None )
			continue;

		if ( this->m_Player == pPlayer )
			continue;
		
		sAuthServer->PlayerServerMoveRequest(pPlayer, server_id, pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY());

		++count;
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Ending Transfer. Transfered %d players", count);*/
}

void ChatHandler::CommandAdminAccountKick(const char * msg)
{
	std::stringstream conversor(msg);
	int32 account_id = -1;

	conversor >> account_id;

	if ( account_id <= 0 )
		return;

	sAuthServer->PlayerAccountKick(uint32(account_id));
}

void ChatHandler::CommandAdminIP(const char * msg)
{

}
	
void ChatHandler::CommandAdminKickIPMAC(const char * msg)
{

}

void ChatHandler::CommandAdminPersonalStore(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 operation = 0;

	conversor >> safe_name >> operation;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
		return;

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	if ( !pPlayer->GetPersonalStore()->IsOpen() ) ///- Este es el check del posible bug
		operation = 1;

	if ( operation == 0 ) ///- Esto cierra el personal store de forma segura
	{
		pPlayer->PersonalStoreClose(false);

		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Closed %s personal store", name);
	}
	else ///- Esto cierra el personal store
	{
		pPlayer->GetPersonalStore()->Clear();

		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Cleared %s personal store", name);
	}
}

void ChatHandler::CommandAdminPostReset(const char * msg)
{
	sGameServer->SetPostManageMACDay(0);

	Player* pPlayer = nullptr;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it )
	{
		pPlayer = it->second;

		if ( !pPlayer )
			continue;

		pPlayer->SetPostDay(0);
		pPlayer->SetPostMonth(0);
	}
}

void ChatHandler::CommandAdminGlobalMute(const char * msg)
{
	sGameServer->SetGlobalMute(!sGameServer->IsGlobalMute());

	if ( sGameServer->IsGlobalMute() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Global Mute enabled.");
	}
	else
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Global Mute disabled.");
	}
}

void ChatHandler::CommandAdminMuteMAC(const char * msg)
{
	std::stringstream conversor(msg);
	std::string data;
	int32 operation = 0;

	conversor >> data >> operation;

	sGameServer->MACMute(data, operation, this->m_Player);
}

void ChatHandler::CommandAdminMACRestrict(const char * msg)
{
	std::stringstream conversor(msg);
	std::string data;
	int32 operation = 0;
	int32 restriction = 0;
	int32 time = 0;

	conversor >> data >> operation >> restriction >> time;

	sGameServer->ApplyMACRestriction(data, operation, restriction, time, this->m_Player);
}

void ChatHandler::CommandAdminItemInfo(const char * msg)
{
	std::stringstream conversor(msg);
	int32 type = 0;
	int32 data = 0;

	conversor >> type >> data;

	if ( data < 0 )
	{
		data = 0;
	}

	Item const* pItem = nullptr;

	if ( type == 0 ) // Slot
	{
		if ( !inventory_range(data) )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong slot %d.", data);
			return;
		}

		pItem = this->m_Player->GetInventory()->GetItem(data);

		if ( !pItem->IsItem() )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "No item in slot %d.", data);
			return;
		}

		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Item %s +%d +%d", 
			pItem->GetName(), pItem->GetLevel(), pItem->GetOption());

		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "DmgMin:%d / DmgMax:%d / Defense:%d", 
			pItem->GetMinDamage(), pItem->GetMaxDamage(), pItem->GetDefense());

		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "MagicPwr:%d / CursePwr:%d / PetPwr:%d", 
			pItem->GetMagicPower(), pItem->GetCursePower(), pItem->GetPetPower());

	}
	else if ( type == 1 ) // Id
	{

	}
}

void ChatHandler::AdminCommandTrace(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;

	conversor >> safe_name;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	this->m_Player->TeleportToLocation(pPlayer->GetWorldId(), pPlayer->GetX(), pPlayer->GetY(), pPlayer->GetDirection(), pPlayer->GetInstance());
}
	
void ChatHandler::AdminCommandTrans(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;

	conversor >> safe_name;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	pPlayer->TeleportToLocation(this->m_Player->GetWorldId(), this->m_Player->GetX(), this->m_Player->GetY(), this->m_Player->GetDirection(), this->m_Player->GetInstance());
}
	
void ChatHandler::AdminCommandGate(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 gate = 0;

	conversor >> safe_name >> gate;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	if ( pPlayer->MoveToGate(gate) )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Player %s moved to gate %d", pPlayer->GetName(), gate);
	}
	else
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Failed to move Player %s to gate %d", pPlayer->GetName(), gate);
	}
}
	
void ChatHandler::AdminCommandMove(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 world = 0;
	int32 x = 0;
	int32 y = 0;

	conversor >> safe_name >> world >> x >> y;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s.", name);
		return;
	}

	World* pWorld = sWorldMgr->GetWorld(world);

	if ( !pWorld )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong world id %d.", world);
		return;
	}

	WorldGrid const& grid = pWorld->GetGrid(x, y);

	if ( grid.IsLocked_1() || grid.IsLocked_2() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong teleport location.");
		return;
	}

	pPlayer->TeleportToLocation(world, x, y, pPlayer->GetDirection(), pPlayer->GetInstance());
}
	
void ChatHandler::AdminCommandFollow(const char * msg)
{
	std::stringstream conversor(msg);
	int32 target = 0;

	conversor >> target;

	Monster* pMonster = sObjectMgr->FindMonster(target);

	if ( !pMonster )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Wrong monster id %d", target);
		return;
	}

	if ( !pMonster->IsPlaying() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Monster %d is not playing", target);
		return;
	}

	this->m_Player->TeleportToLocation(pMonster->GetWorldId(), pMonster->GetX(), pMonster->GetY(), pMonster->GetDirection(), pMonster->GetInstance());
}
	
void ChatHandler::AdminCommandUpdateStat(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;

	conversor >> safe_name;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	POWER_LOOP(i)
	{
		pPlayer->PowerSet(i, pPlayer->PowerGetTotal(i), true);
	}
}

void ChatHandler::CommandTestFunc(const char* msg) {
	/*std::stringstream conversor(msg);
	uint8 score = 1;

	conversor >> score;

	sEvomon->SendScore(this->m_Player, 1, score);*/

}

void ChatHandler::CommandAdminNotify(const char* msg) {
	if (!this->m_Player || !this->m_Player->IsAdministrator())
		return;

	sDataServer->NoticeSend(NOTICE_GLOBAL, msg);
}

void ChatHandler::CommandKickAccount(const char* msg) {
	std::stringstream conversor(msg);
	uint32 accID = 0;

	conversor >> accID;

	sObjectMgr->KickCharactersByAccount(accID);

	this->m_Player->SendNotice(NOTICE_NORMAL_BLUE, "Kick Account ID: %d", accID);
}

void ChatHandler::CommandAdminDeleteItemServer(const char* msg) {
	std::stringstream conversor(msg);
	uint32 itemId = 0;

	conversor >> itemId;

	uint32 count = 0;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();

	for (PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Player* pPlayer = it->second;

		if (!pPlayer)
			continue;

		full_inventory_loop(i) {
			if (!pPlayer->GetInventory()->GetItem(i)->IsItem())
				continue;

			if (pPlayer->GetInventory()->GetItem(i)->GetItem() == itemId) {
				pPlayer->GetInventory()->DeleteItem(i);
				count++;
			}
		}

		for (uint8 i = 0; i < max_shop_item; i++) {
			if (!pPlayer->GetPersonalStore()->GetItem(i)->IsItem())
				continue;

			if (pPlayer->GetPersonalStore()->GetItem(i)->GetItem() == itemId) {
				pPlayer->GetPersonalStore()->DeleteItem(i);
				count++;
			}
		}

		EVENT_INVENTORY_LOOP(i) {
			if (!pPlayer->GetEventInventory()->GetItem(i)->IsItem())
				continue;

			if (pPlayer->GetEventInventory()->GetItem(i)->GetItem() == itemId) {
				pPlayer->GetEventInventory()->DeleteItem(i);
				count++;
			}
		}

		warehouse_loop(i) {
			if (!pPlayer->GetWarehouse()->GetItem(i)->IsItem())
				continue;

			if (pPlayer->GetWarehouse()->GetItem(i)->GetItem() == itemId) {
				pPlayer->GetWarehouse()->DeleteItem(i);
				count++;
			}
		}

		pPlayer->SendInventory();
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Delete Item Count: %d", count);
}

void ChatHandler::CommandAdminDeleteItemErrorPersonal(const char* msg) {
	uint32 count = 0;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();

	for (PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Player* pPlayer = it->second;

		if (!pPlayer)
			continue;

		for (uint8 i = 0; i < PERSONAL_STORE_SELL_MAX; i++) {
			auto item = pPlayer->GetPersonalStore()->GetItem(i);

			if (!item || !item->GetItem())
				continue;

			if (item->GetPersonalStorePrice() == 1 && item->GetPersonalStorePriceJoB() == 0 && item->GetPersonalStorePriceJoS() == 0) {
				for (uint8 j = 1; j < PERSONAL_STORE_BUNDLE_MAX; j++) {
					uint16 slot = (PERSONAL_STORE_SELL_MAX + i * PERSONAL_STORE_BUNDLE_MAX) + j;
					auto itembundle = pPlayer->GetPersonalStore()->GetItem(slot);

					if (!itembundle || !itembundle->GetItem())
						continue;

					pPlayer->GetPersonalStore()->DeleteItem(slot);
					count++;
				}

				pPlayer->GetPersonalStore()->DeleteItem(i);
				count++;
			}
		}
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Delete Item Count: %d", count);
}

void ChatHandler::CommandAdminDeleteItemServerSerial(const char* msg) {
	std::stringstream conversor(msg);
	uint32 itemSerial = 0;

	conversor >> itemSerial;

	uint32 count = 0;

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();

	for (PlayerSessionMap::const_iterator it = characters.begin(); it != characters.end(); ++it)
	{
		Player* pPlayer = it->second;

		if (!pPlayer)
			continue;

		full_inventory_loop(i) {
			if (!pPlayer->GetInventory()->GetItem(i)->IsItem())
				continue;

			if (pPlayer->GetInventory()->GetItem(i)->GetSerial() == itemSerial) {
				pPlayer->GetInventory()->DeleteItem(i);
				count++;
			}
		}

		for (uint8 i = 0; i < max_shop_item; i++) {
			if (!pPlayer->GetPersonalStore()->GetItem(i)->IsItem())
				continue;

			if (pPlayer->GetPersonalStore()->GetItem(i)->GetSerial() == itemSerial) {
				pPlayer->GetPersonalStore()->DeleteItem(i);
				count++;
			}
		}

		EVENT_INVENTORY_LOOP(i) {
			if (!pPlayer->GetEventInventory()->GetItem(i)->IsItem())
				continue;

			if (pPlayer->GetEventInventory()->GetItem(i)->GetSerial() == itemSerial) {
				pPlayer->GetEventInventory()->DeleteItem(i);
				count++;
			}
		}

		warehouse_loop(i) {
			if (!pPlayer->GetWarehouse()->GetItem(i)->IsItem())
				continue;

			if (pPlayer->GetWarehouse()->GetItem(i)->GetSerial() == itemSerial) {
				pPlayer->GetWarehouse()->DeleteItem(i);
				count++;
			}
		}

		pPlayer->SendInventory();
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Delete Item Count: %d", count);
}

void ChatHandler::CommandClearInventory(const char* msg) 
{
	if (!sGameServer->IsCommandClearInventory() && !this->m_Player->IsAdministrator()) {
		return;
	}

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	use_inventory_loop(i) {
		if (this->m_Player->GetInventory()->GetItem(i)) {
			this->m_Player->GetInventory()->DeleteItem(i);
		}
	}

	this->m_Player->SendInventory();

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Clear Inventory Succes!");
}

void ChatHandler::CommandAddExpandedInventoryWareHouse(const char* msg) {
	if (!sGameServer->IsCommandAddExpanded())
		return;

	if (!this->m_Player)
		return;

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if (this->m_Player->GetWarehouse()->GetExpanded() == 0) {
		this->m_Player->GetWarehouse()->SetExpanded(1);
	}

	if (this->m_Player->GetInventory()->GetExpanded() >= sGameServer->GetMaxExpandedInventory()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Expanded inventory is max");
		return;
	}

	for (int i = 0; i < 2; i++) {
		if (this->m_Player->GetInventory()->GetExpanded() < sGameServer->GetMaxExpandedInventory()) {
			this->m_Player->GetInventory()->IncreaseExpanded(1);
		}
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Add Expanded Succes");

	this->m_Player->Close(6, CLOSE_FLAG_SELECT_CHAR);
}

void ChatHandler::CommandClearGremoryCase(const char* msg) 
{
	if (!sGameServer->IsCommandClearGremoryCase()) {
		return;
	}

	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	for (int i = 0; i < 50; i++) {
		sGremoryCase->ItemDel(this->m_Player, GREMORY_CASE_TYPE_ACCOUNT, i);
		this->m_Player->GetGremoryCase(GREMORY_CASE_TYPE_ACCOUNT)->DeleteItem(i);
		sGremoryCase->ItemDel(this->m_Player, GREMORY_CASE_TYPE_CHARACTER, i);
		this->m_Player->GetGremoryCase(GREMORY_CASE_TYPE_CHARACTER)->DeleteItem(i);
		sGremoryCase->ItemDel(this->m_Player, GREMORY_CASE_TYPE_MOBILE, i);
		this->m_Player->GetGremoryCase(GREMORY_CASE_TYPE_MOBILE)->DeleteItem(i);
	}

	for (int i = 0; i < 150; i++) {
		sGremoryCase->ItemDel(this->m_Player, GREMORY_CASE_TYPE_PERSONAL_STORE, i);
		this->m_Player->GetGremoryCase(GREMORY_CASE_TYPE_PERSONAL_STORE)->DeleteItem(i);
	}

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Clear Gremory Case Succes!");
}

void ChatHandler::AdminCommandAcitve(const char * msg)
{
	std::stringstream conversor(msg);
	int32 status = 0;

	conversor >> status;

	sGameServer->SetActive(status == 0 ? false: true);

	if ( sGameServer->IsActive() != sGameServer->IsActiveStatus() )
	{
		sGameServer->SetActiveStatus(sGameServer->IsActive());
		sGameServer->SetInactiveTick(MyGetTickCount());
		sGameServer->SetInactiveNotificationTick(MyGetTickCount());

		if ( this->m_Player )
		{
			if ( sGameServer->IsActiveStatus() )
			{
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Server Activated.");
			}
			else
			{
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Server Deactivated.");
			}
		}
	}
}

void ChatHandler::CommandAdminArcaWar(const char * msg)
{
	std::stringstream conversor(msg);
	std::string command;
	std::string data1;
	std::string data2;

	conversor >> command >> data1 >> data2;

	if ( command == "reg" )
	{
		if ( data1.size() > MAX_GUILD_NAME_LENGTH )
		{
			return;
		}

		STRING_SAFE_COPY(name, MAX_GUILD_NAME_LENGTH + 1, data1.c_str(), data1.size());

		Guild* pGuild = sGuildMgr->GetGuild(name);

		if ( !pGuild )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find Guild %s", name);
			return;
		}

		sDataServer->ArkaWarMasterRegister(this->m_Player, pGuild->GetID());
	}
	else if ( command == "regchar" )
	{

	}
	else if ( command == "sign" )
	{

	}
}

void ChatHandler::CommandAdminInvi(const char * msg)
{
	if (!this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE))
	{
		this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE);

		if (!this->m_Player->HasBuff(BUFF_INVISIBILITY))
		{
			this->m_Player->AddBuff(BUFF_INVISIBILITY, -10, BUFF_FLAG_CONSTANT, this->m_Player);
			this->m_Player->ViewportDeleteSend();
		}
	}
	else
	{
		this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE);

		if (this->m_Player->HasBuff(BUFF_INVISIBILITY))
		{
			this->m_Player->RemoveBuff(BUFF_INVISIBILITY);
			this->m_Player->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
		}
	}

	sDataServer->CharacterOnOff(this->m_Player, 2);

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Invisibility Status: %s", this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE) ? "ON" : "OFF");

	if (!this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_ATTACK))
	{
		this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_ATTACK);
	}
	else
	{
		this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_ATTACK);
	}

	sDataServer->CharacterOnOff(this->m_Player, 2);

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Attack Status: %s", this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_ATTACK) ? "ON" : "OFF");

	if (!this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED))
	{
		this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED);
	}
	else
	{
		this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED);
	}

	sDataServer->CharacterOnOff(this->m_Player, 2);

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Been Attacked Status: %s", this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED) ? "ON" : "OFF");

	if (!this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_WHISPER))
	{
		this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_WHISPER);
	}
	else
	{
		this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_WHISPER);
	}

	sDataServer->CharacterOnOff(this->m_Player, 2);

	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Whisper Status: %s", this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_WHISPER) ? "ON" : "OFF");

	/*std::stringstream conversor(msg);
	int32 type = 0;

	conversor >> type;

	switch ( type )
	{
	case 0:
		{
			if ( !this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE) )
			{
				this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE);

				if ( !this->m_Player->HasBuff(BUFF_INVISIBILITY) )
				{
					this->m_Player->AddBuff(BUFF_INVISIBILITY, -10, BUFF_FLAG_CONSTANT, this->m_Player);
					this->m_Player->ViewportDeleteSend();
				}
			}
			else
			{
				this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE);

				if ( this->m_Player->HasBuff(BUFF_INVISIBILITY) )
				{
					this->m_Player->RemoveBuff(BUFF_INVISIBILITY);
					this->m_Player->ViewportCreate(VIEWPORT_CREATE_FLAG_ME | VIEWPORT_CREATE_FLAG_GEN | VIEWPORT_CREATE_FLAG_GUILD | VIEWPORT_CREATE_FLAG_SIEGE);
				}
			}

			sDataServer->CharacterOnOff(this->m_Player, 2);

			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Invisibility Status: %s", this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_FLAG_VISIBLE) ? "ON" : "OFF");
		} break;

	case 1:
		{
			if ( !this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_ATTACK) )
			{
				this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_ATTACK);
			}
			else
			{
				this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_ATTACK);
			}

			sDataServer->CharacterOnOff(this->m_Player, 2);

			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Attack Status: %s", this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_ATTACK) ? "ON" : "OFF");
		} break;

	case 2:
		{
			if ( !this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED) )
			{
				this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED);
			}
			else
			{
				this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED);
			}

			sDataServer->CharacterOnOff(this->m_Player, 2);

			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Been Attacked Status: %s", this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_BEEN_ATTACKED) ? "ON" : "OFF");
		} break;

	case 3:
		{
			if ( !this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_WHISPER) )
			{
				this->m_Player->AddAdminPanelFlag(ADMIN_PANEL_WHISPER);
			}
			else
			{
				this->m_Player->RemoveAdminPanelFlag(ADMIN_PANEL_WHISPER);
			}

			sDataServer->CharacterOnOff(this->m_Player, 2);

			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Whisper Status: %s", this->m_Player->IsAdminPanelFlag(ADMIN_PANEL_WHISPER) ? "ON" : "OFF");
		} break;
	}
	*/
}

void ChatHandler::CommandAdminTrack(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;

	conversor >> safe_name;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	sGameServer->ProcessTrack(pPlayer);
}

void ChatHandler::CommandAdminChangeQuestState(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 id = 0;
	int32 state = 0;

	conversor >> safe_name >> id >> state;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	if ( state <= QUEST_EVOLUTION_STATE_NONE )
	{
		state = QUEST_EVOLUTION_STATE_CLEAR;
	}

	if ( state >= MAX_QUEST_EVOLUTION_STATE )
	{
		state = QUEST_EVOLUTION_STATE_CLEAR;
	}

	pPlayer->QuestEvolutionSetState(id, state);
}
	
void ChatHandler::CommandAdminChangeQuestKillCount(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;
	int32 id = 0;
	int32 slot = 0;
	int32 count = 0;

	conversor >> safe_name >> id >> slot >> count;

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	QuestEvolutionStatus * pQuestStatus = pPlayer->QuestEvolutionFind(id);

	if ( !pQuestStatus )
	{
		return;
	}

	if ( slot >= QUEST_EVOLUTION_MONSTER_COUNT )
	{
		return;
	}

	pQuestStatus->SetMonsterCount(slot, count);
}

void ChatHandler::CommandAdminSpotCreate(const char * msg)
{
	std::stringstream conversor(msg);
	RandomValue<int32> m_RandomValue(-1);
	int32 count = 0;

	while ( conversor )
	{
		int32 value = 0;
		conversor >> value;

		if ( value == -1 )
		{
			break;
		}

		m_RandomValue.AddValue(value, 0);
	}

	conversor >> count;

	if ( count <= 0 )
	{
		return;
	}

	for ( int32 i = 0; i < count; ++i )
	{
		int32 id = m_RandomValue.GetRandomValue(RANDOM_POOL_RANDOM);

		if ( id == -1 )
		{
			continue;
		}

		sLog->outInfo("crash", "%u	GUID	%d	-1	NN	%u	%d	%d	%d	%d	-1	0	3	0	0	0	15	NN	NN	NN	0	0", sGameServer->GetServerCode(), id,
			this->m_Player->GetWorldId(), this->m_Player->GetX(), this->m_Player->GetY(), this->m_Player->GetX(), this->m_Player->GetY());
	}
}

void ChatHandler::CommandAdminAttrChange(const char * msg)
{
	std::stringstream conversor(msg);
	int32 x = 0;
	int32 y = 0;
	int32 attr = 0;
	int32 apply = 0;
	conversor >> x >> y >> attr >> apply;

	if (x < 0 || x > 255)
	{
		return;
	}

	if (y < 0 || y > 255)
	{
		return;
	}

	World* pWorld = sWorldMgr->GetWorld(this->m_Player->GetWorldId());

	if (!pWorld)
	{
		return;
	}

	pWorld->ApplyAttribute(x, y, attr, apply >= 1 ? true : false);
}

void ChatHandler::CommandAdminTrackCharacter(const char * msg)
{
	std::stringstream conversor(msg);
	std::string safe_name;

	conversor >> safe_name;

	this->m_Player->SetTrackingID(0);

	if ( safe_name.size() > MAX_CHARACTER_LENGTH )
	{
		return;
	}

	if ( safe_name.empty() )
	{
		return;
	}

	STRING_SAFE_COPY(name, MAX_CHARACTER_LENGTH + 1, safe_name.c_str(), safe_name.size());

	Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(name);

	if ( !pPlayer )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s", name);
		return;
	}

	this->m_Player->SetTrackingID(pPlayer->GetGUID());
	
	this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Tracking %s", pPlayer->GetName());
}

void ChatHandler::ReloadShop(const char * msg)
{
	sShopMgr->LoadShopTemplate("../Data/Shops/ShopList.xml");
	sShopMgr->LoadShopScripts();
	sItemMgr->LoadItemPriceRuud("../Data/Shops/RuudPrice.xml");
	sMossMerchant->Load("../Data/Shops/MossMerchantSection.xml");

	if ( this->m_Player )
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Shops reloaded.");
}

void ChatHandler::ReloadQuest(const char * msg)
{
	sQuestMgr->LoadQuestEvolution("../Data/Quest/QuestEvolutionTemplate.xml");
	sQuestMgr->LoadQuestEvolutionData("../Data/Quest/QuestEvolutionData.xml");
	sQuestMgr->LoadQuestEvolutionCondition("../Data/Quest/QuestEvolutionCondition.xml");
	sQuestMgr->LoadQuestEvolutionItemReward("../Data/Quest/QuestEvolutionItemReward.xml");

	sQuestMgr->LoadQuestGuided("../Data/Quest/GuideQuest.xml");
	sQuestMgr->LoadQuestMU("../Data/Quest/MuQuest.xml");

	if ( this->m_Player )
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Quests reloaded.");
}

void ChatHandler::ReloadEvent(const char * msg)
{
	std::stringstream conversor(msg);
	int32 id = 0;
	int32 sub_id = 0;
	conversor >> id >> sub_id;

	switch ( id )
	{
	case -1:
		{
			sEventMgr->LoadEventData("../Data/Event/EventManager.xml");
			sEventMgr->LoadEventSeasonData("../Data/Event/EventSeasonManager.xml");
			sEventMgr->LoadEventContributionReward("../Data/Event/EventContributionReward.xml");
			sEventMgr->LoadEventLevel("../Data/Event/EventLevel.xml");
			sEventMgr->LoadEventEnterCount("../Data/Event/EventEnterCount.xml");

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Event Manager reloaded.");
		} break;

	case EVENT_BLOOD_CASTLE:
		{
			sBloodCastleMgr->LoadBloodCastIni();
			sBloodCastleMgr->LoadBloodCastleSetting();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Blood Castle reloaded.");
		} break;

	case EVENT_DEVIL_SQUARE:
		{
			sDevilSquareMgr->LoadDevilSquareIni();
			sDevilSquareMgr->LoadDevilSquareSetting();
			sDevilSquareMgr->LoadDevilSquareRanking("../Data/Event/DevilSquare/DevilSquareRanking.xml");

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Devil Square reloaded.");
		} break;

	case EVENT_CHAOS_CASTLE:
		{
			sChaosCastleMgr->LoadIni();
			sChaosCastleMgr->LoadChaosCastleSetting();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Chaos Castle reloaded.");
		} break;

	case EVENT_ILLUSION_TEMPLE:
		{
			sIllusionTempleMgr->LoadIllusionTempleSettings();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Illusion Temple reloaded.");
		} break;

	case EVENT_CRYWOLF:
		{
			sCrywolf->LoadIni();
			sCrywolf->LoadData();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Crywolf reloaded.");
		} break;

	case EVENT_IMPERIAL_FORTRESS:
		{
			sImperialFortressMgr->LoadIni();
			sImperialFortressMgr->LoadImperialFortressLevel("../Data/Event/ImperialFortress/EventImperialFortressLevel.xml");
			sImperialFortressMgr->LoadImperialFortressMonsterSettings("../Data/Event/ImperialFortress/EventImperialFortressMonsterSettings.xml");
			sImperialFortressMgr->LoadImperialFortressTraps("../Data/Event/ImperialFortress/EventImperialFortressTrap.xml");

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Imperial Fortress reloaded.");
		} break;

	case EVENT_RAKLION:
		{
			sRaklion->LoadIni();
			sRaklion->LoadData();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Raklion reloaded.");
		} break;

	case EVENT_KANTURU:
		{
			sKanturuMgr->LoadIni();
			sKanturuMgr->Load();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Kanturu reloaded.");
		} break;

	case EVENT_INVASION:
		{
			sInvasionMgr->LoadInvasionData("../Data/Event/EventInvasion.xml");
			sInvasionMgr->LoadInvasionGroup("../Data/Event/EventInvasionGroup.xml");

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Invasion reloaded.");
		} break;

	case EVENT_CASTLE_SIEGE:
		{
			sCastleSiege->LoadIni();
			sCastleSiege->LoadData();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Castle Siege reloaded.");
		} break;

	case EVENT_DOPPELGANGER:
		{
			sDoppelganger->LoadIni();
			sDoppelganger->LoadLevel("../Data/Event/Doppelganger/DoppelgangerLevel.xml");
			sDoppelganger->LoadMonster("../Data/Event/Doppelganger/DoppelgangerMonsterSetting.xml");

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Doppelganger reloaded.");
		} break;

	case EVENT_SCRAMBLE:
		{
			sScramble->LoadIni();
			sScramble->LoadWordList("../Data/Event/Scramble/ScrambleWord.xml");

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Scramble reloaded.");
		} break;

	case EVENT_DUNGEON_RACE:
		{
			sDungeonRace->LoadData();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Dungeon Race reloaded.");
		} break;

	case EVENT_LOSTTOWER_RACE:
		{
			sLosttowerRace->LoadData();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Losttower Race reloaded.");
		} break;

	case EVENT_CHAOS_CASTLE_SURVIVAL:
		{
			sChaosCastleSurvivalMgr->LoadChaosCastleSetting();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Chaos Castle Survival reloaded.");
		} break;

	case EVENT_PROTECTOR_OF_ACHERON:
		{
			sProtectorOfAcheron->LoadIni();
			sProtectorOfAcheron->Load();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Protector of Acheron reloaded.");
		} break;

	case EVENT_TORMENTED_SQUARE:
		{
			sTormentedSquare->LoadIni();
			sTormentedSquare->Load("../Data/Event/TormentedSquare/EventTormentedSquareStage.xml");
			sTormentedSquare->LoadRanking("../Data/Event/TormentedSquare/EventTormentedSquareRanking.xml");

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Tormented Square reloaded.");
		} break;

	case EVENT_ARKA_WAR:
		{
			sArkaWar->LoadIni();
			sArkaWar->Load();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Arca War reloaded.");
		} break;

	case EVENT_LAST_MAN_STANDING:
		{
		} break;

	case EVENT_LABYRINTH_OF_DIMENSIONS:
		{
			sLabyrinthDimensions->LoadIni();
			sLabyrinthDimensions->LoadInfo("../Data/Event/Labyrinth/EventLabyrinthInfo.xml");
			sLabyrinthDimensions->LoadMission("../Data/Event/Labyrinth/EventLabyrinthMission.xml");
			sLabyrinthDimensions->LoadArea("../Data/Event/Labyrinth/EventLabyrinthArea.xml");
			sLabyrinthDimensions->LoadAreaData("../Data/Event/Labyrinth/EventLabyrinthAreaData.xml");
			sLabyrinthDimensions->LoadLevel("../Data/Event/Labyrinth/EventLabyrinthLevel.xml");
			sLabyrinthDimensions->LoadMonster("../Data/Event/Labyrinth/EventLabyrinthMonster.xml");
			sLabyrinthDimensions->LoadReward("../Data/Event/Labyrinth/EventLabyrinthReward.xml");

			if (sub_id == 1)
			{
				sLabyrinthDimensions->LoadRanking();
				sLabyrinthDimensions->LoadSchedule();
			}

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Labyrinth of dimensions reloaded.");
		} break;

	case EVENT_TORMENTED_SQUARE_SURVIVAL:
		{
			sTormentedSquareSurvival->Load();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Tormented Square survival reloaded.");
		} break;

	case EVENT_CASTLE_DEEP:
		{
			sCastleDeep->LoadIni();
			sCastleDeep->Load();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Castle Deep reloaded.");
		} break;

	case EVENT_INSTANCED_DUNGEON:
		{
			sDungeon->LoadIni();
			sDungeon->LoadInstance("../Data/Event/Dungeon/EventDungeon.xml");
			sDungeon->LoadLevel("../Data/Event/Dungeon/EventDungeonLevel.xml");

			if (sub_id == 1)
			{
				sDungeon->LoadSavedInstance();
			}

			if (this->m_Player)
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Instanced Dungeon reloaded.");
		} break;
	}
}

void ChatHandler::ReloadMonster(const char * msg)
{

	std::stringstream conversor(msg);
	int32 guid = 0;
	conversor >> guid;


	sMonsterManager->LoadMonsterTemplate("../Data/Monster/MonsterList.xml");
	sMonsterManager->LoadMonsterSkill("../Data/Monster/Skill/MonsterSkill.xml");
	sMonsterManager->LoadMonsterSkillSpecial("../Data/Monster/Skill/MonsterSkillSpecial.xml");
	sMonsterManager->LoadMonsterAIElement("../Data/Monster/AI/MonsterAiElement.xml");
	sMonsterManager->LoadMonsterAIAutomata("../Data/Monster/AI/MonsterAiAutomata.xml");
	sMonsterManager->LoadMonsterAIUnit("../Data/Monster/AI/MonsterAiUnit.xml");
	sMonsterManager->LoadMonsterAIGroup("../Data/Monster/AI/MonsterAiData.xml");
	sMonsterManager->LoadMonsterRespawnLocation("../Data/Monster/MonsterRespawnLocation.xml");
	sMonsterManager->LoadMonsterEquipment("../Data/Monster/MonsterEquipment.xml");
	sMonsterManager->SetLastUpdate(time(nullptr));

	if (this->m_Player)
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monsters data reloaded.");

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();
		

		if (!pMonster)
			continue;

		if (pMonster->GetConnectStatus() == CONNECT_STATUS_NONE)
			continue;

		pMonster->Remove();
	}

	sObjectMgr->SetRespawnMonsterTime(MyGetTickCount() + (5 * IN_MILLISECONDS));
	sObjectMgr->SetRespawnMonster(true);

	if (this->m_Player)
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monsters reloaded.");

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if (!pMonster)
		{
			continue;
		}

		if (pMonster->GetConnectStatus() == CONNECT_STATUS_NONE)
		{
			continue;
		}

		if (pMonster->GetEntry() < guid)
		{
			continue;
		}

		pMonster->Remove();
	}

	//sMonsterManager->LoadNpcHandleList("../Data/Monster/NPCHandle.xml");
	sMonsterManager->LoadMonster("../Data/Monster/MonsterSpawn.xml");

	if (this->m_Player)
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monsters reloaded.");

	sMonsterManager->LoadMonsterEvent("../Data/Monster/MonsterSpawnEvent.xml");

	if (this->m_Player)
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monsters event reloaded.");

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if (!pMonster)
		{
			continue;
		}

		if (pMonster->GetConnectStatus() != CONNECT_STATUS_PLAYING)
		{
			continue;
		}

		if (pMonster->GetWorldId() != guid)
		{
			continue;
		}

		pMonster->SetLastUpdate(0);
	}

	HASH_MONSTER(i)
	{
		Monster* pMonster = ObjectContainer[i]->ToCreature();

		if (!pMonster)
		{
			continue;
		}

		if (pMonster->GetConnectStatus() != CONNECT_STATUS_PLAYING)
		{
			continue;
		}

		if (pMonster->GetWorldId() != guid)
		{
			continue;
		}

		pMonster->SetLastUpdate(0);
		pMonster->Kill();
	}


	/*std::stringstream conversor(msg);
	int32 id = 0;
	int32 guid = 0;
	conversor >> id >> guid;

	switch ( id )
	{
	case 0:
		{
			sMonsterManager->LoadMonsterTemplate();
			sMonsterManager->LoadMonsterSkill();
			sMonsterManager->LoadMonsterSkillSpecial();
			sMonsterManager->LoadMonsterAIElement();
			sMonsterManager->LoadMonsterAIAutomata();
			sMonsterManager->LoadMonsterAIUnit();
			sMonsterManager->LoadMonsterAIGroup();
			sMonsterManager->LoadMonsterRespawnLocation();
			sMonsterManager->LoadMonsterEquipment();
			sMonsterManager->SetLastUpdate(time(nullptr));

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monsters data reloaded.");
		} break;

	case 1:
		{
			HASH_MONSTER(i)
			{
				Monster* pMonster = ObjectContainer[i]->ToCreature();

				if ( !pMonster )
					continue;

				if ( pMonster->GetConnectStatus() == CONNECT_STATUS_NONE )
					continue;

				pMonster->Remove();
			}

			sObjectMgr->SetRespawnMonsterTime(MyGetTickCount() + (5 * IN_MILLISECONDS));
			sObjectMgr->SetRespawnMonster(true);

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monsters reloaded.");
		} break;

	case 2:
		{
			HASH_MONSTER(i)
			{
				Monster* pMonster = ObjectContainer[i]->ToCreature();

				if ( !pMonster )
				{
					continue;
				}

				if ( pMonster->GetConnectStatus() == CONNECT_STATUS_NONE )
				{
					continue;
				}

				if ( pMonster->GetEntry() < guid )
				{
					continue;
				}

				pMonster->Remove();
			}

			sMonsterManager->LoadMonster(guid);

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monsters reloaded.");
		} break;

	case 3:
		{
			sMonsterManager->LoadMonsterEvent();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monsters event reloaded.");
		} break;

	case 5:
		{
			HASH_MONSTER(i)
			{
				Monster* pMonster = ObjectContainer[i]->ToCreature();

				if ( !pMonster )
				{
					continue;
				}

				if ( pMonster->GetConnectStatus() != CONNECT_STATUS_PLAYING )
				{
					continue;
				}

				if ( pMonster->GetWorldId() != guid )
				{
					continue;
				}

				pMonster->SetLastUpdate(0);
			}
		} break;

	case 7:
		{
			HASH_MONSTER(i)
			{
				Monster* pMonster = ObjectContainer[i]->ToCreature();

				if ( !pMonster )
				{
					continue;
				}

				if ( pMonster->GetConnectStatus() != CONNECT_STATUS_PLAYING )
				{
					continue;
				}

				if ( pMonster->GetWorldId() != guid )
				{
					continue;
				}

				pMonster->SetLastUpdate(0);
				pMonster->Kill();
			}
		} break;
	}*/
}

void ChatHandler::ReloadConfig(const char* msg)
{
	sGameServer->LoadCommonSettings();
	sGameServer->LoadNotice("../Data/Notice.xml");
	sGameServer->LoadFilter("../Data/FilterText.xml");
	sGenMgr->LoadIni();
	sGenMgr->LoadGenReward("../Data/Gens/GenReward.xml");
	sGenMgr->LoadGenRanking("../Data/Gens/GenRanking.xml");
	sGenMgr->LoadGenKillPoints("../Data/Gens/GenKillPoint.xml");
	sMiniMap->Load("../Data/World/MiniMap.xml");
	sMuunSystem->LoadIni();
	sMuunSystem->Load("../Data/Muun/MuunList.xml");
	sMuunSystem->LoadOption("../Data/Muun/MuunOption.xml");
	sMuunSystem->LoadEnergy("../Data/Muun/MuunEnergy.xml");
	sMuunSystem->LoadExchange("../Data/Muun/MuunExchange.xml");
	sSummonScroll->Load("../Data/Item/SummonScroll.xml");
	sMiningSystem->Load("../Data/Mining.xml");
	sEvomon->LoadIni();
	sEvomon->Load("../Data/Event/Evomon/Evomon.xml");
	sEvomon->LoadReward("../Data/Event/Evomon/EvomonReward.xml");
	sGameServer->LoadNotification("../Data/Notifications.xml");
	sGameServer->SendNotification();
	sGameServer->LoadBaseData("../Data/ServerMap.xml");
	sGameServer->LoadOffsetData("../Data/OffsetData.xml");
	sGameServer->SendOffsetData();
	sGameServer->LoadOffsetFPS("../Data/OffsetFPS.xml");
	sGameServer->SendOffsetFPS();
	sGameServer->LoadCheatScanWhiteList("../Data/CheatScanWhitelist.xml");
	sGameServer->LoadOfflineAttackSetting("../Data/Plugin/OfflineAttack/OfflineAttack.xml");
	sGameServer->LoadOfflineAttackWorld("../Data/Plugin/OfflineAttack/OfflineAttackWorld.xml");
	sGameServer->LoadOfflineAttackZone("../Data/Plugin/OfflineAttack/OfflineAttackZone.xml");
	sEventInventory->LoadIni();
	sMiniBomb->LoadReward("../Data/Event/EventInventory/EventMiniBombReward.xml");
	sMuRoomy->LoadReward("../Data/Event/EventInventory/EventMuRoomyReward.xml");
	sJewelBingo->LoadReward("../Data/Event/EventInventory/EventJewelBingoReward.xml");
	sGameServer->LoadGoblinPoint("../Data/GoblinPoints.xml");
	sNumericBaseball->LoadReward("../Data/Event/EventInventory/EventNumericBaseballReward.xml");
	sFormulaMgr->Load("../Data/FormulaData.xml");
	sResetSystemMgr->LoadResetData("../Data/ResetSystem.xml");
	sDynamicExperienceMgr->LoadDynamicExperienceData("../Data/ExpSystem/ExpSystem.xml");
	sNoticeSystem->Load("../Data/Plugin/NoticeSystem.xml");
	sMain->LoadAccountTestGS("../Data/AccountTest.xml");

	if (this->m_Player)
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "All configs reloaded.");
	/* std::stringstream conversor(msg);
	int32 id = 0;
	conversor >> id;

	switch ( id )
	{
	case 0:
		{
			sGameServer->LoadCommonSettings();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Configs reloaded.");
		} break;

	case 3:
		{
			sGameServer->LoadNotice();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Notice reloaded.");
		} break;

	case 4:
		{
			sGameServer->LoadFilter();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Filter reloaded.");
		} break;

	case 5:
		{
			sGenMgr->LoadGenReward();
			sGenMgr->LoadGenRanking();
			sGenMgr->LoadGenKillPoints();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Gens reloaded.");
		} break;

	case 6:
		{
			sMiniMap->Load();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "MiniMap reloaded.");
		} break;

	case 7:
		{
			sMuunSystem->Load();
			sMuunSystem->LoadOption();
			sMuunSystem->LoadEnergy();
			sMuunSystem->LoadExchange();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Muun reloaded.");
		} break;

	case 8:
		{
			sSummonScroll->Load();
			sSummonScroll->LoadMonster();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Summon Scroll reloaded.");
		} break;

	case 9:
		{
			sMiningSystem->Load();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Mining reloaded.");
		} break;

	case 10:
		{
			sEvomon->Load();
			sEvomon->LoadReward();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Evomon reloaded.");
		} break;

	case 11:
		{
			sGameServer->LoadNotification();
			sGameServer->SendNotification();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Notifications reloaded.");
		} break;

	case 12:
		{
			sGameServer->LoadBaseData();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Base data reloaded.");
		} break;

	case 14:
		{
			sGameServer->LoadOffsetData();
			sGameServer->SendOffsetData();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Offset Data reloaded.");
		} break;

	case 15:
		{
			sGameServer->LoadOffsetFPS();
			sGameServer->SendOffsetFPS();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Offset FPS reloaded.");
		} break;

	case 16:
		{
			sGameServer->LoadCheatScanWhiteList();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Cheat Scan White List reloaded.");
		} break;

	case 18:
		{
			sGameServer->LoadOfflineAttackWorld();
			sGameServer->LoadOfflineAttackZone();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Offline Attack World/Zone reloaded.");
		} break;

	case 19:
		{
			sMiniBomb->LoadReward();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Mini Bomb Reward reloaded.");
		} break;

	case 20:
		{
			sMuRoomy->LoadReward();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Mu Roomy Reward reloaded.");
		} break;

	case 21:
		{
			sJewelBingo->LoadReward();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Jewel Bingo Reward reloaded.");
		} break;

	case 22:
		{
			sMossMerchant->Load();

			if ( this->m_Player )
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Moss Merchant reloaded.");
		} break;

	case 23:
		{
			sGameServer->LoadGoblinPoint();

			if (this->m_Player)
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Goblin Point reloaded.");
		} break;

	case 24:
		{
			sNumericBaseball->LoadReward();

			if (this->m_Player)
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Numeric Baseball Reward reloaded.");
		} break;

	case 25:
		{
			sFormulaMgr->Load();

			if (this->m_Player)
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Formula Data reloaded.");
		} break;

	case 26:
	{
			   sMonsterSoul->LoadConverter();
			   sMonsterSoul->LoadTransformation();
			   sMonsterSoul->LoadReward();

			   if (this->m_Player)
				   this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Monster Soul reloaded.");
	} break;
	} */
}

void ChatHandler::ReloadItem(const char * msg)
{
	sItemMgr->LoadItemTemplate("../Data/Item/ItemList.xml");
	sItemMgr->LoadItemAddOption("../Data/Item/ItemAddOption.xml");
	sItemMgr->LoadItemSetType("../Data/Item/ItemSetType.xml");
	sItemMgr->LoadItemSetOption("../Data/Item/ItemSetOption.xml");
	sItemMgr->LoadItemHarmony("../Data/Item/ItemHarmony.xml");
	sItemMgr->LoadItemHarmonyEnhancement("../Data/Item/ItemHarmonyEnhance.xml");
	sItemMgr->LoadItemSocket("../Data/Item/Socket/ItemSocket.xml");
	sItemMgr->LoadItemSocketSeed("../Data/Item/Socket/ItemSocketSeed.xml");
	sItemMgr->LoadItemSocketBonus("../Data/Item/Socket/ItemSocketBonus.xml");
	sItemMgr->LoadItemSocketPackage("../Data/Item/Socket/ItemSocketPackage.xml");
	sItemMgr->LoadItemSocketMix("../Data/Item/Socket/ItemSocketMix.xml");
	sItemMgr->LoadItemSocketUpgrade("../Data/Item/Socket/ItemSocketUpgrade.xml");
	sItemMgr->LoadItemSocketSeedSphere("../Data/Item/Socket/ItemSocketSeedSphere.xml");
	sItemMgr->LoadItemTransformation("../Data/Item/ItemTransformation.xml");
	sItemMgr->LoadItemDrop("../Data/Item/Drop/ItemDrop.xml");
	sItemMgr->LoadItemLevelTemplate("../Data/Item/ItemLevel.xml");
	sItemMgr->LoadItemMixLucky("../Data/Mix/MixLuckyTemplate.xml");
	sItemMgr->LoadItemPriceData("../Data/Item/ItemPrice.xml");
	sItemMgr->LoadItemStack("../Data/Item/ItemStack.xml");
	sItemMgr->LoadItemQuest("../Data/Item/ItemQuest.xml");
	sItemMgr->LoadItemWingValues("../Data/Item/Wings/ItemWingValues.xml");
	sItemMgr->LoadItemReducedDrop("../Data/Item/Drop/ItemReducedDrop.xml");
	sItemMgr->LoadItemCreateData("../Data/Item/ItemCreateData.xml");
	sItemMgr->LoadItemFlagDB("../Data/Item/ItemFlagDB.xml");
	sItemMgr->LoadItemWingGradedOption("../Data/Item/Wings/ItemWingGradedOption.xml");
	sItemMgr->LoadItemWingSocketOption("../Data/Item/Wings/ItemWingSocketOption.xml");
	sItemMgr->LoadItemWingSocketUpgrade("../Data/Item/Wings/ItemWingSocketIpgrade.xml");
	sItemMgr->LoadEarring("../Data/Item/Earring/ItemEarring.xml");
	sItemMgr->LoadEarringBonus("../Data/Item/Earring/ItemEarringBonus.xml");
	sItemMgr->LoadEarringUpgrade("../Data/Item/Earring/ItemEarringUpgrade.xml");
	//sItemMgr->LoadEarringOption("../Data/Item/Earring/ItemEarringOption.xml");
	sItemMgr->LoadMasteryBonus("../Data/Item/ItemMasteryBonus.xml");
	sItemMgr->LoadMysteriousStone("../Data/Item/ItemMysteriousStone.xml");
	sItemMgr->LoadGuardianOption("../Data/Item/Guardian/GuardianOption.xml");
	sItemMgr->SetLastItemUpdate(time(nullptr));
	sMonsterManager->LoadMonsterItems();

	sItemBagMgr->LoadItemBagTemplate("../Data/Item/ItemBagList.xml");

	sShopMgr->LoadShopTemplate("../Data/Shops/ShopList.xml");
	sShopMgr->LoadShopScripts();
	sItemMgr->LoadItemPriceRuud("../Data/Shops/RuudPrice.xml");

	sMonsterSoul->LoadConverter("../Data/Item/MonsterSoul/MonsterSoulConverter.xml");
	sMonsterSoul->LoadTransformation("../Data/Item/MonsterSoul/MonsterSoulTranformation.xml");
	sMonsterSoul->LoadReward("../Data/Item/MonsterSoul/MonsterSoulReward.xml");

	if ( this->m_Player )
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Items reloaded.");
}

void ChatHandler::ReloadCharacter(const char * msg)
{
	sCharacterBase->LoadCharacterBase("../Data/Character/CharacterBase.xml");
	sCharacterBase->LoadCharacterBaseSkill("../Data/Character/CharacterSkill.xml");
	sCharacterBase->LoadCharacterExperienceNormal("../Data/ExpSystem/NormalExp.xml");
	sCharacterBase->LoadCharacterExperienceMaster("../Data/ExpSystem/MasterExp.xml");
	sCharacterBase->LoadCharacterExperienceMajestic("../Data/ExpSystem/MajesticExp.xml");
	sCharacterBase->LoadCharacterHelper("../Data/Character/CharacterHelper.xml");
	sCharacterBase->LoadCharacterFormula("../Data/Character/CharacterFormula.xml");
	sCharacterBase->LoadCharacterStatFruit("../Data/Character/CharacterStatFruit.xml");
	sCharacterBase->LoadCharacterSpecialization("../Data/Character/CharacterSpecialization.xml");
	sCharacterBase->LoadCharacterExperienceAdjust("../Data/Character/CharacterExpAdjust.xml");
	sCharacterBase->LoadCharacterExperienceBonus("../Data/Character/CharacterExpBonus.xml");
	sCharacterBase->LoadCharacterBonus("../Data/Character/CharacterBonus.xml");
	sCharacterBase->LoadCharacterMaxLevelReward("../Data/Character/CharacterMaxLevelReward.xml");
	sCharacterBase->LoadCharacterExperienceTable("../Data/Character/CharacterExpTable.xml");

	if ( this->m_Player )
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Characters data reloaded.");
}

void ChatHandler::ReloadSkill(const char * msg)
{
	sSkillMgr->LoadSkillTemplate("../Data/Skill/SkillList.xml");
	sSkillMgr->LoadSkillLearn("../Data/Skill/SkillLearn.xml");
	sSkillMgr->LoadSkillTree("../Data/Skill/SkillTree.xml");
	sSkillMgr->LoadSkillSpecialElement("../Data/Skill/SkillSpecialElement.xml");
	sSkillMgr->LoadSkillSpecial("../Data/Skill/SkillSpecial.xml");
	sSkillMgr->LoadBuffTemplate("../Data/BuffTemplate.xml");
	sSkillMgr->LoadSkillRequire("../Data/Skill/SkillRequire.xml");
	sSkillMgr->LoadSkillTreeMajestic("../Data/Skill/SkillTreeMajestic.xml");
	sSkillMgr->LoadSkillTreeMajesticStat("../Data/Skill/SkillTreeMajesticStat.xml");
	sSkillMgr->LoadSkillAttackTime("../Data/Skill/SkillAttackTime.xml");

	if ( this->m_Player )
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Skills reloaded.");
}

void ChatHandler::ReloadTeleport(const char * msg)
{
	sTeleport->LoadGateTemplate("../Data/World/GateTemplate.xml");
	sTeleport->LoadTeleportTemplate("../Data/World/TeleportTemplate.xml");
	sTeleport->LoadMoveLevel("../Data/World/MoveLevel.xml");

	if ( this->m_Player )
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Teleport reloaded.");
}

void ChatHandler::ReloadCashShop(const char * msg)
{
	sCashShopMgr->LoadItemList();
	sGameServer->LoadCashShopSettings();

	PlayerSessionMap const& characters = sObjectMgr->GetAllCharacters();
	for (auto const& itr : characters)
	{
		Player* pPlayer = itr.second;

		if (!pPlayer)
		{
			continue;
		}

		pPlayer->CashShopSendDate();
		sCashShopMgr->SendUpdatedData(pPlayer);
	}

	if (this->m_Player)
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "CashShop reloaded.");
}

void ChatHandler::ReloadMix(const char * msg)
{
	sMixMgr->LoadDisabled("../Data/Mix/MixDisable.xml");
	sMixMgr->LoadSpellStone("../Data/Mix/MixSpellStone.xml");
	sMixMgr->LoadGuardian("../Data/Guardian/GuardianMix.xml");
	sJewelMix->Load("../Data/Mix/MixJewelTemplate.xml");

	if ( this->m_Player )
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Mix reloaded.");
}

void ChatHandler::ReloadWorld(const char* msg)
{
	sWorldMgr->LoadWorldList("../Data/World/WorldTemplate.xml");
	sWorldMgr->LoadWorldAIPath("../Data/World/WorldAI.xml");
	sWorldMgr->LoadWorldBuff("../Data/World/WorldBuff.xml");
	sWorldMgr->LoadWorldAreaRestriction("../Data/World/WorldAreaRestriction.xml");
	sWorldMgr->LoadWorldAttribute("../Data/World/WorldAttribute.xml");
	sWorldMgr->LoadWorldExperience("../Data/World/WorldExpParty.xml");
	sWorldMgr->LoadWorldAreaAttribute("../Data/World/WorldAreaAttribute.xml");
	sWorldMgr->LoadWorldFreePK("../Data/World/WorldFreePK.xml");
	sWorldMgr->LoadWorldData("../Data/World/WorldTemplate.xml");
	sWorldMgr->LoadBarrackIni();
	if(sGameServer->IsGSMultiSubEnabled()){
		sWorldMgr->LoadWorldServer("../Data/World/WorldServer.xml");
	}

	if (this->m_Player)
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "World reloaded.");
}

void ChatHandler::ReloadPentagram(const char * msg)
{
	sPentagramSystem->LoadType("../Data/Item/Pentagram/PentagramType.xml");
	sPentagramSystem->LoadOption("../Data/Item/Pentagram/PentagramOption.xml");
	sPentagramSystem->LoadJewelOption("../Data/Item/Pentagram/PentagramJewelOption.xml");
	sPentagramSystem->LoadJewelRemove("../Data/Item/Pentagram/PentagramJewelRemove.xml");
	sPentagramSystem->LoadJewelUpgrade("../Data/Item/Pentagram/PentagramJewelUpgrade.xml");
	//sPentagramSystem->LoadJewelUpgradeLevel();

	if ( this->m_Player )
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Pentagram reloaded.");
}