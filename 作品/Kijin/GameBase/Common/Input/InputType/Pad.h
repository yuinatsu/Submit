#pragma once
#include <DxLib.h>
#include "../Controller.h"

constexpr int motionRange = 30000;                                  // スティックの反応可動域

class Pad :
	public Controller
{
public:
	Pad(int padType);
	~Pad();
	bool Init(void) override;										// 初期化
	void Update(float delta) override;								// 更新 Update(deltaTime)
	CntType GetCntType(void) override { return CntType::Pad; };		// コントローラータイプの取得
private:
	void SetCursorPos(const Vector2& pos = lpApp.screenSize_<float> / 2.0f) final;

	/// <summary>
	/// プレイステーション系のパッドの右スティックの更新
	/// </summary>
	/// <param name=""></param>
	void UpdatePsPad(float delta);

	/// <summary>
	/// xbox系のパッドの右スティックの更新
	/// </summary>
	/// <param name=""></param>
	void UpdateXboxPad(float delta);

	DINPUT_JOYSTATE state_;
	int padState_;
	int nowPadType_;
	void(Pad::* update_)(float);
};

