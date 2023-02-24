#pragma once
#include "rapidxml_utils.hpp"
#include "../Resource/Tmx/TmxObj.h"
#include "../Component/Animation.h"
#include "AnimationData.h"

#define lpAnimMng AnimMng::GetInstance()

class AnimMng
{
public:
	static AnimMng& GetInstance()
	{
		static AnimMng s_Instance;
		return s_Instance;
	}

	// �ǂݍ���
	bool Load(const std::string& fname, const std::string& key);
	// �擾
	const AnimationData& GetDate(const std::string& key, const std::string& state)const;

	
private:
	rapidxml::xml_document<> xmldoc_;

	// �A�j���[�V�����̃f�[�^�p
	std::map<std::string, std::map<std::string, AnimationData>> dataMap_;
};

