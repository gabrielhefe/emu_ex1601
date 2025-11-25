/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "BattleSoccer.cpp"
*
*/
BattleSoccerTeam::BattleSoccerTeam()
{
        this->Reset();
}

void BattleSoccerTeam::Reset()
{
        this->m_Guild = nullptr;
        this->m_Score = 0;
        this->m_PartyID = static_cast<uint16>(-1);

        PARTY_LOOP(i)
        {
                this->m_Player[i].Reset();
        }
}

CBattleSoccerMgr::CBattleSoccerMgr()
{
        this->SetState_None();
}

CBattleSoccerMgr::~CBattleSoccerMgr()
{

}

void CBattleSoccerMgr::Update()
{
	switch ( this->m_State )
	{
	case BATTLE_SOCCER_STATE_NONE:
		{
			this->ProcState_None();
		} break;

	case BATTLE_SOCCER_STATE_STANDBY:
		{
			this->ProcState_StandBy();
		} break;

	case BATTLE_SOCCER_STATE_PLAYING:
		{
			this->ProcState_Playing();
		} break;

	case BATTLE_SOCCER_STATE_PLAYEND:
		{
			this->ProcState_Playend();
		} break;
	}
}

void CBattleSoccerMgr::SetState_None()
{
	this->m_State = BATTLE_SOCCER_STATE_NONE;
	this->m_Time.Start(0);
	this->m_BallMoveTime.Reset();
	this->m_BallMove = false;
	
	GUILD_WAR_TEAM_LOOP(i)
	{
		this->m_Team[i].Reset();
	}
}
	
void CBattleSoccerMgr::SetState_StandBy()
{
	this->m_State = BATTLE_SOCCER_STATE_STANDBY;
	this->m_Time.Start(sGameServer->GetGuildWarMaxWaitInterval() + (5 * IN_MILLISECONDS));
	this->m_BallMoveTime.Reset();
	this->m_BallMove = false;
}
	
void CBattleSoccerMgr::SetState_Playing()
{
	this->m_State = BATTLE_SOCCER_STATE_PLAYING;
	this->m_Time.Start(sGameServer->GetBattleSoccerDuration());
	this->m_BallMoveTime.Reset();
	this->m_BallMove = false;

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = &this->m_Team[i];

		if ( !pTeam )
		{
			continue;
		}

		uint8 oposite = i == GUILD_WAR_TEAM_BLUE ? GUILD_WAR_TEAM_RED: GUILD_WAR_TEAM_BLUE;

		PARTY_LOOP(h)
		{
			Player* pPlayer = pTeam->m_Player[h].m_Player;

			if ( !pPlayer )
				continue;

			sGuildWarMgr->SendStart(pPlayer, this->m_Team[oposite].m_Guild->GetName(), GUILD_WAR_TYPE_SOCCER, i);
			sGuildWarMgr->SendScore(pPlayer, 0, 0);
			pPlayer->TeleportToLocation(WORLD_STADIUM, g_BattleSoccerStartZone[i].x1 + h, g_BattleSoccerStartZone[i].y1, -1, pPlayer->GetInstance());
		}

		pTeam->m_Guild->SendNoticeArgToMembers(NOTICE_GLOBAL, "Battle Soccer against %s started!!", this->m_Team[oposite].m_Guild->GetName());
	}
}
	
void CBattleSoccerMgr::SetState_Playend()
{
	this->m_State = BATTLE_SOCCER_STATE_PLAYEND;
	this->m_Time.Start(10 * IN_MILLISECONDS);
	this->m_BallMoveTime.Reset();
	this->m_BallMove = false;
}

void CBattleSoccerMgr::ProcState_None()
{

}
	
void CBattleSoccerMgr::ProcState_StandBy()
{
	if ( this->m_Time.Elapsed() )
	{
		this->SetState_None();
	}
}
	
void CBattleSoccerMgr::ProcState_Playing()
{
	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = &this->m_Team[i];

		if ( !pTeam )
		{
			continue;
		}

		uint8 oposite = i == GUILD_WAR_TEAM_BLUE ? GUILD_WAR_TEAM_RED: GUILD_WAR_TEAM_BLUE;

		PARTY_LOOP(h)
		{
			Player* pPlayer = pTeam->m_Player[h].m_Player;

			if ( !pPlayer )
				continue;

			if ( pPlayer->GetRegenStatus() != REGEN_NONE )
				continue;

			if ( !pPlayer->Object::IsPlaying() )
			{
				pTeam->m_Player[h].m_Player = nullptr;

				if ( pTeam->m_Player[h].m_Master )
				{
					this->Finish(BATTLE_SOCCER_END_REASON_LEFT, oposite, i, -1);
					return;
				}

				continue;
			}

			if ( pPlayer->GetWorldId() != WORLD_STADIUM || 
				((pPlayer->GetX() < g_BattleSoccerFullGround.x1 || pPlayer->GetX() > g_BattleSoccerFullGround.x2) ||
				 (pPlayer->GetY() < g_BattleSoccerFullGround.y1 || pPlayer->GetY() > g_BattleSoccerFullGround.y2)) )
			{
				sGuildWarMgr->SendEnd(pPlayer, pTeam->m_Guild->GetName(), 7);

				pTeam->m_Player[h].m_Player = nullptr;

				if ( pTeam->m_Player[h].m_Master )
				{
					this->Finish(BATTLE_SOCCER_END_REASON_LEFT, oposite, i, -1);
					return;
				}

				continue;
			}

                        if ( pTeam->m_PartyID != pPlayer->GetPartyID() )
			{
				if ( pTeam->m_Player[h].m_Master )
				{
					this->Finish(BATTLE_SOCCER_END_REASON_LEFT, oposite, i, -1);
					return;
				}
				else
				{
					pTeam->m_Player[h].m_Player = nullptr;

					sGuildWarMgr->SendEnd(pPlayer, pTeam->m_Guild->GetName(), 7);
					pPlayer->TeleportToGate(17);
				}

				continue;
			}
		}
	}

	bool send_score = false;

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = &this->m_Team[i];

		if ( !pTeam )
		{
			continue;
		}

		if ( pTeam->m_Score != pTeam->m_Guild->GetWarData()->GetScore() )
		{
			pTeam->m_Score = pTeam->m_Guild->GetWarData()->GetScore();
			send_score = true;
		}
	}

	if ( send_score )
	{
		this->SendScore();
	}

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = &this->m_Team[i];

		if ( !pTeam )
		{
			continue;
		}

		uint8 oposite = i == GUILD_WAR_TEAM_BLUE ? GUILD_WAR_TEAM_RED: GUILD_WAR_TEAM_BLUE;

		if ( pTeam->m_Score >= sGameServer->GetBattleSoccerMaxPoints() )
		{
			int32 add_score = 1;

			if ( this->m_Team[oposite].m_Score == 0 )
			{
				add_score = 3;
			}
			else if ( this->m_Team[oposite].m_Score <= 10 )
			{
				add_score = 2;
			}

			this->Finish(BATTLE_SOCCER_END_REASON_SCORE, i, oposite, add_score);
			return;
		}
	}

	if ( !this->m_BallMove && this->m_BallMoveTime.Elapsed(10 * IN_MILLISECONDS) )
	{
		this->m_BallMove = true;
	}
	
	if ( this->m_Time.Elapsed() )
	{
		this->Finish(BATTLE_SOCCER_END_REASON_TIME_OUT, GUILD_WAR_TEAM_MAX, GUILD_WAR_TEAM_MAX, -1);
	}
}
	
void CBattleSoccerMgr::ProcState_Playend()
{
	if ( this->m_Time.Elapsed() )
	{
		this->MovePlayersOut();
		this->SetState_None();
	}
}

void CBattleSoccerMgr::UpdateBall(Monster* Ball)
{
	if ( this->m_State != BATTLE_SOCCER_STATE_PLAYING )
		return;

	int16 x = Ball->GetX();
	int16 y = Ball->GetY();

	GUILD_WAR_TEAM_LOOP(i)
	{
		if ( (x >= g_BattleSoccerZones[i].x1 && x <= g_BattleSoccerZones[i].x2) &&
			 (y >= g_BattleSoccerZones[i].y1 && y <= g_BattleSoccerZones[i].y2) )
		{
			Ball->Kill();

			this->m_Team[i].m_Guild->GetWarData()->IncreaseScore(sGameServer->GetBattleSoccerGoalPoints());
		}
	}
}

bool CBattleSoccerMgr::IsInField(Player const* pPlayer) const
{
	if ( !pPlayer )
		return false;

	Guild const* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
		return false;

	if ( this->m_State != BATTLE_SOCCER_STATE_PLAYING &&
		 this->m_State != BATTLE_SOCCER_STATE_PLAYEND )
		return false;

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam const* pTeam = &this->m_Team[i];

		if ( pTeam->m_Guild != pGuild )
			continue;

		PARTY_LOOP(h)
		{
			if ( pTeam->m_Player[h].m_Player == pPlayer )
				return true;
		}
	}

	return false;
}

bool CBattleSoccerMgr::GetRespawnLocation(int16 & x, int16 & y, Player* pPlayer)
{
	if ( !this->IsInField(pPlayer) )
		return false;

	int16 px = g_BattleSoccerGround.x2 - g_BattleSoccerGround.x1;
	int16 py = g_BattleSoccerGround.y2 - g_BattleSoccerGround.y1;

	if ( px < 1 )
		px = 1;

	if ( py < 1 )
		py = 1;
		
	x = g_BattleSoccerGround.x1 + Random(px);
	y = g_BattleSoccerGround.y1 + Random(py);
	return true;
}

bool CBattleSoccerMgr::IsAttackAllowed(Player* pPlayer01, Player* pPlayer02)
{
	if ( !pPlayer01 || !pPlayer02 )
		return true;

	Guild* pGuild01 = pPlayer01->GuildGet();
	Guild* pGuild02 = pPlayer02->GuildGet();

	if ( !pGuild01 || !pGuild02 )
		return true;

	uint8 Team01 = GUILD_WAR_TEAM_MAX;
	uint8 Team02 = GUILD_WAR_TEAM_MAX;

	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam const* pTeam = &this->m_Team[i];

		if ( !pTeam )
		{
			continue;
		}

		if ( pTeam->m_Guild == pGuild01 )
		{
			PARTY_LOOP(h)
			{
				if ( pTeam->m_Player[h].m_Player == pPlayer01 )
				{
					Team01 = i;
				}
			}
		}
		if ( pTeam->m_Guild == pGuild02 )
		{
			PARTY_LOOP(h)
			{
				if ( pTeam->m_Player[h].m_Player == pPlayer02 )
				{
					Team02 = i;
				}
			}
		}
	}

	if ( Team01 == GUILD_WAR_TEAM_MAX && Team02 == GUILD_WAR_TEAM_MAX )
	{
		return true;
	}

	if ( Team01 == GUILD_WAR_TEAM_MAX && Team02 != GUILD_WAR_TEAM_MAX )
	{
		return false;
	}

	if ( Team01 != GUILD_WAR_TEAM_MAX && Team02 == GUILD_WAR_TEAM_MAX )
	{
		return false;
	}

	if ( Team01 == Team02 )
	{
		return false;
	}
	
	if ( this->m_State == BATTLE_SOCCER_STATE_PLAYING )
	{
		return true;
	}

	return false;
}

void CBattleSoccerMgr::MovePlayersOut()
{
	GUILD_WAR_TEAM_LOOP(i)
	{
		BattleSoccerTeam * pTeam = &this->m_Team[i];

		if ( !pTeam )
		{
			continue;
		}

		PARTY_LOOP(h)
		{
			Player* pPlayer = pTeam->m_Player[h].m_Player;

			if ( !pPlayer )
				continue;

			if ( !pPlayer->IsPlaying() || pPlayer->GetWorldId() != WORLD_STADIUM || pPlayer->IsServerQuit() )
				continue;

			pPlayer->TeleportToGate(17);
		}

		pTeam->Reset();
	}
}

void CBattleSoccerMgr::ProcessRequest(Player* pPlayer, const char * guild)
{
	if ( !Player::IsPlayerBasicState(pPlayer, false) )
	{
		return;
	}

	if ( !pPlayer->CanInteract() )
		return;

	if ( !sGameServer->IsBattleSoccerEnabled() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Battle soccer is disabled.");
		return;
	}

	Guild* pGuild = pPlayer->GuildGet();

	if ( !pGuild )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in guild to start a battle soccer.");
		return;
	}

	if ( pGuild->GetWarData()->GetState() != GUILD_WAR_STATE_NONE )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Your guild is currently declaring a war.");
		return;
	}

	if ( pGuild->GetMemberRanking(pPlayer) != GUILD_RANK_MASTER )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be guild master to start a battle soccer.");
		return;
	}

	Party* pParty01 = pPlayer->GetParty();

	if ( !pParty01 )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in party to start a battle soccer.");
		return;
	}

	if ( !this->PartySuccess(pParty01, pGuild) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in a full party.");
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");
		return;
	}
	
	Guild* pGuildTarget = sGuildMgr->GetGuild(guild);

	if ( !pGuildTarget )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Can't find guild %s.", guild);
		return;
	}

	if ( pGuild == pGuildTarget )
	{
		return;
	}

	if ( pGuildTarget->GetWarData()->GetState() != GUILD_WAR_STATE_NONE )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target guild is currently declaring a war.");
		return;
	}

	if ( pGuild->GetAlliance() && pGuild->GetAlliance() == pGuildTarget->GetAlliance() )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You can't declare war to a guild that belogs to the same alliance.");
		return;
	}

	Player* pTarget = sObjectMgr->FindPlayerByGUID(pGuildTarget->GetID());

	if ( !Player::IsPlayerBasicState(pTarget, false) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target guild master is not available.");
		return;
	}

	Party* pParty02 = pTarget->GetParty();

	if ( !pParty02 )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target need to be in party to start a battle soccer.");
		return;
	}

	if ( !this->PartySuccess(pParty02, pGuildTarget) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target need to be in a full party.");
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");
		return;
	}

	if ( this->m_State != BATTLE_SOCCER_STATE_NONE )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "Battle soccer field is currently in use.");
		return;
	}

	this->SetTeamData(pGuild, pGuildTarget);

	sGuildWarMgr->StartWarRequestResult(pPlayer, 1);

	pGuildTarget->GetWarData()->StartRequest(pGuild, GUILD_WAR_TYPE_SOCCER, sGameServer->GetGuildWarMaxWaitInterval());
	pGuild->GetWarData()->StartRequest(pGuildTarget, GUILD_WAR_TYPE_SOCCER, sGameServer->GetGuildWarMaxWaitInterval());

	pPlayer->GetInterfaceState()->Set(InterfaceData::GuildWarRequest, pTarget, 0, sGameServer->GetGuildWarMaxWaitInterval());
	pTarget->GetInterfaceState()->Set(InterfaceData::GuildWarRequest, pPlayer, 0, sGameServer->GetGuildWarMaxWaitInterval());

	sGuildWarMgr->StartWarRequestSend(pTarget, pGuild->GetName(), GUILD_WAR_TYPE_SOCCER);
}

bool CBattleSoccerMgr::PartySuccess(Party* pParty, Guild* pGuild)
{
	PARTY_LOOP(i)
	{
		if ( pParty->GetMember(i)->GetStatus() != PARTY_USER_FLAG_PLAYING )
		{
			return false;
		}

		Player* pMember = pParty->GetMember(i)->GetPlayer();

		if ( !Player::IsPlayerBasicState(pMember, false) )
		{
			return false;
		}

		if ( pMember->GetRegenStatus() != REGEN_NONE )
		{
			return false;
		}

		if ( pMember->GetWorldId() != WORLD_STADIUM )
		{
			return false;
		}

		if ( pMember->GetInterfaceState()->GetID() != InterfaceData::None )
		{
			return false;
		}

		if ( pMember->GuildGet() != pGuild )
		{
			return false;
		}
	}

	return true;
}

void CBattleSoccerMgr::ProcessTeams(Player* pPlayer01, Player* pPlayer02, Guild* pGuild01, Guild* pGuild02)
{
	if ( this->m_State != BATTLE_SOCCER_STATE_STANDBY )
		return;

	if ( this->m_Team[GUILD_WAR_TEAM_BLUE].m_Guild != pGuild01 || this->m_Team[GUILD_WAR_TEAM_RED].m_Guild != pGuild02 )
		return;

	Party* pParty01 = pPlayer01->GetParty();
	Party* pParty02 = pPlayer02->GetParty();

	if ( !pParty01 )
	{
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in party to start battlesoccer.");
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s is not in party.", pPlayer01->GetName());
		return;
	}

	if ( !pParty02 )
	{
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in party to start battlesoccer.");
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "%s is not in party.", pPlayer02->GetName());
		return;
	}

	if ( !this->PartySuccess(pParty01, pGuild01) )
	{
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target need to be in a full party.");
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");

		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in a full party.");
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");
		return;
	}

	if ( !this->PartySuccess(pParty02, pGuild02) )
	{
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "Target need to be in a full party.");
		pPlayer01->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");

		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be in a full party.");
		pPlayer02->SendNotice(CUSTOM_MESSAGE_ID_RED, "All party members must be in Arena and must belong to the same guild.");
		return;
	}

	this->m_Team[GUILD_WAR_TEAM_BLUE].m_PartyID = pParty01->GetID();
	this->m_Team[GUILD_WAR_TEAM_RED].m_PartyID = pParty02->GetID();

	PARTY_LOOP(i)
	{
		this->m_Team[GUILD_WAR_TEAM_BLUE].m_Player[i].Reset();
		this->m_Team[GUILD_WAR_TEAM_BLUE].m_Player[i].m_Player = pParty01->GetMember(i)->GetPlayer();

		if ( this->m_Team[GUILD_WAR_TEAM_BLUE].m_Player[i].m_Player )
		{
			this->m_Team[GUILD_WAR_TEAM_BLUE].m_Player[i].m_Master = this->m_Team[GUILD_WAR_TEAM_BLUE].m_Player[i].m_Player->GuildGetRanking() == GUILD_RANK_MASTER ? true: false;
		}
		
		this->m_Team[GUILD_WAR_TEAM_RED].m_Player[i].Reset();
		this->m_Team[GUILD_WAR_TEAM_RED].m_Player[i].m_Player = pParty02->GetMember(i)->GetPlayer();

		if ( this->m_Team[GUILD_WAR_TEAM_RED].m_Player[i].m_Player )
		{
			this->m_Team[GUILD_WAR_TEAM_RED].m_Player[i].m_Master = this->m_Team[GUILD_WAR_TEAM_RED].m_Player[i].m_Player->GuildGetRanking() == GUILD_RANK_MASTER ? true: false;
		}
	}

	pGuild01->GetWarData()->SetTeam(GUILD_WAR_TEAM_BLUE);
	pGuild02->GetWarData()->SetTeam(GUILD_WAR_TEAM_RED);

	pGuild01->GetWarData()->SetState(GUILD_WAR_STATE_ACTIVE);
	pGuild02->GetWarData()->SetState(GUILD_WAR_STATE_ACTIVE);

	this->SetState_Playing();
}

void CBattleSoccerMgr::SetTeamData(Guild* pGuild01, Guild* pGuild02)
{
        this->SetState_StandBy();

        this->m_Team[GUILD_WAR_TEAM_BLUE].Reset();
        this->m_Team[GUILD_WAR_TEAM_BLUE].m_Guild = pGuild01;

        this->m_Team[GUILD_WAR_TEAM_RED].Reset();
        this->m_Team[GUILD_WAR_TEAM_RED].m_Guild = pGuild02;
}

void CBattleSoccerMgr::Finish(uint8 reason, uint8 winner, uint8 looser, int32 score)
{
	if ( score == -1 )
	{
		if ( winner == GUILD_WAR_TEAM_MAX || looser == GUILD_WAR_TEAM_MAX )
		{
			if ( this->m_Team[GUILD_WAR_TEAM_BLUE].m_Score < this->m_Team[GUILD_WAR_TEAM_RED].m_Score )
			{
				winner = GUILD_WAR_TEAM_RED;
				looser = GUILD_WAR_TEAM_BLUE;
			}
			else if ( this->m_Team[GUILD_WAR_TEAM_BLUE].m_Score > this->m_Team[GUILD_WAR_TEAM_RED].m_Score )
			{
				winner = GUILD_WAR_TEAM_BLUE;
				looser = GUILD_WAR_TEAM_RED;
			}
		}

		if ( winner != GUILD_WAR_TEAM_MAX && looser != GUILD_WAR_TEAM_MAX )
		{
			if ( this->m_Team[winner].m_Score < this->m_Team[looser].m_Score )
			{
				score = 1;
			}
			else if ( this->m_Team[winner].m_Score > this->m_Team[looser].m_Score )
			{
				score = 3;
			}
			else
			{
				score = 2;
			}
		}
	}

	if ( score != -1 && winner != GUILD_WAR_TEAM_MAX )
		sDataServer->GuildScore(this->m_Team[winner].m_Guild->GetID(), score);

	if ( winner == GUILD_WAR_TEAM_MAX || looser == GUILD_WAR_TEAM_MAX ) // Empatados
	{
		PARTY_LOOP(h)
		{
			sGuildWarMgr->SendEnd(this->m_Team[GUILD_WAR_TEAM_BLUE].m_Player[h].m_Player, this->m_Team[GUILD_WAR_TEAM_RED].m_Guild->GetName(), 7);
			sGuildWarMgr->SendEnd(this->m_Team[GUILD_WAR_TEAM_RED].m_Player[h].m_Player, this->m_Team[GUILD_WAR_TEAM_BLUE].m_Guild->GetName(), 7);
		}
	}
	else
	{
		PARTY_LOOP(h)
		{
			sGuildWarMgr->SendEnd(this->m_Team[winner].m_Player[h].m_Player, this->m_Team[looser].m_Guild->GetName(), reason == BATTLE_SOCCER_END_REASON_LEFT ? 2: 1);
			sGuildWarMgr->SendEnd(this->m_Team[looser].m_Player[h].m_Player, this->m_Team[winner].m_Guild->GetName(), reason == BATTLE_SOCCER_END_REASON_LEFT ? 3: 0);
		}
	}
								
	this->m_Team[GUILD_WAR_TEAM_BLUE].m_Guild->GetWarData()->Reset();
	this->m_Team[GUILD_WAR_TEAM_RED].m_Guild->GetWarData()->Reset();

	this->SetState_Playend();
}

void CBattleSoccerMgr::SendScore()
{
	PARTY_LOOP(h)
	{
		sGuildWarMgr->SendScore(this->m_Team[GUILD_WAR_TEAM_BLUE].m_Player[h].m_Player, this->m_Team[GUILD_WAR_TEAM_BLUE].m_Score, this->m_Team[GUILD_WAR_TEAM_RED].m_Score);
		sGuildWarMgr->SendScore(this->m_Team[GUILD_WAR_TEAM_RED].m_Player[h].m_Player, this->m_Team[GUILD_WAR_TEAM_RED].m_Score, this->m_Team[GUILD_WAR_TEAM_BLUE].m_Score);
	}
}