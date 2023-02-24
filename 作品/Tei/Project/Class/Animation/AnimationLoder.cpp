#include <cassert>
#include <type_traits>
#include "AnimationLoder.h"
#include "../XML/RapidXml/rapidxml.hpp"
#include "../XML/RapidXml/rapidxml_utils.hpp"


AnimationLoder::AnimationLoder()
{
	VersionMap();
	tmxNode_ = nullptr;
}

bool AnimationLoder::LoadAnimTmx(const std::string& filepath)
{
	// Tmxä÷òAèÄîı
	rapidxml::file<> file(filepath.c_str());
	TmxDoc_.parse<0>(file.data());

	tmxNode_ = TmxDoc_.first_node("anim");
	if (!tmxNode_)
	{
		//assert(!"TmxÇÃfirst_node(anim)Ç™Ç»Ç¢");
		return false;
	}
	// versitonëŒâûÉ`ÉFÉbÉN
	if (version_.count(tmxNode_->first_attribute("version")->value()) == 0)
	{
		//assert(!"AnimVersionñ¢ëŒâû");
		return false;
	}
	Char_Anim_ID animid = begin(Char_Anim_ID());
	for (rapidxml::xml_node<>* animNode = tmxNode_->first_node("char")->first_node("move");animNode != nullptr && animid != Char_Anim_ID::MAX; animNode = animNode->next_sibling())
	{
		animID_[animid] = animNode->first_attribute("key")->value();
		info_[animID_[animid]].width = animNode->first_attribute("width")->value();
		info_[animID_[animid]].height = animNode->first_attribute("height")->value();
		info_[animID_[animid]].widthCnt = animNode->first_attribute("chipCntW")->value();
		info_[animID_[animid]].heightCnt = animNode->first_attribute("chipCntH")->value();
		info_[animID_[animid]].loop = animNode->first_attribute("loop")->value();
		info_[animID_[animid]].image = animNode->first_attribute("image")->value();

		auto flamedata = animNode->first_node("data");
		if (!flamedata)
		{
			//assert(!"flamedataNodeÇ™Ç†ÇËÇ‹ÇπÇÒ");
			return false;
		}
		animDataS_[animID_[animid]].flameData = flamedata->first_node()->value();
		auto subscriptdata = animNode->first_node("limit");
		if (!subscriptdata)
		{
			//assert(!"subscriptdataNodeÇ™Ç†ÇËÇ‹ÇπÇÒ");
			return false;
		}
		animDataS_[animID_[animid]].subscriptData = subscriptdata->first_node()->value();
		++animid;
	}
	return true;
}

const std::map<Char_Anim_ID, std::string>& AnimationLoder::GetAnimID(void)
{
	return animID_;
}

const AnimDataS& AnimationLoder::GetAnimDataS(Char_Anim_ID animID)
{
	if (animDataS_.count(animID_[animID]) == 0)
	{
		//assert(!"ñ¢ìoò^animDataS");
	}
	return animDataS_[animID_[animID]];
}

const AnimInfoS& AnimationLoder::GetAnimInfoS(Char_Anim_ID animID)
{
	if (info_.count(animID_[animID]) == 0)
	{
		//assert(!"ñ¢ìoò^animInfoS");
	}
	return info_[animID_[animID]];
}

void AnimationLoder::VersionMap(void)
{
	version_["1.0"] = 1;
}

