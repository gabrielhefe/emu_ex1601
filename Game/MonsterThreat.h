/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "MonsterThreat.h"
*/

#ifndef MONSTER_THREAT_H
#define MONSTER_THREAT_H

#include <cstring>
#include <ctime>
#include <map>
#include <vector>

#define MONSTER_THREAT_VALUE_MAX                        100000000000

enum MonsterThreatDamageType
{
        MONSTER_THREAT_DAMAGE_NORMAL,
        MONSTER_THREAT_DAMAGE_REFLECT,
        MONSTER_THREAT_DAMAGE_DEBUFF,
        MONSTER_THREAT_DAMAGE_ELEMENTAL,
        MONSTER_THREAT_DAMAGE_MAX
};

struct DamageData
{
        friend class MonsterThreat;

        explicit DamageData(uint32 guid, const char * name, MonsterThreatDamageType type, int64 value)
        {
                this->m_Guid = guid;
                this->ResetDamage(0);
                this->m_FirstHit = time(nullptr);
                this->AddDamage(name, type, value);
        }

        void AddDamage(const char * name, MonsterThreatDamageType type, int64 value)
        {
                this->ResetName();
                this->SetName(name);
                this->IncreaseDamage(type, value);
                this->m_LastAttackTime = GetTickCount();
        }

        void ResetName()
        {
                std::memset(this->m_Name, 0, sizeof(this->m_Name));
        }

        void SetName(const char * name)
        {
                if ( !name )
                {
                        this->m_Name[0] = '\0';
                        return;
                }

                std::strncpy(this->m_Name, name, MAX_CHARACTER_LENGTH);
                this->m_Name[MAX_CHARACTER_LENGTH] = '\0';
        }

        void ResetDamage(int64 value)
        {
                for ( int32 i = 0; i < MONSTER_THREAT_DAMAGE_MAX; ++i )
                {
                        this->m_Damage[i] = value;
                }
        }

        void IncreaseDamage(int32 type, int64 value)
        {
                if ( type < 0 || type >= MONSTER_THREAT_DAMAGE_MAX )
                {
                        return;
                }

                this->m_Damage[type] += value;
        }

        int64 GetDamage(int32 index) const
        {
                if ( index < 0 || index >= MONSTER_THREAT_DAMAGE_MAX )
                {
                        return 0;
                }

                return this->m_Damage[index];
        }

        int64 GetTotalDamage() const
        {
                int64 damage = 0;

                for ( int32 i = 0; i < MONSTER_THREAT_DAMAGE_MAX; ++i )
                {
                        damage += this->m_Damage[i];
                }

                return damage;
        }

        uint32 m_Guid;
        char m_Name[MAX_CHARACTER_LENGTH + 1];
        int64 m_Damage[MONSTER_THREAT_DAMAGE_MAX];
        uint32 m_LastAttackTime;
        time_t m_FirstHit;
};

struct ThreatData
{
        friend class MonsterThreat;

        explicit ThreatData(uint32 guid, int64 value, Player* pPlayer)
        {
                this->m_Guid = guid;
                this->m_Threat = value;
                this->m_Player = pPlayer;
        }

        void AddThreat(int64 value)
        {
                this->m_Threat += value;

                if ( this->m_Threat > MONSTER_THREAT_VALUE_MAX )
                {
                        this->m_Threat = MONSTER_THREAT_VALUE_MAX;
                }
        }

        uint32 m_Guid;
        int64 m_Threat;
        Player* m_Player;
};

///- TODO: Rebuild a map con char id
typedef std::vector<DamageData*> DamageDataList;
typedef std::map<uint32, ThreatData*> ThreatDataMap;

class MonsterThreat
{
        public:
                friend class Monster;

                void Create(Monster* pOwner);
                void AddThreat(Player* pVictim, int64 threat);
                void AddDamage(Player* pVictim, MonsterThreatDamageType type, int64 damage);

                void Update();
                void UpdateThreat();

                void ClearThreat();
                void ClearDamage();

                Monster* m_Owner;
                uint32 m_MostHatedID;
                int64 m_HighestThreat;

                DamageDataList const& GetDamageData() const { return this->m_DamageList; }
                ThreatDataMap const& GetThreatData() const { return this->m_ThreatMap; }

                bool IsInThreat(Unit* pTarget);
                bool IsUsingThreat() const;

                DamageData* GetDamageData(Player* pPlayer);
                DamageData* GetDamageData(uint32 char_id);

                ThreatData* GetThreatData(uint32 id);
        private:
                DamageDataList m_DamageList;
                ThreatDataMap m_ThreatMap;
};

#endif
