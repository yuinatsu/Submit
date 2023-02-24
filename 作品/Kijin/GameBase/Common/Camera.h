#pragma once
#include <DxLib.h>
#include "Vector3.h"
#include "../Application.h"
class ObjectManager;
class Controller;

// カメラクラス
class Camera
{
public:
	Camera();

	// ここから試し用のカメラ---------------------------------------------------
	static constexpr float HEGHT = 200.0f;
	// 注視点からカメラまでのXZ距離
	static constexpr float TARGET_TO_CAMERA = 500.0f;
	// 注視点からキャラクタまでのXZ距離
	static constexpr float TARGET_TO_CHAR = 300.0f;
	// 画面の中心 (1280,720)
	static constexpr int HALF_SIZE_X = 640;
	static constexpr int HALF_SIZE_Y = 360;

	// カメラの移動制御と注視点の設定
	void Update(Controller& controller);

	Vector3 GetAngle(void);

	/// <summary>
	/// カメラのセットアップをする
	/// </summary>
	/// <param name=""></param>
	void SetUpScreen(void) const;

	/// <summary>
	/// カメラを指定座標の方へ向ける
	/// </summary>
	/// <param name="pos"> 向けたい座標 </param>
	void Look(const Vector3& pos);

	/// <summary>
	/// カメラの上方向を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns> 上方向 </returns>
	const Vector3 GetUp(void)const;

	/// <summary>
	/// カメラの前方向を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetForward(void) const;

	void SetPos(const Vector3& pos)
	{
		pPos_ = pos;
	}
private:
	// カメラの移動制御の計算
	void rotate(float* posX,float* posY, const float angle, const Vector3 mpos);

	// カメラの移動制御
	void ControlCamera(Controller& controller);
	// カメラの注視点の設定
	void SetBefore(void);

	// 確認用のアングル
	Vector3 angle_;

	// 同期先の座標
	Vector3 pPos_;

	// 傾き
	MATRIX rotation_;

	// カメラの座標
	Vector3 pos_;

	// 注視点
	Vector3 target_;
	VECTOR vtarget_;

	Vector2 speed_;

	// カメラのヨーイング
	float yaw;
	// マウスの座標をセット
	Vector2I mousePos_;
};

