#ifndef MAIN_H
#define MAIN_H

#include <cstddef>
#include <string>

enum UPDATE_DOWNLOAD_TYPE
{
	UPDATE_DOWNLOAD_TYPE_FTP,
	UPDATE_DOWNLOAD_TYPE_FTP_TEST,
	UPDATE_DOWNLOAD_TYPE_HTTP,
	UPDATE_DOWNLOAD_TYPE_MAX
};

struct UpdateDownloadData
{
	std::string address;
	uint16 port;
	std::string user;
	std::string password;
};

enum WorldTimers
{
	WUPDATE_PINGDB,
	WUPDATE_CHANNEL,
    WUPDATE_COUNT
};

class MainApplication: public AppConsole
{
        SingletonInstance(MainApplication);

        public:
		MainApplication();
		virtual ~MainApplication();

		void Load() { }
		void CreateSetting(std::string const& key, std::string const& default) { }

                bool Run();
                void Update(uint32 diff);
                void UpdateWin();
                void End();

                uint8 m_Version[3];
                std::string m_Filename;

                uint8 m_DownloadType = 0;
                UpdateDownloadData m_DownloadData[UPDATE_DOWNLOAD_TYPE_MAX] = {};

        public:
                void LoadVersion();
                void LoadAccountTime();
                size_t m_MaxConnectionCount = 0;
                uint32 m_MaxConnectionIdle = 0;

                IntervalTimer m_Timers[WUPDATE_COUNT];

                uint8 m_UpdateHeadcode = 0;
                std::string m_UpdateAddress;
};

#endif
