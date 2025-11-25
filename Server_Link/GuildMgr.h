#ifndef GUILD_MGR_H
#define GUILD_MGR_H

struct GuildMember
{
uint32 m_ID;
uint8 m_Ranking;
char m_Name[(MAX_CHARACTER_LENGTH + 1)];
uint16 m_Server;

void Reset()
{
this->m_ID = 0;
this->m_Ranking = GUILD_RANK_NONE;
memset(this->m_Name, 0, sizeof(this->m_Name));
this->m_Server = -1;
}
};

class Guild
{
	public:
		explicit Guild();
		virtual ~Guild();

		void Create(uint32 id, const char * name, uint8 * emblem, bool db);
		void AddMember(uint32 id, const char * name, uint8 ranking, uint8 slot, uint16 server, bool db);

uint8 GetEmptySlot() const;
uint8 GetMemberSlot(const char * name) const;
		
uint8 GetTotalMembers() const;
std::string BuildLog() const;
		void UpdateRelationshipDB();
GuildMember* GetMemberData(uint32 id);

		void ResetMatching();

bool m_Remove;
uint32 m_ID;
uint32 m_Alliance;
uint32 m_Hostil;
int32 m_Score;
uint32 m_RegisteredMarks;
bool m_CastleSiegeRegistered;
char m_Name[MAX_GUILD_NAME_LENGTH + 1];
uint8 emblem[MAX_GUILD_EMBLEM_LENGTH];
char m_Notice[(MAX_GUILD_NOTICE_LENGTH + 1)];
GuildMember m_Member[MAX_GUILD_MEMBER];

		///- Matching
bool m_MatchingEnabled;
char m_MatchingText[GUILD_MATCHING_TEXT_LENGTH + 1];
uint8 m_MatchingInterestType;
uint8 m_MatchingLevelRange;
uint16 m_MatchingClassType;
uint32 m_MatchingBoardNumber;
uint8 m_MatchingMasterClass;
uint32 m_MatchingMasterLevel;
uint8 m_MatchingGensType;
};

typedef std::map<uint32, Guild*> GuildMap;

class GuildMgr
{
	SingletonInstance(GuildMgr);

	public:
		GuildMgr();
		virtual ~GuildMgr();

		void LoadGuilds();
		void LoadGuildMembers();

		Guild* GetGuild(uint32 id) const;
		Guild* GetGuild(std::string name) const;

		void AddGuild(Guild* pGuild);
		void RemoveGuild(uint32 id);
		uint8 GetInvolvedGuilds(uint32 alliance) const;

		void CreateRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void RemoveRequest(uint8 * Packet);
		void MemberAddRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void MemberDelRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket);
		void MemberStatusRequest(uint8 * Packet);
		void RelationshipRequest(uint8 * Packet);
		void Score(uint8 * Packet);
		void Notice(uint8 * Packet);
		void MemberOnlineOffline(CharacterDataPtr pCharacterData);

	public:
		GuildMap guild_map;
};

#endif