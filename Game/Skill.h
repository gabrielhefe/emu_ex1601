/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "Skill.h"
*
*/

#ifndef SKILL_H
#define SKILL_H

class Skill
{
public:
    Skill(): m_Skill(0), m_Level(0), m_Weapon(false), m_Type(0)
    {
    }

    Skill(uint16 skill, uint8 level): m_Skill(skill), m_Level(level), m_Weapon(false), m_Type(0)
    {
    }

    void Clear();

public:
    uint16 GetSkill() const { return this->m_Skill; }
    void SetSkill(uint16 value) { this->m_Skill = value; }

    uint8 GetLevel() const { return this->m_Level; }
    void SetLevel(uint8 value) { this->m_Level = value; }

    bool IsWeapon() const { return this->m_Weapon; }
    void SetWeapon(bool value) { this->m_Weapon = value; }

    uint8 GetType() const { return this->m_Type; }
    void SetType(uint8 value) { this->m_Type = value; }

    uint16 m_Skill;
    uint8 m_Level;
    bool m_Weapon;
    uint8 m_Type;
};

#endif
