#pragma once
#include <DxLib.h>
#include "Controller.h"
class PadInput :
	public Controller
{
public:
	PadInput();
	~PadInput();
	bool Init(void) override;										// 初期化
	void Update(double delta) override;								// 更新 Update(deltaTime)
	void StickUpdate(void);											// スティックの更新
	CntType GetCntType(void) override { return CntType::Pad; };		// コントローラータイプの取得
private:
	XINPUT_STATE input_;											// コントローラーの情報
	static int count_;												// 今現在のpadの数
	int padID_;														// 自分のpadID
};

