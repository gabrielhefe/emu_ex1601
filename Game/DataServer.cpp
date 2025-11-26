/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "DataServer.cpp"
*
*/

std::unordered_map<uint8, LoginHandler<DataServer>> DataServer::InitHandlers()
{
    std::unordered_map<uint8, LoginHandler<DataServer>> handlers;

	handlers[HEADCODE_DATA_SERVER_ON_CONNECT]							= { "ON_CONNECT", sizeof(DS_ON_CONNECT), &DataServer::HandleHeadcodeOnConnect };

    handlers[HEADCODE_DATA_SERVER_GUILD_CHAT]							= { "GUILD_CHAT", sizeof(DS_CHAT_PACKET), &DataServer::HandleHeadcodeGuildChat };
	handlers[HEADCODE_DATA_SERVER_ALLIANCE_CHAT]						= { "ALLIANCE_CHAT", sizeof(DS_CHAT_PACKET), &DataServer::HandleHeadcodeAllianceChat };
	handlers[HEADCODE_DATA_SERVER_GUILD_NOTICE]							= { "GUILD_NOTICE", sizeof(DS_CHAT_PACKET), &DataServer::HandleHeadcodeGuildNotice };
	handlers[HEADCODE_DATA_SERVER_POST]									= { "POST", sizeof(DS_CHAT_PACKET), &DataServer::HandleHeadcodePost };
	handlers[HEADCODE_DATA_SERVER_SIMPLE_MESSAGE]						= { "SIMPLE_MESSAGE", sizeof(DS_CHAT_PACKET), &DataServer::SimpleMessageResult };
	handlers[HEADCODE_DATA_SERVER_WHISPER]								= { "WHISPER", sizeof(DS_WHISPER), &DataServer::WhisperResult };

	handlers[HEADCODE_DATA_SERVER_GUILD_ADD]							= { "GUILD_ADD", sizeof(DS_GUILD_ADD), &DataServer::GuildCreateResult };
	handlers[HEADCODE_DATA_SERVER_GUILD_REMOVE]							= { "GUILD_REMOVE", sizeof(DS_GUILD_DEL), &DataServer::GuildDeleteResult };
	handlers[HEADCODE_DATA_SERVER_GUILD_MEMBER_ADD]						= { "GUILD_MEMBER_ADD", sizeof(DS_GUILD_MEMBER_ADD), &DataServer::GuildMemberAddResult };
	handlers[HEADCODE_DATA_SERVER_GUILD_MEMBER_DEL]						= { "GUILD_MEMBER_DEL", sizeof(DS_GUILD_MEMBER_DEL), &DataServer::GuildMemberDelResult };
	handlers[HEADCODE_DATA_SERVER_GUILD_MEMBER_STATUS]					= { "GUILD_MEMBER_STATUS", sizeof(DS_GUILD_MEMBER_STATUS), &DataServer::GuildMemberStatusResult };
	handlers[HEADCODE_DATA_SERVER_GUILD_RELATIONSHIP]					= { "GUILD_RELATIONSHIP", sizeof(DS_GUILD_RELATIONSHIP), &DataServer::GuildRelationshipResult };
	handlers[HEADCODE_DATA_SERVER_GUILD_SCORE]							= { "GUILD_SCORE", sizeof(DS_GUILD_SCORE), &DataServer::GuildScoreResult };

	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_LOAD_DATA]				= { "CASTLE_SIEGE_LOAD_DATA", sizeof(DS_CASTLE_SIEGE_LOAD_DATA_RESULT), &DataServer::CastleSiegeLoadDataResult };
	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_LOAD_NPC]				= { "CASTLE_SIEGE_LOAD_NPC", sizeof(DS_CASTLE_SIEGE_LOAD_NPC_HEAD), &DataServer::CastleSiegeLoadNpcResult };
	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_LOAD_REGISTERED_GUILD]	= { "CASTLE_SIEGE_LOAD_REGISTERED_GUILD", sizeof(DS_CASTLE_SIEGE_LOAD_REGISTERED_GUILD_HEAD), &DataServer::CastleSiegeLoadRegisteredGuildResult };
	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_REGISTERED_GUILD]		= { "CASTLE_SIEGE_REGISTERED_GUILD", sizeof(DS_CASTLE_SIEGE_REGISTERED_GUILD_HEAD), &DataServer::CastleSiegeRegisteredGuildResult };
	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_REGISTERED_GUILD_ALL]	= { "CASTLE_SIEGE_REGISTERED_GUILD_ALL", sizeof(DS_CASTLE_SIEGE_REGISTERED_GUILD_HEAD), &DataServer::CastleSiegeRegisteredGuildAllResult };
	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_REGISTER_GUILD]			= { "CASTLE_SIEGE_REGISTER_GUILD", sizeof(DS_CASTLE_SIEGE_REGISTER_GUILD), &DataServer::HandleHeadcodeCastleSiegeRegisterGuild };
	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_SAVE_TAX_RATE]			= { "CASTLE_SIEGE_SAVE_TAX_RATE", sizeof(DS_CASTLE_SIEGE_SAVE_TAX_RATE), &DataServer::HandleHeadcodeCastleSiegeTaxRate };
	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_SAVE_MONEY]				= { "CASTLE_SIEGE_SAVE_MONEY", sizeof(DS_CASTLE_SIEGE_SAVE_MONEY), &DataServer::HandleHeadcodeCastleSiegeMoney };
	handlers[HEADCODE_DATA_SERVER_CASTLE_SIEGE_SAVE_OWNER_STATUS]		= { "CASTLE_SIEGE_SAVE_OWNER_STATUS", sizeof(DS_CASTLE_SIEGE_SAVE_OWNER_STATUS), &DataServer::HandleHeadcodeCastleSiegeUpdateOwner };

	handlers[HEADCODE_DATA_SERVER_NOTICE]								= { "NOTICE", sizeof(DS_NOTICE), &DataServer::HandleHeadcodeNotice };
	handlers[HEADCODE_DATA_SERVER_CHARACTER_ON_OFF]						= { "CHARACTER_ON_OFF", sizeof(DS_CHARACTER_ON_OFF), &DataServer::HandleHeadcodeCharacterOnOff };
	handlers[HEADCODE_DATA_SERVER_CHARACTER_CREATE]						= { "CHARACTER_CREATE", sizeof(DS_CHARACTER_CREATE_RESULT), &DataServer::CharacterCreateResult };
	handlers[HEADCODE_DATA_SERVER_CHARACTER_DELETE]						= { "CHARACTER_DELETE", sizeof(DS_CHARACTER_DELETE_RESULT), &DataServer::CharacterDeleteResult };

	handlers[HEADCODE_DATA_SERVER_ADMIN_COMMAND]						= { "ADMIN_COMMAND", sizeof(DS_ADMIN_COMMAND), &DataServer::AdminCommandResult };

	handlers[HEADCODE_DATA_SERVER_SIGNAL]								= { "SIGNAL", sizeof(DS_SIGNAL), &DataServer::Signal };

	handlers[HEADCODE_DATA_SERVER_CRYWOLF_REQUEST]						= { "CRYWOLF_REQUEST", sizeof(DS_CRYWOLF_REQUEST), &DataServer::CrywolfDataResult };

	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_LIST]					= { "GUILD_MATCHING_LIST", sizeof(DS_GUILD_MATCHING_LIST_HEAD), &DataServer::GuildMatchingList };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_INSERT]				= { "GUILD_MATCHING_INSERT", sizeof(DS_GUILD_MATCHING_INSERT_SEND), &DataServer::GuildMatchingInsert };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_CANCEL]				= { "GUILD_MATCHING_CANCEL", sizeof(DS_GUILD_MATCHING_CANCEL_SEND), &DataServer::GuildMatchingCancel };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_JOIN_INSERT]			= { "GUILD_MATCHING_JOIN_INSERT", sizeof(DS_GUILD_MATCHING_JOIN_INSERT_SEND), &DataServer::GuildMatchingJoinInsert };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_JOIN_CANCEL]			= { "GUILD_MATCHING_JOIN_CANCEL", sizeof(DS_GUILD_MATCHING_JOIN_CANCEL_SEND), &DataServer::GuildMatchingJoinCancel };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_JOIN_ACCEPT]			= { "GUILD_MATCHING_JOIN_ACCEPT", sizeof(DS_GUILD_MATCHING_JOIN_ACCEPT_SEND), &DataServer::GuildMatchingJoinAccept };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_JOIN_LIST]				= { "GUILD_MATCHING_JOIN_LIST", sizeof(DS_GUILD_MATCHING_JOIN_LIST_HEAD), &DataServer::GuildMatchingJoinList };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_JOIN_INFO]				= { "GUILD_MATCHING_JOIN_INFO", sizeof(DS_GUILD_MATCHING_JOIN_INFO_SEND), &DataServer::GuildMatchingJoinInfo };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_NOTIFY]				= { "GUILD_MATCHING_NOTIFY", sizeof(DS_GUILD_MATCHING_NOTIFY_SEND), &DataServer::GuildMatchingNotify };
	handlers[HEADCODE_DATA_SERVER_GUILD_MATCHING_MASTER_NOTIFY]			= { "GUILD_MATCHING_MASTER_NOTIFY", sizeof(DS_GUILD_MATCHING_NOTIFY_MASTER_SEND), &DataServer::GuildMatchingMasterNotify };

	handlers[HEADCODE_DATA_SERVER_EVENT_NOTIFICATION]					= { "EVENT_NOTIFICATION", sizeof(DS_EVENT_NOTIFICATION), &DataServer::EventNotification };

	handlers[HEADCODE_DATA_SERVER_EVENT_STATE_UPDATE]					= { "EVENT_STATE_UPDATE", sizeof(DS_EVENT_STATE_UPDATE), &DataServer::EventStateUpdate };

	handlers[HEADCODE_DATA_SERVER_ARKA_WAR_MASTER_REGISTER]				= { "ARKA_WAR_MASTER_REGISTER", sizeof(DS_ARKA_WAR_MASTER_REGISTER), &DataServer::ArkaWarMasterRegister };
	handlers[HEADCODE_DATA_SERVER_ARKA_WAR_MEMBER_REGISTER]				= { "ARKA_WAR_MEMBER_REGISTER", sizeof(DS_ARKA_WAR_MEMBER_REGISTER), &DataServer::ArkaWarMemberRegister };
	handlers[HEADCODE_DATA_SERVER_ARKA_WAR_SIGN_REGISTER]				= { "ARKA_WAR_SIGN_REGISTER", sizeof(DS_ARKA_WAR_SIGN_REGISTER), &DataServer::ArkaWarSignRegister };
	handlers[HEADCODE_DATA_SERVER_ARKA_WAR_ENTER]						= { "ARKA_WAR_ENTER", sizeof(DS_ARKA_WAR_ENTER), &DataServer::ArkaWarEnter };
	handlers[HEADCODE_DATA_SERVER_ARKA_WAR_LIST]						= { "ARKA_WAR_LIST", sizeof(DS_ARKA_WAR_LIST_HEAD), &DataServer::ArkaWarList };
	handlers[HEADCODE_DATA_SERVER_ARKA_WAR_MEMBER_COUNT]				= { "ARKA_WAR_MEMBER_COUNT", sizeof(DS_ARKA_WAR_MEMBER_COUNT), &DataServer::ArkaWarMemberCount };
	handlers[HEADCODE_DATA_SERVER_ARKA_WAR_GUILD_REQUEST]				= { "ARKA_WAR_GUILD_REQUEST", sizeof(DS_ARKA_WAR_GUILD_HEAD), &DataServer::ArkaWarGuildRequest };
	handlers[HEADCODE_DATA_SERVER_ARKA_WAR_SIGN_REQUEST]				= { "ARKA_WAR_SIGN_REQUEST", sizeof(DS_ARKA_WAR_SIGN_REQUEST_HEAD), &DataServer::ArkaWarSignOfLordCheckResult };

	handlers[HEADCODE_DATA_SERVER_CHARACTER_SIGNAL]						= { "CHARACTER_SIGNAL", sizeof(DS_CHARACTER_SIGNAL), &DataServer::CharacterSignal };

    return handlers;
}

std::unordered_map<uint8, LoginHandler<DataServer>> const Handlers = DataServer::InitHandlers();

DataServer::DataServer(): TCPClientMgr("DataServer")
{
	
}

void DataServerSocket::OnStart()
{
	sLog->outInfo("root", "CONNECTED SERVERLINK");

	sDataServer->IncreaseConnectCount(1);
	sDataServer->SetStopped(false);

	DS_ON_CONNECT pMsg(sGameServer->GetServerCode());
	this->QueuePacket(MAKE_PCT(pMsg));
}

void DataServerSocket::OnStop()
{
	sLog->outInfo("root", "DISCONNECTED SERVERLINK");

	sDataServer->GetReconnectTime()->Start();
	sDataServer->SetStopped(true);
}

void DataServerSocket::ReadHandler()
{
	if ( this->IsStopped() )
		return;

	MessageBuffer& packet = GetReadBuffer();
    while ( packet.GetActiveSize() > 0 )
    {
		uint8 cmd = packet.GetReadPointer()[0];
        auto itr = Handlers.find(cmd);
        if (itr == Handlers.end())
        {
            packet.Reset();
            break;
        }

        uint16 size = itr->second.packetSize;
        if (packet.GetActiveSize() < size)
            break;

		CUSTOM_PACKET_HEAD * head = (CUSTOM_PACKET_HEAD*)packet.GetReadPointer();

		sDataServer->AddPacket(new WorldPacket(cmd, packet.GetReadPointer(), head->size));	

		packet.ReadCompleted(head->size);
    }
}

void DataServer::HandleHeadcodeOnConnect(uint8 * Packet)
{
	if ( this->GetConnectCount() == 1 )
	{
		//this->GuildListRequest();
		this->CastleSiegeLoadDataRequest();
		this->CastleSiegeLoadNpcRequest();
		this->CrywolfDataRequest();
		this->ArkaWarGuildRequest();
	}
	else
	{
		PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
		for ( PlayerSessionMap::const_iterator it = character_map.begin(); it != character_map.end(); ++it )
		{
			Player* pPlayer = it->second;

			if ( !pPlayer )
			{
				continue;
			}

			this->CharacterOnOff(pPlayer, 1);
		}
	}
}

void DataServer::HandleHeadcodeGuildChat(uint8 * packet)
{
	POINTER_PCT(DS_CHAT_PACKET, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	pGuild->SendChatToMembers(lpMsg->name,lpMsg->msg);

	sObjectMgr->SendGuildMessageToAdmin(lpMsg->guild,lpMsg->name,lpMsg->msg);
}

void DataServer::HandleHeadcodeAllianceChat(uint8 * packet)
{
	POINTER_PCT(DS_CHAT_PACKET, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	if ( !pGuild->GetAlliance() )
	{
		pGuild->SendChatToMembers(lpMsg->name,lpMsg->msg);
		sObjectMgr->SendAllianceMessageToAdmin(lpMsg->guild,lpMsg->name,lpMsg->msg);
	}
	else
	{
		sGuildMgr->SendMsgToGuildAlliance(pGuild->GetAlliance(),lpMsg->name,lpMsg->msg);
		sObjectMgr->SendAllianceMessageToAdmin(pGuild->GetAlliance(),lpMsg->name,lpMsg->msg);
	}
}

void DataServer::HandleHeadcodeGuildNotice(uint8 * packet)
{
	POINTER_PCT(DS_CHAT_PACKET, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	pGuild->ResetNotice();
	pGuild->SetNotice(lpMsg->msg);
	pGuild->SendNoticeToMembers(NOTICE_GUILD, lpMsg->msg);

	sObjectMgr->SendGuildNoticeToAdmin(lpMsg->guild,lpMsg->name,lpMsg->msg);
}

void DataServer::HandleHeadcodePost(uint8 * packet)
{
	if ( sGameServer->IsGlobalMute() )
		return;

	POINTER_PCT(DS_CHAT_PACKET, lpMsg, packet, 0);

	sObjectMgr->SendPostToAll(lpMsg->h.server, lpMsg->name, lpMsg->msg);
}

void DataServer::HandleHeadcodeCharacterOnOff(uint8 * packet)
{
	POINTER_PCT(DS_CHARACTER_ON_OFF, lpMsg, packet, 0);

	if ( lpMsg->status == 2 )
		return;

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( pGuild )
	{
		pGuild->UpdateMemberStatus(lpMsg->guid, lpMsg->server, lpMsg->name);
	}

	sObjectMgr->PlayerOnlineUpdate(lpMsg->guid, lpMsg->name, lpMsg->server, lpMsg->status);

	if (lpMsg->status == 1)
	{
		sChaosCastleSurvivalMgr->OnPlayerConnect(lpMsg->guid, lpMsg->name);
		sTormentedSquareSurvival->OnPlayerConnect(lpMsg->guid, lpMsg->name);
	}

	if (sGameServer->IsDuplicatedCharacterCheck())
	{
		if (lpMsg->h.server != sGameServer->GetServerCode() && lpMsg->status == 1 && lpMsg->guid > 0)
		{
			HASH_PLAYER(i)
			{
				Player* pPlayer = ObjectContainer[i]->ToPlayer();

				if (!pPlayer || pPlayer->GetConnectStatus() != CONNECT_STATUS_PLAYING)
				{
					continue;
				}

				if (pPlayer->GetGUID() == lpMsg->guid || pPlayer->GetAccountData()->GetGUID() == lpMsg->account_id)
				{
					KICK_PLAYER(pPlayer, "Duplicated Character");

					sGameServer->LogAntiHack(pPlayer, ANTIHACK_ID_KICK, "Duplicated Character");
				}
			}
		}
	}
}

void DataServer::HandleHeadcodeNotice(uint8 * packet)
{
	POINTER_PCT(DS_NOTICE, lpMsg, packet, 0);

	sObjectMgr->SendNoticeToAllNormal(lpMsg->type, lpMsg->notice);
}

void DataServer::CastleSiegeLoadDataResult(uint8 * packet)
{
	POINTER_PCT(DS_CASTLE_SIEGE_LOAD_DATA_RESULT, lpMsg, packet, 0);

	sCastleSiege->SetCastleOwner(lpMsg->owner);
	sCastleSiege->SetOccupied(lpMsg->status);
	sCastleSiege->SetTaxRateHunt(lpMsg->tax_hunt);
	sCastleSiege->SetTaxRateChaos(lpMsg->tax_chaos);
	sCastleSiege->SetTaxRateStore(lpMsg->tax_store);
	sCastleSiege->SetHuntEnabled(lpMsg->hunt_allowed);
	sCastleSiege->IncreaseTributeMoney(lpMsg->money);
}

void DataServer::CastleSiegeLoadNpcResult(uint8 * packet)
{
	sCastleSiege->LoadNPC(packet);
}

void DataServer::CastleSiegeLoadRegisteredGuildResult(uint8 * packet)
{
	sCastleSiege->LoadGuildRegisterResult(packet);
}

void DataServer::CastleSiegeRegisteredGuildResult(uint8 * packet)
{
	sCastleSiege->RegisterGuildInfoCallBack(packet);
}

void DataServer::CastleSiegeRegisteredGuildAllResult(uint8 * packet)
{
	sCastleSiege->GuildRegisterListCallBack(packet);
}

void DataServer::HandleHeadcodeCastleSiegeRegisterGuild(uint8 * packet)
{
	POINTER_PCT(DS_CASTLE_SIEGE_REGISTER_GUILD, lpMsg, packet, 0);

	if ( Guild* pGuild = sGuildMgr->GetGuild(lpMsg->guild) )
	{
		pGuild->SetRegisteredInCaslteSiege(lpMsg->status);
		pGuild->SetCastleSiegeMarks(lpMsg->mark);
	}
}

void DataServer::HandleHeadcodeCastleSiegeTaxRate(uint8 * packet)
{
	POINTER_PCT(DS_CASTLE_SIEGE_SAVE_TAX_RATE, lpMsg, packet, 0);

	sCastleSiege->SetTaxRate(lpMsg->tax_hunt, lpMsg->tax_chaos, lpMsg->tax_store, lpMsg->hunt_allowed);
}

void DataServer::HandleHeadcodeCastleSiegeMoney(uint8 * packet)
{
	POINTER_PCT(DS_CASTLE_SIEGE_SAVE_MONEY, lpMsg, packet, 0);

	if ( !lpMsg->type )
	{
		sCastleSiege->IncreaseTributeMoney(lpMsg->money);
	}
	else if ( lpMsg->type == 1 )
	{
		sCastleSiege->SetTributeMoney(0);
	}
	else if ( lpMsg->type == 2 )
	{
		sCastleSiege->ReduceTributeMoney(lpMsg->money);
	}

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
		return;

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry, lpMsg->ms_time, lpMsg->account_id, lpMsg->character_id);

	if ( !pPlayer )
		return;

	if ( pPlayer->IsCastleSiegeMoneyRemove() )
	{
		pPlayer->MoneyAdd(lpMsg->money);
		sCastleSiege->MoneyOutResult(pPlayer, 1, sCastleSiege->GetTributeMoney());
		pPlayer->SetCastleSiegeMoneyRemove(false);
	}
}

void DataServer::HandleHeadcodeCastleSiegeUpdateOwner(uint8 * packet)
{
	POINTER_PCT(DS_CASTLE_SIEGE_SAVE_OWNER_STATUS, lpMsg, packet, 0);

	sCastleSiege->SetCastleOwner(lpMsg->owner);
	sCastleSiege->SetOccupied(lpMsg->status);
}

void DataServer::ChatSend(uint8 headcode, Player * pPlayer, const char * msg)
{
	DS_CHAT_PACKET pMsg(headcode, 0, pPlayer->GetName(), msg);
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.guild = pPlayer->GetGuildID();

	if ( pPlayer->IsAdministrator() && pPlayer->IsAdministratorFlag(ADMIN_FLAG_GUILD_TALK) && pPlayer->TalkingGuild != 0 && headcode == HEADCODE_DATA_SERVER_GUILD_CHAT )
	{
		pMsg.guild = pPlayer->TalkingGuild.get();
	}
	else if ( pPlayer->IsAdministrator() && pPlayer->IsAdministratorFlag(ADMIN_FLAG_GUILD_TALK) && pPlayer->TalkingAlliance != 0 && headcode == HEADCODE_DATA_SERVER_ALLIANCE_CHAT )
	{
		pMsg.guild = pPlayer->TalkingAlliance.get();
	}

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::GuildCreateRequest(Player * pPlayer, const char * name, uint8 * emblem)
{
	DS_GUILD_ADD pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.player.entry = pPlayer->GetEntry();
	pMsg.player.ms_time = pPlayer->GetMSTime();
	pMsg.player.guid = pPlayer->GetGUID();
	pMsg.player.account_id = pPlayer->GetAccountData()->GetGUID();
	memcpy(pMsg.player.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH + 1);
	memcpy(pMsg.name, name, MAX_GUILD_NAME_LENGTH);
	memcpy(pMsg.emblem, emblem, MAX_GUILD_EMBLEM_LENGTH);
	
	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::GuildCreateResult(uint8 * Packet)
{
	POINTER_PCT(DS_GUILD_ADD, lpMsg, Packet, 0);

	Player* pPlayer = lpMsg->h.server == sGameServer->GetServerCode() ? sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid) : nullptr;

	if ( lpMsg->result == 1 )
	{
		Guild * pGuild = new Guild;
		pGuild->Create(lpMsg->player.guid, lpMsg->name, lpMsg->emblem);
		pGuild->AddMember(lpMsg->player.guid, lpMsg->player.name, GUILD_RANK_MASTER, 0, pPlayer, lpMsg->h.server);

		sGuildMgr->AddGuild(pGuild);

		if ( pPlayer )
		{
			pPlayer->GuildCreateResult(GUILD_CREATE_SUCCESS);
			pPlayer->GuildUpdateViewport();
			pPlayer->GetInterfaceState()->Reset();

			sLog->outInfo(LOG_GUILD, "%s Created guild [%s]", 
				pPlayer->BuildLog().c_str(), pGuild->GetName());
		}
	}
	else
	{
		if ( pPlayer )
			pPlayer->GuildCreateResult(GUILD_CREATE_ALREADY_EXIST);
	}
}

void DataServer::GuildDeleteRequest(uint32 guild)
{
	DS_GUILD_DEL pMsg(guild);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::GuildDeleteResult(uint8 * Packet)
{
	POINTER_PCT(DS_GUILD_DEL, lpMsg, Packet, 0);

	sGuildMgr->RemoveGuild(lpMsg->guild);
}

void DataServer::GuildMemberAddRequest(Player* pPlayer, Player* pToAdd, uint32 guild)
{
	DS_GUILD_MEMBER_ADD pMsg(guild);
	pMsg.h.server = sGameServer->GetServerCode();
	if ( pPlayer )
	{
		pMsg.player.entry = pPlayer->GetEntry();
		pMsg.player.ms_time = pPlayer->GetMSTime();
		pMsg.player.account_id = pPlayer->GetAccountData()->GetGUID();
		pMsg.player.guid = pPlayer->GetGUID();
		memcpy(pMsg.player.name, pPlayer->GetName(), MAX_CHARACTER_LENGTH + 1);
	}

	pMsg.to_add.entry = pToAdd->GetEntry();
	pMsg.to_add.ms_time = pToAdd->GetMSTime();
	pMsg.to_add.account_id = pToAdd->GetAccountData()->GetGUID();
	pMsg.to_add.guid = pToAdd->GetGUID();
	memcpy(pMsg.to_add.name, pToAdd->GetName(), MAX_CHARACTER_LENGTH + 1);
	
	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::GuildMemberAddRequest(const char * member_name, uint32 guild)
{
	DS_GUILD_MEMBER_ADD pMsg(guild);
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.result = 1;
	memcpy(pMsg.to_add.name, member_name, MAX_CHARACTER_LENGTH + 1);
	
	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::GuildMemberAddResult(uint8 * packet)
{
	POINTER_PCT(DS_GUILD_MEMBER_ADD, lpMsg, packet, 0);

	Player* pPlayer = lpMsg->h.server == sGameServer->GetServerCode() ? sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid) : nullptr;
	Player* pToAdd = lpMsg->h.server == sGameServer->GetServerCode() ? sObjectMgr->FindPlayer(lpMsg->to_add.entry, lpMsg->to_add.ms_time, lpMsg->to_add.account_id, lpMsg->to_add.guid) : nullptr;

	if ( lpMsg->result == 0 )
	{
		if ( pPlayer )
		{
			pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild is full.");
		}

		if ( pToAdd )
		{
			pToAdd->GuildJoinResult(0x00);
		}

		return;
	}

	Guild* pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
	{
		return;
	}

	pGuild->AddMember(lpMsg->to_add.guid, lpMsg->to_add.name, GUILD_RANK_NORMAL, lpMsg->slot, pToAdd, lpMsg->h.server);

	if ( pToAdd )
	{
		pToAdd->GuildJoinResult(0x01);
		pToAdd->GuildUpdateViewport();
	}
}

void DataServer::GuildMemberDelRequest(uint32 guild, uint32 id)
{
	DS_GUILD_MEMBER_DEL pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.guild = guild;
	pMsg.id = id;
	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::GuildMemberDelResult(uint8 * Packet)
{
	POINTER_PCT(DS_GUILD_MEMBER_DEL, lpMsg, Packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if (!pGuild)
	{
		return;
	}

	pGuild->DelMember(lpMsg->id);
}

void DataServer::GuildMemberStatusRequest(uint32 guild, const char * name, uint8 status)
{
	DS_GUILD_MEMBER_STATUS pMsg(guild, name, status);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::GuildMemberStatusResult(uint8 * packet)
{
	POINTER_PCT(DS_GUILD_MEMBER_STATUS, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	pGuild->UpdateMemberRanking(lpMsg->name, lpMsg->status);
	pGuild->UpdateMemberRankingNotify(lpMsg->name, lpMsg->status);
}

void DataServer::GuildRelationshipRequest(uint32 guild01, uint32 guild02, uint8 type, bool operation)
{
	DS_GUILD_RELATIONSHIP pMsg(guild01, guild02, type, operation);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::GuildRelationshipResult(uint8 * packet)
{
	POINTER_PCT(DS_GUILD_RELATIONSHIP, lpMsg, packet, 0);

	if ( lpMsg->result )
	{
		Guild* pGuild01 = sGuildMgr->GetGuild(lpMsg->guild01);
		Guild* pGuild02 = sGuildMgr->GetGuild(lpMsg->guild02);

		if ( !pGuild01 || !pGuild02 )
		{
			return;
		}

		if ( lpMsg->operation ) // Join
		{
			switch ( lpMsg->type )
			{
			case GUILD_RELATIONSHIP_UNION:
				{
					pGuild01->UpdateAlliance(pGuild01->GetID());
					pGuild02->UpdateAlliance(pGuild01->GetID());
				} break;

			case GUILD_RELATIONSHIP_RIVAL:
				{
					pGuild01->UpdateHostil(pGuild02->GetID());
					pGuild02->UpdateHostil(pGuild01->GetID());
				} break;
			}
		}
		else // Break
		{
			switch ( lpMsg->type )
			{
			case GUILD_RELATIONSHIP_UNION:
				{
					if ( sGuildMgr->GetInvolvedGuilds(pGuild01->GetAlliance()) <= 2 )
					{
						pGuild01->UpdateAlliance(0);
					}

					pGuild02->UpdateAlliance(0);
				} break;

			case GUILD_RELATIONSHIP_RIVAL:
				{
					pGuild01->UpdateHostil(0);
					pGuild02->UpdateHostil(0);
				} break;
			}
		}

		pGuild01->UpdateRelationship();
		pGuild02->UpdateRelationship();
	}
}

void DataServer::GuildScore(uint32 guild, int32 score)
{
	DS_GUILD_SCORE pMsg(guild, score);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::GuildScoreResult(uint8 * packet)
{
	POINTER_PCT(DS_GUILD_SCORE, lpMsg, packet, 0);

	Guild * pGuild = sGuildMgr->GetGuild(lpMsg->guild);

	if ( !pGuild )
		return;

	pGuild->IncreaseScore(lpMsg->score);
}

void DataServer::CharacterOnOff(Player* pPlayer, uint8 status)
{
	if ( !pPlayer )
	{
		return;
	}

	DS_CHARACTER_ON_OFF pMsg(pPlayer->GetGuildID(), pPlayer->GetGUID(), status == 0 ? -1 : sGameServer->GetServerCode(), pPlayer->GetName(), status);
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.authority = pPlayer->GetAuthority();
	pMsg.admin_flags = pPlayer->GetAdminPanelFlag();
	pMsg.account_id = pPlayer->GetAccountData()->GetGUID();

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::NoticeSend(uint8 type, const char * notice, ...)
{
	ARG(notice_buffer, notice);

	DS_NOTICE pMsg(notice_buffer, type);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::NoticeSendNormal(uint8 type, const char * notice)
{
	DS_NOTICE pMsg(notice, type);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::NotifySendAllServer(uint8 type, const char* notice) 
{

}

void DataServer::CastleSiegeLoadDataRequest()
{
	DS_CASTLE_SIEGE_LOAD_DATA_REQUEST pMsg;

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::CastleSiegeLoadNpcRequest()
{
	DS_CASTLE_SIEGE_LOAD_NPC_REQUEST pMsg;

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::CastleSiegeLoadRegisteredGuildRequest()
{
	DS_CASTLE_SIEGE_LOAD_REGISTERED_GUILD pMsg;

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::CastleSiegeClearGuild()
{
	DS_CASTLE_SIEGE_CLEAR_GUILD pMsg;

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::CastleSiegeRegisteredGuildRequest(Player* pPlayer, Guild* pGuild)
{
	DS_CASTLE_SIEGE_REGISTERED_GUILD_REQUEST pMsg(pGuild->GetID(), pPlayer->GetEntry());

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::CastleSiegeRegisteredGuildAllRequest(Player* pPlayer)
{
	DS_CASTLE_SIEGE_REGISTERED_GUILD_ALL_REQUEST pMsg(pPlayer->GetEntry());

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::CastleSiegeRegisterGuild(uint32 guild, bool status, uint32 mark)
{
	DS_CASTLE_SIEGE_REGISTER_GUILD pMsg(guild, status, mark);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::CharacterCreateRequest(Player* pPlayer, uint8 race, const char * name, uint8 slot, CharacterBaseData const* info)
{
	DS_CHARACTER_CREATE_REQUEST pMsg;
	pMsg.entry = pPlayer->GetEntry();
	pMsg.ms_time = pPlayer->GetMSTime();
	pMsg.account_id = pPlayer->GetAccountData()->GetGUID();
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.character_class = race;
	memcpy(pMsg.character_name, name, MAX_CHARACTER_LENGTH);
	pMsg.strength = info->GetStat(STRENGTH);
	pMsg.agility = info->GetStat(AGILITY);
	pMsg.vitality = info->GetStat(VITALITY);
	pMsg.energy = info->GetStat(ENERGY);
	pMsg.leadership = info->GetStat(LEADERSHIP);
	pMsg.life = info->GetLife();
	pMsg.mana = info->GetMana();
	pMsg.points = info->GetPoints();
	pMsg.level = info->GetLevel();
	pMsg.world = info->GetWorld();
	pMsg.x = 130;
	pMsg.y = 130;
	pMsg.slot = slot;

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void DataServer::CharacterCreateResult(uint8 * Packet)
{
	POINTER_PCT(DS_CHARACTER_CREATE_RESULT, lpMsg, Packet, 0);

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
		return;

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( !pPlayer )
		return;

	if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_LOGGED )
		return;

	if ( pPlayer->GetMSTime() != lpMsg->ms_time )
		return;

	if ( pPlayer->GetAccountData()->GetCharacterHandleAction() != CHARACTER_HANDLE_ACTION_CREATE )
		return;

	pPlayer->ResetCharacterHandleData();

	if ( lpMsg->result != CHARACTER_CREATE_RESULT_SUCCESS )
	{
		pPlayer->CharacterCreateResult(lpMsg->result);
		return;
	}

	pPlayer->SetCharacterListRequested(false);
	pPlayer->GetCharacterList(lpMsg->slot)->SetName(lpMsg->character_name, MAX_CHARACTER_LENGTH + 1);

	CHARACTER_CREATE_RESULT pMsg(CHARACTER_CREATE_RESULT_SUCCESS);
	pMsg.character_class = lpMsg->character_class;
	memcpy(pMsg.character_name, lpMsg->character_name, MAX_CHARACTER_LENGTH);
	pMsg.level = lpMsg->level;
	pMsg.position = lpMsg->slot;

	pPlayer->sendPacket(MAKE_PCT(pMsg));

	sLog->outInfo(LOG_PLAYER, "[ CHARACTER CREATE ] %s Created character [%s] class %d",
		pPlayer->BuildLog().c_str(), lpMsg->character_name, lpMsg->character_class);
}

void DataServer::CharacterDeleteRequest(Player* pPlayer, const char * name)
{
	DS_CHARACTER_DELETE_REQUEST pMsg;
	pMsg.account_id = pPlayer->GetAccountData()->GetGUID();
	pMsg.entry = pPlayer->GetEntry();
	pMsg.ms_time = pPlayer->GetMSTime();
	pMsg.h.server = sGameServer->GetServerCode();
	memcpy(pMsg.character_name, name, MAX_CHARACTER_LENGTH);

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}
	
void DataServer::CharacterDeleteResult(uint8 * Packet)
{
	POINTER_PCT(DS_CHARACTER_DELETE_RESULT, lpMsg, Packet, 0);

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
		return;

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( !pPlayer )
		return;

	if ( pPlayer->GetConnectStatus() != CONNECT_STATUS_LOGGED )
		return;

	if ( pPlayer->GetMSTime() != lpMsg->ms_time )
		return;

	if ( pPlayer->GetAccountData()->GetCharacterHandleAction() != CHARACTER_HANDLE_ACTION_DELETE )
		return;

	if ( lpMsg->result != CHARACTER_DELETE_RESULT_SUCCESS )
	{
		if ( lpMsg->result != CHARACTER_DELETE_RESULT_WRONG_SECURITY_CODE )
		{
			//pPlayer->CloseSocket();
		}
		else
		{
			//pPlayer->CharacterDeleteResult(lpMsg->result);
		}

		pPlayer->CharacterDeleteResult(lpMsg->result);
	}
	else
	{
		if ( pPlayer->GetAccountData()->GetCurrentCharacterIndex() != uint8(-1) )
		{
			pPlayer->GetCharacterList(pPlayer->GetAccountData()->GetCurrentCharacterIndex())->Reset();
		}

		pPlayer->SetCharacterListRequested(false);
		pPlayer->CharacterDeleteResult(CHARACTER_DELETE_RESULT_SUCCESS);
	}

	pPlayer->ResetCharacterHandleData();
}

void DataServer::AdminCommandRequest(Player* pPlayer, uint8 type, uint16 sub_type, const char * target_name, const char * target_account, time_t time)
{
	DS_ADMIN_COMMAND pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.type = type;
	pMsg.sub_type = sub_type;
	pMsg.entry = pPlayer->GetEntry();
	pMsg.guid = pPlayer->GetGUID();

	if ( target_name && strlen(target_name) > 0 )
		memcpy(pMsg.target_name, target_name, MAX_CHARACTER_LENGTH + 1);

	if ( target_account && strlen(target_account) > 0 )
		memcpy(pMsg.target_account, target_account, MAX_ACCOUNT_LENGTH + 1);

	pMsg.time = time;

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void DataServer::AdminCommandResult(uint8 * Packet)
{
	POINTER_PCT(DS_ADMIN_COMMAND, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( pPlayer && pPlayer->GetGUID() != lpMsg->guid )
		pPlayer = nullptr;

	if ( pPlayer && !pPlayer->IsAdministrator() )
		pPlayer = nullptr;

	if ( !lpMsg->result )
	{
		switch ( lpMsg->type )
		{
		case 0:
			{
				if ( Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name) )
				{
					if ( lpMsg->time > 0 )
					{
						pTarget->SetKickTime(::time(nullptr) + lpMsg->time);
					}

					KICK_PLAYER(pTarget, "Administrator Kick");
				}

				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Player %s kicked.", lpMsg->target_name);
			} break;

		case 1:
			{
				if ( Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name) )
				{
					pTarget->GetRestriction(lpMsg->sub_type)->SetTime(time(nullptr) + lpMsg->time);
					pTarget->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s restriction has been applied for %s", PlayerAction::Name[lpMsg->sub_type].c_str(), secsToTimeString(lpMsg->time).c_str());
				}

				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Applied %s restriction to %s", PlayerAction::Name[lpMsg->sub_type].c_str(), lpMsg->target_name);
			} break;

		case 2:
			{
				if ( Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name) )
				{
					pTarget->ChatMute(lpMsg->time);
				}

				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "%s Muted for %s", lpMsg->target_name, secsToTimeString(lpMsg->time).c_str());
			} break;

		case 3:
			{
				if ( Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name) )
				{
					pTarget->SetAuthority(1);
					KICK_PLAYER(pTarget, "Administrator Banned");

					sGameServer->LogAntiHack(pPlayer, ANTIHACK_ID_KICK, "Administrator Banned");
				}

				PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Player %s banned.", lpMsg->target_name);
			} break;
		}
	}
	else
	{
		PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find player %s.", lpMsg->target_name);
	}
}

void DataServer::WhisperRequest(Player* pPlayer, const char * target_name, const char * message)
{
	DS_WHISPER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.entry = pPlayer->GetEntry();
	pMsg.guid = pPlayer->GetGUID();
	strcpy(pMsg.name, pPlayer->GetName());
	strcpy(pMsg.target_name, target_name);
	strcpy(pMsg.message, message);
	
	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}
	
void DataServer::WhisperResult(uint8 * Packet)
{
	POINTER_PCT(DS_WHISPER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->entry);

	if ( pPlayer && pPlayer->GetGUID() != lpMsg->guid )
		pPlayer = nullptr;

	switch ( lpMsg->result )
	{
	case 0:
		{
			PACKET_CHAT_DATA pMsg(lpMsg->name, lpMsg->message);
			pMsg.h.headcode = HEADCODE_WHISPER;

			//PLAYER_POINTER(pPlayer)->sendPacket(MAKE_PCT(pMsg));

			Player* pTarget = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->target_name);

			if ( pTarget )
			{
				if ( !pTarget->ChatBlockFind(lpMsg->name) )
				{
					pTarget->sendPacket(MAKE_PCT(pMsg));
				}
			}
		} break;

	case 1:
	case 2:
		{
			PLAYER_POINTER(pPlayer)->WhisperOffline();
		} break;

	case 3:
		{
			PLAYER_POINTER(pPlayer)->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not able to whisper yourself.");
		} break;
	}

	//sLog->outInfo(LOG_CHAT, "[ WHISPER ] %s - Say: [%s] To %s",
	//	this->BuildLog().c_str(), lpMsg->message, lpMsg->name);
}

void DataServer::SimpleMessageRequest(const char * name, CustomMessageID type, const char * message, ...)
{
	ARG(buffer, message);

	DS_CHAT_PACKET pMsg(HEADCODE_DATA_SERVER_SIMPLE_MESSAGE, type, name, buffer);
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket((uint8*)&pMsg, pMsg.h.size);
}
	
void DataServer::SimpleMessageResult(uint8 * Packet)
{
	POINTER_PCT(DS_CHAT_PACKET, lpMsg, Packet, 0);

	if ( Player* pPlayer = sObjectMgr->FindPlayerByNameNoSensitive(lpMsg->name) )
	{
		pPlayer->SendNotice(CustomMessageID(lpMsg->guild), lpMsg->msg);
	}
}

void DataServer::Signal(uint8 * Packet)
{
	POINTER_PCT(DS_SIGNAL, lpMsg, Packet, 0);

	if ( lpMsg->h.server == uint16(-1) || lpMsg->h.server == sGameServer->GetServerCode() )
	{
		sGameServer->ProcessSignal(lpMsg->signal, lpMsg->data[0], lpMsg->data[1], lpMsg->data[2]);
	}
}

void DataServer::CrywolfDataRequest()
{
	DS_CRYWOLF_REQUEST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::CrywolfDataResult(uint8 * Packet)
{
	POINTER_PCT(DS_CRYWOLF_REQUEST, lpMsg, Packet, 0);

	sCrywolf->SetOccupationState(lpMsg->status);

	sWorldMgr->statusChange(WORLD_CRYWOLF_FIRST_ZONE, sCrywolf->GetOccupationState());
}

void DataServer::CrywolfDataSave()
{
	DS_CRYWOLF_SAVE pMsg(sCrywolf->GetOccupationState());
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::HandlePacketData(uint8 headcode, uint8 * packet)
{
    auto itr = Handlers.find(headcode);
    if (itr == Handlers.end())
        return;

	(*sDataServer.*Handlers.at(headcode).handler)(packet);
}

void DataServer::GuildMatchingList(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingListRecv(Packet);
}

void DataServer::GuildMatchingInsert(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingInsertRecv(Packet);
}
	
void DataServer::GuildMatchingCancel(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingCancelRecv(Packet);
}
	
void DataServer::GuildMatchingJoinInsert(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinInsertRecv(Packet);
}
	
void DataServer::GuildMatchingJoinCancel(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinCancelRecv(Packet);
}
	
void DataServer::GuildMatchingJoinAccept(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinAcceptRecv(Packet);
}
	
void DataServer::GuildMatchingJoinList(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinListRecv(Packet);
}
	
void DataServer::GuildMatchingJoinInfo(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingJoinInfoRecv(Packet);
}
	
void DataServer::GuildMatchingNotify(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingNotifyRecv(Packet);
}
	
void DataServer::GuildMatchingMasterNotify(uint8 * Packet)
{
	sGuildMatching->DGGuildMatchingNotifyMasterRecv(Packet);
}

void DataServer::EventNotification(uint8 event_id, uint8 open)
{
	DS_EVENT_NOTIFICATION pMsg(event_id, open);
	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::EventNotification(uint8 * Packet)
{
	POINTER_PCT(DS_EVENT_NOTIFICATION, lpMsg, Packet, 0);

	PlayerSessionMap const& players = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator it = players.begin(); it != players.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		if ( !pPlayer->IsPlaying() || pPlayer->IsServerQuit() )
		{
			continue;
		}

		if ( pPlayer->GetEventNotification(lpMsg->event_id) != lpMsg->open )
		{
			pPlayer->SendEventNotification(lpMsg->event_id, lpMsg->open);

			pPlayer->SetEventNotification(lpMsg->event_id, lpMsg->open);
		}
	}
}

void DataServer::EventStateUpdate(uint8 event_id, uint8 state, uint8 occupation_state)
{
	DS_EVENT_STATE_UPDATE pMsg(event_id, state, occupation_state);
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::EventStateUpdate(uint8 * Packet)
{
	POINTER_PCT(DS_EVENT_STATE_UPDATE, lpMsg, Packet, 0);

	if ( lpMsg->h.server != sGameServer->GetServerCode() )
	{
		switch ( lpMsg->event_id )
		{
		case EVENT_CASTLE_SIEGE:
			{
				sCastleSiege->SetState(lpMsg->state);
				sCastleSiege->SetOccupied(lpMsg->occupation_state);
			} break;

		case EVENT_CRYWOLF:
			{
				sCrywolf->SetState(lpMsg->state);
				sCrywolf->SetOccupationState(lpMsg->occupation_state);
			} break;

		case EVENT_ARKA_WAR:
			{
				sArkaWar->ChangeState(lpMsg->state, true);
			} break;

		case EVENT_PROTECTOR_OF_ACHERON:
			{
				sProtectorOfAcheron->SetState(lpMsg->state);
			} break;

		case EVENT_LAST_MAN_STANDING:
			{
				sLastManStanding->SetState(lpMsg->state);
			} break;
		}
	}
}

void DataServer::ArkaWarMasterRegister(Player* pPlayer, uint32 guild)
{
	DS_ARKA_WAR_MASTER_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.guild = guild;

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::ArkaWarMasterRegister(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_MASTER_REGISTER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			sArkaWar->MasterRegisterResult(pPlayer, ARKA_WAR_RESULT_REGISTER_SUCCESS);
		} break;

	case 1:
		{
			sArkaWar->MasterRegisterResult(pPlayer, ARKA_WAR_RESULT_MASTER_REGISTER_ALREADY_REGISTERED);
		} break;
	}
}
	
void DataServer::ArkaWarMemberRegister(Player* pPlayer)
{
	DS_ARKA_WAR_MEMBER_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.guild = pPlayer->GetGuildID();
	pMsg.max_member = sGameServer->GetArkaWarRegisterMaxMembers();

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::ArkaWarMemberRegister(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_MEMBER_REGISTER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			sArkaWar->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_REGISTER_SUCCESS);
		} break;

	case 1:
		{
			sArkaWar->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_ALREADY_REGISTERED);
		} break;

	case 2:
		{
			sArkaWar->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_NOT_IN_GUILD);
		} break;

	case 3:
		{
			sArkaWar->MemberRegisterResult(pPlayer, ARKA_WAR_RESULT_MEMBER_REGISTER_MAXIMUM_MEMBERS);
		} break;
	}
}
	
void DataServer::ArkaWarSignRegister(Player* pPlayer, uint8 result, int32 count)
{
	DS_ARKA_WAR_SIGN_REGISTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	pMsg.guild = pPlayer->GetGuildID();
	pMsg.result = result;
	pMsg.signs = count;

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::ArkaWarSignRegister(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_SIGN_REGISTER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			if ( !Player::InterfaceCheck(pPlayer, InterfaceData::ArcaBattleLesnar, 0) )
			{
				return;
			}

			pPlayer->GetInterfaceState()->Set(InterfaceData::ArkaWarSignRegister, nullptr);
			pPlayer->SetMixCompleted(false);
			pPlayer->GetMixInventory()->Clear();
									
			sArkaWar->SignOfLordRegisterResult(pPlayer, 0, lpMsg->signs);
		} break;

	case 1:
		{

		} break;

	case 2:
		{
			sArkaWar->SignOfLordRegisterResult(pPlayer, ARKA_WAR_RESULT_ENTER_NOT_REGISTERED);
		} break;
	}
}
	
void DataServer::ArkaWarEnter(Player* pPlayer)
{
	DS_ARKA_WAR_ENTER pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::ArkaWarEnter(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_ENTER, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			//sArkaWar->EnterResult(pPlayer, 10);

			pPlayer->MoveToGate(426);
		} break;

	case 1:
		{
			sArkaWar->EnterResult(pPlayer, ARKA_WAR_RESULT_ENTER_NOT_REGISTERED);
		} break;
	}
}

void DataServer::ArkaWarList()
{
	DS_ARKA_WAR_LIST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.min_member = sGameServer->GetArkaWarRegisterMinMembers();
	pMsg.max_guild = MAX_ARKA_WAR_GUILD;

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::ArkaWarList(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_LIST_HEAD, head, Packet, 0);

	if ( head->type == 0 )
	{
		sArkaWar->GuildList(Packet);
	}
	else if ( head->type == 1 )
	{
		sArkaWar->MemberList(Packet);
	}
}
	
void DataServer::ArkaWarClear()
{
	DS_ARKA_WAR_CLEAR pMsg;
	pMsg.h.server = sGameServer->GetServerCode();

	this->SendPacket(MAKE_PCT(pMsg));
}

void DataServer::ArkaWarMemberCount(Player* pPlayer)
{
	DS_ARKA_WAR_MEMBER_COUNT pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.guild = pPlayer->GetGuildID();
	pPlayer->BuildCustomPacketData(pMsg.player);

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::ArkaWarMemberCount(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_MEMBER_COUNT, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayer(lpMsg->player.entry, lpMsg->player.ms_time, lpMsg->player.account_id, lpMsg->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->result )
	{
	case 0:
		{
			sArkaWar->ViewGuildMemberResult(pPlayer, lpMsg->result, lpMsg->count);
		} break;

	case 1:
		{
			sArkaWar->ViewGuildMemberResult(pPlayer, 2, lpMsg->count);
		} break;

	case 2:
		{
			sArkaWar->ViewGuildMemberResult(pPlayer, 2, lpMsg->count);
		} break;
	}
}

void DataServer::ArkaWarGuildRequest()
{
	DS_ARKA_WAR_GUILD_REQUEST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::ArkaWarGuildRequest(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_GUILD_HEAD, head, Packet, 0);
	POINTER_PCT(DS_ARKA_WAR_GUILD_BODY, body, Packet, sizeof(DS_ARKA_WAR_GUILD_HEAD));

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		sArkaWar->GetOccupyGuild(i)->Clear();
	}

	for ( int32 i = 0; i < head->count; ++i )
	{
		if ( body[i].id < MAX_ARKA_WAR_ZONES )
		{
			sArkaWar->GetOccupyGuild(body[i].id)->SetGuild(body[i].guild);
			sArkaWar->GetOccupyGuild(body[i].id)->SetAttribute(body[i].attribute);
		}
	}
}
	
void DataServer::ArkaWarGuildSave()
{
	uint8 buffer[8192];
	POINTER_PCT(DS_ARKA_WAR_GUILD_HEAD, head, buffer, 0);
	POINTER_PCT(DS_ARKA_WAR_GUILD_BODY, body, buffer, sizeof(DS_ARKA_WAR_GUILD_HEAD));
	head->count = 0;

	for ( int32 i = 0; i < MAX_ARKA_WAR_ZONES; ++i )
	{
		if ( sArkaWar->GetOccupyGuild(i)->GetGuild() == 0 )
		{
			continue;
		}

		body[head->count].id = i;
		body[head->count].guild = sArkaWar->GetOccupyGuild(i)->GetGuild();
		body[head->count].attribute = sArkaWar->GetOccupyGuild(i)->GetAttribute();
		++head->count;
	}

	head->h.set(HEADCODE_DATA_SERVER_ARKA_WAR_GUILD_SAVE, sizeof(DS_ARKA_WAR_GUILD_HEAD) + (head->count * sizeof(DS_ARKA_WAR_GUILD_BODY)));
	this->SendPacket(buffer, head->h.get_size());
}

void DataServer::ArkaWarSignOfLordCheckRequest(Player* pPlayer)
{
	DS_ARKA_WAR_SIGN_REQUEST pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pPlayer->BuildCustomPacketData(pMsg.player);
	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::ArkaWarSignOfLordCheckResult(uint8 * Packet)
{
	POINTER_PCT(DS_ARKA_WAR_SIGN_REQUEST_HEAD, lpHead, Packet, 0);
	POINTER_PCT(DS_ARKA_WAR_SIGN_REQUEST_BODY, lpBody, Packet, sizeof(DS_ARKA_WAR_SIGN_REQUEST_HEAD));

	Player* pPlayer = sObjectMgr->FindPlayer(lpHead->player.entry, lpHead->player.ms_time, lpHead->player.account_id, lpHead->player.guid);

	if ( !pPlayer )
	{
		return;
	}

	uint8 buffer[8192];
	POINTER_PCT(ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD, head, buffer, 0);
	POINTER_PCT(ARKA_WAR_SIGN_OF_LORD_CHECK_BODY, body, buffer, sizeof(ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD));
	head->count = 0;
	head->rank = 0;
	memset(head->signs, 0, 4);

	for ( int32 i = 0; i < lpHead->count; ++i )
	{
		if ( Guild * pGuild = sGuildMgr->GetGuild(lpBody[i].guild) )
		{
			if ( lpBody[i].guild == pPlayer->GetGuildID() )
			{
				head->rank = i + 1;
				head->signs[0] = HIBYTE(HIWORD(lpBody[i].signs));
				head->signs[1] = LOBYTE(HIWORD(lpBody[i].signs));
				head->signs[2] = HIBYTE(LOWORD(lpBody[i].signs));
				head->signs[3] = LOBYTE(LOWORD(lpBody[i].signs));
			}
			
			if (head->count < MAX_ARKA_WAR_GUILD)
			{
				body[head->count].rank = i + 1;
				body[head->count].signs[0] = HIBYTE(HIWORD(lpBody[i].signs));
				body[head->count].signs[1] = LOBYTE(HIWORD(lpBody[i].signs));
				body[head->count].signs[2] = HIBYTE(LOWORD(lpBody[i].signs));
				body[head->count].signs[3] = LOBYTE(LOWORD(lpBody[i].signs));
				memset(body[head->count].name, 0, MAX_GUILD_NAME_LENGTH + 1);
				memcpy(body[head->count].name, pGuild->GetName(), MAX_GUILD_NAME_LENGTH);
				++head->count;
			}
		}
	}

	head->h.set(HEADCODE_GEN_SYSTEM, SUBCODE_ARKA_WAR_SIGN_OF_LORD_CHECK_RESULT, sizeof(ARKA_WAR_SIGN_OF_LORD_CHECK_HEAD) + (sizeof(ARKA_WAR_SIGN_OF_LORD_CHECK_BODY) * head->count));
	pPlayer->sendPacket(buffer, head->h.get_size());	
}

void DataServer::CharacterSignal(uint32 id, uint8 type)
{
	DS_CHARACTER_SIGNAL pMsg;
	pMsg.h.server = sGameServer->GetServerCode();
	pMsg.id = id;
	pMsg.type = type;

	this->SendPacket(MAKE_PCT(pMsg));
}
	
void DataServer::CharacterSignal(uint8 * Packet)
{
	POINTER_PCT(DS_CHARACTER_SIGNAL, lpMsg, Packet, 0);

	Player* pPlayer = sObjectMgr->FindPlayerByGUID(lpMsg->id);

	if ( !pPlayer )
	{
		return;
	}

	switch ( lpMsg->type )
	{
case 0:
{
pPlayer->GetTimer(PLAYER_TIMER_MAIL)->m_Timer = 0;
} break;

case 1:
{
pPlayer->GetTimer(PLAYER_TIMER_CASH_SHOP_GIFT)->m_Timer = 0;
} break;
	}
}