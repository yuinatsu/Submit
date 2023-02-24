#pragma once
#include <string>
#include <list>
#include <map>
#include <utility>
#include "../Object/Pawn.h"
#include "../Input/INPUT_ID.h"
#include "../XML/RapidXml/rapidxml.hpp"
#include "../XML/RapidXml/rapidxml_utils.hpp"

namespace
{
	std::map<std::string, CMD_ID> chengeData
	{
		{"1",CMD_ID::LEFT_D_D},
		{"2",CMD_ID::DOWN},
		{"3",CMD_ID::RIGHT_D_D},
		{"4",CMD_ID::LEFT},
		{"5",CMD_ID::NEUTRAL},
		{"6",CMD_ID::RIGHT},
		{"7",CMD_ID::LEFT_U_D},
		{"8",CMD_ID::UP},
		{"9",CMD_ID::RIGHT_U_D},
		{"SWT1",CMD_ID::SWT1},
		{"SWT2",CMD_ID::SWT2},
		{"SWT3",CMD_ID::SWT3},
	};
}

// @@nullptrチェックも何もしてない
struct LoadCommad
{
	std::list<CommandData> operator()(const std::string filepath)
	{
		std::list<CommandData> reComList;
		rapidxml::file<> file(filepath.c_str());
		TmxDoc_.parse<0>(file.data());
		tmxNode_ = TmxDoc_.first_node("char");
		for (auto comNode = tmxNode_->first_node("data"); comNode != nullptr; comNode = comNode->next_sibling("data"))
		{
			CommandData commandData;
			commandData.name_ = comNode->first_attribute("name")->value();
			commandData.allTime_ = std::atoi(comNode->first_attribute("limit")->value());
			for (auto node = comNode->first_node("cnd"); node != nullptr; node = node->next_sibling("cnd"))
			{
				std::string check = node->first_attribute("flag")->value();
				auto required = check == "true" ? true : false;
				CommandData::command tmp = {};
				tmp.id = chengeData[node->first_attribute("key")->value()];
				tmp.time = std::atoi(node->first_attribute("time")->value());
				tmp.required_ = required;
				commandData.command_.push_back(tmp);
			}
			reComList.push_back(commandData);
		}
		return reComList;
	}
private:
	rapidxml::xml_node<>* tmxNode_;
	rapidxml::xml_document<> TmxDoc_;
};
