#include <cassert>
#include <sstream>
#include <memory>
#include "AnimationMng.h"
#include "../Animation/AnimationLoder.h"
#include "../Mng/ImageMng.h"

bool AnimationMng::LoadAnimTmx(const std::string& filepath, Object_ID id)
{
	// AnimInfoSの中身をintに変換しAnimInfoへ格納する
	auto chengeInfoS = [](AnimInfo& info,const AnimInfoS& infoS) {
		info.width = std::atoi(infoS.width.c_str());
		info.height = std::atoi(infoS.height.c_str());
		info.widthCnt = std::atoi(infoS.widthCnt.c_str());
		info.heightCnt = std::atoi(infoS.heightCnt.c_str());
		info.loop = std::atoi(infoS.loop.c_str());
		info.image = infoS.image;
	};
	// .csv形式のデータを,ごとに取り出しlistへ格納する
	auto chengeCsvToList = [](std::list<int>& list, const std::string str){
		// 取り出されるstringの一時保存先
		std::string dataStr;
		std::stringstream sstr(str);
		while (std::getline(sstr, dataStr, ','))
		{
			list.push_back(std::atoi(dataStr.c_str()));
		}
	};
	// AnimDataSの中身を変換しAnimDataへ格納する
	auto chengeAnimDataS = [chengeCsvToList](AnimData& data, const AnimDataS& dataS) {
		chengeCsvToList(data.flameData, dataS.flameData);
		chengeCsvToList(data.subscript, dataS.subscriptData);
	};

	if (animData_.count(id) != 0)
	{
		// 登録済みIDは登録しないが既にデータはあるのでtrue
		return true;
	}
	std::unique_ptr<AnimationLoder> loder = std::make_unique<AnimationLoder>();
	if (!loder->LoadAnimTmx(filepath))
	{
		//assert(!"AnimTmx読み込み失敗");
		return false;
	}
	// 読み込んできたデータの格納
	for (const auto& animid : loder->GetAnimID())
	{
		const auto& infoS = loder->GetAnimInfoS(animid.first);
		chengeInfoS(animData_[id][animid.first].first, infoS);
		const auto& dataS = loder->GetAnimDataS(animid.first);
		chengeAnimDataS(animData_[id][animid.first].second, dataS);
		animData_[id][animid.first].first.imgKey = infoS.image;
		const auto& info = animData_[id][animid.first].first;
		// 読み込んできたAnimationに必要な画像の読み込み
		lpImageMng.GetID(info.imgKey, info.image.c_str(), Vector2{ info.width / info.widthCnt,info.height / info.heightCnt }, Vector2{ info.widthCnt, info.heightCnt });
	}
	
	return true;
}

const Animation AnimationMng::GetAnimationData(Object_ID cID, Char_Anim_ID aID)
{
	if (animData_.count(cID) == 0)
	{
		//assert(!"未登録キャラ");
		return Animation{};
	}
	if (animData_[cID].count(aID) == 0)
	{
		//assert(!"未登録アニメーション");
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
		//assert(!"未登録キャラ");
		return  -1;
	}
	if (animData_[cID].count(aID) == 0)
	{
		//assert(!"未登録アニメーション");
		return -1;
	}
	// Animationの経過時間を進める
	elapsed++;
	// Animationに設定されている経過時間
	int elapsedCnt = 0;
	auto subscriptData = animData_[cID][aID].second.subscript.begin();
	// 現在どの画像を表示するか
	for (const auto& elapsedData : animData_[cID][aID].second.flameData)
	{
		elapsedCnt += elapsedData;
		if (elapsed < elapsedCnt)
		{
			// 現在の経過時間よりAnimationに設定されている経過時間が長いならその時間に対応した位置の画像を返す
			return lpImageMng.GetID(animData_[cID][aID].first.imgKey)[*subscriptData];
		}
		subscriptData++;
	}
	// for分を抜けてきているのでそのAnimationは1周している
	if (animData_[cID][aID].first.loop < 0 || animData_[cID][aID].first.loop > loopNum)
	{
		// 今再生しているAnimationがloopもしくは設定された繰り返し回数に到達していない場合経過時間をリセットし最初に戻す
		elapsed = 0;
		loopNum++;
		return lpImageMng.GetID(animData_[cID][aID].first.imgKey)[*animData_[cID][aID].second.subscript.begin()];
	}
	else
	{
		// ループ終了してるし最大値入れとく
		loopNum = INT_MAX;
		// 再生しているAnimationがloop再生されるものでもなく、設定された繰り返し回数に到達していた場合一番最後の画像を返す
		return lpImageMng.GetID(animData_[cID][aID].first.imgKey)[*(--animData_[cID][aID].second.subscript.end())];
	}
}

const bool AnimationMng::CheckAnimLoopEnd(Object_ID cID, Char_Anim_ID aID, const int& elapsed, const int& loopNum)
{
	if (animData_.count(cID) == 0)
	{
		//assert(!"未登録キャラ");
		return  true;
	}
	if (animData_[cID].count(aID) == 0)
	{
		//assert(!"未登録アニメーション");
		return true;
	}
	// ワンショットアニメはloop回数が0なので0でも再生中かもしれない、Animationが終わっている場合loopNumの中身はINT_MAXになっている
	if (animData_[cID][aID].first.loop <= 0 || animData_[cID][aID].first.loop > loopNum)
	{
		// 今再生しているAnimationがloopもしくは設定された繰り返し回数に到達していない場合再生中
		return false;
	}
	else
	{
		// 再生しているAnimationがloop再生されるものでもなく、設定された繰り返し回数に到達していた場合一番再生終了
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
	// Checkしないと0除算する可能性があるため
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
