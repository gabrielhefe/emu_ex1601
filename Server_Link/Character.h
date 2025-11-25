#ifndef CHARACTER_H
#define CHARACTER_H

struct CharacterData
{
CharacterData()
{
this->m_ID = 0;
memset(this->m_Name, 0, sizeof(this->m_Name));
this->m_Server = -1;
this->m_Guild = 0;
this->m_Authority = 0;
this->m_AdminPanelFlags = 0;
}

uint32 m_ID;
char m_Name[MAX_CHARACTER_LENGTH + 1];
uint16 m_Server;
uint32 m_Guild;
uint8 m_Authority;
uint32 m_AdminPanelFlags;

	std::shared_ptr<ServerSocket> socket;
};

typedef std::shared_ptr<CharacterData> CharacterDataPtr;
typedef std::unordered_map<uint32, CharacterDataPtr> CharacterDataMap;

class CharacterMgr
{
	SingletonInstance(CharacterMgr);

	public:
		CharacterMgr();
		virtual ~CharacterMgr();

		void Update();
		void LoginLogout(uint8 * Packet, std::shared_ptr<ServerSocket> socket);

		CharacterDataPtr GetCharacterData(uint32 id) const;
		CharacterDataPtr GetCharacterData(const char * name) const;

		void CharacterUpdateStatus(uint32 id, uint16 server, bool online);
	private:
		CharacterDataMap character_data_map;
};

#endif