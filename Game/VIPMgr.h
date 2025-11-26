/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "VIPMgr.h"
*
*/

#ifndef VIP_MGR_H
#define VIP_MGR_H

struct vip_template
{
    uint32 m_Id = 0;
    std::string m_Name;
    float m_Experience = 0.0f;
    int32 m_Instance = 0;
    uint32 m_Duration = 0;
    std::string m_Comment;
};

typedef std::map<uint32, vip_template*> VipTemplateMap;

class CVipMgr
{
    SingletonInstance(CVipMgr);

    public:
        CVipMgr();
        virtual ~CVipMgr();

        void LoadVipTemplate(char* pchFileName);
        vip_template * GetVipData(uint32 guid) const;

        void ApplyVipExperience(Player* pPlayer, int64 & experience);
        void SendVipData(Player* pPlayer);
    private:
        VipTemplateMap m_VipTemplateMap;
};

#endif
