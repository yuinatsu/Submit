#pragma once
#include <list>
#include "../../common/MapCollMng.h"
#include "../../common/Math.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "../../common/Camera/Camera.h"
#include "../../common/Structure.h"
#include "../../common/Time.h"
#include "GimicObj.h"

class Collider;

class Alert:
	public GimicObj
{
public:
	Alert(std::weak_ptr< GmkEvent>& gmkEvent,Math::Vector2 pos, Math::Vector2 vec, std::shared_ptr<TmxObj>& tmxObj, std::weak_ptr<Obj>&& player);
	~Alert();
	void Init(void);
	void Update(float delta) override;
	// プレイヤーを見つけていないときの処理
	void UpdateNoDis(const Math::Vector2& pos, float delta);
	// プレイヤーを見つけた時の処理
	void UpdateDis(const Math::Vector2& pos, float delta);
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) override;
	ObjID GetObjID(void) { return ObjID::Alert; };
	
	// プレイヤーを検知したかどうかを受け取る
	bool GetpFlag(void);

private:

	void Hit(Collider& other);

	// マップ情報を受け取る
	std::shared_ptr<TmxObj> tmxObj_;
	// プレイヤーの情報を受け取る
	std::weak_ptr<Obj> player_;

	// 視線の回転を逆回転に
	//bool rotFlag_;
	// 視線の回転を反転するまでの時間
	//float rotTime_;
	// プレイヤーが入ったどうか
	bool pflag_;

	//std::unique_ptr<Fan> fan_;

	// プレイヤーがどれかの視線に入ったかどうか
	bool sFlag_;
	// 経過時間
	float time_;

	// 左右どっちか移転か
	float sig_;

	// 停止フラグ
	bool stopFlag_;
};

