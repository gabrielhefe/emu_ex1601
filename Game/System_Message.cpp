SystemMessage::SystemMessage()
{

}

SystemMessage::~SystemMessage()
{
	MAP_CLEAR(SystemMessageMap::iterator, this->message_map);
}

void SystemMessage::Init(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading System Messages...");

	MAP_CLEAR(SystemMessageMap::iterator, this->message_map);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SystemMessage");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		SystemMessageData* add_message = new SystemMessageData;
		add_message->SetID(data.attribute("ID").as_int());
		add_message->SetLanguage(data.attribute("Language").as_int());
		add_message->SetText(data.attribute("Text").as_string());

		this->message_map[add_message->GetID()] = add_message;
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u system messages", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

std::string SystemMessage::Get(uint32 id) const
{
	SystemMessageMap::const_iterator it = this->message_map.find(id);

	if ( it != this->message_map.end() )
	{
		return it->second->GetText();
	}
	else
	{
		return ">>> System Message Error <<<";
	}
}