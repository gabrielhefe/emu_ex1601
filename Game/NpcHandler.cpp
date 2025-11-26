/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "NpcHandler.cpp"
*
*/

static NpcTalkData npc_talk_data[] = 
{
	{	"npc_talk_warehouse",					&NpcHandler::Warehouse	},
	{	"npc_talk_guild",						&NpcHandler::Guild	},
	{	"npc_talk_quest_evolution",				&NpcHandler::QuestEvolutionNpc	},
	{	"npc_talk_blood_castle_entrance",		&NpcHandler::BloodCastleEntrance	},
	{	"npc_talk_blood_castle_angel",			&NpcHandler::BloodCastleAngel	},
	{	"npc_talk_devil_square_entrance",		&NpcHandler::DevilSquareEntrance	},
	{	"npc_talk_double_goer_entrance",		&NpcHandler::DoubleGoerEntrance	},
	{	"npc_talk_kanturu_gateway",				&NpcHandler::KanturuGateway	},
	{	"npc_talk_lord_mix",					&NpcHandler::LordMix	},
	{	"npc_talk_snowman",						&NpcHandler::Snowman	},
	{	"npc_talk_santa_claus",					&NpcHandler::SantaClaus	},
	{	"npc_talk_land_of_trials_guard",		&NpcHandler::LandOfTrialsGuard	},
	{	"npc_talk_illusion_temple_entrance",	&NpcHandler::IllusionTempleEntrance	},

	{	"npc_talk_tormented_square",			&NpcHandler::NpcTalkTormentedSquare	},
	
	{	"npc_talk_imperial_fortress_entrance",	&NpcHandler::NpcTalkImperialFortress },
	{	"npc_talk_castle_guard",				&NpcHandler::NpcTalkCastleSiegeGuard },
	{	"npc_talk_lesnar",						&NpcHandler::NpcTalkArcaBattleLesnar },
	{	"npc_talk_slove",						&NpcHandler::NpcTalkSloveCaptain },
	{	"npc_talk_personal_shop_board",			&NpcHandler::NpcTalkPersonalShopBoard },
	{	"npc_talk_adniel",						&NpcHandler::NpcTalkAdniel },
	{	"npc_talk_muun_exchange",				&NpcHandler::NpcTalkMuunExchange },
	{	"npc_talk_quest_support",				&NpcHandler::NpcTalkQuestSupport },
	{	"npc_talk_jewel_mix",					&NpcHandler::NpcTalkJewelMix },
	{	"npc_talk_duel",						&NpcHandler::NpcTalkDuel },
	{	"npc_talk_loren_market_traveler",		&NpcHandler::NpcTalkLorenMarketTraveler },
	{	"npc_talk_pet_trainer",					&NpcHandler::NpcTalkPetTrainer },
	{	"npc_talk_chaos_card",					&NpcHandler::NpcTalkChaosCard },
	{	"npc_talk_cherry_blossom",				&NpcHandler::NpcTalkCherryBlossom },
	{	"npc_talk_seed_master",					&NpcHandler::NpcTalkSeedMaster },
	{	"npc_talk_seed_research",				&NpcHandler::NpcTalkSeedResearch },
	{	"npc_talk_chaos_machine",				&NpcHandler::NpcTalkChaosMachine },
	{	"npc_talk_lucky",						&NpcHandler::NpcTalkLucky },
	{	"npc_talk_harmony_gemstone",			&NpcHandler::NpcTalkHarmonyGemstone },
	{	"npc_talk_harmony_smelt_item",			&NpcHandler::NpcTalkHarmonySmeltItem },
	{	"npc_talk_harmony_restore_item",		&NpcHandler::NpcTalkHarmonyRestoreItem },
	{	"npc_talk_buff_santa_healing",			&NpcHandler::NpcTalkBuffSantaHealing },
	{	"npc_talk_buff_santa_protection",		&NpcHandler::NpcTalkBuffSantaProtection },
	{	"npc_talk_buff_santa_strengthener",		&NpcHandler::NpcTalkBuffSantaStrengthener },
	{	"npc_talk_buff_santa_defense",			&NpcHandler::NpcTalkBuffSantaDefense },
	{	"npc_talk_buff_santa_quickness",		&NpcHandler::NpcTalkBuffSantaQuickness },
	{	"npc_talk_buff_santa_fortune",			&NpcHandler::NpcTalkBuffSantaFortune },
	{	"npc_talk_restore_life",				&NpcHandler::NpcTalkSantaRestoreLife },
	{	"npc_talk_restore_mana",				&NpcHandler::NpcTalkSantaRestoreMana },

	{	"npc_talk_last_man_standing",			&NpcHandler::NpcTalkLastManStanding	},

	{	"npc_talk_cent_town",					&NpcHandler::NpcTalkCentTown	},
	{	"npc_talk_cent_entrance",				&NpcHandler::NpcTalkCentEntrance	},

	{	"npc_talk_moss_merchant",				&NpcHandler::NpcTalkMossMerchant	},

	{ "npc_talk_item_recovery", &NpcHandler::NpcTalkItemRecovery },

	{	nullptr,			nullptr}
};

bool NpcHandler::ExecuteCommandInTable(NpcTalkData * table, const char * text)
{
	for ( size_t i = 0; table[i].name != nullptr; ++i )
    {
		if ( strlen(table[i].name) != strlen(text) )
			continue;

		if ( memcmp(table[i].name, text, strlen(table[i].name)) )
			continue;

		if ( !table[i].function )
			continue;

		(this->*table[i].function)();
        return true;
	}

    return false;
}

void NpcHandler::OnTalk()
{
	if (!this->m_Player->IsAuthorizationEnabled()) {
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	this->m_Player->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_FIND_NPC, this->m_Npc);

	if ( this->m_Npc->GetAI() && this->m_Npc->GetAI()->OnTalk(this->m_Player) )
	{
		return;
	}

	if ( this->m_Npc->GetNpcFunction().empty() )
	{
		return;
	}

	if ( this->ExecuteCommandInTable(npc_talk_data, this->m_Npc->GetNpcFunction().c_str()) )
	{
		this->m_Npc->AddTalkReference(this->m_Player);
		return;
	}

	if ( Shop const* shop = sShopMgr->GetShop(this->m_Npc->GetNpcFunction()) )
	{
		if ( shop->IsFlag(SHOP_FLAG_ADMINISTRATOR) && !this->m_Player->IsAdministrator() )
		{
			return;
		}

		if ( this->m_Player->GetPKLevel() > shop->GetMaxPKLevel() )
		{
			if ( shop->IsFlag(SHOP_FLAG_PK_SCORE) )
			{
				this->m_Npc->SayTo(this->m_Player, "You have to earn %d points to get out of Outlaw status..", this->m_Player->GetPKPoints());
			}
			else
			{
				this->m_Npc->SayTo(this->m_Player, shop->GetPKText().c_str());
			}

			return;
		}

		this->SendShopList(shop);
	}
}

uint8 buffer[8192];
uint8 buffer_misc[8192];

void NpcHandler::SendShopList(Shop const* shop)
{
	POINTER_PCT(SHOP_ITEM_LIST_HEAD, head, buffer, 0);
	POINTER_PCT(SHOP_ITEM_LIST_BODY, body, buffer, sizeof(SHOP_ITEM_LIST_HEAD));
	head->count = 0;
	
	POINTER_PCT(SHOP_MISC_HEAD, head_misc, buffer_misc, 0);
	POINTER_PCT(SHOP_MISC_BODY, body_misc, buffer_misc, sizeof(SHOP_MISC_HEAD));
	head_misc->count = 0;
	head_misc->type = shop->GetType();

	for ( uint8 i = 0; i < max_shop_item; ++i )
	{
		if ( shop->GetItem(i)->position == uint8(-1) )
		{
			continue;
		}

		if ( shop->GetType() == SHOP_TYPE_RUUD )
		{
			bool required_class = false;

			for ( int32 n = 0; n < Character::MAX_CLASS; ++n )
			{
				if ( shop->GetItem(i)->GetRequiredClass(n) != 0 )
				{
					required_class = true;
					break;
				}
			}

			if ( required_class )
			{
				if ( !shop->GetItem(i)->IsRequiredClass(this->m_Player->GetClass(), 1, 1, 1) )
				{
					continue;
				}
			}
		}

		body[head->count].slot = shop->GetItem(i)->position.get();
		shop->GetItem(i)->ConvertToBuffer(body[head->count].item_info);
		++head->count;

		if ( shop->GetItem(i)->GetPrice() > 0 )
		{
			body_misc[head_misc->count].slot = shop->GetItem(i)->position.get();
			body_misc[head_misc->count].price = shop->GetItem(i)->GetPrice();
			++head_misc->count;
		}
	}

	if ( head->count == 0 )
	{
		return;
	}

	this->m_Player->GetInterfaceState()->Set(this->m_Npc->GetNpcFunction(), this->m_Npc);
	this->m_Player->SetShopTime(0);
	this->m_Npc->AddTalkReference(this->m_Player);

	TALK_TO_NPC_RESULT pMsg(shop->GetType() == SHOP_TYPE_RUUD ? 0x35 : 0);
	pMsg.data[0] = shop->IsFlag(SHOP_FLAG_REPAIR);
	this->m_Player->sendPacket(MAKE_PCT(pMsg));

	head_misc->h.set(HEADCODE_MISC_CHARACTER_DATA, SUBCODE_MISC_CHARACTER_DATA_MISC_SHOP, sizeof(SHOP_MISC_HEAD) + (head_misc->count * sizeof(SHOP_MISC_BODY)));
	this->m_Player->sendPacket(buffer_misc, head_misc->h.get_size());

	head->type = shop->GetType() == SHOP_TYPE_RUUD ? 0x17 : 0x00;
	head->h.set(HEADCODE_CLOSE_INTERFACE, sizeof(SHOP_ITEM_LIST_HEAD) + (head->count * sizeof(SHOP_ITEM_LIST_BODY)));
	this->m_Player->sendPacket(buffer, head->h.get_size());
	
	if ( shop->IsFlag(SHOP_FLAG_SIEGE_TAX) )
	{
		sCastleSiege->SendTaxRate(this->m_Player, 2);
	}

	if (shop->GetType() == SHOP_TYPE_RUUD)
	{
		sMonsterSoul->SendPurchaseAvailable(this->m_Player);
		sMonsterSoul->SendPurchaseList(this->m_Player);
	}
}

void NpcHandler::Warehouse()
{
	this->m_Player->GetInterfaceState()->Set(InterfaceData::Warehouse, this->m_Npc);

	this->m_Player->GetWarehouse()->Open();
}

void NpcHandler::Guild()
{
	if ( !sGameServer->guild_enabled )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Guild Creation is closed");
		return;
	}

	if ( this->m_Player->GuildGet() != nullptr )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are already in a guild");
		return;
	}

	if ( sGameServer->guild_create_min_level > this->m_Player->GetLevel() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to be level %d to create a guild", sGameServer->guild_create_min_level.get());
		return;
	}

	if ( sGameServer->guild_create_gens.get() && !this->m_Player->GetGen()->IsFamily() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You need to join a gens before creating a guild");
		return;
	}

	GUILD_CREATE_MENU pMsg;
	this->m_Player->sendPacket(MAKE_PCT(pMsg));

	this->m_Player->GetInterfaceState()->Set(InterfaceData::Guild, this->m_Npc);
}

void NpcHandler::QuestEvolutionNpc()
{
	this->m_Player->QuestEvolutionNpcTalk(this->m_Npc);
}

void NpcHandler::BloodCastleEntrance()
{
	if ( !sBloodCastleMgr->HaveTicket(this->m_Player, -1) )
	{
		this->m_Player->CommandSend(1,0x15,0);
		return;
	}

	if ( sBloodCastleMgr->GetState() == EVENT_MGR_STATE_OPEN )
	{
		TALK_TO_NPC_RESULT pMsg(0x06);
		this->m_Player->sendPacket(MAKE_PCT(pMsg));

		this->m_Player->GetInterfaceState()->Set(InterfaceData::BloodCastleEntrance, this->m_Npc);
	}
	else
	{
		this->m_Player->CommandSend(1,0x14,0);
	}
}

void NpcHandler::BloodCastleAngel()
{
	if ( !BC_MAP_RANGE(this->m_Player->GetWorldId()) || !BC_MAP_RANGE(this->m_Npc->GetWorldId()) )
	{
		this->m_Player->CommandSend(1, 0x18, 0);
		return;
	}

	if ( this->m_Player->GetEventId() != EVENT_BLOOD_CASTLE || this->m_Npc->GetEventId() != EVENT_BLOOD_CASTLE )
	{
		this->m_Player->CommandSend(1, 0x18, 0);
		return;
	}

	if ( this->m_Player->GetEventGround() != this->m_Npc->GetEventGround() || this->m_Player->GetEventGround() == -1 )
	{
		this->m_Player->CommandSend(1, 0x18, 0);
		return;
	}

	BloodCastle * Ground = sBloodCastleMgr->GetGround(this->m_Player->GetEventGround());

	if ( !Ground )
	{
		this->m_Player->CommandSend(1, 0x18, 0);
		return;
	}

	if ( Ground->GetState() != BLOOD_CASTLE_STATE_PLAYING )
	{
		this->m_Player->CommandSend(1, 0x18, 0);
		return;
	}

	if ( Ground->GetSubState() == BLOOD_CASTLE_SUBSTATE_COMPLETE )
	{
		this->m_Player->CommandSend(1, 0x2E, 0);
		return;
	}

	if ( Ground->GetSubState() != BLOOD_CASTLE_SUBSTATE_DELIVER_WEAPON )
	{
		this->m_Player->CommandSend(1, 0x18, 0);
		return;
	}

	if ( Ground->GetRemain() > sGameServer->GetBloodCastleDeliverWeaponTime() )
	{
		this->m_Player->CommandSend(1, 0x18, 0);
		return;
	}

	if ( Ground->CheckPlayerComplete(this->m_Player) )
	{
		this->m_Player->CommandSend(1, 0x2E, 0);
		return;
	}

	if ( !Ground->CanGetReward(this->m_Player) )
	{
		this->m_Player->CommandSend(1, 0x18, 0);
		return;
	}

	if ( this->m_Player == Ground->GetWeaponOwner() )
	{
		this->m_Player->QuestMUObjectiveUpdate(QUEST_MU_OBJECTIVE_BLOOD_CASTLE_CLEAR);

		Ground->SetPlayerComplete(this->m_Player);
		Ground->SetSubState(BLOOD_CASTLE_SUBSTATE_WEAPON_DELIVERED);

		this->m_Player->CommandSend(1, 0x17, 0);
		//sItemMgr->ItemSerialCreateGremoryCase(this->m_Player, Item(JEWEL::CHAOS, 0, 1, 0, 0, 0, 0, 0, nullptr, 0xFF, 604800), GREMORY_CASE_TYPE_CHARACTER, GremoryCaseReward::BLOOD_CASTLE);

		//sEventMgr->GiveContributionReward(this->m_Player, EVENT_BLOOD_CASTLE, Ground->GetGround() + 1, 1, GremoryCaseReward::BLOOD_CASTLE);
	}
}

void NpcHandler::DevilSquareEntrance()
{
	if ( !sDevilSquareMgr->EventCommonMgr::HaveTicket(this->m_Player, ITEMGET(14, 19), ITEMGET(13, 46)) )
	{
		this->m_Player->CommandSend(1,2,0);
		return;
	}

	if ( sDevilSquareMgr->GetState() == EVENT_MGR_STATE_OPEN )
	{
		TALK_TO_NPC_RESULT pMsg(0x04);
		this->m_Player->sendPacket(MAKE_PCT(pMsg));

		this->m_Player->GetInterfaceState()->Set(InterfaceData::DevilSquareEntrance, this->m_Npc);
	}
	else
	{
		this->m_Player->CommandSend(1,3,0);
	}
}
	
void NpcHandler::DoubleGoerEntrance()
{
	TALK_TO_NPC_RESULT pMsg(0x23);

	if ( sDoppelganger->GetState() != DOPPELGANGER_STATE_OPEN )
	{
		int32 remain_time = sEventMgr->GetNextRemainTime(EVENT_DOPPELGANGER);

		remain_time /= MINUTE;

		pMsg.data[0] = HIBYTE(remain_time);
		pMsg.data[1] = LOBYTE(remain_time);
	}
	else
	{
		pMsg.data[0] = 0;
		pMsg.data[1] = 0;
	}

	this->m_Player->sendPacket(MAKE_PCT(pMsg));
}

void NpcHandler::KanturuGateway()
{
	sKanturuMgr->EntranceInfoNotify(this->m_Player);
}

void NpcHandler::LordMix()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
	{
		this->m_Npc->SayTo(this->m_Player, "Can't use right now!");
		return;
	}

	if ( !sCastleSiege->CastleOwnerMember(this->m_Player) )
	{
		this->m_Npc->SayTo(this->m_Player, "Only the castle Lord can use!");
		return;
	}

	class Guild* pGuild = this->m_Player->GuildGet();

	if ( !pGuild || (pGuild->GetMemberRanking(this->m_Player) != GUILD_RANK_MASTER && pGuild->GetMemberRanking(this->m_Player) != GUILD_RANK_ASISTANT) )
	{
		this->m_Npc->SayTo(this->m_Player, "Only the castle Lord can use!");
		return;
	}

	this->SendTalkToNpc(0x0C, InterfaceData::LordMix, true, 1);
}

void NpcHandler::Snowman()
{
	this->m_Player->CommandSend(17, 1, 0);
}
	
void NpcHandler::SantaClaus()
{
	if ( !sGameServer->IsSantaVillageSantaClausEnabled() )
		return;

	if ( !sEventMgr->IsSeasonEventOn(sGameServer->GetSantaVillageSeason()) )
		return;

	this->m_Player->SetSantaClausGiftRequest(0);

	if ( this->m_Player->GetSantaClausGiftDate() == 0 )
	{
		this->m_Player->SetSantaClausGiftRequest(1);
		this->m_Player->CommandSend(16, 0, 0);
	}
	else
	{
		time_t diff = (this->m_Player->GetSantaClausGiftDate() + sGameServer->GetSantaVillageSantaClausGiftTime()) - time(nullptr);

		if ( diff > 0 && !this->m_Player->IsAdministrator() )
		{
			this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "Need to wait %s until you can get new gift", 
				secsToTimeString(diff).c_str());

			this->m_Player->CommandSend(16, 2, 0);
		}
		else
		{
			this->m_Player->SetSantaClausGiftRequest(1);
			this->m_Player->CommandSend(16, 1, 0);
		}
	}
}

void NpcHandler::LandOfTrialsGuard()
{
	CASTLE_SIEGE_LAND_OF_TRIALS_GUARD pMsg;
	pMsg.max_price = CASTLE_SIEGE_TAX_RATE_HUNT_MAX;
	pMsg.unit_price = CASTLE_SIEGE_TAX_RATE_HUNT_UNIT;
	pMsg.current_price = sCastleSiege->GetTaxRateHunt(nullptr);
	pMsg.allowed = sCastleSiege->IsHuntEnabled();

	if ( sCastleSiege->CastleOwnerUnionMember(this->m_Player) )
	{
		pMsg.result = 0x02;
	}
	
	if ( sCastleSiege->CastleOwnerMember(this->m_Player) )
	{
		if ( this->m_Player->GuildGetRanking() == GUILD_RANK_MASTER )
		{
			pMsg.result = 0x03;
		}
		else
		{
			pMsg.result = 0x02;
		}
	}

	this->m_Player->sendPacket(MAKE_PCT(pMsg));
}

void NpcHandler::IllusionTempleEntrance()
{
	uint8 ground = sEventMgr->GetEventLevel(EVENT_ILLUSION_TEMPLE, this->m_Player);

	if ( ground >= MAX_ILLUSION_TEMPLE_GROUND )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "[Illusion Temple] is closed. Try again next time.");
		return;
	}

	if ( (this->m_Player->GetPKLevel() >= sIllusionTempleMgr->GetGround(ground)->GetSettings()->GetPKLevelMax()) && sIllusionTempleMgr->GetGround(ground)->GetSettings()->IsPKCheck() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "[Illusion Temple] Your PK level is not allowed to enter.");
		return;
	}

	if ( !sIllusionTempleMgr->HaveTicket(this->m_Player, ground) )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "[Illusion Temple] Ticket is missing.");
		return;
	}

	if ( sIllusionTempleMgr->GetState() == EVENT_MGR_STATE_OPEN )
	{
		TALK_TO_NPC_RESULT pMsg(0x14);
		pMsg.data[0] = sEventMgr->GetNextRemainTime(EVENT_ILLUSION_TEMPLE) / MINUTE;
		pMsg.data[1] = sIllusionTempleMgr->GetGround(ground)->GetPlayersCount(true);
		this->m_Player->sendPacket(MAKE_PCT(pMsg));

		this->m_Player->GetInterfaceState()->Set(InterfaceData::IllusionTempleEntrance, this->m_Npc);
	}
	else
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "[Illusion Temple] is closed. Try again next time.");
	}
}

void NpcHandler::NpcTalkTormentedSquare()
{
	TALK_TO_NPC_RESULT pMsg(0x34);
	this->m_Player->sendPacket(MAKE_PCT(pMsg));
}

void NpcHandler::NpcTalkQuestSupport()
{
	TALK_TO_NPC_QUEST_SUPPORT pMsg(1, this->m_Npc->GetClass(), this->m_Player->GetGen()->GetFamily(), this->m_Player->GetGen()->GetLevel(), this->m_Player->GetGen()->GetContribution());
	this->m_Player->SEND_PCT(pMsg);

	this->m_Player->GetInterfaceState()->Set("interface_quest_support_giver", this->m_Npc);
}

void NpcHandler::NpcTalkLorenMarketTraveler()
{
	TALK_TO_NPC_RESULT pMsg(0x25);
	
	if ( this->m_Player->GetWorldId() == WORLD_LOREN_MARKET )
	{
		pMsg.data[0] = 0x01;
	}

	this->m_Player->SEND_PCT(pMsg);
}

void NpcHandler::NpcTalkArcaBattleLesnar()
{
	SendTalkToNpc(0x28, "interface_arca_battle_lesnar");
}

void NpcHandler::NpcTalkSloveCaptain()
{
	SendTalkToNpc(0x27, "interface_acheron_entrance");
}

void NpcHandler::NpcTalkPersonalShopBoard()
{
	SendTalkToNpc(0x30, "interface_none");
}

void NpcHandler::NpcTalkAdniel()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x29, "interface_adniel");

	/*if (sGameServer->IsMixRecoveryWarning() && this->m_Player->IsRecoveryMixFull())
	{
		this->m_Player->SendMessageBox(0, "WARNING!", "Mix Recovery NPC is full. \n If you fail an upgrade mix without using TCA, you won't be able to recover the item.");
	}*/
}

void NpcHandler::NpcTalkMuunExchange()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x33, "interface_none");
}

void NpcHandler::NpcTalkDuel()
{
	sDuelMgr->SendDuelRoomStatus(this->m_Player);

	SendTalkToNpc(0x21, "interface_duel_gate");
}

void NpcHandler::NpcTalkJewelMix()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x08, "interface_mix_jewel");
}

void NpcHandler::NpcTalkLucky()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x26, "interface_lucky", 1);
}

void NpcHandler::NpcTalkPetTrainer()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x07, "interface_pet_trainer", 1);
}

void NpcHandler::NpcTalkChaosCard()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x15, "interface_chaos_card", 1);
}

void NpcHandler::NpcTalkCherryBlossom()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x16, "interface_chaos_machine", 1);
}

void NpcHandler::NpcTalkSeedMaster()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x17, "interface_seed_creation", 1);
}

void NpcHandler::NpcTalkSeedResearch()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x18, "interface_seed_research", 1);
}

void NpcHandler::NpcTalkChaosMachine()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x03, "interface_chaos_machine", 1);

	/*if (sGameServer->IsMixRecoveryWarning() && this->m_Player->IsRecoveryMixFull())
	{
		this->m_Player->SendMessageBox(0, "WARNING!", "Mix Recovery NPC is full. \n If you fail an upgrade mix without using TCA, you won't be able to recover the item.");
	}*/
}

void NpcHandler::NpcTalkHarmonyGemstone()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	this->SendTalkToNpc(0x11, "interface_harmony_refine", true, sGameServer->harmony_refine_rate.get());
}

void NpcHandler::NpcTalkHarmonySmeltItem()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	this->SendTalkToNpc(0x12, "interface_harmony_smelt", true, sGameServer->harmony_smelt_normal_rate.get(), sGameServer->harmony_smelt_exe_rate.get());
}

void NpcHandler::NpcTalkHarmonyRestoreItem()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	SendTalkToNpc(0x13, "interface_harmony_restore", 1);
}

void NpcHandler::NpcTalkSantaRestoreLife()
{
	this->m_Player->PowerSet(POWER_LIFE, this->m_Player->PowerGetTotal(POWER_LIFE) , true);
}

void NpcHandler::NpcTalkSantaRestoreMana()
{
	this->m_Player->PowerSet(POWER_MANA, this->m_Player->PowerGetTotal(POWER_MANA) , true);
}

void NpcHandler::NpcTalkBuffSantaHealing()
{
	this->m_Player->AddBuff(92, BuffEffect(4, 500), 1800, 12, this->m_Player, true);
}

void NpcHandler::NpcTalkBuffSantaProtection()
{
	this->m_Player->AddBuff(93, BuffEffect(5, 500), 1800, 12, this->m_Player, true);
}

void NpcHandler::NpcTalkBuffSantaStrengthener()
{
	this->m_Player->AddBuff(94, BuffEffect(2, 30), 1800, 12, this->m_Player, true);
}

void NpcHandler::NpcTalkBuffSantaDefense()
{
	this->m_Player->AddBuff(95, BuffEffect(3, 100), 1800, 12, this->m_Player, true);
}

void NpcHandler::NpcTalkBuffSantaQuickness()
{
	this->m_Player->AddBuff(96, BuffEffect(1, 15), 1800, 12, this->m_Player, true);
}

void NpcHandler::NpcTalkBuffSantaFortune()
{
	this->m_Player->AddBuff(97, BuffEffect(31, 10), 1800, 12, this->m_Player, true);
}

void NpcHandler::NpcTalkImperialFortress()
{
	SendTalkToNpc(0x24);
}

void NpcHandler::NpcTalkCastleSiegeGuard()
{
	if ( sCastleSiege->GetState() == CASTLE_SIEGE_STATE_START )
	{
		this->m_Npc->SayTo(this->m_Player, "Information is unavailable!");
		return;
	}

	SendTalkToNpc(0x0D, "interface_castle_guard");
}

void NpcHandler::NpcTalkLastManStanding()
{
	if ( sLastManStanding->GetState() != LAST_MAN_STANDING_STATE_OPEN )
	{
		this->m_Player->SendMessageBox(0, "Last Man Standing", "Event is not open.");
		return;
	}

	sLastManStanding->EnterRequest(this->m_Player);
}

void NpcHandler::SendTalkToNpc(uint8 result, std::string const& interface_id, bool tax, uint8 data1, uint8 data2)
{
	TALK_TO_NPC_RESULT pMsg(result);
	pMsg.data[0] = data1;
	pMsg.data[1] = data2;
	this->m_Player->SEND_PCT(pMsg);

	if ( interface_id != "" )
	{
		this->m_Player->GetInterfaceState()->Set(interface_id, this->m_Npc);
	}

	if ( tax )
	{
		sCastleSiege->SendTaxRate(this->m_Player, 1);
	}

	this->m_Player->GetMixInventory()->Clear();
	this->m_Player->SetMixCompleted(false);
}

void NpcHandler::NpcTalkCentTown()
{
	this->SendTalkToNpc(0x36, InterfaceData::CentTown, false);
}

void NpcHandler::NpcTalkCentEntrance()
{
	this->SendTalkToNpc(0x37, InterfaceData::CentEntrance, false);
}

void NpcHandler::NpcTalkMossMerchant()
{
	if ( !this->m_Player->IsAuthorizationEnabled() )
	{
		this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not authorized for this action.");
		return;
	}

	this->SendTalkToNpc(0x38, InterfaceData::MossMerchant, false);

	sMossMerchant->Open(this->m_Player);
}

void NpcHandler::NpcTalkItemRecovery()
{
	this->m_Player->SendRecoveryMixItemList(this->m_Npc);
}
