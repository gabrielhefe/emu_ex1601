SummonScroll::SummonScroll()
{
	
}

SummonScroll::~SummonScroll()
{
	LIST_CLEAR(SummonScrollInfoList::iterator, this->m_SummonScrollInfo);
	LIST_CLEAR(SummonScrollMonsterInfoList::iterator, this->m_SummonScrollMonsterInfo);
}

void SummonScroll::Load(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Summon Scroll...");

	LIST_CLEAR(SummonScrollInfoList::iterator, this->m_SummonScrollInfo);
	LIST_CLEAR(SummonScrollMonsterInfoList::iterator, this->m_SummonScrollMonsterInfo);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SummonScroll");
	pugi::xml_node ScrollList = Main.child("ScrollList");
	pugi::xml_node ScrollMonster = Main.child("ScrollMonster");

	uint32 count = 0;

	for (pugi::xml_node scroll = ScrollList.child("Scroll"); scroll; scroll = scroll.next_sibling()) {
		SummonScrollInfo* info = new SummonScrollInfo;
		info->SetIndex(scroll.attribute("ID").as_int());
		info->SetItem(scroll.attribute("Item").as_int());

		this->m_SummonScrollInfo.push_back(info);
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u summon scroll definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");

	sLog->outInfo(LOG_DEFAULT, "Loading Summon Scroll Monster...");

	count = 0;

	for (pugi::xml_node scroll = ScrollMonster.child("Scroll"); scroll; scroll = scroll.next_sibling()) {
		SummonScrollMonsterInfo* info = new SummonScrollMonsterInfo;
		info->SetIndex(scroll.attribute("ID").as_int());
		info->SetGroup(scroll.attribute("Group").as_int());
		info->SetMonsterClass(scroll.attribute("MonsterID").as_int());
		info->SetCreateRate(scroll.attribute("CreateRate").as_int());

		this->m_SummonScrollMonsterInfo.push_back(info);
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u summon scroll monster definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");

	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM summon_scroll");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();
			int32 i = 0;

			SummonScrollInfo * info = new SummonScrollInfo;

			info->SetIndex(fields[i++].GetUInt16());
			info->SetItem(fields[i++].GetUInt16());

			this->m_SummonScrollInfo.push_back(info);
			count++;
		}
		while(result->NextRow());
	}*/
}

bool SummonScroll::CheckSummonScroll(uint16 item) const
{
	for ( SummonScrollInfoList::const_iterator it = this->m_SummonScrollInfo.begin(); it != this->m_SummonScrollInfo.end(); ++it )
	{
		if ( (*it)->GetItem() == item )
		{
			return true;
		}
	}

	return false;
}

SummonScrollInfo const* SummonScroll::GetSummonScrollInfo(uint16 item) const
{
	for ( SummonScrollInfoList::const_iterator it = this->m_SummonScrollInfo.begin(); it != this->m_SummonScrollInfo.end(); ++it )
	{
		if ( (*it)->GetItem() == item )
		{
			return (*it);
		}
	}

	return nullptr;
}

bool SummonScroll::CreateSummonScrollMonster(Player* pPlayer, uint16 item, uint16 map, int16 x, int16 y)
{
	SummonScrollInfo const* pSummonScrollInfo = this->GetSummonScrollInfo(item);

	if ( !pSummonScrollInfo )
	{
		return false;
	}

	if ( pPlayer->IsInSafeZone() )
	{
		return false;
	}

	World* pWorld = pPlayer->GetWorld();

	if ( !pWorld )
	{
		return false;
	}

	WorldGrid const& grid = pWorld->GetGrid(x, y);

	if ( grid.IsSafe() || grid.IsLocked_1() || grid.IsLocked_2() )
	{
		return false;
	}

	if ( !pWorld->flagHas(WORLD_FLAG_ALLOW_SUMMON_SCROLL) )
	{
		return false;
	}

	bool created = false;

	for( int32 n = 0; n < MAX_SUMMON_SCROLL_MONSTER_GROUP; ++n )
	{
		RandomValue<SummonScrollMonsterInfo const*> m_RandomValue(nullptr);

		for ( SummonScrollMonsterInfoList::const_iterator it = this->m_SummonScrollMonsterInfo.begin(); it != this->m_SummonScrollMonsterInfo.end(); ++it )
		{
			if ( (*it)->GetGroup() != n )
			{
				continue;
			}

			if ( (*it)->GetIndex() == pSummonScrollInfo->GetIndex() )
			{
				m_RandomValue.AddValue(*it, (*it)->GetCreateRate());
			}
		}

		SummonScrollMonsterInfo const* pSummonScrollMonsterInfo = m_RandomValue.GetRandomValue(RANDOM_POOL_RATE);

		if( pSummonScrollMonsterInfo )
		{
			int16 px = x;
			int16 py = y;

			if( !pWorld->GetRandomDropLocation(x, y, px, py, 3, 3, 50) )
			{
				continue;
			}

			Monster * mMonster = sObjectMgr->MonsterTryAdd(pSummonScrollMonsterInfo->GetMonsterClass(), pPlayer->GetWorldId());

			if ( !mMonster )
			{
				continue;
			}

			mMonster->SetWorldId(pPlayer->GetWorldId());
			mMonster->SetBasicLocation(px, py, px, py);
			mMonster->SetInstance(pPlayer->GetInstance());
			mMonster->SetDirection(-1);
			mMonster->SetRespawnType(GAME_OBJECT_RESPAWN_DELETE);
			mMonster->SetDespawnType(MONSTER_DESPAWN_TIME);
			mMonster->SetDespawnTime(30 * MINUTE * IN_MILLISECONDS);
			mMonster->SetMoveDistance(10);
			mMonster->AddToWorld();

			created = true;
		}
	}

	return created;
}