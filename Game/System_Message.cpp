SystemMessage::SystemMessage()
{

}

SystemMessage::~SystemMessage()
{
        MAP_CLEAR(SystemMessageMap::iterator, this->m_MessageMap);
}

void SystemMessage::Init(char* pchFileName)
{
        sLog->outInfo(LOG_DEFAULT, "Loading System Messages...");

        MAP_CLEAR(SystemMessageMap::iterator, this->m_MessageMap);

        pugi::xml_document file;
        pugi::xml_parse_result res = file.load_file(pchFileName);

        if (res.status != pugi::status_ok) {
                sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
        }

        pugi::xml_node Main = file.child("SystemMessage");

        uint32 count = 0;

        for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
                SystemMessageData* add_message = new SystemMessageData;
                add_message->m_ID = data.attribute("ID").as_int();
                add_message->m_Language = data.attribute("Language").as_int();
                add_message->m_Text = data.attribute("Text").as_string();

                this->m_MessageMap[add_message->m_ID] = add_message;
                count++;
        }

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u system messages", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

std::string SystemMessage::Get(uint32 id) const
{
        SystemMessageMap::const_iterator it = this->m_MessageMap.find(id);

        if ( it != this->m_MessageMap.end() )
        {
                return it->second->m_Text;
        }
        else
        {
                return ">>> System Message Error <<<";
        }
}
