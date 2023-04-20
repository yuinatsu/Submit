#pragma once
#include <DxLib.h>
#include "../Controller.h"

constexpr int motionRange = 30000;                                  // スティックの反応可動域

// ゲームパッド用
class Pad :
	public Controller
{
public:
	Pad(int padType);
	~Pad();

	// 初期化 
	bool Init(void) override;

	// アップデート 
	void Update(float delta) override;

	// コントローラーのタイプの取得 
	CntType GetCntType(void) override { return CntType::Pad; };

	// ゲームパッドの情報を取得 
	const DINPUT_JOYSTATE& GetPadState(void) const&
	{
		return state_;
	}

	const int GetPadType(void) const;
private:

	// カーソルの座標をセット 
	void SetCursorPos(const Vector2& pos = lpSceneMng.screenSize_<float> / 2.0f) final;

	// プレイステーション系のパッドの右スティックの更新 
	void UpdatePsPad(float delta);

	// xbox系のパッドの右スティックの更新 
	void UpdateXboxPad(float delta);

	// ゲームパッドの情報
	DINPUT_JOYSTATE state_;

	// 現在のゲームパッドの種類
	int nowPadType_;

	// 更新処理
	void(Pad::* update_)(float);
};

