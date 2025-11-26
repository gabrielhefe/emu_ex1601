CommonRace::CommonRace()
	{
	this->m_GateReached = 0;
	}

CommonRace::~CommonRace()
	{
	this->Clear();
	}

void CommonRace::Clear()
	{
	LIST_CLEAR(CommonRaceGateList::iterator, this->m_GateList);
	}

void CommonRace::Start(int32 duration, int32 notify)
	{
	if ( this->GetState() != COMMON_RACE_STATE_NONE )
	return;

	this->m_Duration = duration;
	this->SetNotifyTime(notify);

	if ( notify > 0 )
	{
	this->SetState_Notify();
	}
	else
	{
	this->SetState_Playing();
	}
	}

void CommonRace::Update()
	{
	switch ( this->GetState() )
	{
	case COMMON_RACE_STATE_NONE:
	{
	this->ProcState_None();
	} break;

	case COMMON_RACE_STATE_NOTIFY:
	{
	this->ProcState_Notify();
	} break;

	case COMMON_RACE_STATE_PLAYING:
	{
	this->ProcState_Playing();
	} break;
	}
	}

void CommonRace::SetState_None()
	{
	this->SetState(COMMON_RACE_STATE_NONE);
	this->RunTime(0);
	this->m_GateReached = 0;
	this->m_PlayerData.clear();

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "%s race finished.", this->m_RaceName.c_str());

	sLog->outInfo(LOG_RACE, "[ %s ] SetState_None()", this->m_RaceName.c_str());
	}

void CommonRace::SetState_Notify()
	{
	this->SetState(COMMON_RACE_STATE_NOTIFY);
	this->RunTime(this->GetNotifyTime() * MINUTE * IN_MILLISECONDS);
	this->m_GateReached = 0;
	this->m_PlayerData.clear();

	sLog->outInfo(LOG_RACE, "[ %s ] SetState_Notify()", this->m_RaceName.c_str());
	}

void CommonRace::SetState_Playing()
	{
	this->SetState(COMMON_RACE_STATE_PLAYING);
	this->RunTime(this->m_Duration * MINUTE * IN_MILLISECONDS);
	this->m_GateReached = 0;
	this->m_PlayerData.clear();

	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "%s race started.", this->m_RaceName.c_str());

	sLog->outInfo(LOG_RACE, "[ %s ] SetState_Playing()", this->m_RaceName.c_str());
	}

void CommonRace::ProcState_None()
	{
	}

void CommonRace::ProcState_Notify()
	{
	this->SendRemainMinutes("%d minute(s) left before " + this->m_RaceName + " race starts.", 5, 15);

	if ( this->TimePassed() )
	this->SetState_Playing();
	}

void CommonRace::ProcState_Playing()
	{
	this->SendRemainMinutes("%d minute(s) left before " + this->m_RaceName + " race ends.", 5, 10);

	if ( this->TimePassed() )
	this->SetState_None();
	}

void CommonRace::PlayerInGate(Player* pPlayer, int32 gate)
	{
	if ( !pPlayer )
	{
	return;
	}

	if ( this->GetState() != COMMON_RACE_STATE_PLAYING )
	{
	return;
	}

	CommonRacePlayer & PlayerData = this->m_PlayerData[pPlayer->GetGUID()];

	for ( size_t i = 0; i < this->m_GateList.size(); ++i )
	{
	if ( this->m_GateList[i]->m_Gate != gate )
	continue;

	if ( PlayerData.m_GateList.size() == i )
	{
	if ( !PlayerData.m_Participating )
	{
	PlayerData.m_StartedTime = time(nullptr);
	}

	PlayerData.m_Participating = true;
	PlayerData.m_GateList.push_back(gate);

	if ( this->m_GateReached == i )
	{
	this->m_GateReached += 1;

	if ( this->m_GateReached == this->m_GateList.size() )
	{
	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "[ %s race ] Finished! %s is the winner",
	this->m_RaceName.c_str(), pPlayer->GetName());

	if ( this->m_RaceName == "Dungeon" )
	{
	sItemBagMgr->RunItemBag(pPlayer, sGameServer->GetDungeonRaceReward(), Item(), true, GremoryCaseReward::EVENT);
	}
	else if ( this->m_RaceName == "LostTower" )
	{
	sItemBagMgr->RunItemBag(pPlayer, sGameServer->GetLosttowerRaceReward(), Item(), true, GremoryCaseReward::EVENT);
	}

	SQLTransaction trans = MuDatabase.BeginTransaction();

	PreparedStatement* stmt = MuDatabase.GetPreparedStatement(QUERY_MUDATABASE_EVENT_RANKING_RACE);
	stmt->setUInt32(0, pPlayer->GetGUID());
	stmt->setString(1, this->m_RaceName);
	stmt->setString(2, TimeToTimestampStr(PlayerData.m_StartedTime));
	trans->Append(stmt);

	MuDatabase.CommitTransaction(trans);

	this->SetState_None();
	}
	else
	{
	sObjectMgr->SendEventNotification(NOTICE_GLOBAL, "[ %s race ] %s reached gate number %d",
	this->m_RaceName.c_str(), pPlayer->GetName(), i + 1);
	}
	}

	return;
	}

	break;
	}

	this->ResetPlayer(pPlayer);
	}

void CommonRace::ResetPlayer(Player* pPlayer)
	{
	if ( this->GetState() != COMMON_RACE_STATE_PLAYING )
	return;

	CommonRacePlayer & PlayerData = this->m_PlayerData[pPlayer->GetGUID()];

	if ( PlayerData.m_Participating )
	{
	pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "You have been disqualified from the race.");
	}

	PlayerData.m_Participating = false;
	PlayerData.m_GateList.clear();
	}
