/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "VIPMgr.cpp"
*
*/

CVipMgr::CVipMgr()
{

}

CVipMgr::~CVipMgr()
{
        MAP_CLEAR(VipTemplateMap::iterator, this->m_VipTemplateMap);
}

void CVipMgr::LoadVipTemplate(char* pchFileName)
{
        sLog->outInfo(LOG_DEFAULT, "Loading Vip Template...");

        MAP_CLEAR(VipTemplateMap::iterator, this->m_VipTemplateMap);

        pugi::xml_document file;
        pugi::xml_parse_result res = file.load_file(pchFileName);


        if (res.status != pugi::status_ok) {
                sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
        }

        pugi::xml_node Main = file.child("VipSystem");

        uint32 count = 0;

        for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
                vip_template* add_vip = new vip_template;

                add_vip->m_Id = data.attribute("ID").as_int();
                add_vip->m_Name = data.attribute("Name").as_string();
                add_vip->m_Experience = data.attribute("Exp").as_float();
                add_vip->m_Instance = data.attribute("Instance").as_int();
                add_vip->m_Duration = data.attribute("Duration").as_int();
                add_vip->m_Comment = data.attribute("Comment").as_string();

                this->m_VipTemplateMap[add_vip->m_Id] = add_vip;

                count++;
        }

        sLog->outInfo(LOG_DEFAULT, ">> Loaded %u vip template", count);
        sLog->outInfo(LOG_DEFAULT, " ");
}

vip_template * CVipMgr::GetVipData(uint32 guid) const
{
        VipTemplateMap::const_iterator it = this->m_VipTemplateMap.find(guid);

        if ( it != this->m_VipTemplateMap.end() )
                return it->second;

        return nullptr;
}

void CVipMgr::ApplyVipExperience(Player* pPlayer, int64 & experience)
{
        vip_template const* vip_data = this->GetVipData(pPlayer->GetAccountData()->GetVIPStatus());

        if ( !vip_data )
                return;

        experience *= vip_data->m_Experience;
}

void CVipMgr::SendVipData(Player* pPlayer)
{
        vip_template const* vip_data = this->GetVipData(pPlayer->GetAccountData()->GetVIPStatus());

        if ( !vip_data )
                return;

        pPlayer->SendNotice(NOTICE_NORMAL_BLUE, "[ VIP ]: %s.", vip_data->m_Name.c_str());
}
