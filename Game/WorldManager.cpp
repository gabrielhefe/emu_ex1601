WorldMgr::WorldMgr()
{
	
}

WorldMgr::~WorldMgr()
{
	this->world_server_map.clear();
	MAP_CLEAR(WorldExperienceMap::iterator, this->world_experience_map);
	MAP_CLEAR(WorldMap::iterator, this->world_map);
	LIST_CLEAR(WorldFreePKList::iterator, this->m_WorldFreePKList);
}

void WorldMgr::LoadWorldList(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Worlds...");

	std::map<uint16, WorldStatusData> WorldStatusList;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		if ( itr->second->GetStatus() < MAX_WORLD_STATUS )
		{
			WorldStatusList[itr->first].SetStatus(itr->second->GetStatus());

			for ( int32 i = 0; i < WORLD_MAX_SIZE; ++i )
			{
				WorldStatusList[itr->first].GetGrid(i)->attribute = itr->second->WorldStatus[itr->second->GetStatus()].GetGrid(i)->attribute;
			}
		}
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldTemplate");

	uint32 count = 0;
		
	for (pugi::xml_node WorldList = Main.child("World"); WorldList; WorldList = WorldList.next_sibling()) {
		uint16 entry = WorldList.attribute("ID").as_int();
		uint16 display_entry = WorldList.attribute("DisplayID").as_int();
		uint8 status = WorldList.attribute("State").as_int();
		std::string namefile = WorldList.attribute("File").as_string();;
		std::string file = "../Data/World/Terrain/" + namefile;
		uint16 spawn_gate = WorldList.attribute("SpawnGate").as_int();
		uint32 experience = WorldList.attribute("ExpRate").as_int();
		uint32 zen_rate = WorldList.attribute("ZenRate").as_int();
		uint32 flags = WorldList.attribute("Flags").as_int();

		if (status >= MAX_WORLD_STATUS)
			continue;

		World* pWorld = this->GetWorld(entry);

		if (!pWorld) {
			++count;
			pWorld = new World;
			this->world_map[entry] = pWorld;
		}

		pWorld->SetEntry(entry);
		pWorld->SetDisplay(display_entry);

		WorldStatusData& WorldStatusAdd = pWorld->WorldStatus[status];

		if (WorldStatusAdd.LoadWorld(file.c_str()))
		{
			WorldStatusAdd.SetStatus(status);
			WorldStatusAdd.SetName(WorldList.attribute("Name").as_string());
			WorldStatusAdd.SetSpawnGate(spawn_gate);
			WorldStatusAdd.SetExperienceRate(experience);
			WorldStatusAdd.SetZenRate(zen_rate);
			WorldStatusAdd.SetFlags(flags);
			WorldStatusAdd.SetActive(true);
		}
	}

	/*QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_template ORDER BY entry ASC, state ASC");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint16 entry = fields[0].GetUInt16();
			uint16 display_entry = fields[1].GetUInt16();
			uint8 status = fields[2].GetUInt8();
			std::string file = "../Data/World/Terrain/" + fields[3].GetString();
			uint16 spawn_gate = fields[5].GetUInt16();
			uint32 experience = fields[6].GetUInt32();
			uint32 zen_rate = fields[7].GetUInt32();
			uint32 flags = fields[8].GetUInt32();

			if ( status >= MAX_WORLD_STATUS )
				continue;

			World* pWorld = this->GetWorld(entry);

			if ( !pWorld )
			{
				++count;
				pWorld = new World;
				this->world_map[entry] = pWorld;
			}

			pWorld->SetEntry(entry);
			pWorld->SetDisplay(display_entry);

			WorldStatusData & WorldStatusAdd = pWorld->WorldStatus[status];

			if ( WorldStatusAdd.LoadWorld(file.c_str()) )
			{
				WorldStatusAdd.SetStatus(status);
				WorldStatusAdd.SetName(fields[4].GetCString());
				WorldStatusAdd.SetSpawnGate(spawn_gate);
				WorldStatusAdd.SetExperienceRate(experience);
				WorldStatusAdd.SetZenRate(zen_rate);
				WorldStatusAdd.SetFlags(flags);
				WorldStatusAdd.SetActive(true);
			}
		}
		while(result->NextRow());
	}*/

	for ( std::map<uint16, WorldStatusData>::const_iterator itr = WorldStatusList.begin(); itr != WorldStatusList.end(); ++itr )
	{
		World* pWorld = this->GetWorld(itr->first);

		if ( pWorld )
		{
			pWorld->statusChange(itr->second.GetStatus());

			for ( int32 i = 0; i < WORLD_MAX_SIZE; ++i )
			{
				pWorld->WorldStatus[itr->second.GetStatus()].GetGrid(i)->attribute = itr->second.GetGrid(i)->attribute;
			}
		}
	}

	WorldStatusList.clear();

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u worlds", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldData(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Worlds...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldTemplate");
	
	for (pugi::xml_node WorldList = Main.child("World"); WorldList; WorldList = WorldList.next_sibling()) {
		uint16 entry = WorldList.attribute("ID").as_int();
		uint16 display_entry = WorldList.attribute("DisplayID").as_int();
		uint8 status = WorldList.attribute("State").as_int();
		std::string namefile = WorldList.attribute("File").as_string();;
		std::string file = "../Data/World/Terrain/" + namefile;
		uint16 spawn_gate = WorldList.attribute("SpawnGate").as_int();
		uint32 experience = WorldList.attribute("ExpRate").as_int();
		uint32 zen_rate = WorldList.attribute("ZenRate").as_int();
		uint32 flags = WorldList.attribute("Flags").as_int();

		if (status >= MAX_WORLD_STATUS) {
			continue;
		}

		World* pWorld = this->GetWorld(entry);

		if (!pWorld) {
			continue;
		}

		pWorld->SetEntry(entry);
		pWorld->SetDisplay(display_entry);

		WorldStatusData& WorldStatusAdd = pWorld->WorldStatus[status];

		WorldStatusAdd.SetStatus(status);
		WorldStatusAdd.SetName(WorldList.attribute("Name").as_string());
		WorldStatusAdd.SetSpawnGate(spawn_gate);
		WorldStatusAdd.SetExperienceRate(experience);
		WorldStatusAdd.SetZenRate(zen_rate);
		WorldStatusAdd.SetFlags(flags);
		WorldStatusAdd.SetActive(true);
	}

	/*QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_template ORDER BY entry ASC, state ASC");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint16 entry = fields[0].GetUInt16();
			uint16 display_entry = fields[1].GetUInt16();
			uint8 status = fields[2].GetUInt8();
			std::string file = "../Data/World/Terrain/" + fields[3].GetString();
			uint16 spawn_gate = fields[5].GetUInt16();
			uint32 experience = fields[6].GetUInt32();
			uint32 zen_rate = fields[7].GetUInt32();
			uint32 flags = fields[8].GetUInt32();

			if ( status >= MAX_WORLD_STATUS )
			{
				continue;
			}

			World* pWorld = this->GetWorld(entry);

			if ( !pWorld )
			{
				continue;
			}

			pWorld->SetEntry(entry);
			pWorld->SetDisplay(display_entry);

			WorldStatusData & WorldStatusAdd = pWorld->WorldStatus[status];
			WorldStatusAdd.SetStatus(status);
			WorldStatusAdd.SetName(fields[4].GetCString());
			WorldStatusAdd.SetSpawnGate(spawn_gate);
			WorldStatusAdd.SetExperienceRate(experience);
			WorldStatusAdd.SetZenRate(zen_rate);
			WorldStatusAdd.SetFlags(flags);
			WorldStatusAdd.SetActive(true);
		}
		while(result->NextRow());
	}*/

	sLog->outInfo(LOG_DEFAULT, ">> Loaded worlds");
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldAIPath(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading World AI Path...");

	uint32 count = 0;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldAI");

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( !pWorld )
		{
			continue;
		}

		LIST_CLEAR(WorldAIPathList::iterator, pWorld->m_path_list);

		for (pugi::xml_node ai = Main.child("AI"); ai; ai = ai.next_sibling()) {
			if (pWorld->GetEntry() == ai.attribute("World").as_int()) {
				WorldAIPath* ai_path = new WorldAIPath;
				ai_path->SetID(ai.attribute("ID").as_int());
				ai_path->SetType(ai.attribute("Type").as_int());
				ai_path->SetX(ai.attribute("X").as_int());
				ai_path->SetY(ai.attribute("Y").as_int());

				pWorld->m_path_list.push_back(ai_path);

				++count;
			}
		}
		/*QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_ai_path WHERE world = '%u' ORDER BY id ASC", pWorld->GetEntry());

		if ( result )
		{
			do
			{
				Field* fields = result->Fetch();

				WorldAIPath * ai_path = new WorldAIPath;
				ai_path->SetID(fields[1].GetUInt32());
				ai_path->SetType(fields[2].GetUInt8());
				ai_path->SetX(fields[3].GetInt32());
				ai_path->SetY(fields[4].GetInt32());

				pWorld->m_path_list.push_back(ai_path);

				++count;
			}
			while(result->NextRow());
		}*/
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world path definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldBuff(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Buff...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldBuff");

	uint32 count = 0;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( !pWorld )
		{
			continue;
		}

		MAP_CLEAR(WorldBuffMap::iterator, pWorld->m_buff);

		for (pugi::xml_node world = Main.child("Data"); world; world = world.next_sibling()) {
			if (world.attribute("WorldID").as_int() == pWorld->GetEntry()) {
				WorldBuff* buff = new WorldBuff;
					buff->SetBuff(world.attribute("Buff").as_int());
					buff->SetFlag(world.attribute("Flag").as_int());

					pWorld->m_buff[buff->GetBuff()] = buff;

					++count;
			}
		}
		/*QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_buff WHERE world = '%u' ORDER BY buff ASC", pWorld->GetEntry());

		if ( result )
		{
			do
			{
				Field* fields = result->Fetch();

				WorldBuff * buff = new WorldBuff;
				buff->SetBuff(fields[1].GetUInt16());
				buff->SetFlag(fields[2].GetUInt8());
				
				pWorld->m_buff[buff->GetBuff()] = buff;

				++count;
			}
			while(result->NextRow());
		}*/
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world buff definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldAreaRestriction(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Area Restriction...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldAreaRestriction");

	uint32 count = 0;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( !pWorld )
		{
			continue;
		}

		LIST_CLEAR(WorldAreaList::iterator, pWorld->m_area);

		for (pugi::xml_node world = Main.child("Area"); world; world = world.next_sibling()) {
			if (pWorld->GetEntry() == world.attribute("World").as_int()) {
				WorldArea* area = new WorldArea;
				area->SetX(world.attribute("X").as_int());
				area->SetY(world.attribute("Y").as_int());
				area->SetRange(world.attribute("Range").as_int());
				area->SetFlags(world.attribute("Flags").as_int());

				pWorld->m_area.push_back(area);

				++count;
			}
		}

		/*QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_area_restriction WHERE world = '%u'", pWorld->GetEntry());

		if ( result )
		{
			do
			{
				Field* fields = result->Fetch();

				WorldArea * area = new WorldArea;
				area->SetX(fields[1].GetInt32());
				area->SetY(fields[2].GetInt32());
				area->SetRange(fields[3].GetUInt8());
				area->SetFlags(fields[4].GetUInt8());
				
				pWorld->m_area.push_back(area);

				++count;
			}
			while(result->NextRow());
		}*/
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world area restriction definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldServer(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Server...");

	this->world_server_map.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldServer");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Option"); option; option = option.next_sibling()) {
		uint16 server = option.attribute("ServerCode").as_int();
		uint16 world = option.attribute("WorldID").as_int();

		if ((server / MAX_SERVER_PER_GROUP) != sGameServer->GetServerGroup())
			continue;

		this->world_server_map[server].insert(world);

		++count;
	}
	/*QueryResult result = LoginDatabase.Query("SELECT * FROM world_server");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			uint16 server = fields[0].GetUInt16();
			uint16 world = fields[1].GetUInt16();

			if ( (server / MAX_SERVER_PER_GROUP) != sGameServer->GetServerGroup() )
				continue;

			this->world_server_map[server].insert(world);

			++count;
		}
		while(result->NextRow());
	}*/

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world server definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldAttribute(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Attribute...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldAttribute");

	uint32 count = 0;

	for (pugi::xml_node att = Main.child("Attribute"); att; att = att.next_sibling()) {
		World* pWorld = this->GetWorld(att.attribute("WorldID").as_int());
		uint8 status = att.attribute("State").as_int();

		if (status >= MAX_WORLD_STATUS)
		{
			continue;
		}

		if (!pWorld)
		{
			continue;
		}

		const char* data = att.attribute("Data").as_string();

		if (data)
		{
			Tokens tokens(data, ' ');

			for (int32 i = 0; i < WORLD_MAX_SIZE; ++i)
			{
				pWorld->WorldStatus[status].GetGrid(i)->attribute = static_cast<uint8>(atoi(tokens[i]));
			}
		}

		++count;
	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM world_attribute");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			World* pWorld = this->GetWorld(fields[0].GetUInt16());
			uint8 status = fields[1].GetUInt8();

			if ( status >= MAX_WORLD_STATUS )
			{
				continue;
			}

			if ( !pWorld )
			{
				continue;
			}

			const char* data = fields[2].GetCString();
				
			if ( data )
			{
				Tokens tokens(data, ' ');
					
				for ( int32 i = 0; i < WORLD_MAX_SIZE; ++i )
				{
					pWorld->WorldStatus[status].GetGrid(i)->attribute = static_cast<uint8>(atoi(tokens[i]));
				}
			}

			++count;
		}
		while(result->NextRow());
	}*/

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world attribute definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldExperience(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Experience...");

	MAP_CLEAR(WorldExperienceMap::iterator, this->world_experience_map);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldExpParty");

	uint32 count = 0;

	for (pugi::xml_node data = Main.child("Data"); data; data = data.next_sibling()) {
		world_experience* dataexp = new world_experience;
		dataexp->SetWorld(data.attribute("WorldID").as_int());

		dataexp->SetRate(0, data.attribute("RatePartyMem1").as_int());
		dataexp->SetRate(1, data.attribute("RatePartyMem2").as_int());
		dataexp->SetRate(2, data.attribute("RatePartyMem3").as_int());
		dataexp->SetRate(3, data.attribute("RatePartyMem4").as_int());
		dataexp->SetRate(4, data.attribute("RatePartyMem5").as_int());

		this->world_experience_map[dataexp->GetWorld()] = dataexp;
		++count;
	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM world_experience");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			world_experience * data = new world_experience;
			data->SetWorld(fields[loop++].GetUInt16());
			
			for ( int32 i = 0; i < MAX_PARTY_MEMBERS; ++i )
			{
				data->SetRate(i, fields[loop++].GetInt32());
			}

			this->world_experience_map[data->GetWorld()] = data;
			++count;
		}
		while(result->NextRow());
	}*/

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world experience definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldAreaAttribute(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Area Attribute...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldAreaAttribute");

	uint32 count = 0;

	for ( WorldMap::const_iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( !pWorld )
		{
			continue;
		}

		LIST_CLEAR(WorldAreaAttributeList::iterator, pWorld->m_attribute);

		for (pugi::xml_node world = Main.child("Area"); world; world = world.next_sibling()) {
			if (pWorld->GetEntry() == world.attribute("WorldID").as_int()) {
				WorldAreaAttribute* area = new WorldAreaAttribute;
				area->SetX1(world.attribute("X1").as_int());
				area->SetY1(world.attribute("Y1").as_int());
				area->SetX2(world.attribute("X2").as_int());
				area->SetY2(world.attribute("Y2").as_int());
				area->SetAttribute(world.attribute("Attribute").as_int());
				area->SetApply(world.attribute("Apply").as_int());
				area->SetSend(world.attribute("Send").as_int());

				pWorld->m_attribute.push_back(area);

				int16 coord[4] = { area->GetX1(), area->GetY1(), area->GetX2(), area->GetY2() };
				pWorld->ApplyAttribute(coord, area->GetAttribute(), area->IsApply());

				++count;
			}
		}
		/*QueryResult result = GameServerDatabase.PQuery("SELECT * FROM world_area_attribute WHERE world_id = '%u'", pWorld->GetEntry());

		if ( result )
		{
			do
			{
				Field* fields = result->Fetch();

				WorldAreaAttribute * area = new WorldAreaAttribute;
				area->SetX1(fields[1].GetInt16());
				area->SetY1(fields[2].GetInt16());
				area->SetX2(fields[3].GetInt16());
				area->SetY2(fields[4].GetInt16());
				area->SetAttribute(fields[5].GetUInt8());
				area->SetApply(fields[6].GetBool());
				area->SetSend(fields[7].GetBool());
				
				pWorld->m_attribute.push_back(area);

				int16 coord[4] = { area->GetX1(), area->GetY1(), area->GetX2(), area->GetY2() };
				pWorld->ApplyAttribute(coord, area->GetAttribute(), area->IsApply());

				++count;
			}
			while(result->NextRow());
		}*/
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world area attribute definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::LoadWorldFreePK(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading World Free PK...");

	LIST_CLEAR(WorldFreePKList::iterator, this->m_WorldFreePKList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);


	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("WorldFreePK");

	uint32 count = 0;

	for (pugi::xml_node zone = Main.child("Zone"); zone; zone= zone.next_sibling()) {
		WorldFreePK* pData = new WorldFreePK;
		pData->SetWorld(zone.attribute("WorldID").as_int());
		pData->SetX1(zone.attribute("X1").as_int());
		pData->SetY1(zone.attribute("Y1").as_int());
		pData->SetX2(zone.attribute("X2").as_int());
		pData->SetY2(zone.attribute("Y2").as_int());

		this->m_WorldFreePKList.push_back(pData);
		++count;
	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM world_free_pk");

	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			int32 loop = 0;

			WorldFreePK * pData = new WorldFreePK;
			pData->SetWorld(fields[loop++].GetUInt16());
			pData->SetX1(fields[loop++].GetInt16());
			pData->SetY1(fields[loop++].GetInt16());
			pData->SetX2(fields[loop++].GetInt16());
			pData->SetY2(fields[loop++].GetInt16());
			
			this->m_WorldFreePKList.push_back(pData);
			++count;
		} while (result->NextRow());
	}
	*/
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u world free pk definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void WorldMgr::Update()
{
	for ( WorldMap::iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( pWorld )
		{
			pWorld->Update();
			pWorld->UpdateItemQueue();
		}
	}
}

void WorldMgr::UpdateWeather()
{
	for ( WorldMap::iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( pWorld )
		{
			pWorld->UpdateWeather();
		}
	}
}

void WorldMgr::statusChange(uint16 entry, uint8 status)
{
	if ( status >= MAX_WORLD_STATUS )
	{
		return;
	}

	World* pWorld = this->GetWorld(entry);

	if ( !pWorld )
	{
		return;
	}

	pWorld->statusChange(status);
}

void WorldMgr::MakeItemVisible(Player* pPlayer)
{
	for ( WorldMap::iterator itr = this->world_map.begin(); itr != this->world_map.end(); ++itr )
	{
		World* pWorld = itr->second;

		if ( pWorld )
		{
			pWorld->MakeItemVisible(pPlayer);
		}
	}
}

uint16 WorldMgr::AllowMoveToWorld(Player* pPlayer, uint16 world)
{
	if ( !pPlayer )
		return -1;

	WorldServerMap::const_iterator itrr = this->world_server_map.find(sGameServer->GetServerCode());

	if ( itrr != this->world_server_map.end() ) // Si el server tiene mapas y no est�n en la lista entonces no me puedo mover
	{
		WorldServerList::const_iterator it = itrr->second.find(world);

		if ( it != itrr->second.end() )
			return sGameServer->GetServerCode();
	}

	for ( WorldServerMap::const_iterator itr = this->world_server_map.begin(); itr != this->world_server_map.end(); ++itr )
	{
		for ( WorldServerList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			if ( (*it) == world ) // Busco el mapa en la lista
			{
				return itr->first;
			}
		}
	}

	if ( itrr != this->world_server_map.end() )
	{
		return -1;
	}

	return sGameServer->GetServerCode();
}

bool WorldMgr::AllowMoveToWorld(Player* pPlayer, uint16 world, uint16 server)
{
	if ( !pPlayer )
		return false;

	WorldServerMap::const_iterator itr = this->world_server_map.find(server);

	if ( itr != this->world_server_map.end() )
	{
		for ( WorldServerList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			if ( (*it) == world )
			{
				return true;
			}
		}

		return false;
	}

	return true;
}

bool WorldMgr::IsWorldAllowed(uint16 world)
{
	for ( WorldServerMap::const_iterator itr = this->world_server_map.begin(); itr != this->world_server_map.end(); ++itr )
	{
		for ( WorldServerList::const_iterator it = itr->second.begin(); it != itr->second.end(); ++it )
		{
			if ( (*it) == world ) // Busco el mapa en la lista
			{
				if ( itr->first == sGameServer->GetServerCode() ) // Solamente puedo moverme si el mapa pertenece al mismo server
					return true;

				return false;
			}
		}
	}

	// El mapa no est� en la lista
	WorldServerMap::const_iterator itr = this->world_server_map.find(sGameServer->GetServerCode());

	if ( itr != this->world_server_map.end() ) // Si el server tiene mapas y no est�n en la lista entonces no me puedo mover
	{
		return false;
	}

	return true;
}

bool WorldMgr::IsWorld(uint16 world)
{
	return this->GetWorld(world) != nullptr;
}

World* WorldMgr::GetWorld(uint16 world)
{
	WorldMap::iterator itr = this->world_map.find(world);

	if ( itr != this->world_map.end() )
	{
		return itr->second;
	}
	else
	{
		return nullptr;
	}
}

bool g_UrukMountainBlockStatus = false;
bool g_FereaBlockStatus = false;

void WorldMgr::UpdateUrukMountainZone(bool apply)
{
	if ( g_UrukMountainBlockStatus == apply )
	{
		return;
	}

	World * pWorld = this->GetWorld(WORLD_URUK_MOUNTAIN);

	if ( !pWorld )
	{
		return;
	}

	for ( int32 i = 0; i < URUK_MOUNTAIN_BLOCK_ZONE; ++i )
	{
		int16 coords[4] = { g_UrukMountainBlock[i].x1, g_UrukMountainBlock[i].y1, g_UrukMountainBlock[i].x2, g_UrukMountainBlock[i].y2 };
		pWorld->ApplyAttribute(coords, URUK_MOUNTAIN_BLOCK_ATTRIBUTE, apply);
	}

	g_UrukMountainBlockStatus = apply;

	this->SendUrukMountainZone();
}
	
void WorldMgr::SendUrukMountainZone()
{
	World * pWorld = this->GetWorld(WORLD_URUK_MOUNTAIN);

	if ( !pWorld )
	{
		return;
	}

	WorldPlayerMap const& player_map = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		this->SendUrukMountainZone(pPlayer);
	}
}
	
void WorldMgr::SendUrukMountainZone(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_URUK_MOUNTAIN )
	{
		return;
	}

	World * pWorld = this->GetWorld(WORLD_URUK_MOUNTAIN);

	if ( !pWorld )
	{
		return;
	}

	int16 coords[4 * URUK_MOUNTAIN_BLOCK_ZONE];

	for ( int32 i = 0; i < URUK_MOUNTAIN_BLOCK_ZONE; ++i )
	{
		coords[(i * 4) + 0] = g_UrukMountainBlock[i].x1;
		coords[(i * 4) + 1] = g_UrukMountainBlock[i].y1;
		coords[(i * 4) + 2] = g_UrukMountainBlock[i].x2;
		coords[(i * 4) + 3] = g_UrukMountainBlock[i].y2;
	}

	pPlayer->SendWorldAttribute(0, URUK_MOUNTAIN_BLOCK_ATTRIBUTE, URUK_MOUNTAIN_BLOCK_ZONE, g_UrukMountainBlockStatus, coords);
}

void WorldMgr::UpdateFereaZone(bool apply)
{
	PlayerSessionMap const& character_map = sObjectMgr->GetAllCharacters();
	for ( PlayerSessionMap::const_iterator itr = character_map.begin(); itr != character_map.end(); ++itr )
	{
		Player* pPlayer = itr->second;

		if ( !Player::IsPlayerBasicState(pPlayer, false, false) )
		{
			continue;
		}

		if ( pPlayer->GetWorldId() != WORLD_FEREA )
		{
			continue;
		}

		if ( (apply && pPlayer->GetWorldState() == 1) || (!apply && pPlayer->GetWorldState() == 0) )
		{
			continue;
		}

		pPlayer->SetWorldState(apply ? 1 : 0);
		FEREA_DOOR_OPEN_CLOSE pMsg(apply ? 1 : 0);
		pPlayer->SEND_PCT(pMsg);
	}

	if ( g_FereaBlockStatus == apply )
	{
		return;
	}

	World * pWorld = this->GetWorld(WORLD_FEREA);

	if ( !pWorld )
	{
		return;
	}

	for ( int32 i = 0; i < FEREA_BLOCK_ZONE; ++i )
	{
		int16 coords[4] = { g_FereaBlock[i].x1, g_FereaBlock[i].y1, g_FereaBlock[i].x2, g_FereaBlock[i].y2 };
		pWorld->ApplyAttribute(coords, FEREA_BLOCK_ATTRIBUTE, apply);
	}

	g_FereaBlockStatus = apply;

	this->SendFereaZone();
}

void WorldMgr::SendFereaZone()
{
	World * pWorld = this->GetWorld(WORLD_FEREA);

	if ( !pWorld )
	{
		return;
	}

	WorldPlayerMap const& player_map = pWorld->GetAllPlayers();
	for ( WorldPlayerMap::const_iterator it = player_map.begin(); it != player_map.end(); ++it )
	{
		Player* pPlayer = it->second;

		if ( !pPlayer )
		{
			continue;
		}

		this->SendFereaZone(pPlayer);
	}
}

void WorldMgr::SendFereaZone(Player* pPlayer)
{
	if ( !pPlayer )
	{
		return;
	}

	if ( pPlayer->GetWorldId() != WORLD_FEREA )
	{
		return;
	}

	World * pWorld = this->GetWorld(WORLD_FEREA);

	if ( !pWorld )
	{
		return;
	}

	int16 coords[4 * FEREA_BLOCK_ZONE];

	for ( int32 i = 0; i < FEREA_BLOCK_ZONE; ++i )
	{
		coords[(i * 4) + 0] = g_FereaBlock[i].x1;
		coords[(i * 4) + 1] = g_FereaBlock[i].y1;
		coords[(i * 4) + 2] = g_FereaBlock[i].x2;
		coords[(i * 4) + 3] = g_FereaBlock[i].y2;
	}

	pPlayer->SendWorldAttribute(0, FEREA_BLOCK_ATTRIBUTE, FEREA_BLOCK_ZONE, g_FereaBlockStatus, coords);
}

int32 WorldMgr::GetWorldExperienceRate(uint16 world, uint8 count)
{
	if ( count == 0 )
	{
		return 0;
	}

	count = count - 1;

	if ( count >= MAX_PARTY_MEMBERS )
	{
		return 0;
	}
	
	WorldExperienceMap::const_iterator it = this->world_experience_map.find(world);

	if ( it != this->world_experience_map.end() )
	{
		return it->second->GetRate(count);
	}
	else
	{
		return 0;
	}
}

void WorldMgr::SetPKBoss(uint16 world, bool enabled)
{
	World * pWorld = this->GetWorld(world);

	if ( !pWorld )
	{
		return;
	}

	pWorld->SetPKBossEnabled(enabled);
	pWorld->GetPKBossTime()->Start();
}

bool WorldMgr::IsPKBoss(uint16 world)
{
	if ( !sGameServer->IsPKBossEnabled() )
	{
		return false;
	}

	World * pWorld = this->GetWorld(world);

	if ( !pWorld )
	{
		return false;
	}

	if ( pWorld->flagHas(WORLD_FLAG_NO_LIMIT_PK) )
	{
		return true;
	}

	return pWorld->IsPKBossEnabled();
}

bool WorldMgr::IsItemDropAllowed(Player* pPlayer, World* pWorld, int16 x, int16 y)
{
	if ( !pPlayer || !pWorld )
	{
		return false;
	}

	WorldGrid const& attr = pWorld->GetGrid(x, y);

	if ( attr.IsLocked_1() || attr.IsLocked_2() )
	{
		return false;
	}

	if ( pWorld->IsAreaRestriction(x, y, WORLD_AREA_FLAG_ITEM_DROP) )
	{
		pPlayer->SendNotice(CUSTOM_MESSAGE_ID_RED, "You are not allowed to drop items on this area.");
		return false;
	}

	return true;
}

bool WorldMgr::IsFreePK(uint16 world, int16 x, int16 y) const
{
	for (WorldFreePKList::const_iterator itr = this->m_WorldFreePKList.begin(); itr != this->m_WorldFreePKList.end(); ++itr)
	{
		WorldFreePK const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetWorld() != world)
		{
			continue;
		}

		if ((x >= pData->GetX1() && x <= pData->GetX2()) &&
			(y >= pData->GetY1() && y <= pData->GetY2()))
		{
			return true;
		}
	}

	return false;
}

void WorldMgr::LoadBarrackIni() {
	if (boost::filesystem::exists(MOVEMAP_FILEPATH)) {
		sGameServer->SetBarracksMoveEnabled(GetPrivateProfileInt(MOVEMAP_SECTION_BARRACK, "MoveEnabled", 0, MOVEMAP_FILEPATH));
		sGameServer->SetBarracksMoveMinLevel(GetPrivateProfileInt(MOVEMAP_SECTION_BARRACK, "MoveMinLevel", 0, MOVEMAP_FILEPATH));
		sGameServer->SetBarracksMoveCost(GetPrivateProfileInt(MOVEMAP_SECTION_BARRACK, "MoveCost", 0, MOVEMAP_FILEPATH));
		sGameServer->SetBarracksMoveParty(GetPrivateProfileInt(MOVEMAP_SECTION_BARRACK, "MoveParty", 0, MOVEMAP_FILEPATH));

		sGameServer->SetRefugeMoveEnabled(GetPrivateProfileInt(MOVEMAP_SECTION_REFUGE, "MoveEnabled", 0, MOVEMAP_FILEPATH));
		sGameServer->SetRefugeMoveMinLevel(GetPrivateProfileInt(MOVEMAP_SECTION_REFUGE, "MoveMinLevel", 0, MOVEMAP_FILEPATH));
		sGameServer->SetRefugeMoveCost(GetPrivateProfileInt(MOVEMAP_SECTION_REFUGE, "MoveCost", 0, MOVEMAP_FILEPATH));
		sGameServer->SetRefugeMoveParty(GetPrivateProfileInt(MOVEMAP_SECTION_REFUGE, "MoveParty", 0, MOVEMAP_FILEPATH));

		sGameServer->SetSpecialMapPVP(GetPrivateProfileInt(MOVEMAP_SECTION_SPECIAL_MAP, "AllowPVP", 0, MOVEMAP_FILEPATH));
		sGameServer->SetSpecialMapCharacterCount(GetPrivateProfileInt(MOVEMAP_SECTION_SPECIAL_MAP, "CharacterCount", 0, MOVEMAP_FILEPATH));

	}
	else {
		sLog->outError(LOG_DEFAULT, "Error Load Barrack.ini File");
		MessageBox(NULL, "Error Load Barrack.ini File", "Error", ERROR);
		exit(0);
	}

}