#pragma once
#include <map>
#include <string>
#include "Vector2.h"
#include "../../Tiled/TmxObj.h"
#include "../Animation/Animation.h"

#define lpAnimMng AnimMng::GetInstance()

class AnimMng
{
public:
	static AnimMng& GetInstance() 
	{
		static AnimMng s_Instance;
		return s_Instance;
	}
	// AnimをItemから設定する(XmlItem)
	bool CheckAnim(std::string key, std::string state);							// アニメーションがあるかどうか
	bool SetXml(std::string key, std::string f_name);							// xmlデータセット
	bool SetItem(std::string key, const std::string state);						// アイテムセット
	bool SetAnim(std::string key, const std::string state, AnimVector& data);	// アニメーションセット

	int GetAnimID(std::string key, std::string state, int animframe);			// アニメーションID取得
	int GetAnimFrame(std::string key, std::string state, int animframe);		// アニメーションフレーム取得
	int GetAnimSize(std::string key, std::string state);						// アニメーション数取得

	Vector2 GetImageSize(std::string key);										// イメージサイズ取得
private:
	std::map<std::string, std::map<std::string, AnimVector>> animMap_;			// アニメーション
	TmxObj tmx_;																// tmxデータ
	std::map<std::string, XmlItem> xmlItem_;									// xmlデータ
};

