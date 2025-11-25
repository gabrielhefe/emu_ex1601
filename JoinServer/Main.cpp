bool StartNetwork();

Config * sConfig = new Config;
Config * sConfigCommon = sConfig;

boost::asio::io_service _ioService;
boost::asio::signal_set signals(_ioService, SIGINT, SIGTERM);

MainApplication::MainApplication(): AppConsole("JoinServer", "JOIN SERVER", MAIN_DB_FLAG_MU_LOGIN, sConfig)
{
}

MainApplication::~MainApplication()
{
	
}

int main()
{
	sLog->LoadFolder();

	sLog->LoadAppender("Console", APPENDER_CONSOLE, LOG_LEVEL_TRACE, APPENDER_FLAGS_NONE, "", "", "w");
	sLog->LoadAppender("JoinServer", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "JoinServer.log", "w");
	sLog->LoadAppender("Database", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "Database.log", "w");
	sLog->LoadAppender("Player", APPENDER_FILE, LOG_LEVEL_TRACE, AppenderFlags(27), "", "Player.log", "w");

	sLog->LoadLogger("root", LOG_LEVEL_TRACE, " Console JoinServer");
	sLog->LoadLogger("database", LOG_LEVEL_TRACE, "Database");
	sLog->LoadLogger("player", LOG_LEVEL_TRACE, "Player");

	//StartStackLogging();
	if ( !InitConfig("") )
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
	sAuthSocketMgr.StopNetwork();

	AppConsole::End();
}

bool MainApplication::Run()
{
	sServer->Initialize(sConfig->GetIntDefault("ServerStateUpdateDelay", 0));

	LoginDatabase.Execute(LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNT_CLEAN));

	if ( !StartNetwork() )
		return false;

        this->m_GSMultiSubEnabled = GetPrivateProfileInt(SERVER_COMMON_SECTION_GERENAL, "GSMultiSubEnabled", 0, SERVER_COMMON_FILEPATH);

	m_timers[WUPDATE_QUEUE].SetInterval(100);
	m_timers[WUPDATE_PINGDB].SetInterval(this->GetPingDB() * MINUTE * IN_MILLISECONDS);
	m_timers[WUPDATE_UPD].SetInterval(1000);

	return true;
}

bool StartNetwork()
{
	uint16 worldPort = uint16(sConfig->GetIntDefault("LoginNetwork.Port", 0));
	std::string worldListener = sConfig->GetStringDefault("LoginNetwork.BindIP", "0.0.0.0");

	if ( !sAuthSocketMgr.StartNetwork(_ioService, worldListener, worldPort, 1) )
		return false;

	return true;
}

void MainApplication::LoadAccountTime()
{
	if (boost::filesystem::exists(SERVER_COMMON_FILEPATH)) {
                this->m_AccountMovingTime = GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "MovingTime", 0, SERVER_COMMON_FILEPATH);
                this->m_AccountAuthorizationEnabled = GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "AuthorizationEnabled", 0, SERVER_COMMON_FILEPATH);
                this->m_AccountWrongAuthorizationCount = GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "WrongAuthorizationCount", 0, SERVER_COMMON_FILEPATH);
                this->m_AccountWrongAuthorizationTime = GetPrivateProfileInt(SERVER_COMMON_SECTION_ACCOUNT, "WrongAuthorizationTime", 0, SERVER_COMMON_FILEPATH);
	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Common.ini File");
		MessageBox(NULL, "Error Load Common.ini File", "Error", ERROR);
		exit(0);
	}
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

	if ( m_timers[WUPDATE_QUEUE].Passed() )
	{
		m_timers[WUPDATE_QUEUE].Reset();

		sLoginQueue->Update();
	}

	if ( m_timers[WUPDATE_UPD].Passed() )
	{
		m_timers[WUPDATE_UPD].Reset();

		sServer->UpdateServerList();
	}

	if (m_timers[WUPDATE_PINGDB].Passed())
	{
		m_timers[WUPDATE_PINGDB].Reset();

		KeepDatabaseAlive();
	}
}