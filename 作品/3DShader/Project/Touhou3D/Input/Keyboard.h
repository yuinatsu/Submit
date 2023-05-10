#pragma once
#include "Controller.h"
#include "../Common/Vector2.h"

class Keyboard :
	public Controller
{
public:
	Keyboard();
	~Keyboard() {}
	// 更新
	void Update(double delta) override;
	// コントローラータイプの取得
	CntType GetCntType(void) override { return CntType::Key; }
private:
	// キーデータ格納用
	std::array<char, 256> keyData_ = {};
};

