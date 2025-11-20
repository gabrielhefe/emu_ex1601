#define TALKING_TEST_1	"Ta da duoc giai thoat tu do"
#define TALKING_TEST_2	"Mau, han thu, noi dau"
#define TALKING_TEST_3	"The gioi nay se biet den noi dau"

#define TALKING_TEST_4	"Vu khi cua ta se thanh trung tat ca"
#define TALKING_TEST_5	"Noi dau! Noi dau!"

enum ChampionPhase
{
	WORLD_BOSS_PHASE_INACTIVE,
	WORLD_BOSS_PHASE_WAIT,
	WORLD_BOSS_PHASE_TALK_1,
	WORLD_BOSS_PHASE_TALK_2,
	WORLD_BOSS_PHASE_TALK_3,
	WORLD_BOSS_PHASE_SIZE,
	WORLD_BOSS_PHASE_FIGHT,
};

enum
{
	WORLD_BOSS_TIME_AOE_DAMAGE,

	WORLD_BOSS_TIME_MAX
};

class WorldBossScriptNewDW : public MonsterScriptAI
{
public:
	explicit WorldBossScriptNewDW() : ScriptAI("world_boss_ai_dw") { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		DECLARE_ARRAY_STRUCT(TickTimer, Time, WORLD_BOSS_TIME_MAX);

		explicit AI(Monster* monster) : MonsterAI(monster)
		{
			for (int32 i = 0; i < WORLD_BOSS_TIME_MAX; ++i)
			{
				this->GetTime(i)->Start();
			}

			this->GetTime(WORLD_BOSS_TIME_AOE_DAMAGE)->Start(2000);
		}

		virtual ~AI() {}

		bool Update()
		{
			me()->SetFloatData(UNIT_FLOAT_RESIST_STUN, 100);

			sWorldMgr->SetPKBoss(me()->GetWorldId(), true);

			if (this->phaseGet() == WORLD_BOSS_PHASE_INACTIVE)
			{
				if (me()->GetPlayerViewportCount() >= sGameServer->GetWorldBossRequiredPlayer())
				{
					this->phaseSetNext();
				}

				this->PushBackPlayers(false);
				return true;
			}

			if (this->phaseGet() != WORLD_BOSS_PHASE_FIGHT)
			{
				if (this->phaseTimeElapsed())
					this->phaseSetNext();

				this->PushBackPlayers(false);
				return true;
			}
			else
			{
				if (me()->GetPlayerViewportCount() <= 0)
				{
					this->phaseSet(WORLD_BOSS_PHASE_INACTIVE);
					return true;
				}

				this->PushBackPlayers(true);
			}

			return false;
		}

		void OnRespawn()
		{
			this->phaseSet(WORLD_BOSS_PHASE_INACTIVE);
		}

		bool EnableAttack(Unit* /*pAttacker*/, Skill*, int32 count)
		{
			return this->phaseGet() == WORLD_BOSS_PHASE_FIGHT;
		}

		bool UpdateMovement()
		{
			return this->phaseGet() == WORLD_BOSS_PHASE_FIGHT;
		}

		bool InmuneToRadiance() const
		{
			return true;
		}

		bool InmuneToDebuffBlow(bool blow) const
		{
			if (blow)
			{
				if (sGameServer->IsBossDebuffBlow(2))
				{
					return true;
				}
			}
			else
			{
				if (sGameServer->IsBossDebuffBlow(1))
				{
					return true;
				}
			}

			return false;
		}

		void OnDie()
		{
			sWorldBoss->SetState_End();

			Player* pPlayerKill = me()->GetLastAttacker()->ToPlayer();

			Party* party = pPlayerKill->GetParty();

			if (party) {
				for (uint8 i = 0; i < party->GetMemberCount(); i++) {
					Player* pPlayerMember = party->GetMember(i)->GetPlayer();

					if (!pPlayerMember)
						continue;

					if (pPlayerMember == pPlayerKill)
						continue;

					if (!IN_RANGE(pPlayerKill, pPlayerMember, MAX_PARTY_MONSTER_KILL_DISTANCE))
						continue;

					pPlayerMember->SetRuudMoney(pPlayerMember->GetRuudMoney() + sGameServer->GetWorldBossRuudReward());
					pPlayerMember->SendRuudMoney();
				}
			}

			pPlayerKill->SetRuudMoney(pPlayerKill->GetRuudMoney() + sGameServer->GetWorldBossRuudReward());
			pPlayerKill->SendRuudMoney();

			me()->LogKillData(true, false);
		}

		void phaseSet(uint32 phase)
		{
			switch (phase)
			{
			case WORLD_BOSS_PHASE_INACTIVE:
			{
				me()->AddBuff(BUFF_STUN, 0, BUFF_FLAG_CONSTANT, me());
				me()->AddBuff(BUFF_ICE_ARROW, 0, BUFF_FLAG_CONSTANT, me());

				me()->SetSendType(MAIN_OBJECT_FURNITURE);
				me()->SendCustomObjectData();
			} break;

			case WORLD_BOSS_PHASE_WAIT:
			{
				this->phaseTimeSet(3000);

				me()->RemoveBuff(BUFF_STUN);
				me()->RemoveBuff(BUFF_ICE_ARROW);
			} break;

			case WORLD_BOSS_PHASE_TALK_1:
			{
				this->phaseTimeSet(5000);
				me()->Say(TALKING_TEST_1);
			} break;

			case WORLD_BOSS_PHASE_TALK_2:
			{
				this->phaseTimeSet(3000);
				me()->Say(TALKING_TEST_2);
			} break;

			case WORLD_BOSS_PHASE_TALK_3:
			{
				this->phaseTimeSet(3000);
				me()->Say(TALKING_TEST_3);
			} break;

			case WORLD_BOSS_PHASE_SIZE:
			{
				this->phaseTimeSet(2000);
			} break;

			case WORLD_BOSS_PHASE_FIGHT:
			{
				me()->SetSendType(MAIN_OBJECT_MONSTER);
				me()->SendCustomObjectData();
			} break;
			}

			this->MonsterAI::phaseSet(phase);
		}

		void AOEAll()
		{
			Object* pObject = nullptr;

			VIEWPORT_LOOP_OBJECT(me(), pObject)

				if (!pObject->IsUnit())
					continue;

			if (pObject->IsInSafeZone())
				continue;

			if (pObject->IsPlayer() && pObject->ToPlayer()->IsAdministrator())
				continue;

			if (pObject->IsCreature() && (!pObject->ToCreature()->GetSummoner() || !pObject->ToCreature()->GetSummoner()->IsPlayer()))
				continue;

			if (!IN_RANGE(me(), pObject, sGameServer->GetWorldBossAOERange()))
				continue;

			me()->AttackProc(pObject->ToUnit(), nullptr, false, pObject->ToUnit()->PowerGetTotal(POWER_LIFE) * 10 / 100, 0, DAMAGE_TYPE_DEBUFF);
			pObject->ToUnit()->AddBuff(BUFF_LIGHTNING_SHOCK, 1, 0, me());

			VIEWPORT_CLOSE
		}
	};
};

void AddSC_WorldBossNew()
{
	sScriptAI->AddScriptAI(new WorldBossScriptNewDW());
}