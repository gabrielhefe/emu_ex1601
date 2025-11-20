#ifndef SERVER_LIST_H
#define SERVER_LIST_H

#include <map>
#include <string>

struct ServerData
{
        uint16 server = 0;
        uint16 displayID = 0;
        std::string name;
        uint16 port = 0;
        std::string ip;
        uint8 flag = 0;
        uint8 percent = 0;
        bool online = false;
        uint8 type = 0;
        bool active = false;
        void AddFlag(uint8 value) { flag |= value; }
        void RemoveFlag(uint8 value) { flag &= ~value; }
        bool IsFlag(uint8 value) const { return (flag & value) != 0; }
};

typedef std::map<uint16, ServerData*> ServerDataMap;

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

class CServerList
{
	SingletonInstance(CServerList);

        public:
                CServerList();
                virtual ~CServerList();

                ServerDataMap m_ServerMap;
		ServerData * FindServer(uint16 code);
		ServerData const* FindServerByDisplay(uint16 id) const;

		void Initialize(uint32 updateInterval);
		void LoadServerList(char* pchFileName);
		void UpdateServerList();
		void SendChannel();
		
		void ServerListRequest(std::shared_ptr<MainSocket> client);
		void ServerSelectRequest(std::shared_ptr<MainSocket> client, uint8 * Packet);
		void ServerSetInfo(std::shared_ptr<GSSocket> client, uint8 * Packet);
		void ServerVersionCheck(std::shared_ptr<MainSocket> client, uint8 * Packet);
		void LauncherError(std::shared_ptr<MainSocket> client, uint8 * Packet);

		void ServerConnect(uint16 server);
		void ServerClose(uint16 server);

		uint32   m_UpdateInterval;
		time_t   m_NextUpdateTime;
};

#endif
