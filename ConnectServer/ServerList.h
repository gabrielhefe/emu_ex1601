#ifndef SERVER_LIST_H
#define SERVER_LIST_H

#include <map>
#include <string>

struct ServerData
{
uint16 m_Server = 0;
uint16 m_DisplayID = 0;
std::string m_Name;
uint16 m_Port = 0;
std::string m_Ip;
uint8 m_Flag = 0;
uint8 m_Percent = 0;
bool m_Online = false;
uint8 m_Type = 0;
bool m_Active = false;
void AddFlag(uint8 value) { this->m_Flag |= value; }
void RemoveFlag(uint8 value) { this->m_Flag &= ~value; }
bool IsFlag(uint8 value) const { return (this->m_Flag & value) != 0; }
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
