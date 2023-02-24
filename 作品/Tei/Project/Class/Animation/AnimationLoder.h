#pragma once
#include <map>
#include <vector>
#include <utility>
#include <string>
#include "../XML/RapidXml/rapidxml.hpp"
#include "../Object/characterID.h"
#include "Anim_ID.h"


struct AnimDataS
{
	std::string flameData;
	std::string subscriptData;
};

struct AnimInfoS
{
	std::string width = "";
	std::string height = "";
	std::string widthCnt = "";
	std::string heightCnt = "";
	std::string loop = "";
	std::string image = "";
};

/// <summary>
/// �A�j���[�V������TMX�ǂݍ��݃N���X
/// std::string�ŏ��͎����Ă�
/// </summary>
class AnimationLoder
{
public:
	AnimationLoder();
	~AnimationLoder() = default;
	bool LoadAnimTmx(const std::string& filepath);


	const std::map<Char_Anim_ID, std::string>& GetAnimID(void);
	const AnimInfoS& GetAnimInfoS(Char_Anim_ID animID);
	const AnimDataS& GetAnimDataS(Char_Anim_ID animid);
private:
	void VersionMap(void);

	rapidxml::xml_document<> TmxDoc_;

	rapidxml::xml_node<>* tmxNode_;		// ��ԍŏ��̃m�[�h ��{�I�ɂ�������A�N�Z�X����

	std::map<std::string, int> version_;

	std::map<Char_Anim_ID, std::string> animID_;
	// animation���Ƃ̉摜�֌W�f�[�^
	std::map<std::string, AnimInfoS> info_;
	// std::map<animID,AnimInfo>>
	std::map<std::string, AnimDataS> animDataS_;
};

