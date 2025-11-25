CharacterMgr::CharacterMgr()
{

}

CharacterMgr::~CharacterMgr()
{
	this->character_data_map.clear();
}

void CharacterMgr::Update()
{
	for ( CharacterDataMap::iterator it = this->character_data_map.begin(); it != this->character_data_map.end(); )
	{
		CharacterDataPtr pCharacterData = it->second;

		if ( !pCharacterData )
		{
			this->character_data_map.erase(it++);
			continue;
		}

if ( !pCharacterData->socket || !pCharacterData->socket->IsOpen() )
{
pCharacterData->m_Server = -1;
pCharacterData->socket = nullptr;

sLog->outInfo("player", "Disconnecting Character: %u (%s).", pCharacterData->m_ID, pCharacterData->m_Name);

sGuildMgr->MemberOnlineOffline(pCharacterData);

this->CharacterUpdateStatus(pCharacterData->m_ID, pCharacterData->m_Server, false);

this->character_data_map.erase(it++);
continue;
}

		++it;
	}
}

void CharacterMgr::LoginLogout(uint8 * Packet, std::shared_ptr<ServerSocket> socket)
{
	POINTER_PCT(DS_CHARACTER_ON_OFF, lpMsg, Packet, 0);

CharacterDataPtr pCharacterData = this->GetCharacterData(lpMsg->guid);
bool add = false;

	if ( !pCharacterData )
	{
		pCharacterData = CharacterDataPtr(new CharacterData);
		add = true;
	}

pCharacterData->m_ID = lpMsg->guid;
memcpy(pCharacterData->m_Name, lpMsg->name, sizeof(pCharacterData->m_Name));
pCharacterData->m_Guild = lpMsg->guild;
pCharacterData->m_Authority = lpMsg->authority;
pCharacterData->m_AdminPanelFlags = lpMsg->admin_flags;
uint16 server = pCharacterData->m_Server;

if ( lpMsg->status == 0 )
{
if ( server == lpMsg->h.server )
pCharacterData->m_Server = -1;
}
else if ( lpMsg->status == 1 )
{
pCharacterData->m_Server = lpMsg->server;
}

if ( lpMsg->status == 1 )
{
sLog->outInfo("player", "Connecting Character: %u (%s).", pCharacterData->m_ID, pCharacterData->m_Name);
sGuildMgr->MemberOnlineOffline(pCharacterData);
this->CharacterUpdateStatus(lpMsg->guid, lpMsg->server, true);
}

GuildMatchingJoinInfo * pGuildMatchingJoinInfo = sGuildMatching->GetGuildMatchingJoinInfo(pCharacterData->m_ID);

if ( pGuildMatchingJoinInfo )
{
memcpy(pGuildMatchingJoinInfo->m_Name, pCharacterData->m_Name, sizeof(pGuildMatchingJoinInfo->m_Name));
}

if ( lpMsg->status == 0 )
{
if ( server == lpMsg->h.server )
{
pCharacterData->socket = nullptr;
this->CharacterUpdateStatus(pCharacterData->m_ID, pCharacterData->m_Server, false);
}
}
else if ( lpMsg->status == 1 )
{
pCharacterData->socket = socket;
}

	if ( add )
		this->character_data_map[lpMsg->guid] = pCharacterData;
}

CharacterDataPtr CharacterMgr::GetCharacterData(uint32 id) const
{
CharacterDataMap::const_iterator it = this->character_data_map.find(id);

	if ( it == this->character_data_map.end() )
		return nullptr;

	return it->second;
}

CharacterDataPtr CharacterMgr::GetCharacterData(const char * name) const
{
for ( CharacterDataMap::const_iterator it = this->character_data_map.begin(); it != this->character_data_map.end(); ++it )
{
CharacterDataPtr pCharacterData = it->second;

if ( !pCharacterData )
continue;

if ( !memcmp(pCharacterData->m_Name, name, MAX_CHARACTER_LENGTH) )
{
return pCharacterData;
}
}

	return nullptr;
}

void CharacterMgr::CharacterUpdateStatus(uint32 id, uint16 server, bool online)
{
	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(CHARACTER_UPDATE_ONLINE);
	stmt->setBool(0, online);
	stmt->setUInt16(1, server);
	stmt->setUInt32(2, id);
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);
}