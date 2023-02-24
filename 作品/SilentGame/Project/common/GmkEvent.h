#pragma once
#include <list>
#include <memory>
#include <functional>
#include <unordered_map>
#include "Math.h"

class Obj;
class ObjManager;
class ResultData;

enum class EventType
{
	Alert,		// 監視カメラとかに見つかった時にサイレンがなるタイプ
	Clear,		// クリア時になるサイレン
	Non,		// 特に何も起きていないとき
};


class GmkEvent
{
	using GmkFuncMap = std::unordered_map < EventType, std::function<void(float)>>;
public:
	GmkEvent(std::shared_ptr< ResultData>& resultData);
	~GmkEvent();
	
	/// <summary> 指定のイベントを開始する </summary>
	/// <param name="pos"> 座標 </param>
	/// <param name="type"> イベントの種類 </param>
	void StartEvent(const Math::Vector2& pos,EventType type);

	void StartAlert();
	void EndAlert();

	/// <summary> イベントの更新処理 </summary>
	/// <param name="delta">  デルタタイム </param>
	void Update(float delta);

	void Draw(void);

	// 警戒時の描画など
	void MainDraw(void);

	// 現在のタイプを取得
	EventType GetNowEventType(void);

	/// <summary> ObjManagerをセットする </summary>
	/// <param name="objMng"> ObjManagerをweak_ptrで </param>
	void SetObjMng(std::weak_ptr<ObjManager> objMng) { objMng_ = objMng; }

	const float Color(void) const { return sColor_; }

	void SetCollFunc(std::function<void(void)>&& startFunc, std::function<void(void)>&& endFunc);
private:
	// アラートの更新と描画
	void UpdateAlert(float delta);
	
	// ギミックイベントごとの更新と描画処理をstateで切り替える用に
	GmkFuncMap gmkFunc_;

	// 経過時間
	float gmkStepTime_;

	// 現在のイベントの種類
	EventType nowType_;

	// 発見時の描画輝度の調整
	bool flag_;
	float sColor_;
	bool scFlag_;

	std::weak_ptr<ObjManager> objMng_;

	std::shared_ptr< ResultData> resultData_;

	// アラート開始時に呼ばれるfunction
	std::list<std::function<void(void)>> startCollFunc_;

	// アラート終了時に呼ばれるfunction
	std::list<std::function<void(void)>> endCollFunc_;
};

