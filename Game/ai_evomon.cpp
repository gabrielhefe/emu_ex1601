class EvomonPortalScript : public MonsterScriptAI
{
public:
	explicit EvomonPortalScript() : ScriptAI("evomon_portal_ai") { }
	virtual ~EvomonPortalScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
                uint32 m_PlayerGUID;
                Player* m_Player;
                Monster* m_Evomon;
                uint8 m_EvomonID;
                uint16 m_EvomonMonster;

                bool m_Finished;
                TickTimer m_EvomonTime;
                bool m_EvomonTimeFinished;

                int32 m_Score;
                bool m_EvomonSpecial;

                TickTimer m_GlobalTime;

		explicit AI(Monster* monster) : MonsterAI(monster)
		{
                        this->m_PlayerGUID = 0;
                        this->m_Player = nullptr;
                        this->m_Evomon = nullptr;
                        this->m_EvomonID = 0;
                        this->m_EvomonMonster = -1;
                        this->m_Finished = false;
                        this->m_EvomonTime.Reset();
                        this->m_EvomonTimeFinished = false;
                        this->m_Score = 0;
                        this->m_EvomonSpecial = false;
		}

		virtual ~AI() { }

		bool Update()
		{
			me()->SetNextActionTime(1000);

			if (!this->m_Player)
			{
				this->RemoveEvomon();
				me()->Remove();
				return true;
			}

			if (this->m_Player->GetConnectStatus() != CONNECT_STATUS_PLAYING)
			{
				this->m_Player = nullptr;
				return true;
			}

			if (this->m_Player->GetGUID() != this->m_PlayerGUID)
			{
				this->m_Player = nullptr;
				return true;
			}

			if (this->m_Finished)
			{
				if (this->m_Player)
				{
					this->m_Player->SetEvomonPortal(nullptr);
					this->m_Player->SetEvomonTime(0);
				}

				this->RemoveEvomon();
				me()->Remove();

				return true;
			}

			if (MyGetTickCount() > this->m_Player->GetEvomonTime())
			{
				this->m_Finished = true;
				return true;
			}

			this->CreateEvomon();

			return true;
		}

		void OnCreate()
		{
                        this->m_PlayerGUID = me()->m_AdditionalDataInt[0];
                        this->m_Player = static_cast<Player*>(me()->m_AdditionalDataPtr[0]);
                        this->m_EvomonMonster = me()->m_AdditionalDataInt[1];
		}

		void CreateEvomon()
		{
			if (this->m_Finished)
			{
				return;
			}

			if (this->m_Evomon)
			{
				if (!this->m_Evomon->IsPlaying())
				{
					if (this->m_EvomonSpecial)
					{
						sEvomon->GiveSpecialReward(this->m_Player);
					}

					this->m_Evomon = nullptr;
                                        this->m_EvomonID = sEvomon->GetNextEvomon(this->m_EvomonID, this->m_EvomonMonster);
                                        this->m_EvomonTime.Reset();
                                        this->m_EvomonTimeFinished = false;
                                        me()->ViewportCreate(VIEWPORT_CREATE_FLAG_ME);
                                }

				return;
			}

			if (!this->m_EvomonTimeFinished)
			{
                                if (!this->m_EvomonTime.Elapsed(3000))
                                {
                                        return;
                                }

				this->m_EvomonTimeFinished = true;
			}

			EvomonInfo const* pEvomonInfo = sEvomon->GetEvomon(this->m_EvomonID, this->m_EvomonMonster);

			if (!pEvomonInfo)
			{
				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Awakening Failed. Awakening has failed due to low chance.");
				sEvomon->GiveReward(this->m_Player, this->m_Score);

				me()->AddBuff(BUFF_EVOMON_AWAKENING_FAILURE, 5, 0, me());
				this->m_Finished = true;
				return;
			}

			me()->SetLevel(pEvomonInfo->GetLevel());
			this->m_Score = pEvomonInfo->GetLevel();

                        this->m_EvomonSpecial = roll_chance_i(pEvomonInfo->GetSpecialRate(), 10000);

			uint16 monster_id = this->m_EvomonSpecial ? pEvomonInfo->GetSpecialMonster() : pEvomonInfo->GetMonster();

			Monster* pMonster = sObjectMgr->MonsterTryAdd(monster_id, me()->GetWorldId());

			if (pMonster)
			{
				pMonster->SetLevel(pEvomonInfo->GetLevel());
				pMonster->PowerSet(POWER_LIFE, pEvomonInfo->GetLife());
				pMonster->PowerSetMax(POWER_LIFE, pEvomonInfo->GetLife());
				pMonster->SetScriptMaxPower(POWER_LIFE, pEvomonInfo->GetLife());
				pMonster->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MIN, pEvomonInfo->GetAttackDamageMin());
				pMonster->SetIntData(UNIT_INT_PHYSICAL_DAMAGE_MAX, pEvomonInfo->GetAttackDamageMax());
				pMonster->SetIntData(UNIT_INT_ATTACK_RATE, pEvomonInfo->GetAttackSuccessRate());
				pMonster->SetIntData(UNIT_INT_DEFENSE, pEvomonInfo->GetDefense());
				pMonster->SetIntData(UNIT_INT_DEFENSE_RATE, pEvomonInfo->GetDefenseSuccessRate());
				pMonster->SetSummonType(2);
				pMonster->SetWorldId(me()->GetWorldId());
				pMonster->SetBasicLocation(me()->GetX(), me()->GetY(), me()->GetX(), me()->GetY());
				pMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
				pMonster->SetSummonPlayer(this->m_Player);
				pMonster->SetInstance(me()->GetInstance());
				pMonster->SetScriptName("evomon_ai");
				pMonster->AddToWorld();

				this->m_Evomon = pMonster;

				me()->AddBuff(BUFF_EVOMON_AWAKENING_SUCCESS, 3, 0, me());

				this->m_Player->SendNotice(CUSTOM_MESSAGE_ID_BLUE, "Awakening Succeeded. After a moment awakened monster appears.");
			}
		}

		void RemoveEvomon()
		{
			if (this->m_Evomon)
			{
				this->m_Evomon->Remove();
				this->m_Evomon = nullptr;
			}
		}
	};
};

class EvomonScript : public MonsterScriptAI
{
public:
	explicit EvomonScript() : ScriptAI("evomon_ai") { }
	virtual ~EvomonScript() { }

	MonsterAI* GetAI(Monster* monster) const { return new AI(monster); }

	struct AI : public MonsterAI
	{
		explicit AI(Monster* monster) : MonsterAI(monster) { }

		virtual ~AI() { }

		bool Drop()
		{
			return true;
		}
	};
};

void AddSC_Evomon()
{
	sScriptAI->AddScriptAI(new EvomonPortalScript());
	sScriptAI->AddScriptAI(new EvomonScript());
}