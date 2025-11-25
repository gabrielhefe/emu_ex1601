#ifndef SERVER_LIST_H
#define SERVER_LIST_H

struct SERVER_DATA_HEAD
{
	PWMSG_HEAD2 h;
	uint8 countH;
	uint8 countL;
};

struct SERVER_DATA_BODY
{
	uint16 server_code;
	uint8 percent;
	uint8 type;
};

struct SERVER_INFO_REQUEST
{
	PBMSG_HEAD2 h;
	uint16 server_code;
};

typedef std::set<uint16> WorldServerList;
typedef std::map<uint16, WorldServerList> WorldServerMap;

struct ServerData
{
        ServerData()
        {
                this->m_Online = false;
                this->m_Percent = 0;
                this->m_Port = -1;
                memset(this->m_IP, 0, sizeof(this->m_IP));
                this->m_Flag = 0;
                this->m_Type = 0;
        }

        bool m_Online;
        uint8 m_Percent;
        uint16 m_Port;
        char m_IP[16];
        uint8 m_Flag;
        uint8 m_Type;
};

typedef std::map<uint16, ServerData> ServerDataMap;

class CServerList
{
	SingletonInstance(CServerList);

	public:
		CServerList();
		virtual ~CServerList();

		bool IsServerOnlineAndFree(uint16 server);
		bool IsWorldInList(uint16 world, uint16 server_group);

		void Initialize(uint32 updateInterval);
		void LoadWorldServer(char* pchFileName);
		void UpdateServerList();
		
		void ServerOpen(uint16 server, uint16 port, const char * ip, uint8 flag, uint8 type);
		void ServerClose(uint16 server);
		void ServerSetInfo(uint16 server, uint8 percent, uint8 flag, uint8 type);

		uint16 CheckDestServer(uint16 server_group, uint16 world, uint16 server, uint16 start_server);

		uint32   m_UpdateInterval;
		time_t   m_NextUpdateTime;

		WorldServerMap world_server_map;
		ServerDataMap server_map;
};

#endif