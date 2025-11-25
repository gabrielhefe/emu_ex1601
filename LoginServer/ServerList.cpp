CServerList::CServerList() : m_UpdateInterval(0), m_NextUpdateTime(time(NULL))
{

}

CServerList::~CServerList()
{
	this->world_server_map.clear();
	this->server_map.clear();
}

void CServerList::Initialize(uint32 updateInterval)
{
    m_UpdateInterval = updateInterval;

	this->LoadWorldServer("../Data/World/WorldServer.xml");
	sMain->LoadAccountTime();
}

void CServerList::UpdateServerList()
{
	 if (!m_UpdateInterval || m_NextUpdateTime > time(NULL))
        return;

    m_NextUpdateTime = time(NULL) + m_UpdateInterval;

	this->LoadWorldServer("../Data/World/WorldServer.xml");
	sMain->LoadAccountTime();
}

void CServerList::LoadWorldServer(char* pchFileName)
{
        if (!sMain->m_GSMultiSubEnabled)
                return;

	sLog->outInfo(LOG_DEFAULT, "Loading World Server...");

	this->world_server_map.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldServer");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Option"); option; option = option.next_sibling()) {
		uint16 server = option.attribute("ServerCode").as_int();
		uint16 world = option.attribute("WorldID").as_int();

		this->world_server_map[server].insert(world);

		++count;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world server definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void CServerList::ServerOpen(uint16 server, uint16 port, const char * ip, uint8 flag, uint8 type)
{
	ServerData & data = this->server_map[server];
        data.m_Port = port;
        memcpy(data.m_IP, ip, sizeof(data.m_IP));
        data.m_Percent = 0;
        data.m_Flag = flag;
        data.m_Type = type;
        data.m_Online = true;
}

void CServerList::ServerClose(uint16 server)
{
	ServerData & data = this->server_map[server];
        data.m_Online = false;
        data.m_Percent = 0;

	sLoginQueue->CloseServer(server);
}

void CServerList::ServerSetInfo(uint16 server, uint8 percent, uint8 flag, uint8 type)
{
	ServerData & data = this->server_map[server];

        if ( !data.m_Online )
        {
                return;
        }

        data.m_Percent = percent;
        data.m_Flag = flag;
        data.m_Type = type;
}

bool CServerList::IsServerOnlineAndFree(uint16 server)
{
	ServerData & data = this->server_map[server];

        if ( !data.m_Online )
        {
                return false;
        }

        if ( (data.m_Flag & SERVER_FLAG_DISABLED) != 0 )
        {
                return false;
        }

        return data.m_Percent < 100;
}

bool CServerList::IsWorldInList(uint16 world, uint16 server_group)
{
	for ( WorldServerMap::const_iterator itr = this->world_server_map.begin(); itr != this->world_server_map.end(); ++itr )
	{
		if ( (itr->first / MAX_SERVER_PER_GROUP) != server_group )
			continue;

		WorldServerList::const_iterator it = itr->second.find(world);

		if ( it != itr->second.end() )
			return true;
	}

	return false;
}

uint16 CServerList::CheckDestServer(uint16 server_group, uint16 world, uint16 server, uint16 start_server)
{
	bool is_in_list = this->IsWorldInList(world, server_group);
	
	if ( !is_in_list )
	{
		server = start_server;
	}
	
	if ( !this->IsServerOnlineAndFree(server) )
	{
		server = -1;
	}
	else
	{
		WorldServerMap::const_iterator world_list_iterator = this->world_server_map.find(server);

		if ( is_in_list )
		{
			if ( world_list_iterator != this->world_server_map.end() )
			{
				WorldServerList::const_iterator it = world_list_iterator->second.find(world);

				if ( it != world_list_iterator->second.end() )
					return server;
			}
		}
		else
		{
			if ( world_list_iterator == this->world_server_map.end() )
				return server;
		}

		server = -1;
	}

	if ( is_in_list )
	{
		for ( WorldServerMap::const_iterator itr = this->world_server_map.begin(); itr != this->world_server_map.end(); ++itr )
		{
			if ( server_group != (itr->first / MAX_SERVER_PER_GROUP) )
				continue;

			WorldServerList::const_iterator it = itr->second.find(world);

			if ( it != itr->second.end() )
			{
				if ( this->IsServerOnlineAndFree(itr->first) )
					return itr->first;
			}
		}
	}
	else
	{
		for ( ServerDataMap::const_iterator it = this->server_map.begin(); it != this->server_map.end(); ++it )
		{
			if ( server_group != (it->first / MAX_SERVER_PER_GROUP) )
				continue;

			WorldServerMap::const_iterator itr = this->world_server_map.find(it->first);

			if ( itr != this->world_server_map.end() ) // Si el servidor tiene mapas sigo buscando
				continue;

			if ( this->IsServerOnlineAndFree(it->first) )
				return it->first;
		}
	}

	return -1;
}