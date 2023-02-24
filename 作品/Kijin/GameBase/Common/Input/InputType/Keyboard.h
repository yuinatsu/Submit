#pragma once
#include "../Controller.h"

class Keyboard :
    public Controller
{
public:
	Keyboard();
	~Keyboard();
	bool Init(void)override;										// 初期化
	void Update(float delta) override;								// 更新 Update(deltaTime)
	CntType GetCntType(void) override { return CntType::Key; }		// コントローラータイプの取得private:
private:
	void SetCursorPos(const Vector2& pos = lpApp.screenSize_<float> / 2.0f);
	std::array<char, 256> keyData_ = {};							// キーデータ格納用
};

