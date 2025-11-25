ArkaWar::ArkaWar()
{

}

ArkaWar::~ArkaWar()
{
	this->Clear();
}

bool ArkaWarGuildSort(ArkaWarGuild const& pGuild01, ArkaWarGuild const& pGuild02)
{
	if ( pGuild01.m_Signs == pGuild02.m_Signs )
{
	return pGuild01.m_RegisterTime < pGuild02.m_RegisterTime;
}
	else
{
	return pGuild01.m_Signs >= pGuild02.m_Signs;
}
}

void ArkaWar::GetArkaWarGuildSort(std::vector<ArkaWarGuild> & guild_list, int32 min_member)
{
	for ( ArkaWarGuildMap::const_iterator it = this->arka_war_guild.begin(); it != this->arka_war_guild.end(); ++it )
{
	if ( it->second->m_MemberCount < min_member )
{
		continue;
}

	ArkaWarGuild add_guild;
		add_guild.m_ID = it->second->m_ID;
		add_guild.m_Signs = it->second->m_Signs;
		add_guild.m_MemberCount = it->second->m_MemberCount;
		add_guild.m_RegisterTime = it->second->m_RegisterTime;
	guild_list.push_back(add_guild);
}

	std::sort(guild_list.begin(), guild_list.end(), ArkaWarGuildSort);
}

void ArkaWar::MasterRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_ARKA_WAR_MASTER_REGISTER, lpMsg, Packet, 0);

	DS_ARKA_WAR_MASTER_REGISTER pMsg;
	pMsg.player = lpMsg->player;
	pMsg.guild = lpMsg->guild;

	ArkaWarGuildMap::const_iterator it = this->arka_war_guild.find(lpMsg->guild);

	if ( it != this->arka_war_guild.end() )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	ArkaWarGuild * add_guild = new ArkaWarGuild;
	add_guild->m_ID = lpMsg->guild;
	add_guild->m_Signs = 0;
	add_guild->m_MemberCount = 1;
	add_guild->m_Admited = false;
	add_guild->m_RegisterTime = time(nullptr);
	this->arka_war_guild[lpMsg->guild] = add_guild;

	ArkaWarMember * add_member = new ArkaWarMember;
	add_member->m_ID = lpMsg->player.guid;
	memcpy(add_member->m_Name, lpMsg->player.name, MAX_CHARACTER_LENGTH + 1);
	add_member->m_Guild = lpMsg->guild;

	this->arka_war_member[add_member->m_ID] = add_member;

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}
	
void ArkaWar::MemberRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_ARKA_WAR_MEMBER_REGISTER, lpMsg, Packet, 0);

	DS_ARKA_WAR_MEMBER_REGISTER pMsg;
	pMsg.player = lpMsg->player;
	pMsg.guild = lpMsg->guild;

	ArkaWarMemberMap::const_iterator it = this->arka_war_member.find(lpMsg->player.guid);

	if ( it != this->arka_war_member.end() )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	ArkaWarGuildMap::iterator itr = this->arka_war_guild.find(lpMsg->guild);

	if ( itr == this->arka_war_guild.end() )
	{
		pMsg.result = 2;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	if ( itr->second->m_MemberCount > lpMsg->max_member )
{
	pMsg.result = 3;
	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
	return;
}

	itr->second->m_MemberCount += 1;

	ArkaWarMember * add_member = new ArkaWarMember;
	add_member->m_ID = lpMsg->player.guid;
	memcpy(add_member->m_Name, lpMsg->player.name, MAX_CHARACTER_LENGTH + 1);
	add_member->m_Guild = lpMsg->guild;

	this->arka_war_member[add_member->m_ID] = add_member;

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}
	
void ArkaWar::SignRegister(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_ARKA_WAR_SIGN_REGISTER, lpMsg, Packet, 0);

	DS_ARKA_WAR_SIGN_REGISTER pMsg;
	pMsg.player = lpMsg->player;
	pMsg.guild = lpMsg->guild;
	pMsg.result = lpMsg->result;

	ArkaWarGuildMap::const_iterator it = this->arka_war_guild.find(lpMsg->guild);

	if ( it == this->arka_war_guild.end() )
	{
		pMsg.result = 2;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	if ( lpMsg->result == 1 )
{
		it->second->m_Signs += lpMsg->signs;
}

	pMsg.signs = it->second->m_Signs;

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}
	
void ArkaWar::Enter(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_ARKA_WAR_ENTER, lpMsg, Packet, 0);

	DS_ARKA_WAR_ENTER pMsg;
	pMsg.player = lpMsg->player;

	ArkaWarMemberMap::const_iterator it = this->arka_war_member.find(lpMsg->player.guid);

	if ( it == this->arka_war_member.end() )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}

void ArkaWar::MemberCount(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_ARKA_WAR_MEMBER_COUNT, lpMsg, Packet, 0);

	DS_ARKA_WAR_MEMBER_COUNT pMsg;
	pMsg.player = lpMsg->player;
	pMsg.guild = lpMsg->guild;

	ArkaWarGuildMap::const_iterator it = this->arka_war_guild.find(lpMsg->guild);

	if ( it == this->arka_war_guild.end() )
	{
		pMsg.result = 1;
		socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
		return;
	}

	pMsg.count = it->second->m_MemberCount;
	socket->QueuePacket((uint8*)&pMsg, pMsg.h.get_size());
}

void ArkaWar::GuildRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_ARKA_WAR_GUILD_REQUEST, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(DS_ARKA_WAR_GUILD_HEAD, head, buffer, 0);
	POINTER_PCT(DS_ARKA_WAR_GUILD_BODY, body, buffer, sizeof(DS_ARKA_WAR_GUILD_HEAD));
	head->count = 0;

	PreparedQueryResult result = MuDatabase.QueryStatement(QUERY_MUDATABASE_ARKA_WAR_SELECT);

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			body[head->count].id = fields[0].GetUInt8();
			body[head->count].guild = fields[1].GetUInt32();
			body[head->count].attribute = fields[2].GetUInt8();
			++head->count;
		}
		while(result->NextRow());
	}

	head->h.set(HEADCODE_DATA_SERVER_ARKA_WAR_GUILD_REQUEST, sizeof(DS_ARKA_WAR_GUILD_HEAD) + (head->count * sizeof(DS_ARKA_WAR_GUILD_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}
	
void ArkaWar::GuildSave(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_GUILD_HEAD, head, Packet, 0);
	POINTER_PCT(DS_ARKA_WAR_GUILD_BODY, body, Packet, sizeof(DS_ARKA_WAR_GUILD_HEAD));

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ARKA_WAR_DELETE);
	trans->Append(stmt);

	for ( int32 i = 0; i < head->count; ++i )
	{
		stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_ARKA_WAR_INSERT);
		stmt->setUInt8(0, body[i].id);
		stmt->setUInt32(1, body[i].guild);
		stmt->setUInt8(2, body[i].attribute);
		trans->Append(stmt);
	}

	MuDatabase.CommitTransaction(trans);

	head->h.set(HEADCODE_DATA_SERVER_ARKA_WAR_GUILD_REQUEST, sizeof(DS_ARKA_WAR_GUILD_HEAD) + (head->count * sizeof(DS_ARKA_WAR_GUILD_BODY)));
	sServerSocketMgr.SendPacketAll(Packet, head->h.get_size());
}

void ArkaWar::SignRequest(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_ARKA_WAR_SIGN_REQUEST, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(DS_ARKA_WAR_SIGN_REQUEST_HEAD, head, buffer, 0);
	POINTER_PCT(DS_ARKA_WAR_SIGN_REQUEST_BODY, body, buffer, sizeof(DS_ARKA_WAR_SIGN_REQUEST_HEAD));
	head->count = 0;
	head->player = lpMsg->player;

	std::vector<ArkaWarGuild> ArkaWarGuildList;
	this->GetArkaWarGuildSort(ArkaWarGuildList, 0);

	for ( std::vector<ArkaWarGuild>::const_iterator it = ArkaWarGuildList.begin(); it != ArkaWarGuildList.end(); ++it )
{
	body[head->count].guild = it->m_ID;
	body[head->count].signs = it->m_Signs;
		++head->count;
}

	head->h.set(HEADCODE_DATA_SERVER_ARKA_WAR_SIGN_REQUEST, sizeof(DS_ARKA_WAR_SIGN_REQUEST_HEAD) + (head->count * sizeof(DS_ARKA_WAR_SIGN_REQUEST_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}

void ArkaWar::GuildRemove(uint32 id)
{
	ArkaWarGuildMap::const_iterator it = this->arka_war_guild.find(id);

	if ( it != this->arka_war_guild.end() )
	{
		this->MemberGuildRemove(id);

		delete it->second;
		this->arka_war_guild.erase(it);
	}
}

void ArkaWar::MemberGuildRemove(uint32 id)
{
	for ( ArkaWarMemberMap::const_iterator it = this->arka_war_member.begin(); it != this->arka_war_member.end(); )
{
	if ( it->second->m_Guild == id )
{
		delete it->second;
	this->arka_war_member.erase(it++);
		}
		else
		{
			++it;
		}
	}
}
	
void ArkaWar::MemberRemove(uint32 id)
{
	ArkaWarMemberMap::const_iterator it = this->arka_war_member.find(id);

	if ( it != this->arka_war_member.end() )
	{
	ArkaWarGuildMap::const_iterator it2 = this->arka_war_guild.find(it->second->m_Guild);

	if ( it2 != this->arka_war_guild.end() )
{
		it2->second->m_MemberCount -= 1;
}

		delete it->second;
		this->arka_war_member.erase(it);
	}
}

void ArkaWar::GetGuildList(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_ARKA_WAR_LIST, lpMsg, Packet, 0);

	uint8 buffer[8192];
	POINTER_PCT(DS_ARKA_WAR_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(DS_ARKA_WAR_LIST_GUILD_BODY, body, buffer, sizeof(DS_ARKA_WAR_LIST_HEAD));
	head->count = 0;
	head->type = 0;

	std::vector<ArkaWarGuild> ArkaWarGuildList;
	this->GetArkaWarGuildSort(ArkaWarGuildList, lpMsg->min_member);

	int32 guild_count = ArkaWarGuildList.size();

	if ( guild_count > lpMsg->max_guild )
	{
		guild_count = lpMsg->max_guild;
	}

	for ( int32 i = 0; i < guild_count; ++i )
{
	ArkaWarGuildMap::iterator it = this->arka_war_guild.find(ArkaWarGuildList[i].m_ID);

	if ( it != this->arka_war_guild.end() )
{
		it->second->m_Admited = true;
}
}

	for ( ArkaWarGuildMap::const_iterator it = this->arka_war_guild.begin(); it != this->arka_war_guild.end(); )
	{
	if ( !it->second->m_Admited )
{
		this->MemberGuildRemove(it->second->m_ID);
		delete it->second;
	this->arka_war_guild.erase(it++);
}
		else
		{
			++it;
		}
	}

	for ( ArkaWarGuildMap::const_iterator it = this->arka_war_guild.begin(); it != this->arka_war_guild.end(); ++it )
	{
	body[head->count].guild = it->second->m_ID;
	body[head->count].members = it->second->m_MemberCount;
		++head->count;
}

	head->h.set(HEADCODE_DATA_SERVER_ARKA_WAR_LIST, sizeof(DS_ARKA_WAR_LIST_HEAD) + (head->count * sizeof(DS_ARKA_WAR_LIST_GUILD_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}
	
void ArkaWar::GetMemberList(std::shared_ptr<ServerSocket> socket)
{
	uint8 buffer[8192];
	POINTER_PCT(DS_ARKA_WAR_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(DS_ARKA_WAR_LIST_MEMBER_BODY, body, buffer, sizeof(DS_ARKA_WAR_LIST_HEAD));
	head->count = 0;
	head->type = 1;

	for ( ArkaWarMemberMap::const_iterator it = this->arka_war_member.begin(); it != this->arka_war_member.end(); ++it )
{
	body[head->count].id = it->second->m_ID;
		memcpy(body[head->count].name, it->second->m_Name, MAX_CHARACTER_LENGTH + 1);
	body[head->count].guild = it->second->m_Guild;
		++head->count;
}

	head->h.set(HEADCODE_DATA_SERVER_ARKA_WAR_LIST, sizeof(DS_ARKA_WAR_LIST_HEAD) + (head->count * sizeof(DS_ARKA_WAR_LIST_MEMBER_BODY)));
	socket->QueuePacket(buffer, head->h.get_size());
}

void ArkaWar::Clear()
{
	MAP_CLEAR(ArkaWarGuildMap::iterator, this->arka_war_guild);
	MAP_CLEAR(ArkaWarMemberMap::iterator, this->arka_war_member);
}