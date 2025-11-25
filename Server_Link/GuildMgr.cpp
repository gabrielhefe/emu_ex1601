Guild::Guild()
{
this->m_ID = 0;
this->m_Alliance = 0;
this->m_Hostil = 0;
this->m_Score = 0;
this->m_RegisteredMarks = 0;
memset(this->m_Name, 0, sizeof(this->m_Name));
memset(this->emblem, 0, MAX_GUILD_EMBLEM_LENGTH);
memset(this->m_Notice, 0, sizeof(this->m_Notice));

for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
{
this->m_Member[i].Reset();
}

this->m_Remove = false;
this->m_CastleSiegeRegistered = false;

this->ResetMatching();
}

Guild::~Guild()
{
}

void Guild::Create(uint32 id, const char * name, uint8 * emblem, bool db)
{
this->m_ID = id;
memcpy(this->m_Name, name, sizeof(this->m_Name));
memcpy(this->emblem, emblem, MAX_GUILD_EMBLEM_LENGTH);

	if ( db )
	{
		std::ostringstream ss;
		for ( uint8 i = 0; i < MAX_GUILD_EMBLEM_LENGTH; ++i )
			ss << (uint32)emblem[i] << " ";

		SQLTransaction trans = MuDatabase.BeginTransaction();

PreparedStatement *stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_INSERT);
stmt->setUInt32(0, this->m_ID);
stmt->setString(1, base64::encode((uint8*)this->m_Name, MAX_GUILD_NAME_LENGTH));
stmt->setString(2, ss.str());
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}
}

void Guild::AddMember(uint32 id, const char * name, uint8 ranking, uint8 slot, uint16 server, bool db)
{
this->m_Member[slot].m_ID = id;
memcpy(this->m_Member[slot].m_Name, name, sizeof(this->m_Member[slot].m_Name));
this->m_Member[slot].m_Ranking = ranking;
this->m_Member[slot].m_Server = server;

	if ( db )
	{
		SQLTransaction trans = MuDatabase.BeginTransaction();

PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_INSERT_MEMBER);
stmt->setUInt8(0, slot);
stmt->setUInt32(1, this->m_ID);
stmt->setUInt32(2, id);
stmt->setUInt8(3, ranking);
trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}
}

uint8 Guild::GetEmptySlot() const
{
for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
{
if ( !this->m_Member[i].m_ID )
return i;
}

	return 0;
}

uint8 Guild::GetMemberSlot(const char * name) const
{
for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
{
if ( memcmp(this->m_Member[i].m_Name, name, MAX_CHARACTER_LENGTH) )
{
continue;
}

		return i;
	}

	return -1;
}

uint8 Guild::GetTotalMembers() const
{
uint8 count = 0;

for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
{
if ( !this->m_Member[i].m_ID )
continue;

		++count;
	}

	return count;
}

std::string Guild::BuildLog() const
{
std::ostringstream stream;

stream << "[" << this->m_ID << " - " << this->m_Name << "]";
stream << " - ";
stream << "Master [" << this->m_Member[0].m_Name << "]";

return stream.str();
}

void Guild::UpdateRelationshipDB()
{
SQLTransaction trans = MuDatabase.BeginTransaction();

PreparedStatement*stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_ALLIANCE);
stmt->setUInt32(0, this->m_Alliance);
stmt->setUInt32(1, this->m_ID);
trans->Append(stmt);

stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_HOSTIL);
stmt->setUInt32(0, this->m_Hostil);
stmt->setUInt32(1, this->m_ID);
trans->Append(stmt);

MuDatabase.CommitTransaction(trans);
}

GuildMember* Guild::GetMemberData(uint32 id)
{
for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
{
if ( this->m_Member[i].m_ID == id )
return &this->m_Member[i];
}

return nullptr;
}

void Guild::ResetMatching()
{
this->m_MatchingEnabled = false;
memset(this->m_MatchingText, 0, sizeof(this->m_MatchingText));
this->m_MatchingInterestType = 0;
this->m_MatchingLevelRange = 0;
this->m_MatchingClassType = 0;
this->m_MatchingBoardNumber = 0;
this->m_MatchingMasterClass = 0;
this->m_MatchingMasterLevel = 0;
this->m_MatchingGensType = 0;
}

GuildMgr::GuildMgr()
{
	
}

GuildMgr::~GuildMgr()
{
	MAP_CLEAR(GuildMap::iterator, guild_map);
}

void GuildMgr::LoadGuilds()
{
	uint32 oldMSTime = getMSTime();

	sLog->outInfo(LOG_DEFAULT, "Loading guilds...");

	PreparedQueryResult result = MuDatabase.Query(MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_SELECT));

	uint32 count = 0;

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

Guild* pGuild = new Guild;
pGuild->m_ID = fields[0].GetUInt32();
memcpy(pGuild->m_Name, fields[1].GetBase64String().c_str(), sizeof(pGuild->m_Name));
			
			const char* data = fields[2].GetCString();

			if ( data )
			{
				Tokens tokens(data, ' ');

				for ( uint8 i = 0; i < MAX_GUILD_EMBLEM_LENGTH; ++i )
					pGuild->emblem[i] = cast(uint8, atoi(tokens[i]));
			}

memcpy(pGuild->m_Notice, fields[3].GetBase64String().c_str(), sizeof(pGuild->m_Notice));
pGuild->m_Alliance = fields[4].GetUInt32();
pGuild->m_Hostil = fields[5].GetUInt32();
pGuild->m_Score = fields[6].GetUInt32();
pGuild->m_RegisteredMarks = fields[7].GetUInt32();
pGuild->m_CastleSiegeRegistered = fields[8].GetUInt32() ? true: false;

			this->AddGuild(pGuild);

sLog->outInfo("guild", "[ LOAD ] Added Guild [%u - %s]", pGuild->m_ID, pGuild->m_Name);

			++count;
		}
		while (result->NextRow());
	}

	this->LoadGuildMembers();

	for (GuildMap::iterator it = this->guild_map.begin(); it != this->guild_map.end(); ++it)
	{
		Guild* pGuild = it->second;

		if (!pGuild)
		{
			continue;
		}

pGuild->m_Remove = pGuild->GetTotalMembers() <= 0;
	}

	for ( GuildMap::iterator it = this->guild_map.begin(); it != this->guild_map.end(); )
	{
		Guild* pGuild = it->second;

		if (!pGuild)
		{
			this->guild_map.erase(it++);
			continue;
		}


if (!pGuild->m_Remove)
{
++it;
continue;
}

		SQLTransaction trans = MuDatabase.BeginTransaction();

		PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE_MEMBER);
		stmt->setUInt32(0, it->first);
		trans->Append(stmt);

		stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE);
		stmt->setUInt32(0, it->first);
		trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);

sLog->outInfo("guild", "[ LOAD ] Removed Guild [%u - %s]", pGuild->m_ID, pGuild->m_Name);

		delete it->second;
		this->guild_map.erase(it++);
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u guilds in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
	sLog->outInfo(LOG_DEFAULT, " ");
}

void GuildMgr::LoadGuildMembers()
{
	/*for ( GuildMap::iterator it = this->guild_map.begin(); it != this->guild_map.end(); ++it )
	{
	Guild * pGuild = it->second;

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_MEMBER_SELECT);
	stmt->setUInt32(0, pGuild->GetID());

	PreparedQueryResult result = MuDatabase.Query(stmt);

	if ( result )
	{
	do
	{
	Field* fields = result->Fetch();

	uint8 slot = fields[0].GetUInt8();

	if ( slot >= MAX_GUILD_MEMBER )
	continue;

	pGuild->GetMember(slot)->SetID(fields[1].GetUInt32());
	pGuild->GetMember(slot)->SetRanking(fields[2].GetUInt8());
	pGuild->GetMember(slot)->SetName(fields[3].GetCString());
	pGuild->GetMember(slot)->SetServer(fields[4].GetBool() ? fields[5].GetUInt16(): -1);

	sLog->outInfo("guild", "[ LOAD ] Added Guild [%u - %s] Member [%d - %u - %s]",
	pGuild->GetID(), pGuild->GetName(), slot, pGuild->GetMember(slot)->GetID(), pGuild->GetMember(slot)->GetName());
	}
	while (result->NextRow());
	}
	else
	{
	it->second->SetRemove(true);
	}
	}*/

	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_GUILD_MEMBER_SELECT);

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			uint32 guild = fields[0].GetUInt32();

			Guild* pGuild = this->GetGuild(guild);

			if (!pGuild)
			{
				continue;
			}

			uint8 slot = fields[1].GetUInt8();

			if (slot >= MAX_GUILD_MEMBER)
			{
				continue;
			}

pGuild->m_Member[slot].m_ID = fields[2].GetUInt32();
pGuild->m_Member[slot].m_Ranking = fields[3].GetUInt8();
memcpy(pGuild->m_Member[slot].m_Name, fields[4].GetCString(), sizeof(pGuild->m_Member[slot].m_Name));
pGuild->m_Member[slot].m_Server = fields[5].GetBool() ? fields[6].GetUInt16() : -1;

sLog->outInfo("guild", "[ LOAD ] Added Guild [%u - %s] Member [%d - %u - %s]",
pGuild->m_ID, pGuild->m_Name, slot, pGuild->m_Member[slot].m_ID, pGuild->m_Member[slot].m_Name);
		} while (result->NextRow());
	}
}

Guild* GuildMgr::GetGuild(uint32 id) const
{
	GuildMap::const_iterator it = guild_map.find(id);

	return it != guild_map.end() ? it->second: nullptr;
}
	
Guild* GuildMgr::GetGuild(std::string name) const
{
	strToLower(name);

	for ( GuildMap::const_iterator it = guild_map.begin(); it != guild_map.end(); ++it )
	{
std::string lower_name = it->second->m_Name;
		strToLower(lower_name);

		if ( lower_name == name )
		{
			return it->second;
		}
	}

	return nullptr;
}

void GuildMgr::AddGuild(Guild* pGuild)
{
this->guild_map[pGuild->m_ID] = pGuild;
}

void GuildMgr::RemoveGuild(uint32 id)
{
	GuildMap::const_iterator it = guild_map.find(id);

	if ( it != guild_map.end() )
	{
		delete it->second;
		guild_map.erase(it);
	}
}

uint8 GuildMgr::GetInvolvedGuilds(uint32 alliance) const
{
	if ( !alliance )
		return 0;

	uint8 count = 0;

	for ( GuildMap::const_iterator it = this->guild_map.begin(); it != this->guild_map.end(); ++it )
	{
		if ( !it->second )
			continue;

if ( it->second->m_Alliance != alliance )
			continue;

		++count;
	}

	return count;
}

void GuildMgr::CreateRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_ADD, lpMsg, Packet, 0);

	GuildCreateDataPtr add_data(new GuildCreateData(lpMsg->name, lpMsg->emblem));
	memcpy(&add_data->player, &lpMsg->player);
	add_data->server = lpMsg->h.server;
	add_data->socket = socket;

	sGuildCreateQueue->Queue(add_data);
}

void GuildMgr::RemoveRequest(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_DEL, lpMsg, Packet, 0);
	SL_GUILD_DEL pMsg(lpMsg->guild);
	pMsg.h.server = lpMsg->h.server;

	if ( Guild* pGuild = this->GetGuild(lpMsg->guild) )
	{
		sLog->outInfo("guild", "Server: %d - Removed Guild %s",
			lpMsg->h.server, pGuild->BuildLog().c_str());
	}

	this->RemoveGuild(lpMsg->guild);
	sArkaWar->GuildRemove(lpMsg->guild);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement * stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE_MEMBER);
	stmt->setUInt32(0, lpMsg->guild);
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE);
	stmt->setUInt32(0, lpMsg->guild);
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_MATCHING_DELETE);
	stmt->setUInt32(0, lpMsg->guild);
	trans->Append(stmt);

	stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_GUILD_SCORE_DELETE);
	stmt->setUInt32(0, lpMsg->guild);
	trans->Append(stmt);

    MuDatabase.CommitTransaction(trans);

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMgr::MemberAddRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MEMBER_ADD, lpMsg, Packet, 0);
	SL_GUILD_MEMBER_ADD pMsg(lpMsg->guild);
	pMsg.h.server = lpMsg->h.server;
	memcpy(&pMsg.player, &lpMsg->player);
	memcpy(&pMsg.to_add, &lpMsg->to_add);

	Guild* pGuild = this->GetGuild(lpMsg->guild);

	if ( !pGuild )
	{
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	pMsg.slot = pGuild->GetEmptySlot();

	if ( pMsg.slot == 0 )
	{
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	uint32 add_guid = lpMsg->to_add.guid;
	uint16 server = lpMsg->result == 1 ? uint16(-1): lpMsg->h.server;
	
	if ( lpMsg->result == 1 )
	{
		PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_MAIL_ADD_REQUEST);
		stmt->setString(0, lpMsg->to_add.name);

		PreparedQueryResult result = MuDatabase.Query(stmt);

		if ( result )
		{
			Field* fields = result->Fetch();
			add_guid = fields[0].GetUInt32();
		}
	}

	if ( !add_guid )
	{
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

if (pGuild->GetMemberData(add_guid))
{
pMsg.to_add.entry = -1;
pMsg.player.entry = -1;
socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
return;
}

	lpMsg->to_add.guid = add_guid;
	pMsg.to_add.guid = add_guid;

	pGuild->AddMember(lpMsg->to_add.guid, lpMsg->to_add.name, GUILD_RANK_NORMAL, pMsg.slot, server, true);

	pMsg.result = 1;
	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}
	
void GuildMgr::MemberDelRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(SL_GUILD_MEMBER_DEL, lpMsg, Packet, 0);
	SL_GUILD_MEMBER_DEL pMsg;
	pMsg.h.server = lpMsg->h.server;
	pMsg.guild = lpMsg->guild;
	pMsg.id = lpMsg->id;

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( pGuild )
	{
GuildMember * pGuildMember = pGuild->GetMemberData(lpMsg->id);

if (pGuildMember)
{
SQLTransaction trans = MuDatabase.BeginTransaction();

PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_DELETE_MEMBER_ID);
stmt->setUInt32(0, pGuildMember->m_ID);
trans->Append(stmt);

MuDatabase.CommitTransaction(trans);

CharacterDataPtr pCharacterData = sCharacterMgr->GetCharacterData(pGuildMember->m_ID);

if ( pCharacterData )
{
pCharacterData->m_Guild = 0;
}

sArkaWar->MemberRemove(pGuildMember->m_ID);

pGuildMember->Reset();
}
	}

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMgr::MemberStatusRequest(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_MEMBER_STATUS, lpMsg, Packet, 0);
	SL_GUILD_MEMBER_STATUS pMsg(lpMsg->guild, lpMsg->name, lpMsg->status);
	pMsg.h.server = lpMsg->h.server;

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( pGuild )
	{
uint8 member_slot = pGuild->GetMemberSlot(lpMsg->name);

if ( member_slot != uint8(-1) )
{
SQLTransaction trans = MuDatabase.BeginTransaction();

PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_MEMBER_RANKING);
stmt->setUInt8(0, lpMsg->status);
stmt->setUInt32(1, pGuild->m_Member[member_slot].m_ID);
trans->Append(stmt);

MuDatabase.CommitTransaction(trans);

pGuild->m_Member[member_slot].m_Ranking = lpMsg->status;
}
}

	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMgr::RelationshipRequest(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_RELATIONSHIP, lpMsg, Packet, 0);

	Guild* pGuild01 = this->GetGuild(lpMsg->guild01);
	Guild* pGuild02 = this->GetGuild(lpMsg->guild02);

	SL_GUILD_RELATIONSHIP pMsg(lpMsg->guild01, lpMsg->guild02, lpMsg->type, lpMsg->operation);
	pMsg.h.server = lpMsg->h.server;

	if ( !pGuild01 || !pGuild02 )
	{
		sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

if ( lpMsg->operation ) // join
{
switch( lpMsg->type )
{
case GUILD_RELATIONSHIP_UNION:
{
pGuild01->m_Alliance = pGuild01->m_ID;
pGuild02->m_Alliance = pGuild01->m_ID;
} break;

case GUILD_RELATIONSHIP_RIVAL:
{
pGuild01->m_Hostil = pGuild02->m_ID;
pGuild02->m_Hostil = pGuild01->m_ID;
} break;
}
}
else // break
{
switch( lpMsg->type )
{
case GUILD_RELATIONSHIP_UNION:
{
if ( this->GetInvolvedGuilds(pGuild01->m_Alliance) <= 2 )
{
pGuild01->m_Alliance = 0;
}

pGuild02->m_Alliance = 0;
} break;

case GUILD_RELATIONSHIP_RIVAL:
{
pGuild01->m_Hostil = 0;
pGuild02->m_Hostil = 0;
} break;
}
	}

	pGuild01->UpdateRelationshipDB();
	pGuild02->UpdateRelationshipDB();

	pMsg.result = 1;
	sServerSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size());
}

void GuildMgr::Score(uint8 * Packet)
{
	POINTER_PCT(SL_GUILD_SCORE, lpMsg, Packet, 0);

Guild * pGuild = this->GetGuild(lpMsg->guild);

if ( pGuild )
{
pGuild->m_Score += lpMsg->score;

SQLTransaction trans = MuDatabase.BeginTransaction();

PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_SCORE);
stmt->setInt32(0, pGuild->m_Score);
stmt->setUInt32(1, lpMsg->guild);
trans->Append(stmt);

		MuDatabase.CommitTransaction(trans);
	}

	sServerSocketMgr.SendPacketAll(Packet, lpMsg->h.get_size());
}

void GuildMgr::Notice(uint8 * Packet)
{
	POINTER_PCT(SL_CHAT_PACKET, lpMsg, Packet, 0);

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_GUILD_UPDATE_NOTICE);
	stmt->setString(0, base64::encode((uint8*)lpMsg->msg, MAX_GUILD_NOTICE_LENGTH));
	stmt->setUInt32(1, lpMsg->guild);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
	
	sServerSocketMgr.SendPacketAll(Packet, lpMsg->h.get_size());
}

void GuildMgr::MemberOnlineOffline(CharacterDataPtr pCharacterData)
{
if ( !pCharacterData )
return;

if ( !pCharacterData->m_ID )
return;

Guild* pGuild = this->GetGuild(pCharacterData->m_Guild);

if ( !pGuild )
return;

for ( uint8 i = 0; i < MAX_GUILD_MEMBER; ++i )
{
if ( pGuild->m_Member[i].m_ID == pCharacterData->m_ID )
{
memcpy(pGuild->m_Member[i].m_Name, pCharacterData->m_Name, sizeof(pGuild->m_Member[i].m_Name));
pGuild->m_Member[i].m_Server = pCharacterData->m_Server;
break;
}
}
}