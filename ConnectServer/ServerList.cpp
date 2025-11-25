CServerList::CServerList() : m_UpdateInterval(0), m_NextUpdateTime(time(NULL))
{

}

CServerList::~CServerList()
{
MAP_CLEAR(ServerDataMap::iterator, this->m_ServerMap);
}

ServerData * CServerList::FindServer(uint16 code)
{
        ServerDataMap::const_iterator it = this->m_ServerMap.find(code);

        return it != this->m_ServerMap.end() ? it->second : nullptr;
}

ServerData const* CServerList::FindServerByDisplay(uint16 id) const
{
        for (ServerDataMap::const_iterator itr = this->m_ServerMap.begin(); itr != this->m_ServerMap.end(); ++itr)
        {
                ServerData const* pData = itr->second;

		if (!pData)
		{
			continue;
		}

                if (pData->m_DisplayID == id)
                {
                        return pData;
                }
        }

	return nullptr;
}

void CServerList::Initialize(uint32 updateInterval)
{
    this->m_UpdateInterval = updateInterval;

	this->LoadServerList("../Data/ServerMap.xml");
	sMain->LoadAccountTime();
	
	//LoginDatabase.Execute("UPDATE server_list SET online = 0");
}

void CServerList::UpdateServerList()
{
         if (!this->m_UpdateInterval || this->m_NextUpdateTime > time(NULL))
        return;

    this->m_NextUpdateTime = time(NULL) + this->m_UpdateInterval;

	this->LoadServerList("../Data/ServerMap.xml");
	sMain->LoadAccountTime();
	sMain->LoadVersion();
}

void CServerList::SendChannel()
{
	uint8 buffer[4092];
	POINTER_PCT(CS_CHANNEL_HEAD, head, buffer, 0);
	POINTER_PCT(CS_CHANNEL_BODY, body, buffer, sizeof(CS_CHANNEL_HEAD));

	for (int32 i = 0; i < 10; ++i)
	{
		head->count = 0;

                for (ServerDataMap::const_iterator itr = this->m_ServerMap.begin(); itr != this->m_ServerMap.end(); ++itr)
		{
			ServerData const* pData = itr->second;

			if (!pData)
			{
				continue;
			}

                        if ((pData->m_Server / MAX_SERVER_PER_GROUP) != i)
			{
				continue;
			}

                        if (!pData->m_Active || !pData->m_Online || !pData->IsFlag(SERVER_FLAG_DISPLAY) || pData->IsFlag(SERVER_FLAG_DISABLED))
                        {
                                continue;
                        }

                        body[head->count].server = pData->m_Server;
                        body[head->count].data1 = 0;
                        body[head->count].data2 = 0;
                        body[head->count].type = pData->m_Type & 1;
                        body[head->count].gold = pData->m_Type >= 2 ? 1 : 0;
			++head->count;
		}

		if (head->count <= 0)
		{
			continue;
		}

		head->h.set(HEADCODE_CONNECT_SERVER_CHANNELS, sizeof(CS_CHANNEL_HEAD) + (head->count * sizeof(CS_CHANNEL_BODY)));

		sGSSocketMgr.SendPacketAll(buffer, head->h.size, i);
	}
}

void CServerList::LoadServerList(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Reading Server List...");

	/*QueryResult result = LoginDatabase.Query("SELECT * FROM server_list");
	int32 count = 0;

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			uint16 server = fields[0].GetUInt16();
			bool add = false;

			ServerData * add_server = this->FindServer(server);

			if ( !add_server )
			{
				add_server = new ServerData;
				add = true;
			}

			add_server->SetServer(server);
			add_server->SetDisplayID(fields[1].GetUInt16());
			add_server->SetName(fields[2].GetString());
			add_server->SetPort(fields[3].GetUInt16());
			add_server->SetIP(fields[4].GetString());
			add_server->SetFlag(fields[5].GetUInt8());
			add_server->SetType(fields[10].GetUInt8());

			if ( add )
			{
				add_server->SetPercent(0);
				add_server->SetOnline(false);
				add_server->SetActive(false);
                                this->m_ServerMap[add_server->m_Server] = add_server;
			}

			if ( !add_server->IsActive() )
			{
				add_server->AddFlag(SERVER_FLAG_DISABLED);
			}
			else
			{
				add_server->RemoveFlag(SERVER_FLAG_DISABLED);
			}
			
			sLog->outInfo(LOG_DEFAULT, "Added Server %u [%s]    Port: %u   IP: %s   Display: %d	AccountAllowed: %d	Disabled: %d", add_server->m_Server,
			add_server->m_Name.c_str(), add_server->m_Port, add_server->m_Ip.c_str(), 
			add_server->IsFlag(SERVER_FLAG_DISPLAY), 
			add_server->IsFlag(SERVER_FLAG_ACCOUNT_ALLOWED),
			add_server->IsFlag(SERVER_FLAG_DISABLED));

			++count;
		}
		while(result->NextRow());
	}*/

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	uint8 count = 0;

	pugi::xml_node Main = file.child("ServerMap");

	for (pugi::xml_node server = Main.child("Server"); server; server = server.next_sibling()) {
		uint16 servercode = server.attribute("ServerCode").as_int();
		bool add = false;
		ServerData* add_server = this->FindServer(servercode);

		if (!add_server)
		{
			add_server = new ServerData;
			add = true;
		}

                add_server->m_Server = servercode;
                add_server->m_DisplayID = server.attribute("ServerIndex").as_int();
                add_server->m_Name = server.attribute("ServerName").as_string();
                add_server->m_Port = server.attribute("ServerPort").as_int();
                add_server->m_Ip = server.attribute("ServerIP").as_string();
                add_server->m_Flag = server.attribute("Display").as_int();
                add_server->m_Type = server.attribute("ServerType").as_int();

                if (add)
                {
                        add_server->m_Percent = 0;
                        add_server->m_Online = false;
                        add_server->m_Active = false;
                        this->m_ServerMap[add_server->m_Server] = add_server;
                }

                if (!add_server->m_Active)
                {
                        add_server->AddFlag(SERVER_FLAG_DISABLED);
                }
		else
		{
			add_server->RemoveFlag(SERVER_FLAG_DISABLED);
		}

		sLog->outInfo(LOG_DEFAULT, "Added Server %u [%s]    Port: %u   IP: %s   Display: %d	AccountAllowed: %d	Disabled: %d", add_server->m_Server,
			add_server->m_Name.c_str(), add_server->m_Port, add_server->m_Ip.c_str(),
			add_server->IsFlag(SERVER_FLAG_DISPLAY),
			add_server->IsFlag(SERVER_FLAG_ACCOUNT_ALLOWED),
			add_server->IsFlag(SERVER_FLAG_DISABLED));

		++count;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u servers", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CServerList::ServerListRequest(std::shared_ptr<MainSocket> client)
{
	this->UpdateServerList();

        if ( this->m_ServerMap.empty() )
	{
		client->CloseSocket();
		return;
	}

	uint8 buffer[10000];
	POINTER_PCT(SERVER_DATA_HEAD, head, buffer, 0);
	POINTER_PCT(SERVER_DATA_BODY, body, buffer, sizeof(SERVER_DATA_HEAD));

	head->h.set(HEADCODE_CONNECT_SERVER, SUBCODE_CONNECT_SERVER_SERVER_LIST, sizeof(SERVER_DATA_HEAD));

	uint16 size = sizeof(SERVER_DATA_HEAD);
	uint16 count = 0;

        for (ServerDataMap::const_iterator it = this->m_ServerMap.begin(); it != this->m_ServerMap.end(); ++it)
	{
		ServerData const* pData = it->second;

		if (!pData)
		{
			continue;
		}

		if (!pData->IsFlag(SERVER_FLAG_DISPLAY) || pData->IsFlag(SERVER_FLAG_DISABLED) || !pData->m_Online)
		{
			continue;
		}

		body[count].server_code = pData->m_DisplayID;
		body[count].percent = pData->m_Percent;
		body[count].type = pData->m_Type;
		size += sizeof(SERVER_DATA_BODY);
		count++;
	}

	if ( count <= 0)
	{
		client->CloseSocket();
		return;
	}

	head->countH = SET_NUMBERH(count);
	head->countL = SET_NUMBERL(count);
	head->h.set_size(size);

	client->SendPacket(buffer, size);
}

void CServerList::ServerSelectRequest(std::shared_ptr<MainSocket> client, uint8 * Packet)
{
	POINTER_PCT(SERVER_INFO_REQUEST, lpMsg, Packet, 0);

        ServerData const* server_data = this->FindServerByDisplay(lpMsg->server_code);

	if ( !server_data )
	{
		client->CloseSocket();
		return;
	}

        if ( !server_data->IsFlag(SERVER_FLAG_DISPLAY) || server_data->IsFlag(SERVER_FLAG_DISABLED) || !server_data->m_Online )
        {
                client->CloseSocket();
                return;
        }

	SERVER_INFO_RESULT pMsg(server_data->m_Ip.c_str(), server_data->m_Port);

	client->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void CServerList::ServerSetInfo(std::shared_ptr<GSSocket> client, uint8 * Packet)
{
	POINTER_PCT(CS_GAMESERVER_USER_PERCENT, lpMsg, Packet, 0);

	ServerData * server_data = this->FindServer(lpMsg->h.server);

	if ( !server_data )
	{
		return;
	}

        server_data->m_Percent = lpMsg->percent;
        server_data->m_Active = lpMsg->active;
	
        if ( !lpMsg->active )
        {
                server_data->AddFlag(SERVER_FLAG_DISABLED);
        }
	else
	{
		server_data->RemoveFlag(SERVER_FLAG_DISABLED);
	}

	CS_GAMESERVER_FLAG pMsg(server_data->m_Flag);
	client->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void CServerList::ServerVersionCheck(std::shared_ptr<MainSocket> client, uint8 * Packet)
{
	POINTER_PCT(MU_VERSION_CHECK, lpMsg, Packet, 0);

	MU_VERSION_CHECK_RESULT pMsg;
        memcpy(pMsg.address, sMain->m_UpdateAddress.c_str(), sMain->m_UpdateAddress.length());
	pMsg.data[0] = 1;
	pMsg.data[1] = 1;

	client->SendPacket((uint8*)&pMsg, pMsg.h.get_size());
}

void CServerList::LauncherError(std::shared_ptr<MainSocket> client, uint8 * Packet)
{
	POINTER_PCT(MU_LAUNCHER_ERROR, lpMsg, Packet, 0);

	sLog->outError("root", "LauncherError: %s", lpMsg->path_error);
}

void CServerList::ServerConnect(uint16 server)
{
	if ( ServerData * server_data = this->FindServer(server) )
	{
                server_data->m_Online = true;
		sLog->outInfo("root", "[ SERVER CONNECT ] Connecting Server - %s / %d", server_data->m_Name.c_str(), server);
	}

	/*SQLTransaction trans = LoginDatabase.BeginTransaction();

	PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GAMESERVER);
	stmt->setUInt8(0, 1);
	stmt->setUInt16(1, server);
	trans->Append(stmt);

	LoginDatabase.CommitTransaction(trans);*/
}

void CServerList::ServerClose(uint16 server)
{
	if ( ServerData * server_data = this->FindServer(server) )
	{
                server_data->m_Online = false;
		sLog->outInfo("root", "[ SERVER CLOSE ] Closing Server - %s / %d", server_data->m_Name.c_str(), server);
	}

	/*SQLTransaction trans = LoginDatabase.BeginTransaction();

	PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_GAMESERVER);
	stmt->setUInt8(0, 0);
	stmt->setUInt16(1, server);
	trans->Append(stmt);

	LoginDatabase.CommitTransaction(trans);*/
}
