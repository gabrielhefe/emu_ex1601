/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Main.cpp"
*
*/

bool StartNetwork();
Config * sConfig = new Config;
Config * sConfigCommon = new Config;

int MaxPlayersSet = 0;
int ReservedPlayers = 0;

boost::asio::io_service _ioService;
boost::asio::signal_set signals(_ioService, SIGINT, SIGTERM);

MainApplication::MainApplication(): AppConsole("GameServer", "GAME SERVER", MAIN_DB_FLAG_CONFIG | MAIN_DB_FLAG_MU_LOGIN, sConfigCommon)
{
	sGameServer->SetServerType(sConfig->GetIntDefault("GameServer.ServerType", SERVER_TYPE_NORMAL));
	sGameServer->SetServerCode(sConfig->GetIntDefault("GameServer.Code", 0));

	this->AddDBFlag(MAIN_DB_FLAG_MU);
	this->AddDBFlag(MAIN_DB_FLAG_MU_LOG);
}

MainApplication::~MainApplication()
{
	
}

int main(int argc, char *argv[])
{
	sLog->LoadFolder();

	sLog->LoadAppender("Console", APPENDER_CONSOLE, LOG_LEVEL_TRACE, APPENDER_FLAGS_NONE, "", "", "w");
	sLog->LoadAppender("GameServer", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "System\\GameServer", "GameServer.log", "w");
	sLog->LoadAppender("Trade", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Trade", "Trade.log", "w");
	sLog->LoadAppender("CashShop", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "CashShop", "CashShop.log", "w");
	sLog->LoadAppender("Network", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "System\\Network", "Network.log", "w");
	sLog->LoadAppender("ChaosMix", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "ChaosMix", "ChaosMix.log", "w");
	sLog->LoadAppender("Quest", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Quest", "Quest.log", "w");
	sLog->LoadAppender("BloodCastle", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\BloodCastle", "BloodCastle.log", "w");
	sLog->LoadAppender("DevilSquare", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\DevilSquare", "DevilSquare.log", "w");
	sLog->LoadAppender("ChaosCastle", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\ChaosCastle", "ChaosCastle.log", "w");
	sLog->LoadAppender("Crywolf", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Crywolf", "Crywolf.log", "w");
	sLog->LoadAppender("Party", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Party", "Party.log", "w");
	sLog->LoadAppender("Invasion", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Invasion", "Invasion.log", "w");
	sLog->LoadAppender("Kanturu", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Kanturu", "Kanturu.log", "w");
	sLog->LoadAppender("CastleSiege", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\CastleSiege", "CastleSiege.log", "w");
	sLog->LoadAppender("Database", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "System\\Database", "Database.log", "w");
	sLog->LoadAppender("Raklion", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Raklion", "Raklion.log", "w");
	sLog->LoadAppender("PersonalShop", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "PersonalShop", "PersonalShop.log", "w");
	sLog->LoadAppender("Doppelganger", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Doppelganger", "Doppelganger.log", "w");
	sLog->LoadAppender("Guild", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Guild", "Guild.log", "w");
	sLog->LoadAppender("Items", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Items", "Items.log", "w");
	sLog->LoadAppender("Gens", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Gens", "Gens.log", "w");
	sLog->LoadAppender("Skill", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Skill", "Skill.log", "w");
	sLog->LoadAppender("Chat", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Chat", "Chat.log", "w");
	sLog->LoadAppender("Player", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Player", "Player.log", "w");
	sLog->LoadAppender("Monster", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Monster", "Monster.log", "w");
	sLog->LoadAppender("Crash", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "System\\Crash", "Crash.log", "w");
	sLog->LoadAppender("Scramble", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Scramble", "Scramble.log", "w");
	sLog->LoadAppender("Race", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\Race", "Race.log", "w");
	sLog->LoadAppender("IllusionTemple", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\IllusionTemple", "IllusionTemple.log", "w");
	sLog->LoadAppender("Jewel", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Jewel", "Jewel.log", "w");
	sLog->LoadAppender("ImperialFortress", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\ImperialFortress", "ImperialFortress.log", "w");
	sLog->LoadAppender("AntiHack", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "AntiHack", "AntiHack.log", "w");
	sLog->LoadAppender("ProtectorOfAcheron", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\ProtectorOfAcheron", "ProtectorOfAcheron.log", "w");
	sLog->LoadAppender("TormentedSquare", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\TormentedSquare", "TormentedSquare.log", "w");
	sLog->LoadAppender("ArkaWar", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\ArkaWar", "ArkaWar.log", "w");
	sLog->LoadAppender("NixiesLake", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\NixiesLake", "NixiesLake.log", "w");
	sLog->LoadAppender("LabyrinthDimension", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\LabyrinthDimension", "LabyrinthDimension.log", "w");
	sLog->LoadAppender("WorldBoss", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\WorldBoss", "WorldBoss.log", "w");
	sLog->LoadAppender("MuRoomy", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\MuRoomy", "MuRoomy.log", "w");
	sLog->LoadAppender("MiniBomb", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\MiniBomb", "MiniBomb.log", "w");
	sLog->LoadAppender("JewelBingo", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\JewelBingo", "JewelBingo.log", "w");
	sLog->LoadAppender("AutoLogin", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "AutoLogin", "AutoLogin.log", "w");
	sLog->LoadAppender("MossMerchant", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\MossMerchant", "MossMerchant.log", "w");
	sLog->LoadAppender("SwampOfDarkness", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\SwampOfDarkness", "SwampOfDarkness.log", "w");
	sLog->LoadAppender("NumericBaseball", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\NumericBaseball", "NumericBaseball.log", "w");
	sLog->LoadAppender("LastManStanding", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\LastManStanding", "LastManStanding.log", "w");
	sLog->LoadAppender("InstancedDungeon", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "Event\\InstancedDungeon", "InstancedDungeon.log", "w");
	sLog->LoadAppender("QuestMU", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "QuestMU", "QuestMU.log", "w");
	sLog->LoadAppender("DamagePvP", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "DamagePvP", "DamagePvP.log", "w");

	//sLog->LoadAppender("ConfigTemp", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(16), "ConfigTemp", "ConfigTemp.log", "w");

	sLog->LoadLogger("root", LOG_LEVEL_TRACE, " Console GameServer");
	sLog->LoadLogger("trade", LOG_LEVEL_TRACE, "Trade");
	sLog->LoadLogger("cashshop", LOG_LEVEL_TRACE, "CashShop");
	sLog->LoadLogger("network", LOG_LEVEL_TRACE, "Network");
	sLog->LoadLogger("chaosmix", LOG_LEVEL_TRACE, "ChaosMix");
	sLog->LoadLogger("quest", LOG_LEVEL_TRACE, "Quest");
	sLog->LoadLogger("bloodcastle", LOG_LEVEL_TRACE, "BloodCastle");
	sLog->LoadLogger("devilsquare", LOG_LEVEL_TRACE, "DevilSquare");
	sLog->LoadLogger("chaoscastle", LOG_LEVEL_TRACE, "ChaosCastle");
	sLog->LoadLogger("crywolf", LOG_LEVEL_TRACE, "Crywolf");
	sLog->LoadLogger("party", LOG_LEVEL_TRACE, "Party");
	sLog->LoadLogger("invasion", LOG_LEVEL_TRACE, "Invasion");
	sLog->LoadLogger("kanturu", LOG_LEVEL_TRACE, "Kanturu");
	sLog->LoadLogger("castlesiege", LOG_LEVEL_TRACE, "CastleSiege");
	sLog->LoadLogger("database", LOG_LEVEL_TRACE, "Database");
	sLog->LoadLogger("raklion", LOG_LEVEL_TRACE, "Raklion");
	sLog->LoadLogger("personalshop", LOG_LEVEL_TRACE, "PersonalShop");
	sLog->LoadLogger("doppelganger", LOG_LEVEL_TRACE, "Doppelganger");
	sLog->LoadLogger("guild", LOG_LEVEL_TRACE, "Guild");
	sLog->LoadLogger("items", LOG_LEVEL_TRACE, "Items");
	sLog->LoadLogger("gens", LOG_LEVEL_TRACE, "Gens");
	sLog->LoadLogger("skill", LOG_LEVEL_TRACE, "Skill");
	sLog->LoadLogger("chat", LOG_LEVEL_TRACE, "Chat");
	sLog->LoadLogger("player", LOG_LEVEL_TRACE, "Player");
	sLog->LoadLogger("monster", LOG_LEVEL_TRACE, "Monster");
	sLog->LoadLogger("crash", LOG_LEVEL_TRACE, "Crash");
	sLog->LoadLogger("scramble", LOG_LEVEL_TRACE, "Scramble");
	sLog->LoadLogger("race", LOG_LEVEL_TRACE, "Race");
	sLog->LoadLogger("illusiontemple", LOG_LEVEL_TRACE, "IllusionTemple");
	sLog->LoadLogger("jewel", LOG_LEVEL_TRACE, "Jewel");
	sLog->LoadLogger("imperial_fortress", LOG_LEVEL_TRACE, "ImperialFortress");
	sLog->LoadLogger("antihack", LOG_LEVEL_TRACE, "AntiHack");
	sLog->LoadLogger("protectorofacheron", LOG_LEVEL_TRACE, "ProtectorOfAcheron");
	sLog->LoadLogger("tormentedsquare", LOG_LEVEL_TRACE, "TormentedSquare");
	sLog->LoadLogger("arkawar", LOG_LEVEL_TRACE, "ArkaWar");
	sLog->LoadLogger("nixieslake", LOG_LEVEL_TRACE, "NixiesLake");
	sLog->LoadLogger("labyrinthdimension", LOG_LEVEL_TRACE, "LabyrinthDimension");
	sLog->LoadLogger("world_boss", LOG_LEVEL_TRACE, "WorldBoss");
	sLog->LoadLogger("mu_roomy", LOG_LEVEL_TRACE, "MuRoomy");
	sLog->LoadLogger("mini_bomb", LOG_LEVEL_TRACE, "MiniBomb");
	sLog->LoadLogger("jewel_bingo", LOG_LEVEL_TRACE, "JewelBingo");
	sLog->LoadLogger("auto_login", LOG_LEVEL_TRACE, "AutoLogin");
	sLog->LoadLogger("moss_merchant", LOG_LEVEL_TRACE, "MossMerchant");
	sLog->LoadLogger("swamp_of_darkness", LOG_LEVEL_TRACE, "SwampOfDarkness");
	sLog->LoadLogger("numeric_baseball", LOG_LEVEL_TRACE, "NumericBaseball");
	sLog->LoadLogger("last_man_standing", LOG_LEVEL_TRACE, "LastManStanding");
	sLog->LoadLogger("instanced_dungeon", LOG_LEVEL_TRACE, "InstancedDungeon");
	sLog->LoadLogger("quest_mu", LOG_LEVEL_TRACE, "QuestMU");
	sLog->LoadLogger("damage_pvp", LOG_LEVEL_TRACE, "DamagePvP");

	//sLog->LoadLogger("config_temp", LOG_LEVEL_TRACE, "ConfigTemp");

	//StartStackLogging();
	if ( !InitConfig("../Data/game_common.conf") )
	{
		system("PAUSE");
		return 1;
	}

	if ( !sMain->Start() )
		return 0;

	sMain->UpdateLoop();

	sMain->End();

	return sMain->GetExitCode();
}

void MainApplication::End()
{
	sObjectMgr->KickAll();

	sWorldSocketMgr.StopNetwork();
	sDataServer->Stop();
	sAuthServer->Stop();
	sConnectServer->Stop();
	
	AppConsole::End();
}

void MainApplication::Load()
{
	this->Init();
	/*QueryResult result1 = GameServerDatabase.Query("SELECT `key`, `value` FROM settings WHERE server_id = -1");

	if ( result1 )
	{
		do
		{
			Field* fields = result1->Fetch();

			this->Add(fields[0].GetString(), fields[1].GetString());
		}
		while(result1->NextRow());
	}

	QueryResult result2 = GameServerDatabase.PQuery("SELECT `key`, `value` FROM settings WHERE server_id = '%u'", sGameServer->GetServerCode());

	if ( result2 )
	{
		do
		{
			Field* fields = result2->Fetch();

			this->Add(fields[0].GetString(), fields[1].GetString());
		}
		while(result2->NextRow());
	}*/

	this->LoadCommonFile();
	this->LoadAntiHackFile();
	this->LoadCalcCharacter();
	this->LoadJewelManagerFile();
	this->LoadCommandIni();
	this->LoadSkillIni();
	this->LoadEventIni();
	this->LoadItemIni();
	this->LoadCharacterIni();
	this->LoadChaosMixIni();
	sCharacterBase->LoadCalcCharacterPvPDame();
}

void MainApplication::LoadCommonFile() {
	char PersonalMerchantShop[255];
	char GameVersion[8];
	char GameSerial[17];
	char welcomemess[100];
	char secreckeylogin[20];
	if (boost::filesystem::exists(SERVER_COMMON_FILEPATH)) {
		//section account
		sGameServer->SetConnectCheckResetOnAction(GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "ConnectCheckResetOnAction", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetConnectCheckTimeDelay(GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "ConnectCheckTime", 120, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetAccountDisconnectTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "DisconnectTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetGameCloseTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "GameCloseTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetMaxLoggingAttempts(GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "MaxLoginAttempts", 3, SERVER_COMMON_FILEPATH));
		sGameServer->SetServerMoveKickTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "ServerMoveKickTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetSecurityCode(GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "SecurityCode", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetAuthorizationEnabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "AuthorizationEnabled", 0, SERVER_COMMON_FILEPATH));

		//section gerenal
		GetPrivateProfileString(SERVER_COMMON_SECTION_GERENAL, "ClientVersion", "", GameVersion, sizeof(GameVersion), SERVER_COMMON_FILEPATH);
		GetPrivateProfileString(SERVER_COMMON_SECTION_GERENAL, "ClientSerial", "", GameSerial, sizeof(GameSerial), SERVER_COMMON_FILEPATH);

		std::string temp_client_version = GameVersion;
		char temp_client_version_buff[8];
		memset(temp_client_version_buff, 0, 8);
		memcpy(temp_client_version_buff, temp_client_version.c_str(), temp_client_version.size() > 8 ? 8 : temp_client_version.size());

		Tokens tokens(temp_client_version_buff, '.');

		sGameServer->SetGameServerVersion(0, tokens[0][0]);
		sGameServer->SetGameServerVersion(1, tokens[1][0]);
		sGameServer->SetGameServerVersion(2, tokens[1][1]);
		sGameServer->SetGameServerVersion(3, tokens[2][0]);
		sGameServer->SetGameServerVersion(4, tokens[2][1]);


		std::string serial = GameSerial;
		sGameServer->ResetGameServerSerial();
		sGameServer->SetGameServerSerial(serial.c_str());

		sGameServer->SetMaxConnectionsPerIP(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "MaxConnectionPerIP", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetMaxConnectionPerMAC(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "MaxConnectionPerMAC", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetMaxConnectionPerDiskSerial(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "MaxConnectionPerDiskSerial", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetCheckConnectAccountMacEnable(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "CheckConnectAccountMacEnable", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetCheckConnectAccountDiskSerialEnable(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "CheckConnectAccountDiskSerialEnable", 0, SERVER_COMMON_FILEPATH));

		GetPrivateProfileString(SERVER_COMMON_SECTION_GERENAL, "WelcomeMessage", "", welcomemess, sizeof(welcomemess), SERVER_COMMON_FILEPATH);
		sGameServer->SetWelcomeMessage(welcomemess);

		sGameServer->SetAutoSaveOfflineTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "AutoSaveOfflineTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetAutoSaveTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "AutosaveTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);

		sGameServer->SetCharacterDieExperienceDecrease(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "DieExperienceDecrease", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetCharacterDieZenDecrease(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "DieZenDecrease", 0, SERVER_COMMON_FILEPATH));

		sGameServer->SetPlayerHeroCount(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "HeroCount", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPlayerHeroTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "HeroTime", 0, SERVER_COMMON_FILEPATH));

		sGameServer->SetMaxCharacterZen(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "MaxZen", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPlayerWalkSpeed(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "PlayerWalkSpeed", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetTopRankingReloadTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "TimeReloadTopRanking", 0, SERVER_COMMON_FILEPATH));

		sGameServer->SetGSMultiSubEnabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "GSMultiSubEnabled", 0, SERVER_COMMON_FILEPATH));

		sGameServer->SetMaxLevel(LEVEL_DATA_NORMAL, GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "LevelNormalMax", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetMaxLevel(LEVEL_DATA_MASTER, GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "LevelMasterMax", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetMaxLevel(LEVEL_DATA_MAJESTIC, GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "LevelMajesticMax", 0, SERVER_COMMON_FILEPATH));


		//Section ClassCreate
		sGameServer->SetDarkLordCreate(GetPrivateProfileInt(SERVER_COMMON_SECTION_CLASS_CREATE, "DarkLordCreateLevel", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGrowLancerCreate(GetPrivateProfileInt(SERVER_COMMON_SECTION_CLASS_CREATE, "GrowLancerCreateLevel", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetMagicGladiatorCreate(GetPrivateProfileInt(SERVER_COMMON_SECTION_CLASS_CREATE, "MagicGladiatorCreateLevel", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetRageFighterCreate(GetPrivateProfileInt(SERVER_COMMON_SECTION_CLASS_CREATE, "RageFighterCreate", 0, SERVER_COMMON_FILEPATH));

		//Section CustomBoss
		sGameServer->SetCustomBossSummonCount(GetPrivateProfileInt(SERVER_COMMON_SECTION_CUSTOM_BOSS, "SummonCount", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetCustomBossSummonDespawnTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_CUSTOM_BOSS, "SummonDespawnTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetCustomBossSummonTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_CUSTOM_BOSS, "SummonTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);

		//Section Common
		sGameServer->SetFenrirRepairRate(GetPrivateProfileInt(SERVER_COMMON_SECTION_COMMON, "FenrirRepairRate", 0, SERVER_COMMON_FILEPATH));

		sGameServer->SetServerTestCodeIndex(GetPrivateProfileInt(SERVER_COMMON_SECTION_COMMON, "ServerTestCodeIndex", 9, SERVER_COMMON_FILEPATH));

		sGameServer->item_duration_time.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_COMMON, "ItemDurationTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);
		sGameServer->looting_time.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_COMMON, "LootingTime", 0, SERVER_COMMON_FILEPATH) * IN_MILLISECONDS);
		GetPrivateProfileString(SERVER_COMMON_SECTION_COMMON, "PersonalMerchantShop", "", PersonalMerchantShop, sizeof(PersonalMerchantShop), SERVER_COMMON_FILEPATH);
		sGameServer->SetPersonalMerchantShop(PersonalMerchantShop);

		GetPrivateProfileString(SERVER_COMMON_SECTION_COMMON, "SecrecKeyLogin", "", secreckeylogin, sizeof(secreckeylogin), SERVER_COMMON_FILEPATH);
		sGameServer->SetSecrecKeyLogin(secreckeylogin);
		//sLog->outError(LOG_DEFAULT, "%s", sGameServer->GetPersonalMerchantShop());

		//Section WareHouse
		sGameServer->SetWarehouseLock(GetPrivateProfileInt(SERVER_COMMON_SECTION_WARE_HOUSE, "Lock", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetWarehouseMaxMoney(GetPrivateProfileInt(SERVER_COMMON_SECTION_WARE_HOUSE, "MaxMoney", 0, SERVER_COMMON_FILEPATH));

		//Section SwitchChannel
		sGameServer->SetChannelChangeNotUseHighPKEnabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_SWITCH_CHANNEL, "ChannelChangeNotUseHighPKEnabled", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetChannelChangeNotUseHighPKLevel(GetPrivateProfileInt(SERVER_COMMON_SECTION_SWITCH_CHANNEL, "ChannelChangeNotUseHighPKLevel", 0, SERVER_COMMON_FILEPATH));

		//Section Duel
		sGameServer->SetDuelEnabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_DUEL, "Enabled", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetDuelBetItem(GetPrivateProfileInt(SERVER_COMMON_SECTION_DUEL, "BetItem", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetDuelClassic(GetPrivateProfileInt(SERVER_COMMON_SECTION_DUEL, "Classic", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetDuelRequiredZen(GetPrivateProfileInt(SERVER_COMMON_SECTION_DUEL, "RequiredZen", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetDuelBuffDisabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_DUEL, "BuffDisabled", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetDuelMuunDisabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_DUEL, "MuunDisabled", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetDuelPentagramDisabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_DUEL, "PentagramDisabled", 0, SERVER_COMMON_FILEPATH));

		//Section Party
		sGameServer->SetPartyMoveEnabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_PARTY, "MoveEnable", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPartySummon(GetPrivateProfileInt(SERVER_COMMON_SECTION_PARTY, "PartySummon", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPartyMoveCost(GetPrivateProfileInt(SERVER_COMMON_SECTION_PARTY, "Movecost", 0, SERVER_COMMON_FILEPATH));
		sGameServer->party_in_battlezone.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_PARTY, "PartyBattleZone", 0, SERVER_COMMON_FILEPATH));
		sGameServer->party_allow_pk.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_PARTY, "AllowPK", 0, SERVER_COMMON_FILEPATH));
		sGameServer->party_allow_dif_gens.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_PARTY, "AllDifGens", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPartyKeepExpSolo(GetPrivateProfileInt(SERVER_COMMON_SECTION_PARTY, "PartyKeepExpSolo", 0, SERVER_COMMON_FILEPATH));

		//PersonalStore
		sGameServer->SetPersonalStoreEnabled(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "Enabled", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPersonalStoreDisconnect(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "Disconnect", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPersonalStoreMinLevel(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "MinLevel", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPersonalStoreOff(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "PersonalStoreOff", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPersonalStoreOffCount(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "PersonalStoreOffCount", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPersonalStoreOffDuration(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "PersonalStoreOffDuration", 0, SERVER_COMMON_FILEPATH) * HOUR * IN_MILLISECONDS);
		sGameServer->SetPersonalStoreOffTotalCount(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "PersonalStoreOffTotalCount", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPersonalStoreLockSamePC(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "PersonalStoreLockSamePC", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetPersonalStoreCreditsPurcharse(GetPrivateProfileInt(SERVER_COMMON_SECTION_PERSONAL_STORE, "PersonalStoreCreditsPurcharse", 0, SERVER_COMMON_FILEPATH));

		//
		sGameServer->kundun_refill_time.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_KUNDUN, "RefillTime", 0, SERVER_COMMON_FILEPATH)* IN_MILLISECONDS);
		sGameServer->kundun_refill_hp.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_KUNDUN, "RefillHp", 0, SERVER_COMMON_FILEPATH));
		sGameServer->kundun_refill_hp_sec.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_KUNDUN, "RefillHpSec", 0, SERVER_COMMON_FILEPATH));

		sGameServer->SetFereaBossHPRecoveryCount(GetPrivateProfileInt(SERVER_COMMON_SECTION_FEREABOSS, "HPRecoveryCount", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetFereaBossHPRecoveryTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_FEREABOSS, "HPRecoveryTime", 0, SERVER_COMMON_FILEPATH)* IN_MILLISECONDS);

		//section guild
		sGameServer->guild_alliance_min_members.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "AllianceMinMembers", 0, SERVER_COMMON_FILEPATH));
		sGameServer->guild_alliance_gens.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "AllianceGens", 0, SERVER_COMMON_FILEPATH));
		sGameServer->guild_alliance_max.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "AllianceMax", 0, SERVER_COMMON_FILEPATH));
		sGameServer->guild_hostil_event_kill.set(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "HostilEventKill", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGuildAllianceCastleSiegeCheck(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "AllianceCastleSiegeCheck", 0, SERVER_COMMON_FILEPATH));

		sGameServer->SetGuildWarMaxWaitInterval(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "WarMaxWaitInterval", 0, SERVER_COMMON_FILEPATH)* IN_MILLISECONDS);
		sGameServer->SetGuildWarMax(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "WarMax", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGuildWarIntervalTime(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "WarIntervalTime", 0, SERVER_COMMON_FILEPATH)* IN_MILLISECONDS);
		sGameServer->SetGuildWarMaxPoints(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "WarMaxPoints", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGuildWarKillPoints(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "WarKillPoints", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGuildWarKillAssistantPoints(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "WarKillAssistantPoints", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGuildWarKillBattlePoints(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "WarKillBattlePoints", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGuildWarKillMasterPoints(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "WarKillMasterPoints", 0, SERVER_COMMON_FILEPATH));

		sGameServer->SetGuildScoreCastleSiege(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "GuildScoreCastleSiege", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGuildScoreCastleSiegeMembers(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "GuildScoreCastleSiegeMembers", 0, SERVER_COMMON_FILEPATH));
		sGameServer->SetGuildScoreArkaWar(GetPrivateProfileInt(SERVER_COMMON_SECTION_GUILD, "GuildScoreArkaWar", 0, SERVER_COMMON_FILEPATH));

		//custom
		sGameServer->SetRankServerEnable(GetPrivateProfileInt(SERVER_COMMON_SECTION_CUSTOM, "RankServerEnable", 0, SERVER_COMMON_FILEPATH));
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Common.ini File");
		MessageBox(NULL, "Error Load Common.ini File", "Error", ERROR);
		exit(0);
	}

}

void MainApplication::LoadAntiHackFile() {
	if (boost::filesystem::exists(SERVER_ANTIHACK_FILEPATH)) {
		//Section AntiHack
		sGameServer->SetAntiHackCoordinateAttackCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "CoordinateAttackCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackCoordinateVerify(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "CoordinateVerify", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackCoordinateVerifyTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "CoordinateVerifyTime", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetSkillCheckEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "SkillCheckEnabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackSkillContinue(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "SkillContinue", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackSkillCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "SkillCount", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackSkillCountContinue(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "SkillCountContinue", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackSkillHeadcodeCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "SkillHeadcodeCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackSkillBan(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "SkillBan", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackSkillKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "SkillKick", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackSkillLogCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "SkillLogCount", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetNormalAttackDiffCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "NormalAttackDiffCheck", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetAntiHackWalkCheck1(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkCheck1", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackWalkCheck2(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkCheck2", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackWalkCheck3(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkCheck3", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackWalkCheck4(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkCheck4", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackWalkCheck5(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkCheck5", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackWalkCheckDistance1(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkCheckDistance1", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiHackWalkCheckError1(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkCheckError1", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetAntiDupeBlock(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AntiDupeBlock", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiDupeKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AntiDupeKick", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiDupeBoxEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AntiDupeBoxEnabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiDupeBoxRemove(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AntiDupeBoxRemove", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiDupeBoxCheckTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AntiDupeBoxCheckTime", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);

		sGameServer->SetAttackSpeedCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AttackSpeedCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAttackSpeedCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AttackSpeedCount", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAttackSpeedRange(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AttackSpeedRange", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAttackSpeedSendTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AttackSpeedSendTime", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetAttackSpeedBan(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AttackSpeedBan", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAttackSpeedKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AttackSpeedKick", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetAttackStunCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "AttackStunCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetDupeCompleteBan(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "DupeCompleteBan", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetHackCheckEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckEnabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetHackCheckResetTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckResetTime", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetHackCheckTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckTime", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetHackCheckLoopTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckLoopTime", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetHackCheckCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckCount", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetHackCheckFrameCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckFrameCount", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetHackCheckKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckKick", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetHackCheckBan(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckBan", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetHackCheckFullBan(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "HackCheckFullBan", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetWalkSpeedDistance(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkSpeedDistance1", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetWalkSpeedCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkSpeedCount1", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetWalkSpeedDistance2(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkSpeedDistance2", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetWalkSpeedCount2(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkSpeedCount2", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetWalkSpeedFix(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkSpeedFix", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetWalkSpeedKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkSpeedKick", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetWalkSpeedBan(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIHACK, "WalkSpeedBan", 0, SERVER_ANTIHACK_FILEPATH));

		//Section AntiMacro
		sGameServer->SetAntiMacroEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIMACRO, "Enabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiMacroComboEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIMACRO, "ComboEnabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiMacroComboTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIMACRO, "ComboTime", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiMacroTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_ANTIMACRO, "Time", 0, SERVER_ANTIHACK_FILEPATH));

		//Section CheatScan
		sGameServer->SetCheatScanEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_CHEATSCAN, "Enabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetCheatScanBan(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_CHEATSCAN, "Ban", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetCheatScanCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_CHEATSCAN, "Count", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetCheatScanKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_CHEATSCAN, "Kick", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetCheatScanTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_CHEATSCAN, "Time", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);

		//Section ComboCheck
		sGameServer->SetComboCheckEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_COMBOCHECK, "Enabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetComboCheckCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_COMBOCHECK, "Count", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetComboCheckDatabaseLog(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_COMBOCHECK, "DatabaseLog", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetComboCheckKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_COMBOCHECK, "Kick", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetComboCheckStop(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_COMBOCHECK, "Stop", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetComboCheckTimeDiff(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_COMBOCHECK, "TimeDiff", 0, SERVER_ANTIHACK_FILEPATH));

		//Section GsCheck
		sGameServer->SetAntiFloodCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AntiFloodCount", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiFloodKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AntiFloodKick", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetAntiSpamEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AntiSpamEnabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiSpamCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AntiSpamCount", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAntiSpamTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AntiSpamTime", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetAntiSpamMuteTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AntiSpamMuteTime", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetAttackSafeEnabled(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AttackSafeEnabled", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAttackSafeTime(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AttackSafeTime", 0, SERVER_ANTIHACK_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetAttackSafeCount(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AttackSafeCount", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetAttackSafeKick(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "AttackSafeKick", 0, SERVER_ANTIHACK_FILEPATH));

		sGameServer->SetLoginDiskSerialMacCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "LoginDiskSerialMacCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetLuckyItemMixCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "LuckyItemMixCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetMultiAttackSkillCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "MultiAttackSkillCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetItemBuffCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "ItemBuffCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetDebuffStopCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "DebuffStopCheck", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetDobleFrustrum(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "DobleFrustrum", 0, SERVER_ANTIHACK_FILEPATH));
		sGameServer->SetElfSkillWeaponCheck(GetPrivateProfileInt(SERVER_ANTIHACK_SECTION_GSCHECK, "ElfSkillWeaponCheck", 0, SERVER_ANTIHACK_FILEPATH));
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load AntiHack.ini File");
		MessageBox(NULL, "Error Load AntiHack.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::LoadCalcCharacter() {
	if (boost::filesystem::exists(SERVER_CALCCHARACTER_FILEPATH)) {
		sGameServer->SetDamageModPVE(Character::DARK_KNIGHT, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "DKDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::DARK_KNIGHT, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "DKDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::DARK_LORD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "DLDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::DARK_LORD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "DLDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::DARK_WIZARD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "DWDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::DARK_WIZARD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "DWDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::FAIRY_ELF, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "FEDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::FAIRY_ELF, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "FEDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::GUN_CRUSHER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "GCDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::GUN_CRUSHER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "GCDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::GROW_LANCER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "GLDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::GROW_LANCER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "GLDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::MAGIC_GLADIATOR, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "MGDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::MAGIC_GLADIATOR, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "MGDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::RAGE_FIGHTER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "RFDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::RAGE_FIGHTER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "RFDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::RUNE_WIZARD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "RWDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::RUNE_WIZARD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "RWDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::SLAYER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "SLDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::SLAYER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "SLDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVE(Character::SUMMONER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "SMDamageModPVE", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageModPVP(Character::SUMMONER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_CALCDAMEMOD, "SMDamageModPVP", 0, SERVER_CALCCHARACTER_FILEPATH));

		sGameServer->SetMaxStat(STRENGTH, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "MaxStrength", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxStat(AGILITY, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "MaxAgility", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxStat(VITALITY, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "MaxVitality", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxStat(ENERGY, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "MaxEnergy", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxStat(LEADERSHIP, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "MaxLeadership", 0, SERVER_CALCCHARACTER_FILEPATH));

		sGameServer->SetSDAttackSuccessRate(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "SDAttackSuccessRate", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->shield_combo_miss_init.set(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "SDComboMissInit", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->shield_damage_to_sd_rate.set(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "SDDamageToSDRate", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamagePlayerAddPercent(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "DamagePlayerAddPercent", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDamageToSDPersent(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_GERENAL, "DamageToSDPersent", 100, SERVER_CALCCHARACTER_FILEPATH));

		sGameServer->SetMaxAttackSpeed(Character::DARK_KNIGHT, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedDK", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::DARK_LORD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedDL", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::DARK_WIZARD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedDW", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::FAIRY_ELF, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedELF", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::GUN_CRUSHER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedGC", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::GROW_LANCER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedGL", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::MAGIC_GLADIATOR, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedMG", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::RAGE_FIGHTER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedRF", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::RUNE_WIZARD, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedRW", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::SLAYER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedSL", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetMaxAttackSpeed(Character::SUMMONER, GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_ATTACK_SPEED, "MaxAttackSpeedSUM", 0, SERVER_CALCCHARACTER_FILEPATH));

		sGameServer->SetExcellentDamageRateLimit(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_DAMAGE_RATE, "ExcellentDamageRateLimit", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetCriticalDamageRateLimit(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_DAMAGE_RATE, "CriticalDamageRateLimit", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetDecreaseDamageRateLimit(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_DAMAGE_RATE, "DecreaseDamageRateLimit", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetReflectDamageRateLimit(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_DAMAGE_RATE, "ReflectDamageRateLimit", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetIgnoreDefenseRateLimit(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_DAMAGE_RATE, "IgnoreDefenseRateLimit", 0, SERVER_CALCCHARACTER_FILEPATH));
		sGameServer->SetShieldDefenseRateLimit(GetPrivateProfileInt(SERVER_CALCCHARACTER_SECTION_DAMAGE_RATE, "ShieldDefenseRateLimit", 0, SERVER_CALCCHARACTER_FILEPATH));

	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load CalcCharacter.ini File");
		MessageBox(NULL, "Error Load CalcCharacter.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::LoadJewelManagerFile() {
	if (boost::filesystem::exists(SERVER_JEWELSMANAGER_FILEPATH)) {
		sGameServer->SetJewelOfLifeEnabled(true);
		sGameServer->SetJewelOfLifeMax(7);
		sGameServer->SetJewelOfLifeReset(true);
		sGameServer->SetJewelOfLifeResetWarning("");
		sGameServer->SetJewelOfSoulEnabled(true);
		sGameServer->jewel_of_life_max_rate.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_LIFE, "MaxRate", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->jewel_of_life_rate_ancient.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_LIFE, "RateAncient", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->jewel_of_life_rate_exe.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_LIFE, "RateExe", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->jewel_of_life_rate_normal.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_LIFE, "RateNormal", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->jewel_of_life_rate_socket.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_LIFE, "RateSocket", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetJewelOfLifeResetRate(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_LIFE, "ResetRate", 0, SERVER_JEWELSMANAGER_FILEPATH));

		sGameServer->SetJewelOfSoulRate(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_SOUL, "Rate", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetJewelOfSoulRateLuck(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_SOUL, "RateLuck", 0, SERVER_JEWELSMANAGER_FILEPATH));

		sGameServer->harmony_refine_rate.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_HARMONY, "RefineRate", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->harmony_strengthen_rate.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_HARMONY, "StrengthenRate", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->harmony_smelt_normal_rate.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_HARMONY, "SmeltNormalRate", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->harmony_smelt_exe_rate.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_HARMONY, "SmeltExeRate", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->harmony_strengthen_update_normal_rate.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_HARMONY, "StrengthenUpdateNormalRate", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->harmony_strengthen_update_exe_rate.set(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_HARMONY, "StrengthenUpdateExeRate", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetHarmonyOnAncient(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_OF_HARMONY, "HarmonyOnAncient", 0, SERVER_JEWELSMANAGER_FILEPATH));

		sGameServer->SetJewelsOfBlessSellPrice(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_PRICE, "JewelsOfBlessSellPrice", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetJewelsOfSoulSellPrice(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_PRICE, "JewelsOfSoulSellPrice", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetJewelsOfLifeSellPrice(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_PRICE, "JewelsOfLifeSellPrice", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetJewelsOfChaosSellPrice(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_PRICE, "JewelsOfChaosSellPrice", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetJewelsOfCreationSellPrice(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_PRICE, "JewelsOfCreationSellPrice", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetJewelsOfGuardianSellPrice(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_PRICE, "JewelsOfGuardianSellPrice", 0, SERVER_JEWELSMANAGER_FILEPATH));
		sGameServer->SetJewelsOfHarmonySellPrice(GetPrivateProfileInt(SERVER_JEWELSMANAGER_SECTION_JEWEL_PRICE, "JewelsOfHarmonySellPrice", 0, SERVER_JEWELSMANAGER_FILEPATH));

	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load JewelsManager.ini File");
		MessageBox(NULL, "Error Load JewelsManager.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::LoadCommandIni() {
	if (boost::filesystem::exists(SERVER_COMMAND_FILEPATH)) {
		sGameServer->SetCommandAddStatCost(GetPrivateProfileInt(SERVER_COMMAND_SECTION_ADDSTAT, "AddStatCost", 0, SERVER_COMMAND_FILEPATH));
		sGameServer->SetCommandAddExpanded(GetPrivateProfileInt(SERVER_COMMAND_SECTION_ADDSTAT, "AddExpanded", 0, SERVER_COMMAND_FILEPATH));

		sGameServer->SetCommandPostEnabled(GetPrivateProfileInt(SERVER_COMMAND_SECTION_POST, "PostEnabled", 0, SERVER_COMMAND_FILEPATH));
		sGameServer->SetCommandPostCost(GetPrivateProfileInt(SERVER_COMMAND_SECTION_POST, "PostCost", 0, SERVER_COMMAND_FILEPATH));
		sGameServer->SetCommandPostDelay(GetPrivateProfileInt(SERVER_COMMAND_SECTION_POST, "PostDelay", 0, SERVER_COMMAND_FILEPATH));
		sGameServer->SetCommandPostMinLevel(GetPrivateProfileInt(SERVER_COMMAND_SECTION_POST, "MinLevelPost", 0, SERVER_COMMAND_FILEPATH));

		sGameServer->SetCommandClearInventory(GetPrivateProfileInt(SERVER_COMMAND_SECTION, "CmdClearInventory", 0, SERVER_COMMAND_FILEPATH));
		sGameServer->SetCommandClearGremoryCase(GetPrivateProfileInt(SERVER_COMMAND_SECTION, "CmdClearGremoryCase", 0, SERVER_COMMAND_FILEPATH));
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Command.ini File");
		MessageBox(NULL, "Error Load Command.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::LoadItemIni() {
	if (boost::filesystem::exists(SERVER_ITEM_FILEPATH)) {
		//Section ItemDrop
		sGameServer->SetItemDropExeOptionMin(GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "ExeMinOption", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropExeOptionMax(GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "ExeMaxOption", 0, SERVER_ITEM_FILEPATH));
		sGameServer->item_drop_luck_rate_exe.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "LuckRateExe", 0, SERVER_ITEM_FILEPATH));
		sGameServer->item_drop_luck_rate_normal.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "LuckRateNormal", 0, SERVER_ITEM_FILEPATH));
		sGameServer->item_drop_rate_exe.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "RateExe", 0, SERVER_ITEM_FILEPATH));
		sGameServer->item_drop_rate_normal.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "RateNormal", 0, SERVER_ITEM_FILEPATH));
		sGameServer->item_drop_skill_rate_normal.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "SkillRateNormal", 0, SERVER_ITEM_FILEPATH));

		sGameServer->SetItemDropSocketRate(0, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket1RateNormal", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropSocketRate(1, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket2RateNormal", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropSocketRate(2, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket3RateNormal", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropSocketRate(3, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket4RateNormal", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropSocketRate(4, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket5RateNormal", 0, SERVER_ITEM_FILEPATH));

		sGameServer->SetItemDropSocketRateExe(0, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket1RateExe", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropSocketRateExe(1, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket2RateExe", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropSocketRateExe(2, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket3RateExe", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropSocketRateExe(3, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket4RateExe", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetItemDropSocketRateExe(4, GetPrivateProfileInt(SERVER_ITEM_SECTION_DROP, "Socket5RateExe", 0, SERVER_ITEM_FILEPATH));


		sGameServer->SetNormalZen(GetPrivateProfileInt(SERVER_ITEM_SECTION_ZEN_DROP, "ZenRate", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetMasterZen(GetPrivateProfileInt(SERVER_ITEM_SECTION_ZEN_DROP, "ZenRate", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetMajesticZen(GetPrivateProfileInt(SERVER_ITEM_SECTION_ZEN_DROP, "ZenRate", 0, SERVER_ITEM_FILEPATH));

		sGameServer->option_380_enabled.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_380_ITEM, "Enabled", 0, SERVER_ITEM_FILEPATH));
		sGameServer->option_380_price.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_380_ITEM, "Price", 0, SERVER_ITEM_FILEPATH));
		sGameServer->option_380_rate_grade_1.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_380_ITEM, "RateGrade1", 0, SERVER_ITEM_FILEPATH));
		sGameServer->option_380_rate_grade_2.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_380_ITEM, "RateGrade2", 0, SERVER_ITEM_FILEPATH));
		sGameServer->option_380_rate_grade_3.set(GetPrivateProfileInt(SERVER_ITEM_SECTION_380_ITEM, "RateGrade3", 0, SERVER_ITEM_FILEPATH));

		sGameServer->SetRuudWeaponShopLuckRate(GetPrivateProfileInt(SERVER_ITEM_SECTION_RUUD_WEAPON_SHOP, "LuckRate", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetRuudWeaponShopSkillRate(GetPrivateProfileInt(SERVER_ITEM_SECTION_RUUD_WEAPON_SHOP, "LuckRate", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetRuudWeaponShopExeOptionMin(GetPrivateProfileInt(SERVER_ITEM_SECTION_RUUD_WEAPON_SHOP, "ExeOptionMin", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetRuudWeaponShopExeOptionMax(GetPrivateProfileInt(SERVER_ITEM_SECTION_RUUD_WEAPON_SHOP, "ExeOptionMax", 0, SERVER_ITEM_FILEPATH));

		sGameServer->SetMonsterSoulPurchaseAmount(GetPrivateProfileInt(SERVER_ITEM_SECTION_MONSTER_SOUL, "MonsterSoulPurchaseAmount", 0, SERVER_ITEM_FILEPATH));
		sGameServer->SetMonsterSoulDurationTime(GetPrivateProfileInt(SERVER_ITEM_SECTION_MONSTER_SOUL, "MonsterSoulDurationTime", 0, SERVER_ITEM_FILEPATH));
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Item.ini File");
		MessageBox(NULL, "Error Load Item.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::LoadCharacterIni() {
	if (boost::filesystem::exists(SERVER_CHAR_FILEPATH)) {
		//section character
		sGameServer->SetAutoRecuperationEnabled(POWER_LIFE, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "HPAutorecuperationEnabled", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationEnabled(POWER_MANA, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "MPAutorecuperationEnabled", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationEnabled(POWER_STAMINA, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "AGAutorecuperationEnabled", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationEnabled(POWER_SHIELD, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "SDAutorecuperationEnabled", 0, SERVER_CHAR_FILEPATH));

		sGameServer->SetAutoRecuperationCount(POWER_LIFE, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "HPAutorecuperationCount", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationCount(POWER_MANA, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "MPAutorecuperationCount", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationCount(POWER_STAMINA, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "AGAutorecuperationCount", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationCount(POWER_SHIELD, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "SDAutorecuperationCount", 0, SERVER_CHAR_FILEPATH));

		sGameServer->SetAutoRecuperationCountRest(POWER_LIFE, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "HPAutorecuperationCountRest", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationCountRest(POWER_MANA, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "MPAutorecuperationCountRest", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationCountRest(POWER_STAMINA, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "AGAutorecuperationCountRest", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationCountRest(POWER_SHIELD, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "SDAutorecuperationCountRest", 0, SERVER_CHAR_FILEPATH));

		sGameServer->SetAutoRecuperationMaxLevel(POWER_LIFE, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "HPAutorecuperationMaxLevel", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationMaxLevel(POWER_MANA, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "MPAutorecuperationMaxLevel", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationMaxLevel(POWER_STAMINA, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "AGAutorecuperationMaxLevel", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetAutoRecuperationMaxLevel(POWER_SHIELD, GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "SDAutorecuperationMaxLevel", 0, SERVER_CHAR_FILEPATH));

		sGameServer->SetPlayerSelfDefenseEnabled(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "SelfDefenseEnabled", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerSelfDefenseTime(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER, "SelfDefenseTime", 0, SERVER_CHAR_FILEPATH) * IN_MILLISECONDS);

		//PKDrop
		sGameServer->SetPlayerPKItemDrop(GetPrivateProfileInt(SERVER_CHAR_SECTION_PK_DROP, "PKItemDrop", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerPKItemDrop380(GetPrivateProfileInt(SERVER_CHAR_SECTION_PK_DROP, "PKItemDrop380", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerPKItemDropAncient(GetPrivateProfileInt(SERVER_CHAR_SECTION_PK_DROP, "PKItemDropAncient", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerPKItemDropExcellent(GetPrivateProfileInt(SERVER_CHAR_SECTION_PK_DROP, "PKItemDropExcellent", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerPKItemDropHarmony(GetPrivateProfileInt(SERVER_CHAR_SECTION_PK_DROP, "PKItemDropHarmony", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerPKItemDropMinLevel(GetPrivateProfileInt(SERVER_CHAR_SECTION_PK_DROP, "PKItemDropItemMinLevel", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerPKItemDropItemMaxLevel(GetPrivateProfileInt(SERVER_CHAR_SECTION_PK_DROP, "PKItemDropItemMaxLevel", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerPKWarpCostIncrease(GetPrivateProfileInt(SERVER_CHAR_SECTION_PK_DROP, "PKWarpCostIncrease", 0, SERVER_CHAR_FILEPATH));

		//Section CharacterPK
		sGameServer->SetPVPEnabled(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER_PK, "PVPEnabled", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerPVPMinLevel(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER_PK, "PVPMinLevel", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetExpPoolMuderPerLevel(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER_PK, "ExpPoolMuderPerLevel", 50000, SERVER_CHAR_FILEPATH));

		sGameServer->SetMultiAttackFireScream(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER_PK, "MultiAttackFireScream", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetMultiAttackPenetration(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER_PK, "MultiAttackPenetration", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerMurderCount(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER_PK, "MurderCount", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerMurderLimitCount(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER_PK, "MurderLimitCount", 0, SERVER_CHAR_FILEPATH));
		sGameServer->SetPlayerMurderTime(GetPrivateProfileInt(SERVER_CHAR_SECTION_CHARACTER_PK, "MurderTime", 0, SERVER_CHAR_FILEPATH));

		//Section Inventory
		sGameServer->SetSellNpcRecoveryEnabled(GetPrivateProfileInt(SERVER_CHAR_SECTION_INVENTORY, "SellNpcRecoveryEnabled", 0, SERVER_CHAR_FILEPATH));
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Character.ini File");
		MessageBox(NULL, "Error Load Character.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::LoadSkillIni() {
	if (boost::filesystem::exists(SERVER_SKILL_FILEPATH)) {
		sGameServer->SetSkillAbsorbHPRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "AbsorbHPRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillAbsorbSDRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "AbsorbSDRate", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillAddDamageEnabled(GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillAddDamageEnabled", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillArchangelWillDamage(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ArchangelWillDamage", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillArchangelWillSkillDamage(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ArchangelWillSkillDamage", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillArchangelWillDuration(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ArchangelWillDuration", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillBastionRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BastionRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBastionSD(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BastionSD", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBastionReduction(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BastionReduction", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillBlindAdd(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BlindAdd", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBlindDiv(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BlindDiv", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBlindValue(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BlindValue", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBlindTime(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BlindTime", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillBloodHowlingRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BloodHowlingRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBloodHowlingDamage(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BloodHowlingDamage", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBloodHowlingDuration(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BloodHowlingDuration", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetBossDebuffBlow(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BossDebuffBlow", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillBurstRWEffect1Div(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BurstRWEffect1Div", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBurstRWEffect2Div(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BurstRWEffect2Div", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBurstRWDurationAdd(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BurstRWDurationAdd", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillBurstRWDurationDiv(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BurstRWDurationDiv", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillCircleShieldDuration(GetPrivateProfileInt(SERVER_SKILL_SECTION, "CircleShieldDuration", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillCircleShieldReduceAGRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "CircleShieldReduceAGRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillCircleShieldReduceAG(GetPrivateProfileInt(SERVER_SKILL_SECTION, "CircleShieldReduceAG", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillCureBuffRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "CureBuffRate", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillDarkPlasmaAttackDistance(GetPrivateProfileInt(SERVER_SKILL_SECTION, "DarkPlasmaAttackDistance", 8, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillDarkPlasmaAttackSpeed(GetPrivateProfileInt(SERVER_SKILL_SECTION, "DarkPlasmaAttackSpeed", 5, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillDarkPlasmaTargetCount(GetPrivateProfileInt(SERVER_SKILL_SECTION, "DarkPlasmaTargetCount", 5, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillDetectionRange(GetPrivateProfileInt(SERVER_SKILL_SECTION, "DetectionRange", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillEarthPrisonStunRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "EarthPrisonStunRate", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillBerserkerlDecHP(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BerserkerlDecHP", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillDarkNessDecHP(GetPrivateProfileInt(SERVER_SKILL_SECTION, "DarkNessDecHP", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillElementalImmuneI(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ElementalImmuneI", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillElementalImmuneII(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ElementalImmuneII", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillElementalBerserkerI(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ElementalBerserkerI", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillElementalBerserkerIDamage(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ElementalBerserkerIDamage", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillElementalBerserkerIDefense(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ElementalBerserkerIDefense", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillFireScreamExplosionRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "FireScreamExplosionRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetFixPowersOnBuff(GetPrivateProfileInt(SERVER_SKILL_SECTION, "FixPowersOnBuff", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetGLElementalDamageFix(GetPrivateProfileInt(SERVER_SKILL_SECTION, "GLElementalDamageFix", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillHasteRWEffect1Div(GetPrivateProfileInt(SERVER_SKILL_SECTION, "HasteRWEffect1Div", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillHasteRWEffect2Div(GetPrivateProfileInt(SERVER_SKILL_SECTION, "HasteRWEffect2Div", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillHasteRWDurationAdd(GetPrivateProfileInt(SERVER_SKILL_SECTION, "HasteRWDurationAdd", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillHasteRWDurationDiv(GetPrivateProfileInt(SERVER_SKILL_SECTION, "HasteRWDurationDiv", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillIllusionDuration(GetPrivateProfileInt(SERVER_SKILL_SECTION, "IllusionDuration", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillIronDefenseValue1(GetPrivateProfileInt(SERVER_SKILL_SECTION, "IronDefenseValue1", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillIronDefenseValue2(GetPrivateProfileInt(SERVER_SKILL_SECTION, "IronDefenseValue2", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillIronDefenseDuration(GetPrivateProfileInt(SERVER_SKILL_SECTION, "IronDefenseDuration", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillMagicArrowProjectiles(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MagicArrowProjectiles", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillMagicArrowSplashDamage(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MagicArrowSplashDamage", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillMagicArrowSplashTargets(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MagicArrowSplashTargets", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillMagicArrowSplashDistance(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MagicArrowSplashDistance", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillMagicPinExplosionRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MagicPinExplosionRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillMagicPinExplosionDamage(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MagicPinExplosionDamage", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillManaShieldLimit(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ManaShieldLimit", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillManaShieldCastleSiegeLimit(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ManaShieldCastleSiegeLimit", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetMultiAttackCount(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MultiAttackCount", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetMultiAttackTime(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MultiAttackTime", 0, SERVER_SKILL_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetMultiAttackNew(GetPrivateProfileInt(SERVER_SKILL_SECTION, "MultiAttackNew", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillParalizeDebuffRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ParalizeDebuffRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPoisonArrowDebuffRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PoisonArrowDebuffRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillSplashDamagePercent(GetPrivateProfileInt(SERVER_SKILL_SECTION, "SplashDamagePercent", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillPlasmaBallMoveDistance(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PlasmaBallMoveDistance", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPlasmaBallAttackDistance(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PlasmaBallAttackDistance", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPlasmaBallMoveSpeed(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PlasmaBallMoveSpeed", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPlasmaBallAttackCount(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PlasmaBallAttackCount", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPlasmaBallAttackSpeed(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PlasmaBallAttackSpeed", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPlasmaBallTargetCount(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PlasmaBallTargetCount", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillPunishRatePVM(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PunishRatePVM", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPunishRatePVP(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PunishRatePVP", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPunishFirstHit(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PunishFirstHit", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillPunishMaxDamage(GetPrivateProfileInt(SERVER_SKILL_SECTION, "PunishMaxDamage", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillReflectLimit(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ReflectLimit", 25, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillSleepLimit(GetPrivateProfileInt(SERVER_SKILL_SECTION, "SleepLimit", 70, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillSleepLimitTime(GetPrivateProfileInt(SERVER_SKILL_SECTION, "SleepLimitTime", 70, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillSleepReuseTime(GetPrivateProfileInt(SERVER_SKILL_SECTION, "SleepReuseTime", 1000, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillTeleportStateTime(TELEPORT_NONE, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateTime0", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateTime(TELEPORT_STANDBY, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateTime1", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateTime(TELEPORT_READY, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateTime2", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateTime(TELEPORT_UNK, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateTime3", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillTeleportStateAttack(TELEPORT_NONE, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateAttack0", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateAttack(TELEPORT_STANDBY, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateAttack1", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateAttack(TELEPORT_READY, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateAttack2", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateAttack(TELEPORT_UNK, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateAttack3", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetSkillTeleportStateRemoveVP(TELEPORT_NONE, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateRemoveVP0", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateRemoveVP(TELEPORT_STANDBY, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateRemoveVP1", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateRemoveVP(TELEPORT_READY, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateRemoveVP2", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetSkillTeleportStateRemoveVP(TELEPORT_UNK, GetPrivateProfileInt(SERVER_SKILL_SECTION, "SkillTeleportStateRemoveVP3", 0, SERVER_SKILL_FILEPATH));

		sGameServer->SetRadianceBindingDuration(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BindingDuration", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetRadianceBlindingDuration(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BlindingDuration", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetRadianceBleedingDuration(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BleedingDuration", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetRadianceParalysisEffect(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ParalysisEffect", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetRadianceBlindingRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BlindingRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetRadianceBleedingRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "BleedingRate", 0, SERVER_SKILL_FILEPATH));
		sGameServer->SetRadianceParalysisRate(GetPrivateProfileInt(SERVER_SKILL_SECTION, "ParalysisRate", 0, SERVER_SKILL_FILEPATH));


	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Skill.ini File");
		MessageBox(NULL, "Error Load Skill.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::LoadChaosMixIni() {
	if (boost::filesystem::exists(SERVER_CHAOSMIX_FILEPATH)) {
		sGameServer->SetItemUpgradeSuccessRate(0 ,GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE, "SuccessRate10", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetItemUpgradeSuccessRate(1, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE, "SuccessRate11", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetItemUpgradeSuccessRate(2, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE, "SuccessRate12", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetItemUpgradeSuccessRate(3, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE, "SuccessRate13", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetItemUpgradeSuccessRate(4, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE, "SuccessRate14", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetItemUpgradeSuccessRate(5, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE, "SuccessRate15", 0, SERVER_CHAOSMIX_FILEPATH));

		sGameServer->SetNoticeEnhancementSuccesAllEnable(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE, "NoticeEnhancementSuccesAllEnable", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetNoticeEnhancementSuccesMinLevel(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_ITEM_UPGRADE, "NoticeEnhancementSuccesMinLevel", 10, SERVER_CHAOSMIX_FILEPATH));

		sGameServer->SetSealedBloodAngelExcellentMin(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX, "SealedBloodAngelExcellentMin", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetSealedBloodAngelExcellentMax(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX, "SealedBloodAngelExcellentMax", 0, SERVER_CHAOSMIX_FILEPATH));

		sGameServer->SetUpgradeArchageWeaponOptionMin(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX, "UpgradeArchageWeaponOptionMin", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetUpgradeArchageWeaponOptionMax(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX, "UpgradeArchageWeaponOptionMax", 0, SERVER_CHAOSMIX_FILEPATH));

		sGameServer->SetNoticeMixSuccesWingEnable(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX, "NoticeMixSuccesWingEnable", 0, SERVER_CHAOSMIX_FILEPATH));

		sGameServer->SetMixSpellStonePriceDivision(SPIRIT_STONE_LESSER, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SPELL_STONE, "LesserSpellStonePriceDivision", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixSpellStonePriceDivision(SPIRIT_STONE_MEDIUM, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SPELL_STONE, "MediumSpellStonePriceDivision", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixSpellStonePriceDivision(SPIRIT_STONE_GREATER, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SPELL_STONE, "GreaterSpellStonePriceDivision", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixSpellStoneRate(SPIRIT_STONE_LESSER, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SPELL_STONE, "LesserSpellStoneRate", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixSpellStoneRate(SPIRIT_STONE_MEDIUM, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SPELL_STONE, "MediumSpellStoneRate", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixSpellStoneRate(SPIRIT_STONE_GREATER, GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SPELL_STONE, "GreaterSpellStoneRate", 0, SERVER_CHAOSMIX_FILEPATH));

		sGameServer->SetSealedMasteryAddLuckRate(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SEALED_MASTERY, "AddLuckRate", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetSealedMasteryAddExcellentEnabled(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SEALED_MASTERY, "AddExcellentEnabled", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetSealedMasteryExcellentOptionMin(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SEALED_MASTERY, "ExcellentOptionMin", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetSealedMasteryExcellentOptionMax(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SEALED_MASTERY, "ExcellentOptionMax", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetSealedMasteryAddOptionLevelEnabled(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SEALED_MASTERY, "AddOptionLevelEnabled", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetSealedMasteryAddOptionLevelMax(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_SEALED_MASTERY, "AddOptionLevelMax", 0, SERVER_CHAOSMIX_FILEPATH));

		sGameServer->SetMixRecoveryEnabled(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX_RECOVERY, "Enabled", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetErrtelMixFailCount(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX_RECOVERY, "ErrtelMixFailCount", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetErrtelToRecovery(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX_RECOVERY, "ErrtelOnFail", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetErrtelRemoveFailCount(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX_RECOVERY, "ErrtelRemoveFailCount", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixRecoveryFailTime(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX_RECOVERY, "FailTime", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixRecoveryPriceBless(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX_RECOVERY, "PriceBless", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixRecoveryPriceWC(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX_RECOVERY, "PriceWC", 0, SERVER_CHAOSMIX_FILEPATH));
		sGameServer->SetMixRecoveryTime(GetPrivateProfileInt(SERVER_CHAOSMIX_SECTION_MIX_RECOVERY, "Time", 0, SERVER_CHAOSMIX_FILEPATH));

	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load ChaosMix.ini File");
		MessageBox(NULL, "Error Load ChaosMix.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::LoadEventIni() {
	if (boost::filesystem::exists(SERVER_EVENT_FILEPATH)) {
		char itembag[255];
		char servercodelake[10];
		char itembag1lake[100];
		char itembag2lake[100];
		char itembag3lake[100];
		char itembag4lake[100];
		char itembag5lake[100];

		char servercodelostrace[10];
		char itembagelostrace[100];

		char servercodepkboss[10];

		char servercodelastman[10];

		char santaitembag[100];

		sGameServer->SetWorldBossEnabled(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "Enabled", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossStartNotification(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "StartNotification", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossNormalRespawn(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "NormalRespawn", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossRequiredPlayer(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "RequiredPlayer", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossRequiredPointsToAttack(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "RequiredPointsToAttack", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossGuildPK(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "GuildPK", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossPKGAP(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "PKGAP", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossPKRange(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "PKRange", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossMaxPKPoints(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "MaxPKPoints", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossAOERange(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "AOERange", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossIncreaseDamagePerPoint(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "IncreaseDamagePerPoint", 0, SERVER_EVENT_FILEPATH));
		GetPrivateProfileString(SERVER_EVENT_SECTION_WORLD_BOSS, "ItemBag", "", itembag, sizeof(itembag), SERVER_EVENT_FILEPATH);
		sGameServer->SetWorldBossItemBag(itembag);
		sGameServer->SetWorldBossRuudReward(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "WorldBossRuudReward", 0, SERVER_EVENT_FILEPATH));

		sGameServer->SetWorldBossMaxPKPoints(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "MaxPKPoints", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetWorldBossAOERange(GetPrivateProfileInt(SERVER_EVENT_SECTION_WORLD_BOSS, "AOERange", 0, SERVER_EVENT_FILEPATH));

		sGameServer->SetBattleSoccerEnabled(GetPrivateProfileInt(SERVER_EVENT_SECTION_SOCCER, "Enabled", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetBattleSoccerDuration(GetPrivateProfileInt(SERVER_EVENT_SECTION_SOCCER, "Duration", 0, SERVER_EVENT_FILEPATH) * MINUTE * IN_MILLISECONDS);
		sGameServer->SetBattleSoccerGoalPoints(GetPrivateProfileInt(SERVER_EVENT_SECTION_SOCCER, "GoalPoints", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetBattleSoccerMaxPoints(GetPrivateProfileInt(SERVER_EVENT_SECTION_SOCCER, "MaxScore", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetBattleSoccerKillPoints(GetPrivateProfileInt(SERVER_EVENT_SECTION_SOCCER, "KillPoints", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetBattleSoccerKillAssistantPoints(GetPrivateProfileInt(SERVER_EVENT_SECTION_SOCCER, "KillAssistantPoints", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetBattleSoccerKillBattlePoints(GetPrivateProfileInt(SERVER_EVENT_SECTION_SOCCER, "KillBattlePoints", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetBattleSoccerKillMasterPoints(GetPrivateProfileInt(SERVER_EVENT_SECTION_SOCCER, "KillMasterPoints", 0, SERVER_EVENT_FILEPATH));

		sGameServer->happy_hour_experience_add.set(GetPrivateProfileInt(SERVER_EVENT_SECTION_HAPPY_HOUR, "ExperienceAdd", 0, SERVER_EVENT_FILEPATH));
		sGameServer->happy_hour_drop_add.set(GetPrivateProfileInt(SERVER_EVENT_SECTION_HAPPY_HOUR, "DropAdd", 0, SERVER_EVENT_FILEPATH));

		sGameServer->SetSwampOfDarknessEnabled(GetPrivateProfileInt(SERVER_EVENT_SECTION_SWAMP_DARKNESS, "Enabled", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetSwampOfDarknessMaxPlayers(GetPrivateProfileInt(SERVER_EVENT_SECTION_SWAMP_DARKNESS, "MaxPlayers", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetSwampOfDarknessBattleTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_SWAMP_DARKNESS, "BattleTime", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetSwampOfDarknessBossAppearTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_SWAMP_DARKNESS, "BossAppearTime", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetSwampOfDarknessClosedTimeMin(GetPrivateProfileInt(SERVER_EVENT_SECTION_SWAMP_DARKNESS, "ClosedTimeMin", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetSwampOfDarknessClosedTimeMax(GetPrivateProfileInt(SERVER_EVENT_SECTION_SWAMP_DARKNESS, "ClosedTimeMax", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);

		GetPrivateProfileString(SERVER_EVENT_SECTION_NIXIESLAKE, "Server", "", servercodelake, sizeof(servercodelake), SERVER_EVENT_FILEPATH);
		sGameServer->SetNixiesLakeEnabled(IsServerIN(servercodelake));
		sGameServer->SetNixiesLakeStandbyTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_NIXIESLAKE, "StandbyTime", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetNixiesLakeClosedTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_NIXIESLAKE, "ClosedTime", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetNixiesLakeBattleTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_NIXIESLAKE, "BattleTime", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetNixiesLakeFree(GetPrivateProfileInt(SERVER_EVENT_SECTION_NIXIESLAKE, "Free", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetNixiesLakeBossRecoveryStandyTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_NIXIESLAKE, "BossRecoveryStandyTime", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetNixiesLakeBossRecoveryPercent(GetPrivateProfileInt(SERVER_EVENT_SECTION_NIXIESLAKE, "BossRecoveryPercent", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetNixiesLakeBossRecoveryTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_NIXIESLAKE, "BossRecoveryTime", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);

		GetPrivateProfileString(SERVER_EVENT_SECTION_NIXIESLAKE, "ItemBag1", "", itembag1lake, sizeof(itembag1lake), SERVER_EVENT_FILEPATH);
		GetPrivateProfileString(SERVER_EVENT_SECTION_NIXIESLAKE, "ItemBag2", "", itembag2lake, sizeof(itembag2lake), SERVER_EVENT_FILEPATH);
		GetPrivateProfileString(SERVER_EVENT_SECTION_NIXIESLAKE, "ItemBag3", "", itembag3lake, sizeof(itembag3lake), SERVER_EVENT_FILEPATH);
		GetPrivateProfileString(SERVER_EVENT_SECTION_NIXIESLAKE, "ItemBag4", "", itembag4lake, sizeof(itembag4lake), SERVER_EVENT_FILEPATH);
		GetPrivateProfileString(SERVER_EVENT_SECTION_NIXIESLAKE, "ItemBag5", "", itembag5lake, sizeof(itembag5lake), SERVER_EVENT_FILEPATH);
		sGameServer->SetNixiesLakeItemBag1(itembag1lake);
		sGameServer->SetNixiesLakeItemBag2(itembag2lake);
		sGameServer->SetNixiesLakeItemBag3(itembag3lake);
		sGameServer->SetNixiesLakeItemBag4(itembag4lake);
		sGameServer->SetNixiesLakeItemBag5(itembag5lake);

		GetPrivateProfileString(SERVER_EVENT_SECTION_LOST_RACE, "Server", "", servercodelostrace, sizeof(servercodelostrace), SERVER_EVENT_FILEPATH);
		GetPrivateProfileString(SERVER_EVENT_SECTION_LOST_RACE, "Reward", "", itembagelostrace, sizeof(itembagelostrace), SERVER_EVENT_FILEPATH);
		sGameServer->SetLosttowerRaceEnabled(IsServerIN(servercodelostrace));
		sGameServer->SetLosttowerRaceReward(itembagelostrace);

		GetPrivateProfileString(SERVER_EVENT_SECTION_PK_BOSS, "Server", "", servercodepkboss, sizeof(servercodepkboss), SERVER_EVENT_FILEPATH);
		sGameServer->SetPKBossEnabled(IsServerIN(servercodepkboss));
		sGameServer->SetPKBossTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_PK_BOSS, "PKBossTime", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);

		GetPrivateProfileString(SERVER_EVENT_SECTION_LAST_MAN, "Server", "", servercodelastman, sizeof(servercodelastman), SERVER_EVENT_FILEPATH);
		sGameServer->SetLastManStandingEnabled(IsServerIN(servercodelastman));
		sGameServer->SetLastManStandingDuration(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "Duration", 0, SERVER_EVENT_FILEPATH) * IN_MILLISECONDS);
		sGameServer->SetLastManStandingKillScore(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "KillScore", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetLastManStandingWinScore(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "WinScore", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetLastManStandingMaxPerPC(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "MaxPerPC", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetLastManStandingDamageReduction(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "DamageReduction", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetLastManStandingMinLevel(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "MinLevel", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetLastManStandingClassic(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "Classic", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetLastManStandingHideCharacter(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "HideCharacter", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetLastManStandingLevelRange(GetPrivateProfileInt(SERVER_EVENT_SECTION_LAST_MAN, "LevelRange", 0, SERVER_EVENT_FILEPATH));

		sGameServer->SetSantaVillageEnabled(GetPrivateProfileInt(SERVER_EVENT_SECTION_SANTA, "Enabled", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetSantaVillageSantaClausEnabled(GetPrivateProfileInt(SERVER_EVENT_SECTION_SANTA, "SantaClausEnabled", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetSantaVillageSantaClausGiftTime(GetPrivateProfileInt(SERVER_EVENT_SECTION_SANTA, "SantaClausGiftTime", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetSantaVillageSeason(GetPrivateProfileInt(SERVER_EVENT_SECTION_SANTA, "Season", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetSantaVillageCheckSeason(GetPrivateProfileInt(SERVER_EVENT_SECTION_SANTA, "CheckSeason", 0, SERVER_EVENT_FILEPATH));

		GetPrivateProfileString(SERVER_EVENT_SECTION_SANTA, "SantaClausItemBag", "", santaitembag, sizeof(santaitembag), SERVER_EVENT_FILEPATH);
		sGameServer->SetSantaVillageSantaClausItemBag(santaitembag);

		sGameServer->SetEventKillRankingEnabled(GetPrivateProfileInt(SERVER_EVENT_SECTION_OTHER, "KillRankingEnabled", 0, SERVER_EVENT_FILEPATH));
		sGameServer->SetEventTimeRankingEnabled(GetPrivateProfileInt(SERVER_EVENT_SECTION_OTHER, "TimeRankingEnabled", 0, SERVER_EVENT_FILEPATH));

		sGameServer->SetLoDeEnabled(GetPrivateProfileInt(SERVER_EVENT_SECTION_LODE, "LoDeEnabled", 0, SERVER_EVENT_FILEPATH));
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Event.ini File");
		MessageBox(NULL, "Error Load Event.ini File", "Error", ERROR);
		exit(0);
	}
}

void MainApplication::CreateSetting(std::string const& key, std::string const& default)
{
	if ( key.empty() )
	{
		return;
	}

	//GameServerDatabase.PExecute("INSERT IGNORE INTO settings (server_id, `key`, `value`) VALUES ('%d', '%s', '%s')", 
	//	-1, key.c_str(), default.c_str());
}

bool MainApplication::Run()
{
	if ( !MixHandler::DuplicatedMixCheck() )
	{
		sLog->outError("root", "Duplicated MixID");
		return false;
	}

	MaxPlayersSet = GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "MaxPlayers", 0, SERVER_COMMON_FILEPATH);
	ReservedPlayers = GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "ReservedPlayers", 0, SERVER_COMMON_FILEPATH);

	if (!this->LoadServerMap("../Data/ServerMap.xml"))
		return false;

	/*if (strcmp(sGameServer->GetNetworkIP(), "103.116.104.123") != 0) {
		exit(0);
		return false;
	}*/

	if (sGameServer->GetServerCode() == 0) {
		QueryResult result = MuDatabase.PQuery("UPDATE account_data SET current_server=%d", 65535);
		QueryResult deleteparty1 = MuDatabase.PQuery("DELETE FROM party");
		QueryResult deleteparty2 = MuDatabase.PQuery("DELETE FROM party_member");
	}
	else {
		QueryResult result = MuDatabase.PQuery("UPDATE account_data SET current_server=65535 where current_server=%d", sGameServer->GetServerCode());
	}

	sGameServer->LoadCommonSettings();
	sGameServer->LoadCashShopSettings();

	sMessage->Init("../Data/SystemMessage.xml");

	sFormulaMgr->Load("../Data/FormulaData.xml");

	sWorldMgr->LoadWorldList("../Data/World/WorldTemplate.xml");
	sWorldMgr->LoadWorldAIPath("../Data/World/WorldAI.xml");
	sWorldMgr->LoadWorldBuff("../Data/World/WorldBuff.xml");
	sWorldMgr->LoadWorldAreaRestriction("../Data/World/WorldAreaRestriction.xml");
	if(sGameServer->IsGSMultiSubEnabled()){
		sWorldMgr->LoadWorldServer("../Data/World/WorldServer.xml");
	}
	sWorldMgr->LoadWorldAttribute("../Data/World/WorldAttribute.xml");
	sWorldMgr->LoadWorldExperience("../Data/World/WorldExpParty.xml");
	sWorldMgr->LoadWorldAreaAttribute("../Data/World/WorldAreaAttribute.xml");
	sWorldMgr->LoadWorldFreePK("../Data/World/WorldFreePK.xml");
	sWorldMgr->LoadBarrackIni();

	sObjectMgr->Init();

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
	sItemMgr->LoadEarringOption("../Data/Item/Earring/ItemEarringOption.xml");
	sItemMgr->LoadEarringBonus("../Data/Item/Earring/ItemEarringBonus.xml");
	sItemMgr->LoadEarringUpgrade("../Data/Item/Earring/ItemEarringUpgrade.xml");
	sItemMgr->LoadMasteryBonus("../Data/Item/ItemMasteryBonus.xml");
	sItemMgr->LoadMysteriousStone("../Data/Item/ItemMysteriousStone.xml");
	sItemMgr->LoadGuardianOption("../Data/Item/Guardian/GuardianOption.xml");
	sItemMgr->SetLastItemUpdate(time(nullptr));
	
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

	sJewelMix->Load("../Data/Mix/MixJewelTemplate.xml");

	sScriptAI->LoadScriptAI();

	sMonsterManager->LoadMonsterTemplate("../Data/Monster/MonsterList.xml");
	sMonsterManager->LoadMonsterItems();
	sMonsterManager->LoadMonsterSkill("../Data/Monster/Skill/MonsterSkill.xml");
	sMonsterManager->LoadMonsterSkillSpecial("../Data/Monster/Skill/MonsterSkillSpecial.xml");
	sMonsterManager->LoadMonsterAIElement("../Data/Monster/AI/MonsterAiElement.xml");
	sMonsterManager->LoadMonsterAIAutomata("../Data/Monster/AI/MonsterAiAutomata.xml");
	sMonsterManager->LoadMonsterAIUnit("../Data/Monster/AI/MonsterAiData.xml");
	sMonsterManager->LoadMonsterAIGroup("../Data/Monster/AI/MonsterAiUnit.xml");
	sMonsterManager->LoadMonsterRespawnLocation("../Data/Monster/MonsterRespawnLocation.xml");
	sMonsterManager->LoadMonsterEquipment("../Data/Monster/MonsterEquipment.xml");
	sMonsterManager->LoadMonsterExclusive("../Data/Monster/MonsterExclusive.xml");
	//sMonsterManager->LoadNpcHandleList("../Data/Monster/NPCHandle.xml");
	sMonsterManager->LoadMonster("../Data/Monster/MonsterSpawn.xml");
	sMonsterManager->LoadMonsterEvent("../Data/Monster/MonsterSpawnEvent.xml");
	sMonsterManager->SetLastUpdate(time(nullptr));

	sTeleport->LoadGateTemplate("../Data/World/GateTemplate.xml");
	sTeleport->LoadTeleportTemplate("../Data/World/TeleportTemplate.xml");
	sTeleport->LoadMoveLevel("../Data/World/MoveLevel.xml");
	
	sVipMgr->LoadVipTemplate("../Data/VipSystem.xml");

	sQuestMgr->LoadQuestEvolution("../Data/Quest/QuestEvolutionTemplate.xml");
	sQuestMgr->LoadQuestEvolutionData("../Data/Quest/QuestEvolutionData.xml");
	sQuestMgr->LoadQuestEvolutionCondition("../Data/Quest/QuestEvolutionCondition.xml");
	sQuestMgr->LoadQuestEvolutionItemReward("../Data/Quest/QuestEvolutionItemReward.xml");

	sQuestMgr->LoadQuestGuided("../Data/Quest/GuideQuest.xml");
	sQuestMgr->LoadQuestMU("../Data/Quest/MuQuest.xml");

	sItemBagMgr->LoadItemBagTemplate("../Data/Item/ItemBagList.xml");
	sGenMgr->LoadIni();
	sGenMgr->LoadGenReward("../Data/Gens/GenReward.xml");
	sGenMgr->LoadGenRanking("../Data/Gens/GenRanking.xml");
	sGenMgr->LoadGenKillPoints("../Data/Gens/GenKillPoint.xml");
	sEventMgr->LoadEventData("../Data/Event/EventManager.xml");
	sEventMgr->LoadEventSeasonData("../Data/Event/EventSeasonManager.xml");
	sEventMgr->LoadEventContributionReward("../Data/Event/EventContributionReward.xml");
	sEventMgr->LoadEventLevel("../Data/Event/EventLevel.xml");
	sEventMgr->LoadEventEnterCount("../Data/Event/EventEnterCount.xml");
	sBloodCastleMgr->LoadBloodCastIni();
	sBloodCastleMgr->LoadBloodCastleSetting();
	sDevilSquareMgr->LoadDevilSquareIni();
	sDevilSquareMgr->LoadDevilSquareSetting();
	sDevilSquareMgr->LoadDevilSquareRanking("../Data/Event/DevilSquare/DevilSquareRanking.xml");
	sChaosCastleMgr->LoadIni();
	sChaosCastleMgr->LoadChaosCastleSetting();
	sChaosCastleSurvivalMgr->LoadChaosCastleSetting();
	sIllusionTempleMgr->LoadIllusionTempleSettings();
	sTormentedSquareSurvival->Load();
	sRaklion->LoadIni();
	sRaklion->LoadData();
	sKanturuMgr->LoadIni();
	sKanturuMgr->Load();
	sImperialFortressMgr->LoadIni();
	sImperialFortressMgr->LoadImperialFortressLevel("../Data/Event/ImperialFortress/EventImperialFortressLevel.xml");
	sImperialFortressMgr->LoadImperialFortressMonsterSettings("../Data/Event/ImperialFortress/EventImperialFortressMonsterSettings.xml");
	sImperialFortressMgr->LoadImperialFortressTraps("../Data/Event/ImperialFortress/EventImperialFortressTrap.xml");
	sDoppelganger->LoadIni();
	sDoppelganger->LoadLevel("../Data/Event/Doppelganger/DoppelgangerLevel.xml");
	sDoppelganger->LoadMonster("../Data/Event/Doppelganger/DoppelgangerMonsterSetting.xml");
	sInvasionMgr->LoadInvasionData("../Data/Event/EventInvasion.xml");
	sInvasionMgr->LoadInvasionGroup("../Data/Event/EventInvasionGroup.xml");
	sCastleSiege->LoadIni();
	sCastleSiege->LoadData();
	sCrywolf->LoadIni();
	sCrywolf->LoadData();
	sDungeonRace->LoadData();
	sLosttowerRace->LoadData();
	sCastleDeep->LoadIni();
	sCastleDeep->Load();
	sCashShopMgr->LoadItemList();
	sGameServer->LoadFilter("../Data/FilterText.xml");
	sGameServer->LoadNotice("../Data/Notice.xml");
	sGameServer->LoadGoblinPoint("../Data/GoblinPoints.xml");
	sGameServer->LoadNonPKTime("../Data/MiscNonPkTime.xml");
	sGameServer->LoadNotification("../Data/Notifications.xml");
	sGameServer->LoadOffsetData("../Data/OffsetData.xml");
	sGameServer->LoadOffsetFPS("../Data/OffsetFPS.xml");
	sGameServer->LoadCheatScanWhiteList("../Data/CheatScanWhitelist.xml");
	sGameServer->LoadOfflineAttackSetting("../Data/Plugin/OfflineAttack/OfflineAttack.xml");
	sGameServer->LoadOfflineAttackWorld("../Data/Plugin/OfflineAttack/OfflineAttackWorld.xml");
	sGameServer->LoadOfflineAttackZone("../Data/Plugin/OfflineAttack/OfflineAttackZone.xml");
	
	sScramble->LoadIni();
	sScramble->LoadWordList("../Data/Event/Scramble/ScrambleWord.xml");
	sMixMgr->LoadDisabled("../Data/Mix/MixDisable.xml");
	sMixMgr->LoadSpellStone("../Data/Mix/MixSpellStone.xml");
	sMixMgr->LoadGuardian("../Data/Item/Guardian/GuardianMix.xml");

	sPentagramSystem->LoadType("../Data/Item/Pentagram/PentagramType.xml");
	sPentagramSystem->LoadOption("../Data/Item/Pentagram/PentagramOption.xml");
	sPentagramSystem->LoadJewelOption("../Data/Item/Pentagram/PentagramJewelOption.xml");
	sPentagramSystem->LoadJewelRemove("../Data/Item/Pentagram/PentagramJewelRemove.xml");
	sPentagramSystem->LoadJewelUpgrade("../Data/Item/Pentagram/PentagramJewelUpgrade.xml");
	//sPentagramSystem->LoadJewelUpgradeLevel();

	sMuunSystem->LoadIni();
	sMuunSystem->Load("../Data/Muun/MuunList.xml");
	sMuunSystem->LoadOption("../Data/Muun/MuunOption.xml");
	sMuunSystem->LoadEnergy("../Data/Muun/MuunEnergy.xml");
	sMuunSystem->LoadExchange("../Data/Muun/MuunExchange.xml");

	sMiniMap->Load("../Data/World/MiniMap.xml");

	sSummonScroll->Load("../Data/Item/SummonScroll.xml");
	sMiningSystem->Load("../Data/Mining.xml");
	sProtectorOfAcheron->LoadIni();
	sProtectorOfAcheron->Load();
	sTormentedSquare->LoadIni();
	sTormentedSquare->Load("../Data/Event/TormentedSquare/EventTormentedSquareStage.xml");
	sTormentedSquare->LoadRanking("../Data/Event/TormentedSquare/EventTormentedSquareRanking.xml");
	
	sEvomon->LoadIni();
	sEvomon->Load("../Data/Event/Evomon/Evomon.xml");
	sEvomon->LoadReward("../Data/Event/Evomon/EvomonReward.xml");

	sArkaWar->LoadIni();
	sArkaWar->Load();

	sLabyrinthDimensions->LoadIni();
	sLabyrinthDimensions->LoadInfo("../Data/Event/Labyrinth/EventLabyrinthInfo.xml");
	sLabyrinthDimensions->LoadMission("../Data/Event/Labyrinth/EventLabyrinthMission.xml");
	sLabyrinthDimensions->LoadArea("../Data/Event/Labyrinth/EventLabyrinthArea.xml");
	sLabyrinthDimensions->LoadAreaData("../Data/Event/Labyrinth/EventLabyrinthAreaData.xml");
	sLabyrinthDimensions->LoadLevel("../Data/Event/Labyrinth/EventLabyrinthLevel.xml");
	sLabyrinthDimensions->LoadMonster("../Data/Event/Labyrinth/EventLabyrinthMonster.xml");
	sLabyrinthDimensions->LoadReward("../Data/Event/Labyrinth/EventLabyrinthReward.xml");

	sShopMgr->LoadShopTemplate("../Data/Shops/ShopList.xml");
	sShopMgr->LoadShopScripts();
	sItemMgr->LoadItemPriceRuud("../Data/Shops/RuudPrice.xml");

	sMossMerchant->Load("../Data/Shops/MossMerchantSection.xml");
	
	sDungeon->LoadIni();
	sDungeon->LoadInstance("../Data/Event/Dungeon/EventDungeon.xml");
	sDungeon->LoadLevel("../Data/Event/Dungeon/EventDungeonLevel.xml");

	sMonsterSoul->LoadConverter("../Data/Item/MonsterSoul/MonsterSoulConverter.xml");
	sMonsterSoul->LoadTransformation("../Data/Item/MonsterSoul/MonsterSoulTranformation.xml");
	sMonsterSoul->LoadReward("../Data/Item/MonsterSoul/MonsterSoulReward.xml");

	sResetSystemMgr->LoadResetData("../Data/ResetSystem.xml");
	sDynamicExperienceMgr->LoadDynamicExperienceData("../Data/ExpSystem/ExpSystem.xml");

/* CHARACTER DATABASE */
	sGuildMgr->LoadGuild();

	sPartyMgr->LoadPartyList();

	sEventInventory->LoadIni();
	sMiniBomb->LoadReward("../Data/Event/EventInventory/EventMiniBombReward.xml");
	sMuRoomy->LoadReward("../Data/Event/EventInventory/EventMuRoomyReward.xml");
	sJewelBingo->LoadReward("../Data/Event/EventInventory/EventJewelBingoReward.xml");
	sNumericBaseball->LoadReward("../Data/Event/EventInventory/EventNumericBaseballReward.xml");

	sEventMgr->GetEventStage(EVENT_CHAOS_CASTLE_SURVIVAL);
	sEventMgr->GetEventStage(EVENT_TORMENTED_SQUARE_SURVIVAL);
	sChaosCastleSurvivalMgr->UpdateRanking();
	sLabyrinthDimensions->LoadRanking();
	sLabyrinthDimensions->LoadSchedule();

	sDungeon->LoadSavedInstance();

	sObjectMgr->LoadGameServerData();

	sCustomRankServer->LoadRankServer();

	sNoticeSystem->Load("../Data/Plugin/NoticeSystem.xml");

	this->LoadAccountTestGS("../Data/AccountTest.xml");

	QueryResult getresetmonstersoul = MuDatabase.PQuery("SELECT * FROM server_monster_soul_reset");

	if (getresetmonstersoul) {
		Field* field = getresetmonstersoul->Fetch();

		sMonsterSoul->SetResetMonsterSoul(field[0].GetUInt8());
	}

	PreparedStatement* stmt = nullptr;

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_CHARACTER_UPDATE_ON_SERVER_START);
	stmt->setUInt16(0, sGameServer->GetServerCode());
	MuDatabase.Execute(stmt);

	if ( !sDataServer->Start() )
		return false;

	if ( !sAuthServer->Start() )
		return false;

	if ( !sConnectServer->Start() )
		return false;

	if ( !StartNetwork() )
		return false;



	sObjectMgr->SetRespawnMonsterTime(MyGetTickCount() + sGameServer->GetSpawnMonsterTime());
	sObjectMgr->SetRespawnMonster(true);

	m_timers[WUPDATE_WEATHERS].SetInterval(500);
	m_timers[WUPDATE_VIEWPORT].SetInterval(IN_MILLISECONDS);
	m_timers[WUPDATE_PINGDB].SetInterval(this->GetPingDB() * MINUTE * IN_MILLISECONDS);
	m_timers[WUPDATE_SHUTDOWN].SetInterval(1 * IN_MILLISECONDS);
	m_timers[WUPDATE_EVENT_ANIMATION].SetInterval(1 * IN_MILLISECONDS);
	m_timers[WUPDATE_TOP_RANKING].SetInterval(sGameServer->GetTopRankingReloadTime() * MINUTE * IN_MILLISECONDS);
	m_timers[WUPDATE_MONSTER_SOUL_RESET].SetInterval(5 * IN_MILLISECONDS);

	return true;
}

bool StartNetwork()
{
    std::string worldListener = sMain->GetString("Network.BindIP", "0.0.0.0");

    return sWorldSocketMgr.StartNetwork(_ioService, worldListener, sGameServer->GetNetworkPort(), 1);
}

bool MainApplication::LoadServerMap(char* pchFileName) {
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	uint8 count = 0;

	pugi::xml_node Main = file.child("ServerMap");

	for (pugi::xml_node server = Main.child("Server"); server; server = server.next_sibling()) {
		uint8 servercode = server.attribute("ServerCode").as_int();
		if (servercode == sGameServer->GetServerCode()) {
			sGameServer->SetNetworkPort(server.attribute("ServerPort").as_int());
			sGameServer->SetNetworkIP(server.attribute("ServerIP").as_string());
			sGameServer->SetFlag(server.attribute("Display").as_int());
			sGameServer->SetDefaultWorld(server.attribute("DefaultWorld").as_int());
			sGameServer->SetDefaultX(server.attribute("DefaultX").as_int());
			sGameServer->SetDefaultY(server.attribute("DefaultY").as_int());
			sGameServer->SetServerMode(server.attribute("ServerType").as_int());
			count++;
		}
	}

	if (count == 0) {
		sLog->outError("root", "Can't find Server Code %u info.", sGameServer->GetServerCode());
		return false;
	}

	return true;
}

void MainApplication::Update(uint32 diff)
{
	for (int32 i = 0; i < WUPDATE_COUNT; ++i)
	{
		if (m_timers[i].GetCurrent() >= 0)
			m_timers[i].Update(diff);
		else
			m_timers[i].SetCurrent(0);
	}

	if (m_timers[WUPDATE_SHUTDOWN].Passed())
	{
		m_timers[WUPDATE_SHUTDOWN].Reset();

		sGameServer->UpdateShutdown();
	}

	sObjectMgr->UpdateSessions(diff);

	if (m_timers[WUPDATE_VIEWPORT].Passed())
	{
		m_timers[WUPDATE_VIEWPORT].Reset();

		sObjectMgr->UpdateSessionsViewport(diff);
	}

	if (m_timers[WUPDATE_WEATHERS].Passed())
	{
		m_timers[WUPDATE_WEATHERS].Reset();

		sWorldMgr->UpdateWeather();
		sWorldMgr->Update();
	}

	sObjectMgr->Update();

	if (sGameServer->IsRankServerEnable()) {
		if (m_timers[WUPDATE_TOP_RANKING].Passed()) {
			m_timers[WUPDATE_TOP_RANKING].Reset();

			sCustomRankServer->LoadRankServer();
			sLog->outInfo(LOG_DEFAULT, "Update Top Ranking Succes");
		}
	}

	if (m_timers[WUPDATE_MONSTER_SOUL_RESET].Passed()) {
		m_timers[WUPDATE_MONSTER_SOUL_RESET].Reset();
		sMonsterSoul->CheckResetPurchase();
	}

	if (m_timers[WUPDATE_EVENT_ANIMATION].Passed())
	{
		m_timers[WUPDATE_EVENT_ANIMATION].Reset();

		sObjectMgr->UpdateInvasionAnimation();
		sConnectServer->SendPlayerPercent();
		sAuthServer->SendPlayerPercent();
		sTeleport->Update();
		sPartyMgr->Update();
		sObjectMgr->SaveGameServerData();
		sDuelMgr->UpdateTime();
		sGuildWarMgr->Update();
		sGameServer->NonPKTimeUpdate();

		sGameServer->Update();
		sGameServer->PostManageUpdate();
		sGameServer->UpdateMACRestriction();

		sShopMgr->Update();

		sEventMgr->UpdateSeason();

		sQuestMgr->Update4thQuestInstance();

		sObjectMgr->CharacterAutoLogin();
	}

	sEventMgr->Update();

	if (sGameServer->IsConnectServerReconnect() && sConnectServer->IsStopped())
	{
		if (sConnectServer->GetReconnectTime()->Elapsed(sGameServer->GetConnectServerReconnecTime()))
		{
			sConnectServer->Start();
		}
	}

	sConnectServer->UpdatePacket();

	if (sGameServer->IsAuthServerReconnect() && sAuthServer->IsStopped())
	{
		if (sAuthServer->GetReconnectTime()->Elapsed(sGameServer->GetAuthServerReconnecTime()))
		{
			sAuthServer->Start();
		}
	}

	sAuthServer->UpdatePacket();

	if (sGameServer->IsDataServerServerReconnect() && sDataServer->IsStopped())
	{
		if (sDataServer->GetReconnectTime()->Elapsed(sGameServer->GetDataServerServerReconnecTime()))
		{
			sDataServer->Start();
		}
	}

	sDataServer->UpdatePacket();

	Custom::SystemTimer cur_time = Custom::SystemTimer();

	if (cur_time.GetDay() != g_SiegeLordMixDay)
	{
		g_SiegeLordMixDay = cur_time.GetDay();
		g_SiegeLordMixCount = sGameServer->GetCastleSiegeLordMixMax();
	}

	if (m_timers[WUPDATE_PINGDB].Passed())
	{
		m_timers[WUPDATE_PINGDB].Reset();

		KeepDatabaseAlive();
	}

}

void MainApplication::LoadAccountTestGS(char* pchFileName) {
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("AccountTest");

	for (pugi::xml_node account = Main.child("Account"); account; account = account.next_sibling()) {
		AccTest* data = new AccTest;
		sprintf(data->UserName, account.attribute("UserName").as_string());

		this->m_acc_test.push_back(data);
	}
}

bool MainApplication::CheckAccountTestLoginSV(const char* UserName) {
	for (AccountTest::const_iterator acc = this->m_acc_test.begin(); acc != this->m_acc_test.end(); acc++) {
		AccTest const* acc_data = (*acc);

		if (!acc_data)
			continue;

		if (strcmp(acc_data->UserName, UserName) == 0)
			return true;
	}

	return false;
}