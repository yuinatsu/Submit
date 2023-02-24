#pragma once
#include <memory>
#include <functional>
#include "../Math.h"


// カメラの制限用構造体
struct CameraRect 
{
	Math::Vector2 leftTop;		// 左と上
	Math::Vector2 rightBottom;	// 右と下
};

// カメラのモード
enum class CamMode
{
	Trace,			// 特定の座標を追跡する
	MoveEvent,			// カメラのターゲットから別のターゲットへ向かう
	StartEvent,			// 開始時の演出用のカメラの動き
	Non,
};



class Obj;
class CameraModeBase;
class TmxObj;
class Controller;

class Camera
{
public:

	/// <summary> カメラの初期化処理(追跡モードで開始) </summary>
	/// <param name="isSmallMap"> マップが小さいか? </param>
	/// <param name="tmxObj"></param>
	/// <param name="traceTarget"> 追跡するオブジェクト </param>
	/// <returns></returns>
	bool Init(bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj, const std::weak_ptr<Obj>& traceTarget);

	/// <summary> カメラの初期化(開始時の演出用) </summary>
	/// <param name="isSmallMap"> マップが小さいか? </param>
	/// <param name="tmxObj"></param>
	/// <param name="start"> スタート時のオブジェクト </param>
	/// <param name="end"> 末端のオブジェクト </param>
	/// <returns></returns>
	bool Init(Controller& controller,bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj, const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end);


	bool Update(float delta);
	//bool Draw(TmxObj& tmxObj);  // 自分自身を描画
	Math::Vector2I GetViewSize();


	/// <summary> 描画用オフセットを取得する </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Math::Vector2 GetOffset(void) const;

	/// <summary> カメラの座標を取得する </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Math::Vector2 GetPos(void) const { return pos_; };

	/// <summary> 追跡するカメラを開始する </summary>
	/// <param name="traceTarget"> 追跡したいオブジェクト </param>
	void StartTraceMode(const std::weak_ptr<Obj>& traceTarget);

	/// <summary>  </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="allTime"></param>
	/// <param name="waitTime"></param>
	void StartMoveEvent(const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end, float allTime,float waitTime);


	void StartEvent(Controller& controller,const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end);

	/// <summary> 現在のカメラのモードを取得 </summary>
	/// <param name=""> 現在のカメラのモード </param>
	/// <returns></returns>
	const CamMode GetMode(void) const { return nowMode_; }

	void Draw(void);

	/// <summary> カメラの表示サイズ移動制限等を初期化 </summary>
	/// <param name="isSmallMap"> マップが小さいか? </param>
	/// <param name="tmxObj"></param>
	/// <returns></returns>
	bool Init(bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj);

	void Loaded(void);
private:

	
	bool isSmallMap_ = false;

	// モードごとのカメラの動きを処理するクラス
	std::unique_ptr<CameraModeBase> camMode_;

	// カメラの制限
	CameraRect rect_;

	// カメラの初期位置
	Math::Vector2 defPos_;

	// カメラの表示サイズ
	Math::Vector2 viewsize_;

	// カメラの座標
	Math::Vector2 pos_;




	// 現在のカメラのモード
	CamMode nowMode_ = CamMode::Non;


	// カメラの経過時間
	float cameraTime_ = 0.0f;

	friend class TraceModeCamera;
	friend class MoveEventModeCamera;
	friend class StartEventMode;

};

