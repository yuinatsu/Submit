#include <cassert>
#include <sstream>
#include <memory>
#include "AnimationMng.h"
#include "../Animation/AnimationLoder.h"
#include "../Mng/ImageMng.h"

bool AnimationMng::LoadAnimTmx(const std::string& filepath, Object_ID id)
{
	// AnimInfoS�̒��g��int�ɕϊ���AnimInfo�֊i�[����
	auto chengeInfoS = [](AnimInfo& info,const AnimInfoS& infoS) {
		info.width = std::atoi(infoS.width.c_str());
		info.height = std::atoi(infoS.height.c_str());
		info.widthCnt = std::atoi(infoS.widthCnt.c_str());
		info.heightCnt = std::atoi(infoS.heightCnt.c_str());
		info.loop = std::atoi(infoS.loop.c_str());
		info.image = infoS.image;
	};
	// .csv�`���̃f�[�^��,���ƂɎ��o��list�֊i�[����
	auto chengeCsvToList = [](std::list<int>& list, const std::string str){
		// ���o�����string�̈ꎞ�ۑ���
		std::string dataStr;
		std::stringstream sstr(str);
		while (std::getline(sstr, dataStr, ','))
		{
			list.push_back(std::atoi(dataStr.c_str()));
		}
	};
	// AnimDataS�̒��g��ϊ���AnimData�֊i�[����
	auto chengeAnimDataS = [chengeCsvToList](AnimData& data, const AnimDataS& dataS) {
		chengeCsvToList(data.flameData, dataS.flameData);
		chengeCsvToList(data.subscript, dataS.subscriptData);
	};

	if (animData_.count(id) != 0)
	{
		// �o�^�ς�ID�͓o�^���Ȃ������Ƀf�[�^�͂���̂�true
		return true;
	}
	std::unique_ptr<AnimationLoder> loder = std::make_unique<AnimationLoder>();
	if (!loder->LoadAnimTmx(filepath))
	{
		//assert(!"AnimTmx�ǂݍ��ݎ��s");
		return false;
	}
	// �ǂݍ���ł����f�[�^�̊i�[
	for (const auto& animid : loder->GetAnimID())
	{
		const auto& infoS = loder->GetAnimInfoS(animid.first);
		chengeInfoS(animData_[id][animid.first].first, infoS);
		const auto& dataS = loder->GetAnimDataS(animid.first);
		chengeAnimDataS(animData_[id][animid.first].second, dataS);
		animData_[id][animid.first].first.imgKey = infoS.image;
		const auto& info = animData_[id][animid.first].first;
		// �ǂݍ���ł���Animation�ɕK�v�ȉ摜�̓ǂݍ���
		lpImageMng.GetID(info.imgKey, info.image.c_str(), Vector2{ info.width / info.widthCnt,info.height / info.heightCnt }, Vector2{ info.widthCnt, info.heightCnt });
	}
	
	return true;
}

const Animation AnimationMng::GetAnimationData(Object_ID cID, Char_Anim_ID aID)
{
	if (animData_.count(cID) == 0)
	{
		//assert(!"���o�^�L����");
		return Animation{};
	}
	if (animData_[cID].count(aID) == 0)
	{
		//assert(!"���o�^�A�j���[�V����");
		return Animation{};
	}
	return animData_[cID][aID];
}

const AnimInfo AnimationMng::GetAnimInfo(Object_ID cID, Char_Anim_ID aID)
{
	return GetAnimationData(cID,aID).first;
}

bool AnimationMng::CheckAnimInfo(Object_ID cID, Char_Anim_ID aID)
{
	if (animData_.count(cID) == 0)
	{
		return  false;
	}
	if (animData_[cID].count(aID) == 0)
	{
		return false;
	}
	return true;
}

const int AnimationMng::GetAnimImag(Object_ID cID, Char_Anim_ID aID, int& elapsed,int& loopNum)
{
	if (animData_.count(cID) == 0)
	{
		//assert(!"���o�^�L����");
		return  -1;
	}
	if (animData_[cID].count(aID) == 0)
	{
		//assert(!"���o�^�A�j���[�V����");
		return -1;
	}
	// Animation�̌o�ߎ��Ԃ�i�߂�
	elapsed++;
	// Animation�ɐݒ肳��Ă���o�ߎ���
	int elapsedCnt = 0;
	auto subscriptData = animData_[cID][aID].second.subscript.begin();
	// ���݂ǂ̉摜��\�����邩
	for (const auto& elapsedData : animData_[cID][aID].second.flameData)
	{
		elapsedCnt += elapsedData;
		if (elapsed < elapsedCnt)
		{
			// ���݂̌o�ߎ��Ԃ��Animation�ɐݒ肳��Ă���o�ߎ��Ԃ������Ȃ炻�̎��ԂɑΉ������ʒu�̉摜��Ԃ�
			return lpImageMng.GetID(animData_[cID][aID].first.imgKey)[*subscriptData];
		}
		subscriptData++;
	}
	// for���𔲂��Ă��Ă���̂ł���Animation��1�����Ă���
	if (animData_[cID][aID].first.loop < 0 || animData_[cID][aID].first.loop > loopNum)
	{
		// ���Đ����Ă���Animation��loop�������͐ݒ肳�ꂽ�J��Ԃ��񐔂ɓ��B���Ă��Ȃ��ꍇ�o�ߎ��Ԃ����Z�b�g���ŏ��ɖ߂�
		elapsed = 0;
		loopNum++;
		return lpImageMng.GetID(animData_[cID][aID].first.imgKey)[*animData_[cID][aID].second.subscript.begin()];
	}
	else
	{
		// ���[�v�I�����Ă邵�ő�l����Ƃ�
		loopNum = INT_MAX;
		// �Đ����Ă���Animation��loop�Đ��������̂ł��Ȃ��A�ݒ肳�ꂽ�J��Ԃ��񐔂ɓ��B���Ă����ꍇ��ԍŌ�̉摜��Ԃ�
		return lpImageMng.GetID(animData_[cID][aID].first.imgKey)[*(--animData_[cID][aID].second.subscript.end())];
	}
}

const bool AnimationMng::CheckAnimLoopEnd(Object_ID cID, Char_Anim_ID aID, const int& elapsed, const int& loopNum)
{
	if (animData_.count(cID) == 0)
	{
		//assert(!"���o�^�L����");
		return  true;
	}
	if (animData_[cID].count(aID) == 0)
	{
		//assert(!"���o�^�A�j���[�V����");
		return true;
	}
	// �����V���b�g�A�j����loop�񐔂�0�Ȃ̂�0�ł��Đ�����������Ȃ��AAnimation���I����Ă���ꍇloopNum�̒��g��INT_MAX�ɂȂ��Ă���
	if (animData_[cID][aID].first.loop <= 0 || animData_[cID][aID].first.loop > loopNum)
	{
		// ���Đ����Ă���Animation��loop�������͐ݒ肳�ꂽ�J��Ԃ��񐔂ɓ��B���Ă��Ȃ��ꍇ�Đ���
		return false;
	}
	else
	{
		// �Đ����Ă���Animation��loop�Đ��������̂ł��Ȃ��A�ݒ肳�ꂽ�J��Ԃ��񐔂ɓ��B���Ă����ꍇ��ԍĐ��I��
		return true;
	}
}

const Vector2 AnimationMng::GetDrawOffSet(Object_ID cID, Char_Anim_ID aID)
{
	auto info = GetAnimInfo(cID, aID);
	return Vector2(info.widthOffset, info.heigthOffset);
}

Vector2 AnimationMng::GetDivImageSize(Object_ID cID, Char_Anim_ID aID)
{
	// Check���Ȃ���0���Z����\�������邽��
	if (!CheckAnimInfo(cID,aID))
	{
		return Vector2();
	}
	const auto info = GetAnimInfo(cID, aID);
	return Vector2(info.width / info.widthCnt, info.height / info.heightCnt);
}

AnimationMng::AnimationMng()
{
}
