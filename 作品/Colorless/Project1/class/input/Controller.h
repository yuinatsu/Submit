#pragma once
#include <array>
#include <map>
#include <vector>
#include <string>
#include "inputID.h"

// トリガー情報
enum class Trg 
{
	Now,			// 現在
	Old,			// 1つ前
	Max
};

// コントーローラータイプ
enum class CntType 
{
	Key,			// キーボード
	Pad,			// パッド
	Max
};

using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;
using CntData = std::map<InputID,TrgBool>;

class Controller
{
public:
	bool operator()(double delta) 
	{
		if (this != nullptr) 
		{
			Update(delta);
		}
		return this != nullptr;
	}
	virtual ~Controller();
	virtual bool Init(void) = 0;									// 初期化
	virtual void Update(double delta) = 0;							// 更新 Update(deltaTime)
	virtual CntType GetCntType(void) = 0;							// コントロールする機種の取得
	const CntData& GetCntData(void) { return cntData_; };			// 設定したmapを格納してるcntData_の取得
	bool Press(InputID id);											// 今押しているか
	bool Pressed(InputID id);										// 押した瞬間
	bool Released(InputID id);										// 離した瞬間
	bool SpPress(InputID id);										// 特定のボタンだけを押しているかどうか
private:
protected:
	CntData cntData_;
	std::map<InputID, unsigned int> keyList_ = {};
};