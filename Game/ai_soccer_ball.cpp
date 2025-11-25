class SoccerBallScript: public MonsterScriptAI
{
public:
	explicit SoccerBallScript(): ScriptAI("soccer_ball_ai") { }
	virtual ~SoccerBallScript() {}

	MonsterAI* GetAI(Monster* monster) const { return new SoccerBallAI(monster); }

        struct SoccerBallAI: public MonsterAI
        {
                BattleSoccerState m_State;

                explicit SoccerBallAI(Monster* monster): MonsterAI(monster)
                {
                        this->m_State = BATTLE_SOCCER_STATE_NONE;
                        me()->SetRespawnLocation(MONSTER_RESPAWN_ZONE);
                }

		virtual ~SoccerBallAI() {}

                bool Update()
                {
                        if ( this->m_State != sBattleSoccerMgr->m_State )
                        {
                                this->m_State = sBattleSoccerMgr->m_State;

                                if ( this->m_State == BATTLE_SOCCER_STATE_STANDBY )
                                {
                                        me()->Kill();
                                }
                        }

                        sBattleSoccerMgr->UpdateBall(me());
                        return true;
                }

                bool EnableAttack(Unit* pAttacker, Skill* pSkill, int32 count)
                {
                        if ( sBattleSoccerMgr->m_State == BATTLE_SOCCER_STATE_PLAYING && sBattleSoccerMgr->m_BallMove && count <= 1 )
                        {
                                if ( pSkill )
                                        pAttacker->PushBackCount(me(), 3);
                                else
                                        pAttacker->PushBackCount(me(), 2);
                        }

                        if ( pSkill )
                                pAttacker->MagicAttackSend(ENTRY(me()), pSkill->GetSkill());

			return false;
		}

		bool SendHPBar()
		{
			return false;
		}
	};
};

void AddSC_SoccerBall()
{
	sScriptAI->AddScriptAI(new SoccerBallScript());
}