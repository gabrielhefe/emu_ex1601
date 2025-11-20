/*
*
* Copyright (C) 2008-2017 Dimension Gamers <http://www.dimensiongamers.net>
*
* File: "SkillMgr.cpp"
*
*/

bool skill_template::IsValidStatus(uint8 status) const
{
	if ( this->GetStatus(0) == 0 && this->GetStatus(1) == 0 && this->GetStatus(2) == 0 )
		return true;

	return status == GUILD_RANK_MASTER && this->GetStatus(0) == 1 ||
		   status == GUILD_RANK_ASISTANT && this->GetStatus(1) == 1 ||
		   status == GUILD_RANK_BATTLE_MASTER && this->GetStatus(2) == 1;
}

float skill_tree_majestic_stat::GetValue(uint8 level)
{
	for (int32 i = 0; i < MAX_SKILL_TREE_MAJESTIC_STAT_GROUP; ++i)
	{
		if (level >= this->GetStartPoint(i) && level <= this->GetEndPoint(i))
		{
			return sFormulaMgr->GetOrCreateValue(3, this->GetFormulaID(i), level);
		}
	}

	return 0;
}

float skill_tree_majestic::GetValue(uint8 level)
{
	return sFormulaMgr->GetOrCreateValue(3, this->GetFormulaID(), level);
}

float SkillTree::GetValue(uint8 level)
{
	return sFormulaMgr->GetOrCreateValue(3, this->GetFormulaID(), level);
}

SkillMgr::SkillMgr()
{
}

SkillMgr::~SkillMgr()
{
	MAP_CLEAR(SkillTemplateMap::iterator, this->skill_template_map);
	LIST_CLEAR(SkillTreeList::iterator, this->m_SkillTreeList);
	MAP_CLEAR(BuffTemplateMap::iterator, this->buff_template_map);
	MAP_CLEAR(SkillSpecialMap::iterator, this->skill_special_map);
	MAP_CLEAR(SkillSpecialElementMap::iterator, this->skill_special_element_map);
	LIST_CLEAR(SkillTreeMajesticList::iterator, this->skill_tree_majestic_list);
	LIST_CLEAR(SkillTreeMajesticStatList::iterator, this->skill_tree_majestic_stat_list);
	LIST_CLEAR(SkillAttackTimeList::iterator, this->m_SkillAttackTimeList);
}

void SkillMgr::LoadSkillTemplate(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Template...");

	MAP_CLEAR(SkillTemplateMap::iterator, this->skill_template_map);
	
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillList");

	uint32 count = 0;

	for (pugi::xml_node skill = Main.child("Skill"); skill; skill = skill.next_sibling()) {
		if (!skill.attribute("Disable").as_bool()) {
			skill_template* add_skill = new skill_template;

			add_skill->SetID(skill.attribute("ID").as_int());
			add_skill->SetName(skill.attribute("Name").as_string());
			add_skill->level.set(skill.attribute("Level").as_int());
			add_skill->SetDamage(skill.attribute("Damage").as_int());
			add_skill->mana_cost.set(skill.attribute("ManaCost").as_int());
			add_skill->stamina_cost.set(skill.attribute("StaminaCost").as_int());
			add_skill->SetEffectRange(skill.attribute("Distance").as_int());
			add_skill->SetEffectRadio(skill.attribute("EffectRadio").as_int());
			add_skill->SetCooldown(skill.attribute("Delay").as_int());
			add_skill->SetType(skill.attribute("Type").as_int());
			add_skill->SetAttribute(skill.attribute("Attribute").as_int());
			add_skill->SetBuffIcon(skill.attribute("BuffIcon").as_int());
			add_skill->SetRequiredLevel(skill.attribute("ReqLevel").as_int());

			add_skill->SetRequiredStat(0, skill.attribute("ReqStrength").as_int());
			add_skill->SetRequiredStat(1, skill.attribute("RegAgility").as_int());
			add_skill->SetRequiredStat(2, skill.attribute("ReqVitality").as_int());
			add_skill->SetRequiredStat(3, skill.attribute("ReqEnergy").as_int());
			add_skill->SetRequiredStat(4, skill.attribute("ReqLeaderShip").as_int());

			add_skill->SetRequiredKillCount(skill.attribute("ReqKillCount").as_int());
			add_skill->SetStatus(0, skill.attribute("Status1").as_int());
			add_skill->SetStatus(1, skill.attribute("Status2").as_int());
			add_skill->SetStatus(2, skill.attribute("Status3").as_int());
			add_skill->SetBaseSkill(skill.attribute("BaseSkill").as_int());

			add_skill->SetRequiredClass(0, skill.attribute("AllowDW").as_int());
			add_skill->SetRequiredClass(1, skill.attribute("AllowDK").as_int());
			add_skill->SetRequiredClass(2, skill.attribute("AllowElf").as_int());
			add_skill->SetRequiredClass(3, skill.attribute("AllowMG").as_int());
			add_skill->SetRequiredClass(4, skill.attribute("AllowDL").as_int());
			add_skill->SetRequiredClass(5, skill.attribute("AllowSum").as_int());
			add_skill->SetRequiredClass(6, skill.attribute("AllowRF").as_int());
			add_skill->SetRequiredClass(7, skill.attribute("AllowGL").as_int());
			add_skill->SetRequiredClass(8, skill.attribute("AllowRW").as_int());
			add_skill->SetRequiredClass(9, skill.attribute("AllowSL").as_int());
			add_skill->SetRequiredClass(10, skill.attribute("AllowGC").as_int());

			add_skill->animation.set(skill.attribute("Animation").as_int());
			add_skill->SetElementalAttribute(skill.attribute("ElementAttribute").as_int());
			add_skill->SetElementalBonusDamage(skill.attribute("ElementBonusDamage").as_int());
			add_skill->SetAttackDelay(skill.attribute("AttackDelay").as_int());
			add_skill->SetMountCheck(skill.attribute("MountCheck").as_int());
			add_skill->SetPVMDamage(skill.attribute("PVMDamage").as_int());
			add_skill->SetPVPDamage(skill.attribute("PVPDamage").as_int());
			add_skill->SetStat1(skill.attribute("Stat1").as_int());
			add_skill->SetStatValue1(skill.attribute("Stat1Value").as_int());
			add_skill->SetStat2(skill.attribute("Stat2").as_int());
			add_skill->SetStatValue2(skill.attribute("Stat2Value").as_int());

			switch (add_skill->GetStat1())
			{
			case 1:
			{
				add_skill->SetStat1(STRENGTH);
			}break;

			case 2:
			{
				add_skill->SetStat1(AGILITY);
			}break;

			case 3:
			{
				add_skill->SetStat1(ENERGY);
			}break;

			case 4:
			{
				add_skill->SetStat1(VITALITY);
			}break;

			default:
			{
				add_skill->SetStat1(MAX_STAT_TYPE);
				add_skill->SetStatValue1(0);
			}break;
			}

			switch (add_skill->GetStat2())
			{
			case 1:
			{
				add_skill->SetStat2(STRENGTH);
			}break;

			case 2:
			{
				add_skill->SetStat2(AGILITY);
			}break;

			case 3:
			{
				add_skill->SetStat2(ENERGY);
			}break;

			case 4:
			{
				add_skill->SetStat2(VITALITY);
			}break;

			default:
			{
				add_skill->SetStat2(MAX_STAT_TYPE);
				add_skill->SetStatValue2(0);
			}break;
			}

			add_skill->SetLearnItem(-1);
			add_skill->SetLearnItemLevel(0);
			add_skill->SetRequireSkill(0);
			add_skill->SetRequireMasterSkill(0);
			add_skill->SetRequireMasterLevel(0);

			this->skill_template_map[add_skill->GetID()] = add_skill;

			count++;
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillLearn(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Learn...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillLearn");

	uint32 count = 0;

	for (pugi::xml_node option = Main.child("Item"); option; option = option.next_sibling()) {
		uint16 item = ITEMGET(option.attribute("ItemCat").as_int(), option.attribute("ItemIndex").as_int());
		uint8 level = option.attribute("ItemLevel").as_int();

		if (!sItemMgr->IsItem(item))
		{
			sLog->outError(LOG_DEFAULT, "LoadSkillLearn() item: %u +%u", item, level);
			continue;
		}

		uint16 skill = option.attribute("SkillID").as_int();

		skill_template* skill_info = this->GetSkill(skill);

		if (!skill_info)
		{
			sLog->outError(LOG_DEFAULT, "LoadSkillLearn() skill: %u", skill);
			continue;
		}

		skill_info->SetLearnItem(item);
		skill_info->SetLearnItemLevel(level);
		count++;
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill learn definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillTree(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Tree...");

	LIST_CLEAR(SkillTreeList::iterator, this->m_SkillTreeList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillTree");

	uint32 count = 0;

	for (pugi::xml_node tree = Main.child("Tree"); tree; tree = tree.next_sibling()) {
		SkillTree* add_skill = new SkillTree;

		add_skill->SetID(tree.attribute("ID").as_int());
		add_skill->SetClass(tree.attribute("Class").as_int());
		add_skill->SetGroup(tree.attribute("Group").as_int());
		add_skill->SetRequiredPoints(tree.attribute("ReqPoints").as_int());
		add_skill->SetMaxLevel(tree.attribute("MaxLevel").as_int());
		add_skill->SetLink(tree.attribute("Link").as_int());
		add_skill->SetRequiredSkill(0, tree.attribute("ReqSkill1").as_int());
		add_skill->SetRequiredSkillPoints(0, tree.attribute("ReqPoints1").as_int());
		add_skill->SetRequiredSkill(1, tree.attribute("ReqSkill2").as_int());
		add_skill->SetRequiredSkillPoints(1, tree.attribute("ReqPoints2").as_int());
		add_skill->SetSkill(tree.attribute("Skill").as_int());
		add_skill->SetRelatedSkill(tree.attribute("RelatedSkill").as_int());
		add_skill->SetReplaceSkill(tree.attribute("ReplaceSkill").as_int());
		add_skill->SetFormulaID(tree.attribute("FormulaData").as_int());
		add_skill->SetUnk5(tree.attribute("Unk5").as_int());
		add_skill->SetUnk6(tree.attribute("Unk6").as_int());
		add_skill->SetUnk7(tree.attribute("Unk7").as_int());
		add_skill->SetType(tree.attribute("Type").as_int());

		this->m_SkillTreeList.push_back(add_skill);

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill tree", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillSpecial(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Special...");

	MAP_CLEAR(SkillSpecialMap::iterator, this->skill_special_map);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillSpecial");

	uint32 count = 0;

	for (pugi::xml_node skill = Main.child("Skill"); skill; skill = skill.next_sibling()) {
		skill_special* add_skill = new skill_special;

		add_skill->guid.set(skill.attribute("ID").as_int());
		add_skill->SetName(skill.attribute("Name").as_string());
		add_skill->target.set(skill.attribute("Target").as_int());
		add_skill->scope_type.set(skill.attribute("ScopeType").as_int());
		add_skill->scope_angle.set(skill.attribute("ScopeAngle").as_int());
		add_skill->scope_value.set(skill.attribute("ScopeValue").as_int());

		add_skill->element[0].set(skill.attribute("Element1").as_int());
		add_skill->SetElement(0, this->GetSkillSpecialElement(skill.attribute("Element1").as_int()));
		add_skill->element[1].set(skill.attribute("Element2").as_int());
		add_skill->SetElement(1, this->GetSkillSpecialElement(skill.attribute("Element2").as_int()));
		add_skill->element[2].set(skill.attribute("Element3").as_int());
		add_skill->SetElement(2, this->GetSkillSpecialElement(skill.attribute("Element3").as_int()));
		add_skill->element[3].set(skill.attribute("Element4").as_int());
		add_skill->SetElement(3, this->GetSkillSpecialElement(skill.attribute("Element4").as_int()));
		add_skill->element[4].set(skill.attribute("Element5").as_int());
		add_skill->SetElement(4, this->GetSkillSpecialElement(skill.attribute("Element5").as_int()));


		this->skill_special_map[add_skill->guid.get()] = add_skill;
		count++;
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill special definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillSpecialElement(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Special Element...");

	MAP_CLEAR(SkillSpecialElementMap::iterator, this->skill_special_element_map);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillSpecialElement");

	uint32 count = 0;

	for (pugi::xml_node skill = Main.child("Skill"); skill; skill = skill.next_sibling()) {
		skill_special_element* add_skill = new skill_special_element;

		add_skill->guid.set(skill.attribute("ID").as_int());
		add_skill->SetName(skill.attribute("Name").as_string());
		add_skill->type.set(skill.attribute("Type").as_int());
		add_skill->rate.set(skill.attribute("Rate").as_int());
		add_skill->duration.set(skill.attribute("Duration").as_int());
		add_skill->calc_type.set(skill.attribute("CalcType").as_int());
		add_skill->calc_value.set(skill.attribute("CalcValue").as_int());
		add_skill->magic.set(skill.attribute("Magic").as_int());

		this->skill_special_element_map[add_skill->guid.get()] = add_skill;
		count++;
	}
	/*QueryResult result = GameServerDatabase.Query("SELECT * FROM skill_special_element");

	if ( result )
	{
		do
		{
			Field* fields = result->Fetch();

			skill_special_element * add_skill = new skill_special_element;

			add_skill->guid.set(fields[0].GetUInt32());
			add_skill->SetName(fields[1].GetString());
			add_skill->type.set(fields[2].GetUInt8());
			add_skill->rate.set(fields[3].GetUInt8());
			add_skill->duration.set(fields[4].GetInt32());
			add_skill->calc_type.set(fields[5].GetUInt8());
			add_skill->calc_value.set(fields[6].GetInt32());
			add_skill->magic.set(fields[7].GetInt32());

			this->skill_special_element_map[add_skill->guid.get()] = add_skill;
			count++;
		}
		while(result->NextRow());
	}*/
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill special element definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadBuffTemplate(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Buff Template...");

	MAP_CLEAR(BuffTemplateMap::iterator, this->buff_template_map);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("BuffTemplate");

	uint32 count = 0;

	for (pugi::xml_node buff = Main.child("Buff"); buff; buff = buff.next_sibling()) {
		buff_template* add_buff = new buff_template;

		add_buff->SetBuff(buff.attribute("ID").as_int());
		add_buff->SetGroup(buff.attribute("Group").as_int());
		add_buff->SetItem(ITEMGET(buff.attribute("ItemCat").as_int(), buff.attribute("ItemIndex").as_int()));
		add_buff->SetName(buff.attribute("Name").as_string());
		add_buff->SetDescription(buff.attribute("Description").as_string());
		add_buff->SetSend(buff.attribute("Send").as_bool());
		add_buff->SetDebuff(buff.attribute("Debuff").as_bool());
		add_buff->SetEffect(0, buff.attribute("Effect1").as_int());
		add_buff->SetValue(0, buff.attribute("Value1").as_int());
		add_buff->SetEffect(1, buff.attribute("Effect2").as_int());
		add_buff->SetValue(1, buff.attribute("Value2").as_int());
		add_buff->SetDuration(buff.attribute("Duration").as_int());

		this->buff_template_map[add_buff->GetBuff()] = add_buff;

		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u buff definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillRequire(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Require...");

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillRequire");

	uint32 count = 0;

	for (pugi::xml_node req = Main.child("Skill"); req; req = req.next_sibling()) {
		uint16 skill = req.attribute("ID").as_int();

		skill_template* skill_info = this->GetSkill(skill);

		if (!skill_info)
		{
			sLog->outError(LOG_DEFAULT, "LoadSkillRequire() skill: %u", skill);
			continue;
		}

		skill_info->SetRequireSkill(req.attribute("ReqSkill").as_int());
		skill_info->SetRequireMasterSkill(req.attribute("ReqMasterSkill").as_int());
		skill_info->SetRequireMasterLevel(req.attribute("ReqMasterLevel").as_int());
		count++;
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill require definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillTreeMajestic(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Tree Majestic...");

	LIST_CLEAR(SkillTreeMajesticList::iterator, this->skill_tree_majestic_list);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillTreeMajestic");

	uint32 count = 0;

	for (pugi::xml_node info = Main.child("Info"); info; info = info.next_sibling()) {
		for(pugi::xml_node skill = info.child("Skill"); skill; skill=skill.next_sibling()){
			if (!skill.attribute("Disable").as_bool()) {
				skill_tree_majestic* add_data = new skill_tree_majestic;
				add_data->SetClass(info.attribute("Class").as_int());
				add_data->SetID(skill.attribute("ID").as_int());
				add_data->SetSection(skill.attribute("Section").as_int());
				add_data->SetUIGroup(skill.attribute("UIGroup").as_int());
				add_data->SetPositionTemp(skill.attribute("PositionTemp").as_int());
				add_data->SetSkillID(skill.attribute("SkillID").as_int());
				add_data->SetBuffID(skill.attribute("BuffID").as_int());
				add_data->SetRank(skill.attribute("Rank").as_int());
				add_data->SetFormulaID(skill.attribute("FormulaData").as_int());

				add_data->SetLinkSlot(0, skill.attribute("LinkSlot1").as_int());
				add_data->SetLinkLine(0, skill.attribute("LinkLine1").as_int());
				add_data->SetLinkSlot(1, skill.attribute("LinkSlot2").as_int());
				add_data->SetLinkLine(1, skill.attribute("LinkLine2").as_int());
				add_data->SetLinkSlot(2, skill.attribute("LinkSlot3").as_int());
				add_data->SetLinkLine(2, skill.attribute("LinkLine3").as_int());
				add_data->SetLinkSlot(3, skill.attribute("LinkSlot4").as_int());
				add_data->SetLinkLine(3, skill.attribute("LinkLine4").as_int());
				add_data->SetLinkSlot(4, skill.attribute("LinkSlot5").as_int());
				add_data->SetLinkLine(4, skill.attribute("LinkLine5").as_int());

				add_data->SetRequiredPoints(skill.attribute("ReqPoints").as_int());
				add_data->SetMaxPoints(skill.attribute("MaxPoints").as_int());
				add_data->SetActiveNextPoints(skill.attribute("ActiveNextPoints").as_int());
				add_data->SetParentSkill1(skill.attribute("ParentSkill1").as_int());
				add_data->SetParentSkillPoints1(skill.attribute("ParentSkillPoints1").as_int());
				add_data->SetParentSkill2(skill.attribute("ParentSkill2").as_int());
				add_data->SetParentSkillPoints2(skill.attribute("ParentSkillPoints2").as_int());
				add_data->SetParentSkill3(skill.attribute("ParentSkill3").as_int());
				add_data->SetParentSkillPoints3(skill.attribute("ParentSkillPoints3").as_int());

				add_data->SetParentID1(-1);
				add_data->SetParentID2(-1);
				add_data->SetParentID3(-1);

				this->skill_tree_majestic_list.push_back(add_data);
				count++;
			}
		}
	}

	for ( SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr )
	{
		skill_tree_majestic * pSkill = *itr;

		if ( !pSkill )
		{
			continue;
		}

		for ( SkillTreeMajesticList::const_iterator it = this->skill_tree_majestic_list.begin(); it != this->skill_tree_majestic_list.end(); ++it )
		{
			skill_tree_majestic const* pSkill2 = *it;

			if ( !pSkill2 )
			{
				continue;
			}

			if ( pSkill->GetClass() == pSkill2->GetClass() &&
				 pSkill->GetSection() == pSkill2->GetSection() &&
				 pSkill->GetID() == pSkill2->GetID() )
			{
				continue;
			}

			if ( pSkill->GetSection() != pSkill2->GetSection() )
			{
				continue;
			}

			if (pSkill->GetClass() != pSkill2->GetClass())
			{
				continue;
			}

			if (pSkill->GetParentSkill2() != uint16(-1))
			{
				if (pSkill->GetParentSkill2() == pSkill2->GetSkillID())
				{
					pSkill->SetParentID2(pSkill2->GetID());
				}
			}

			if (pSkill->GetParentSkill3() != uint16(-1))
			{
				if (pSkill->GetParentSkill3() == pSkill2->GetSkillID())
				{
					pSkill->SetParentID3(pSkill2->GetID());
				}
			}
		}
	}
	
	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill tree majestic definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillTreeMajesticStat(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Tree Majestic Stat...");

	LIST_CLEAR(SkillTreeMajesticStatList::iterator, this->skill_tree_majestic_stat_list);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillTreeMajesticStat");

	uint32 count = 0;

	for (pugi::xml_node info = Main.child("Info"); info; info = info.next_sibling()) {
		for (pugi::xml_node skill = info.child("Skill"); skill; skill = skill.next_sibling()) {
			skill_tree_majestic_stat* add_data = new skill_tree_majestic_stat;
			add_data->SetClass(info.attribute("Class").as_int());
			add_data->SetSection(skill.attribute("Section").as_int());
			add_data->SetID(skill.attribute("ID").as_int());
			add_data->SetSkillID(skill.attribute("SkillID").as_int());

			add_data->SetPointCondition(0, skill.attribute("PointCondition1").as_int());
			add_data->SetRequiredPoints(0, skill.attribute("ReqPoints1").as_int());
			add_data->SetMaxPoints(0, skill.attribute("MaxPoints1").as_int());
			add_data->SetPointCondition(1, skill.attribute("PointCondition2").as_int());
			add_data->SetRequiredPoints(1, skill.attribute("ReqPoints2").as_int());
			add_data->SetMaxPoints(1, skill.attribute("MaxPoints2").as_int());
			add_data->SetPointCondition(2, skill.attribute("PointCondition3").as_int());
			add_data->SetRequiredPoints(2, skill.attribute("ReqPoints3").as_int());
			add_data->SetMaxPoints(2, skill.attribute("MaxPoints3").as_int());

			add_data->SetStartPoint(0, skill.attribute("StatPoint1").as_int());
			add_data->SetEndPoint(0, skill.attribute("EndPoint1").as_int());
			add_data->SetFormulaID(0, skill.attribute("FormulaID1").as_int());
			add_data->SetStartPoint(1, skill.attribute("StatPoint2").as_int());
			add_data->SetEndPoint(1, skill.attribute("EndPoint2").as_int());
			add_data->SetFormulaID(1, skill.attribute("FormulaID2").as_int());


			this->skill_tree_majestic_stat_list.push_back(add_data);
			count++;
		}
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill tree majestic stat definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

void SkillMgr::LoadSkillAttackTime(char* pchFileName)
{
	sLog->outInfo(LOG_DEFAULT, "Loading Skill Attack Time...");

	LIST_CLEAR(SkillAttackTimeList::iterator, this->m_SkillAttackTimeList);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok) {
		sLog->outError(LOG_DEFAULT, "%s file load fail (%s)", pchFileName, res.description());
	}

	pugi::xml_node Main = file.child("SkillAttackTime");

	uint32 count = 0;

	for (pugi::xml_node skill = Main.child("Skill"); skill; skill = skill.next_sibling()) {
		SkillAttackTime* pData = new SkillAttackTime();
		pData->SetClass(skill.attribute("Class").as_int());
		pData->SetSkill(skill.attribute("SkillID").as_int());
		pData->SetStep(skill.attribute("Step").as_int());
		pData->SetSpeedMin(skill.attribute("SpeedMin").as_int());
		pData->SetSpeedMax(skill.attribute("SpeedMax").as_int());
		pData->SetTime(skill.attribute("Time").as_int());
		pData->SetRealTime(skill.attribute("RealTime").as_int());

		this->m_SkillAttackTimeList.push_back(pData);
		count++;
	}

	sLog->outInfo(LOG_DEFAULT, ">> Loaded %u skill attack time definitions", count);
	sLog->outInfo(LOG_DEFAULT, " ");
}

bool SkillMgr::IsSkill(uint16 skill) const
{
	return this->GetSkill(skill) != nullptr;
}

skill_template * SkillMgr::GetSkill(uint16 skill)
{
	SkillTemplateMap::iterator it = this->skill_template_map.find(skill);

	if ( it != this->skill_template_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

skill_template const* SkillMgr::GetSkill(uint16 skill) const
{
	SkillTemplateMap::const_iterator it = this->skill_template_map.find(skill);

	if ( it != this->skill_template_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

skill_template const* SkillMgr::GetSkillFromItem(uint16 item, uint8 level) const
{
	for ( SkillTemplateMap::const_iterator it = this->skill_template_map.begin(); it != this->skill_template_map.end(); ++it )
	{
		if ( it->second->GetLearnItem() == item &&
			 it->second->GetLearnItemLevel() == level )
		{
			return it->second;
		}
	}

	return nullptr;
}

SkillTree const* SkillMgr::GetSkillTree(uint16 skill, uint8 character) const
{
	for (SkillTreeList::const_iterator itr = this->m_SkillTreeList.begin(); itr != this->m_SkillTreeList.end(); ++itr)
	{
		SkillTree const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetSkill() == skill && pData->GetClass() == character)
		{
			return pData;
		}
	}

	return nullptr;
}

SkillTree * SkillMgr::GetSkillTree(uint16 skill, uint8 character)
{
	for (SkillTreeList::const_iterator itr = this->m_SkillTreeList.begin(); itr != this->m_SkillTreeList.end(); ++itr)
	{
		SkillTree * pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetSkill() == skill && pData->GetClass() == character)
		{
			return pData;
		}
	}

	return nullptr;
}

buff_template const* SkillMgr::GetBuff(uint16 buff) const
{
	BuffTemplateMap::const_iterator it = this->buff_template_map.find(buff);

	if ( it != this->buff_template_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

buff_template const* SkillMgr::GetBuffItem(uint16 item) const
{
	if ( item == uint16(-1) )
	{
		return nullptr;
	}

	for ( BuffTemplateMap::const_iterator it = this->buff_template_map.begin(); it != this->buff_template_map.end(); ++it )
	{
		if ( it->second->GetItem() == item )
		{
			return it->second;
		}
	}

	return nullptr;
}

skill_special const* SkillMgr::GetSkillSpecial(uint16 guid) const
{
	SkillSpecialMap::const_iterator it = this->skill_special_map.find(guid);

	if ( it != this->skill_special_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

skill_special_element const* SkillMgr::GetSkillSpecialElement(uint32 guid) const
{
	SkillSpecialElementMap::const_iterator it = this->skill_special_element_map.find(guid);

	if ( it != this->skill_special_element_map.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

uint8 SkillMgr::GetSkillAttribute(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetAttribute();
	}
	else
	{
		return uint8(-1);
	}
}

uint16 SkillMgr::GetSkillBaseSkill(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetBaseSkill();
	}
	else
	{
		return 0;
	}
}
	
int8 SkillMgr::GetSkillType(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetType();
	}
	else
	{
		return SkillNone;
	}
}

int32 SkillMgr::GetSkillDamage(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetDamage();
	}
	else
	{
		return 0;
	}
}

uint16 SkillMgr::GetSkillBuffIcon(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetBuffIcon();
	}
	else
	{
		return 0;
	}
}

uint8 SkillMgr::GetSkillElementalAttribute(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetElementalAttribute();
	}
	else
	{
		return ELEMENTAL_ATTRIBUTE_NONE;
	}
}
	
uint16 SkillMgr::GetSkillElementalBonus(uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->GetElementalBonusDamage();
	}
	else
	{
		return 0;
	}
}

bool SkillMgr::IsSkillRequiredClass(uint16 skill, uint8 race, uint8 change_up_1, uint8 change_up_2, uint8 change_up_3) const
{
	skill_template const* skill_info = this->GetSkill(skill);

	if ( skill_info != nullptr )
	{
		return skill_info->IsRequiredClass(race, change_up_1, change_up_2, change_up_3);
	}
	else
	{
		return false;
	}
}

bool SkillMgr::IsSkillRequire(Player* pPlayer, uint16 skill) const
{
	skill_template const* skill_info = this->GetSkill(skill);
	int32 count = 0;

	if ( skill_info != nullptr )
	{
		if ( skill_info->GetRequireSkill() == 0 && skill_info->GetRequireMasterSkill() == 0 )
		{
			return true;
		}

		if ( skill_info->GetRequireSkill() != 0 )
		{
			SKILL_LOOP(i)
			{
				if ( !pPlayer->GetSkill(i)->GetSkill() )
				{
					continue;
				}

				if ( this->GetSkillBaseSkill(pPlayer->GetSkill(i)->GetSkill()) == skill_info->GetRequireSkill() )
				{
					if ( count == 0 )
					{
						count++;
					}
				}
			}
		}
		else
		{
			count++;
		}

		if ( skill_info->GetRequireMasterSkill() != 0 )
		{
			Skill* pMasterSkill = pPlayer->GetSkillTreeSkill(skill_info->GetRequireMasterSkill());

			if ( pMasterSkill )
			{
				if ( pMasterSkill->GetLevel() >= skill_info->GetRequireMasterLevel() )
				{
					count++;
				}
			}
		}
		else
		{
			count++;
		}

		if ( count >= 2 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

skill_tree_majestic const* SkillMgr::GetSkillTreeMajestic(uint8 race, uint8 section, uint16 id) const
{
	for ( SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr )
	{
		skill_tree_majestic const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetClass() == race && pData->GetSection() == section && pData->GetID() == id )
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic const* SkillMgr::GetSkillTreeMajestic(uint8 race, uint16 skill) const
{
	for ( SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr )
	{
		skill_tree_majestic const* pData = *itr;

		if ( !pData )
		{
			continue;
		}

		if ( pData->GetClass() == race && pData->GetSkillID() == skill )
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic * SkillMgr::GetSkillTreeMajestic(uint8 race, uint8 section, uint16 id)
{
	for (SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr)
	{
		skill_tree_majestic * pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == race && pData->GetSection() == section && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic * SkillMgr::GetSkillTreeMajestic(uint8 race, uint16 skill)
{
	for (SkillTreeMajesticList::const_iterator itr = this->skill_tree_majestic_list.begin(); itr != this->skill_tree_majestic_list.end(); ++itr)
	{
		skill_tree_majestic * pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == race && pData->GetSkillID() == skill)
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic_stat const* SkillMgr::GetSkillTreeMajesticStat(uint8 race, uint8 section, uint8 id) const
{
	for (SkillTreeMajesticStatList::const_iterator itr = this->skill_tree_majestic_stat_list.begin(); itr != this->skill_tree_majestic_stat_list.end(); ++itr)
	{
		skill_tree_majestic_stat const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == race && pData->GetSection() == section && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

skill_tree_majestic_stat * SkillMgr::GetSkillTreeMajesticStat(uint8 race, uint8 section, uint8 id)
{
	for (SkillTreeMajesticStatList::const_iterator itr = this->skill_tree_majestic_stat_list.begin(); itr != this->skill_tree_majestic_stat_list.end(); ++itr)
	{
		skill_tree_majestic_stat * pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == race && pData->GetSection() == section && pData->GetID() == id)
		{
			return pData;
		}
	}

	return nullptr;
}

SkillAttackTime const* SkillMgr::GetSkillAttackTime(Player* pPlayer, uint16 skill) const
{
	for (SkillAttackTimeList::const_iterator itr = this->m_SkillAttackTimeList.begin(); itr != this->m_SkillAttackTimeList.end(); ++itr)
	{
		SkillAttackTime const* pData = *itr;

		if (!pData)
		{
			continue;
		}

		if (pData->GetClass() == pPlayer->GetClass() && pData->GetSkill() == skill)
		{
			if (pPlayer->GetIntData(UNIT_INT_ATTACK_SPEED) >= pData->GetSpeedMin() && pPlayer->GetIntData(UNIT_INT_ATTACK_SPEED) <= pData->GetSpeedMax())
			{
				return pData;
			}
		}
	}
	
	return nullptr;
}