#pragma once
#include <list>
#include "GimicObj.h"
#include "../../common/Math.h"

struct Squar;
class GmkEvent;
class Collider;


class TrapAlarm :
	public GimicObj
{
public:
	TrapAlarm(const Math::Vector2& pos, std::weak_ptr< GmkEvent>& gmkEvent);
	~TrapAlarm();

	void Init(void);
	void Update(float delta) override;
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) override;
	// 見つけた時の座標を渡す
	Math::Vector2 GetSpos(void);
	
private:
	ObjID GetObjID(void) override { return ObjID::TrapAlarm; };
 
	void Hit(Collider& othre);



	// 空いたかどうかのフラグ
	bool flag_;

	// 空いている時間
	float sTime_;

	// 空いた瞬間のプレイヤーの位置
	Math::Vector2 sPos_;

};

