#ifndef SYSTEM_MESSAGE_H
#define SYSTEM_MESSAGE_H

#include <map>
#include <string>

struct SystemMessageData
{
        uint32 m_ID;
        uint8 m_Language;
        std::string m_Text;
};

typedef std::map<uint32, SystemMessageData*> SystemMessageMap;

class SystemMessage
{
        SingletonInstance(SystemMessage);

        public:
                SystemMessage();
                virtual ~SystemMessage();

                void Init(char* pchFileName);
                std::string Get(uint32 id) const;
        private:
                SystemMessageMap m_MessageMap;
};

#endif
