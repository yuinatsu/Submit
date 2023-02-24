#pragma once
#include "Controller.h"
class KeyInput :
	public Controller
{
public:
	KeyInput();
	~KeyInput();
	bool Init(void)override;										// 初期化
	void Update(double delta)override;								// 更新 Update(deltaTime)
	CntType GetCntType(void)override{ return CntType::Key; }		// コントローラータイプの取得
private:
	std::array<char, 256> keyData_ = {};							// キーデータ格納用
};