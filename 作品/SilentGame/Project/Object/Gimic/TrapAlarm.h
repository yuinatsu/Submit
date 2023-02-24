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
	// ���������̍��W��n��
	Math::Vector2 GetSpos(void);
	
private:
	ObjID GetObjID(void) override { return ObjID::TrapAlarm; };
 
	void Hit(Collider& othre);



	// �󂢂����ǂ����̃t���O
	bool flag_;

	// �󂢂Ă��鎞��
	float sTime_;

	// �󂢂��u�Ԃ̃v���C���[�̈ʒu
	Math::Vector2 sPos_;

};

