LoginQueue::LoginQueue()
{
        this->m_UpdateTick = getMSTime();
        this->m_VerifyTick = getMSTime();
}

LoginQueue::~LoginQueue()
{
	for ( AccountLoggedMap::iterator itr = account_logged_map.begin(); itr != account_logged_map.end(); ++itr )
	{
		itr->second.clear();
	}

	account_logged_map.clear();
	this->account_wrong_authorization_list.clear();

	this->LoginDataQueue.clear();
}

void LoginQueue::Queue(LoginDataPtr data)
{
	LoginDataQueue.add(data);
}

void LoginQueue::Update()
{
        if ( GetMSTimeDiffToNow(this->m_UpdateTick) > 500 )
        {
                this->m_UpdateTick = getMSTime();

		for ( AccountLoggedMap::iterator itr = account_logged_map.begin(); itr != account_logged_map.end(); ++itr )
		{
			for ( AccountLoggedList::iterator it = itr->second.begin(); it != itr->second.end(); )
			{
				AccountDataPtr pAccountData = it->second;

				if ( !pAccountData )
				{
					itr->second.erase(it++);
					continue;
				}

                                if ( pAccountData->m_Offline )
				{
					this->DBAccountStatus(pAccountData, false, false);
					itr->second.erase(it++);
					continue;
				}

                                if ( pAccountData->m_Moving )
                                {
                                        if ( sMain->m_AccountMovingTime > 0 )
                                        {
                                                if ( GetMSTimeDiffToNow(pAccountData->m_MovingTick) > sMain->m_AccountMovingTime )
                                                {
                                                        sLog->outInfo("player", "Removing Player [%u][%s] - Server Move Time Out.", pAccountData->guid, pAccountData->account);
                                                        pAccountData->m_Offline = true;
                                                }
                                        }
                                }

				++it;
			}
		}
	}

        if ( GetMSTimeDiffToNow(this->m_VerifyTick) > 30000 )
        {
                this->m_VerifyTick = getMSTime();
        }

	LoginDataPtr data;
	uint32 processedPackets = 0;

	while ( !this->LoginDataQueue.empty() && this->LoginDataQueue.next(data) )
	{
		this->Process(data);

#define MAX_PROCESSED_PACKETS_IN_SAME_WORLDSESSION_UPDATE 100

		processedPackets++;

		if (processedPackets > MAX_PROCESSED_PACKETS_IN_SAME_WORLDSESSION_UPDATE)
			break;
	}
}

void LoginQueue::Process(LoginDataPtr data)
{
	ACCOUNT_LOGIN_RESULT pMsg(LOGIN_RESULT_WRONG_ACCOUNT, data->entry);
	pMsg.h.server = data->server;
	pMsg.ms_time = data->ms_time;
	memcpy(pMsg.account, data->account, MAX_ACCOUNT_LENGTH);

	std::string password = std::string(data->password);

	strToLower(password);

	PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNT_LOGIN);
        stmt->setUInt16(0, data->GetServerGroup());
	stmt->setUInt16(1, data->server);
	stmt->setUInt32(2, data->disk_serial);
	stmt->setUInt32(3, data->disk_serial);
	stmt->setUInt32(4, data->disk_serial);
	stmt->setString(5, data->mac);
	stmt->setString(6, data->account);
	stmt->setString(7, sha256(std::string(data->account) + std::string(":") + password));
	stmt->setUInt32(8, data->autologin ? data->account_id : 0);
	stmt->setString(9, data->account);
	stmt->setUInt32(10, data->disk_serial);
	stmt->setString(11, data->mac);

	PreparedQueryResult result = LoginDatabase.Query(stmt);

	uint8 acctype = 0;

	if ( result )
	{
		time_t current_time = time(nullptr);

		Field* fields = result->Fetch();
		pMsg.account_id = fields[0].GetUInt32();

		if (pMsg.account_id != 0)
		{
			uint8 blocked = fields[1].GetUInt8();
			pMsg.golden_channel = fields[6].GetInt64();
			uint32 validate_account = fields[7].GetUInt32();
			uint32 warning_account = fields[8].GetUInt32();
			time_t warning_time = fields[9].GetInt64();
			uint8 secured = fields[10].GetUInt8();
			std::string account = fields[11].GetString();
			uint32 banned_account_id = fields[12].GetUInt32();
			time_t unban_date = fields[13].GetInt64();
			uint32 master_disk = fields[14].GetUInt32();
			int32 count = 0;

			if (fields[2].GetCString())
			{
				memcpy(pMsg.security_code, fields[2].GetCString(), MAX_SECURE_CODE_LENGTH);
			}

			pMsg.facebook_status = fields[5].GetUInt8();

			bool online = fields[3].GetBool();

			stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNTS_STATUS_COUNT_SELECT);
			stmt->setUInt32(0, pMsg.account_id);
			PreparedQueryResult count_result = LoginDatabase.Query(stmt);

			if (count_result)
			{
				Field* fields = count_result->Fetch();
				count = fields[0].GetInt32();
			}

			if (!strlen(pMsg.account))
			{
				strcpy(pMsg.account, account.c_str());
			}

                        if (!warning_account || (sMain->m_AccountWrongAuthorizationTime <= 0) || (current_time >= (warning_time + sMain->m_AccountWrongAuthorizationTime)))
                        {
                                warning_account = 0;
                                this->ClearWrongAccountAuthorization(pMsg.account_id, data->disk_serial);
                        }

                        if (validate_account || !sMain->m_AccountAuthorizationEnabled || !secured || count <= 0 || master_disk != 0)
                        {
                                pMsg.authorized = true;
                        }

			pMsg.result = LOGIN_RESULT_SUCCESS;

			QueryResult resulttype = LoginDatabase.PQuery("select type from accounts_status where account_id=%d", pMsg.account_id);

			if (resulttype) {
				Field* field = resulttype->Fetch();
				acctype = field[0].GetUInt8();
			}

			if (master_disk == 0 && banned_account_id && (!unban_date || current_time < unban_date))
			{
				pMsg.result = LOGIN_RESULT_BANNED;
			}
			else if (master_disk == 0 && (blocked || warning_account)) // Retorno error si la cuenta est� bloqueada
			{
				pMsg.result = LOGIN_RESULT_BANNED;
			}
			//else if (online && acctype == 0) // Retorno error si la cuenta est� conectada dentro del mismo grupo de servidores
			/*else if (online)
			{
				pMsg.result = LOGIN_RESULT_ONLINE;
			}*/
			else
			{
				ServerData & server_data = sServer->server_map[data->server];

                                if (server_data.m_Online && server_data.m_Port != uint16(-1))
                                {
                                        if ((server_data.m_Flag & SERVER_FLAG_DISABLED) != 0) // Retorno error si el servidor está deshabilitado
                                        {
                                                pMsg.result = LOGIN_RESULT_MAX_USER;
                                        }
                                        else if (master_disk == 0 && !fields[4].GetUInt32() && (server_data.m_Flag & SERVER_FLAG_ACCOUNT_ALLOWED)) // Retorno error si el servidor tiene acceso solamente a cuentas de la lista
                                        {
                                                pMsg.result = LOGIN_RESULT_MAX_USER;
                                        }
                                        else if (master_disk == 0 && (current_time > pMsg.golden_channel) && (server_data.m_Type == SERVER_TYPE_GOLDEN_PVP || server_data.m_Type == SERVER_TYPE_GOLDEN_NO_PVP))
                                        {
                                                pMsg.result = LOGIN_RESULT_ERROR;
                                        }
				}
				else // Retorno error si el servidor no existe en la base de datos ( conexi�n ileg�tima )
				{
					pMsg.result = LOGIN_RESULT_THREE_TIMES;
				}
			}

			if (pMsg.result == LOGIN_RESULT_SUCCESS)
			{
				stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_BLOCK_MAC_SELECT);
				stmt->setString(0, data->mac);

				PreparedQueryResult mac_result = LoginDatabase.Query(stmt);

				if (mac_result)
				{
					pMsg.result = LOGIN_RESULT_WRONG_ACCOUNT;
				}
				else
				{
					stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_BLOCK_DISK_SERIAL_SELECT);
					stmt->setUInt32(0, data->disk_serial);

					PreparedQueryResult disk_serial_result = LoginDatabase.Query(stmt);

					if (disk_serial_result)
					{
						pMsg.result = LOGIN_RESULT_WRONG_ACCOUNT;
					}
					else
					{
						stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_BLOCK_IP_SELECT);
						stmt->setString(0, data->ip);

						PreparedQueryResult ip_result = LoginDatabase.Query(stmt);

						if (ip_result)
						{
							pMsg.result = LOGIN_RESULT_WRONG_ACCOUNT;
						}
					}
				}
			}

			if (pMsg.result == LOGIN_RESULT_SUCCESS)
			{
                                if (acctype >= 0 || !this->IsAccountLogged(data->GetServerGroup(), pMsg.account))
                                {
                                        this->ConnectAccount(data->server, data->server, pMsg.account, pMsg.account_id, pMsg.security_code, data->ip, data->mac, pMsg.facebook_status, pMsg.golden_channel, data->disk_serial, pMsg.authorized, acctype);

					ACCOUNT_KICK pMsg(pMsg.account_id);
					pMsg.h.server = data->server;
					pMsg.type = 1;
					sAuthSocketMgr.SendPacketAll((uint8*)&pMsg, pMsg.h.get_size(), pMsg.h.GetServerGroup());
					this->KickAccount(data->GetServerGroup(), pMsg.account_id);

					if (count <= 0)
					{
						SQLTransaction trans = LoginDatabase.BeginTransaction();

						PreparedStatement* stmt_validate = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNTS_VALIDATION_INSERT);
						stmt_validate->setUInt32(0, pMsg.account_id);
						stmt_validate->setUInt32(1, data->disk_serial);
						trans->Append(stmt_validate);

						LoginDatabase.CommitTransaction(trans);
					}
				}
				else
				{
					pMsg.result = LOGIN_RESULT_ONLINE;
				}
			}
		}
	}

	sLog->outInfo("player", "%s -- [Acc: %s][IP: %s][MAC: %s][SERIAL: %u] Result: %u", __FUNCTION__, pMsg.account, data->ip, data->mac, data->disk_serial, pMsg.result);

	if ( pMsg.result == LOGIN_RESULT_ONLINE )
	{
		ACCOUNT_ONLINE pMsg2(pMsg.account);
		pMsg2.account_id = pMsg.account_id;
		sAuthSocketMgr.SendPacketAll((uint8*)&pMsg2, pMsg2.h.get_size(), data->GetServerGroup());
	}

	if (pMsg.result == LOGIN_RESULT_SUCCESS)
	{
		DS_ACCOUNT_DUPLICATE pMsg3;
		pMsg3.h.server = pMsg.h.server;
		pMsg3.account_id = pMsg.account_id;
		sAuthSocketMgr.SendPacketAll((uint8*)&pMsg3, pMsg3.h.get_size(), data->GetServerGroup());
	}


	if ( data->socket && data->socket->IsOpen() )
		data->socket->SendPacket((uint8*)&pMsg, pMsg.h.size);
}

void LoginQueue::ConnectAccount(uint16 start_server, uint16 current_server, const char * account, uint32 guid, const char * secure_code, const char * ip, const char * mac, uint8 facebook_status, time_t golden_channel, uint32 disk_serial, bool authorized, uint8 acctype)
{
	AccountDataPtr pAccountData = this->GetAccountData(start_server / MAX_SERVER_PER_GROUP, guid);
	bool add = false;

	if ( !pAccountData )
	{
		pAccountData = AccountDataPtr(new AccountData());
		add = true;
	}

	memcpy(pAccountData->account, account, MAX_ACCOUNT_LENGTH);
        pAccountData->m_StartServer = start_server;
        pAccountData->m_CurrentServer = current_server;
        memcpy(pAccountData->m_MAC, mac, sizeof(pAccountData->m_MAC));
        memcpy(pAccountData->m_IP, ip, sizeof(pAccountData->m_IP));
        memcpy(pAccountData->m_SecureCode, secure_code, sizeof(pAccountData->m_SecureCode));
        pAccountData->m_FacebookStatus = facebook_status;
        pAccountData->m_GoldenChannel = golden_channel;
        pAccountData->m_DiskSerial = disk_serial;
        pAccountData->guid = guid;
        pAccountData->m_Time = time(nullptr);
        pAccountData->m_DestServer = -1;
        pAccountData->m_DestWorld = -1;
        pAccountData->m_DestX = -1;
        pAccountData->m_DestY = -1;
        pAccountData->m_Authorized = authorized;
        pAccountData->m_AccType = acctype;

	if ( add )
	{
		this->account_logged_map[start_server / MAX_SERVER_PER_GROUP][guid] = pAccountData;

		sLog->outInfo("player", "Adding Player [%u][%s]", pAccountData->guid, pAccountData->account);
	}
	
	this->DBAccountStatus(pAccountData, true, true);
}

void LoginQueue::DisconnectAccount(uint16 server, uint32 account)
{
	AccountDataPtr pAccountData = this->GetAccountData(server / MAX_SERVER_PER_GROUP, account);

	if ( !pAccountData )
	{
		return;
	}

        if ( pAccountData->m_CurrentServer != server )
        {
                return;
        }

        if ( !pAccountData->m_Moving )
        {
                if (pAccountData->m_AccType == 0) {
                        sLog->outInfo("player", "Removing Player [%u][%s] - Disconnected.", pAccountData->guid, pAccountData->account);
                        pAccountData->m_Offline = true;
                }
        }

        pAccountData->m_AccType = 0;
}

void LoginQueue::CloseServer(uint16 server)
{
	for ( AccountLoggedMap::iterator itr = account_logged_map.begin(); itr != account_logged_map.end(); ++itr )
	{
		for ( AccountLoggedList::iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			AccountDataPtr pAccountData = it->second;

			if ( !pAccountData )
			{
				continue;
			}

                        if ( pAccountData->m_CurrentServer == server )
                        {
                                sLog->outInfo("player", "Removing Player [%u][%s] - Server Closed.", pAccountData->guid, pAccountData->account);
                                pAccountData->m_Offline = true;
                        }
                }
        }
}

void LoginQueue::KickAccount(uint16 server_group, uint32 account_id)
{
	AccountDataPtr pAccountData = this->GetAccountData(server_group, account_id);

        if ( !pAccountData )
        {
                return;
        }

        sLog->outInfo("player", "Removing Player [%u][%s] - Kicked.", pAccountData->guid, pAccountData->account);
        pAccountData->m_Offline = true;
}

bool LoginQueue::IsAccountLogged(uint16 server_group, const char * account)
{
	AccountLoggedMap::iterator itr = this->account_logged_map.find(server_group);

	if ( itr == this->account_logged_map.end() )
	{
		return false;
	}

	for ( AccountLoggedList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
	{
		AccountDataPtr pAccountData = it->second;

		if ( !pAccountData )
		{
			continue;
		}

		if ( !memcmp(pAccountData->account, account, MAX_ACCOUNT_LENGTH) )
		{
			return true;
		}
	}

	return false;
}

AccountDataPtr LoginQueue::GetAccountData(uint16 server_group, uint32 account)
{
	AccountLoggedMap::iterator itr = this->account_logged_map.find(server_group);

	if ( itr == this->account_logged_map.end() )
	{
		return nullptr;
	}

	AccountLoggedList::iterator it = itr->second.find(account);

	if ( it != itr->second.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

AccountDataPtr LoginQueue::GetAccountData(uint16 server_group, const char * account)
{
	AccountLoggedMap::iterator itr = this->account_logged_map.find(server_group);

	if ( itr == this->account_logged_map.end() )
	{
		return nullptr;
	}

	for ( AccountLoggedList::iterator it = itr->second.begin(); it != itr->second.end(); ++it )
	{
		AccountDataPtr pAccountData = it->second;

		if ( !pAccountData )
		{
			continue;
		}

		if ( !memcmp(account, pAccountData->account, MAX_ACCOUNT_LENGTH) )
		{
			return pAccountData;
		}
	}

	return nullptr;
}

void LoginQueue::DBAccountStatus(AccountDataPtr pAccount, bool online, bool on_connect)
{
	SQLTransaction trans = LoginDatabase.BeginTransaction();

	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNT_STATUS_DELETE);
        stmt->setUInt32(0, pAccount->guid);
        stmt->setUInt16(1, pAccount->m_StartServer / MAX_SERVER_PER_GROUP);
	trans->Append(stmt);

        stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNT_STATUS_UPDATE);
        stmt->setUInt32(0, pAccount->guid);
        stmt->setUInt16(1, pAccount->m_StartServer / MAX_SERVER_PER_GROUP);
        stmt->setUInt16(2, pAccount->m_CurrentServer);
        stmt->setUInt16(3, pAccount->m_StartServer);
        stmt->setInt16(4, pAccount->m_DestServer);
        stmt->setInt16(5, pAccount->m_DestWorld);
        stmt->setInt16(6, pAccount->m_DestX);
        stmt->setInt16(7, pAccount->m_DestY);
        stmt->setUInt32(8, pAccount->m_MovingTick);
        stmt->setUInt32(9, pAccount->m_Auth[0]);
        stmt->setUInt32(10, pAccount->m_Auth[1]);
        stmt->setUInt32(11, pAccount->m_Auth[2]);
        stmt->setUInt32(12, pAccount->m_Auth[3]);
        stmt->setString(13, pAccount->m_IP);
        stmt->setString(14, pAccount->m_MAC);
        stmt->setString(15, ConvertTimeToString(pAccount->m_Time));
        stmt->setBool(16, online);
        stmt->setUInt32(17, pAccount->m_DiskSerial);
        stmt->setUInt8(8, pAccount->m_AccType);
        trans->Append(stmt);

	if ( on_connect )
	{
                /*stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNT_SECURITY_DELETE);
                stmt->setUInt32(0, pAccount->guid);
                stmt->setString(1, pAccount->m_IP);
                stmt->setString(2, pAccount->m_MAC);
                stmt->setUInt32(3, pAccount->m_DiskSerial);
                trans->Append(stmt);*/

                stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_ACCOUNT_SECURITY_INSERT);
                stmt->setUInt32(0, pAccount->guid);
                stmt->setString(1, pAccount->account);
                stmt->setString(2, pAccount->m_IP);
                stmt->setString(3, pAccount->m_MAC);
                stmt->setUInt32(4, pAccount->m_DiskSerial);
                trans->Append(stmt);
        }

	LoginDatabase.CommitTransaction(trans);
}

void LoginQueue::CompleteAccountBan(const char * ip, const char * mac, uint32 disk_serial, uint8 type)
{
	SQLTransaction trans = LoginDatabase.BeginTransaction();

	PreparedStatement* stmt = nullptr;

	/*stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_COMPLETE_BAN_ACCOUNT_1);
	stmt->setUInt32(0, disk_serial);
	trans->Append(stmt);

	stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_COMPLETE_BAN_ACCOUNT_2);
	stmt->setString(0, mac);
	trans->Append(stmt);

	stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_COMPLETE_BAN_ACCOUNT_3);
	stmt->setString(0, ip);
	trans->Append(stmt);*/

	stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_COMPLETE_BAN_INSERT_1);
	stmt->setUInt32(0, disk_serial);
	stmt->setString(1, std::string("Automatic BAN Type " + std::to_string(type)));
	trans->Append(stmt);

	stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_COMPLETE_BAN_INSERT_2);
	stmt->setString(0, mac);
	stmt->setString(1, std::string("Automatic BAN Type " + std::to_string(type)));
	trans->Append(stmt);

	stmt = LoginDatabase.GetPreparedStatement(QUERY_LOGIN_COMPLETE_BAN_INSERT_3);
	stmt->setString(0, ip);
	stmt->setString(1, std::string("Automatic BAN Type " + std::to_string(type)));
	trans->Append(stmt);

	LoginDatabase.CommitTransaction(trans);
}

void LoginQueue::ClearWrongAccountAuthorization(uint32 account_id, uint32 disk_serial)
{
	for ( WrongAuthorizationDataList::iterator it = this->account_wrong_authorization_list.begin(); it != this->account_wrong_authorization_list.end(); )
	{
		if ( it->account_id == account_id && it->disk_serial == disk_serial )
		{
			it = this->account_wrong_authorization_list.erase(it);
		}
		else
		{
			++it;
		}
	}
}
	
bool LoginQueue::IncreaseWrongAccountAuthorization(uint32 account_id, uint32 disk_serial)
{
	int32 count = 0;

	for ( WrongAuthorizationDataList::iterator it = this->account_wrong_authorization_list.begin(); it != this->account_wrong_authorization_list.end(); ++it )
	{
		if ( it->account_id == account_id && it->disk_serial == disk_serial )
		{
			++it->count;
			count = it->count;
			break;
		}
	}

	if ( count <= 0 )
	{
		this->account_wrong_authorization_list.push_back(WrongAuthorizationData(account_id, disk_serial, 1));
		count = 1;
	}
	
        if ( sMain->m_AccountWrongAuthorizationCount > 0 && (count >= sMain->m_AccountWrongAuthorizationCount) )
        {
                return true;
        }

	return false;
}