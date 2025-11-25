#ifndef LOGIN_QUEUE_H
#define LOGIN_QUEUE_H

struct LoginData
{
	explicit LoginData(uint16 entry, const char * account, const char * password): 
		entry(entry),
		server(-1),
		ms_time(0),
		disk_serial(0),
		account_id(0),
		autologin(0)
	{
		memcpy(this->account, account, MAX_ACCOUNT_LENGTH + 1);
		memcpy(this->password, password, MAX_PASSWORD_LENGTH + 1);
	}

	uint16 server;
	uint16 entry;
	uint32 ms_time;
	char account[MAX_ACCOUNT_LENGTH + 1];
	char password[MAX_PASSWORD_LENGTH + 1];
	std::shared_ptr<AuthSocket> socket;
	char ip[MAX_ACCOUNT_IP_LENGTH + 1];
	char mac[MAX_ACCOUNT_MAC_LENGTH + 1];
	uint32 disk_serial;
	uint32 account_id;
	uint8 autologin;

	uint16 GetServerGroup() const { return this->server / 20; }
};

typedef std::shared_ptr<LoginData> LoginDataPtr;

struct AccountData
{
        AccountData()
        {
                memset(this->account, 0, MAX_ACCOUNT_LENGTH + 1);
                this->guid = 0;
                this->m_StartServer = -1;
                this->m_CurrentServer = -1;
                this->m_Offline = false;
                this->m_Moving = false;
                this->m_Time = time(nullptr);
                this->m_DestServer = -1;
                this->m_DestWorld = -1;
                this->m_DestX = 0;
                this->m_DestY = 0;
                memset(this->m_Auth, 0, sizeof(this->m_Auth));
                memset(this->m_MAC, 0, sizeof(this->m_MAC));
                memset(this->m_SecureCode, 0, sizeof(this->m_SecureCode));
                this->m_FacebookStatus = 0;
                this->m_MovingTick = 0;
                memset(this->m_IP, 0, sizeof(this->m_IP));
                this->m_GoldenChannel = 0;
                this->m_DiskSerial = 0;
                this->m_Authorized = false;
                this->m_AccType = 0;
        }

	char account[MAX_ACCOUNT_LENGTH + 1];
	
	uint32 guid;

        time_t m_Time;
        uint16 m_StartServer;
        uint16 m_CurrentServer;
        bool m_Offline;
        bool m_Moving;
        uint32 m_MovingTick;
        int16 m_DestServer;
        int16 m_DestWorld;
        int16 m_DestX;
        int16 m_DestY;
        int32 m_Auth[4];
        char m_MAC[MAX_ACCOUNT_MAC_LENGTH + 1];
        char m_SecureCode[MAX_SECURE_CODE_LENGTH + 1];
        char m_IP[MAX_ACCOUNT_IP_LENGTH + 1];
        uint8 m_FacebookStatus;
        time_t m_GoldenChannel;
        uint32 m_DiskSerial;
        bool m_Authorized;
        uint8 m_AccType;
};

typedef std::shared_ptr<AccountData> AccountDataPtr;
typedef std::unordered_map<uint32, AccountDataPtr> AccountLoggedList;
typedef std::unordered_map<uint16, AccountLoggedList> AccountLoggedMap;

struct WrongAuthorizationData
{
	WrongAuthorizationData(uint32 account_id, uint32 disk_serial, int32 count): account_id(account_id), disk_serial(disk_serial), count(count)
	{

	}

	uint32 account_id;
	uint32 disk_serial;
	int32 count;
};

typedef std::vector<WrongAuthorizationData> WrongAuthorizationDataList;

class LoginQueue
{
	SingletonInstance(LoginQueue);

	public:
		LoginQueue();
		virtual ~LoginQueue();

		void Queue(LoginDataPtr data);
		void Update();
		void Process(LoginDataPtr data);

		void DisconnectAccount(uint16 server, uint32 account);

		void CloseServer(uint16 server);

		void KickAccount(uint16 server_group, uint32 account_id);

		bool IsAccountLogged(uint16 server_group, const char * account);
		AccountDataPtr GetAccountData(uint16 server_group, uint32 account);
		AccountDataPtr GetAccountData(uint16 server_group, const char * account);

		void DBAccountStatus(AccountDataPtr pAccount, bool online, bool on_connect);

		void ConnectAccount(uint16 start_server, uint16 current_server, const char * account, uint32 guid, const char * secure_code, const char * ip, const char * mac, uint8 facebook_status, time_t golden_channel, uint32 disk_serial, bool authorized, uint8 acctype);

		void CompleteAccountBan(const char * ip, const char * mac, uint32 disk_serial, uint8 type);

		void ClearWrongAccountAuthorization(uint32 account_id, uint32 disk_serial);
		bool IncreaseWrongAccountAuthorization(uint32 account_id, uint32 disk_serial);
        private:
                LockedQueue<LoginDataPtr> LoginDataQueue;

                AccountLoggedMap account_logged_map;
                WrongAuthorizationDataList account_wrong_authorization_list;

                uint32 m_UpdateTick;
                uint32 m_VerifyTick;
};

#endif