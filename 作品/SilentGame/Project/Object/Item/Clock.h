#pragma once
#include <functional>
#include "../Obj.h"

class ObjManager;

class Clock :
    public Obj
{
public:
	Clock(const Math::Vector2& pos, ObjManager& objMng);
	~Clock();

	// �A���[�������Ȃ��Ă�����~�߂�
	void StopAlarm(void) const;

	// �A���[�������Ȃ�͂��Ȃ�ēx�炷
	void RestartAlarm(void) const;
private:
	bool Init(void);
	void Update(float delta) override;
	void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;
	ObjID GetObjID(void) override { return ObjID::Clock; };
	
	/// <summary> �����Ȃ�܂ł̏��� </summary>
	/// <param name=""></param>
	void NonUpdate(void);

	/// <summary> �����Ȃ��Ă鎞�̏��� </summary>
	/// <param name=""></param>
	void AlarmUpdate(void);

	// �o�ߎ���
	float stepTime_;

	// �X�V����
	std::function<void(void)> updateFunc_;
	ObjManager& objMng_;
	// �\�����ɂ��炷����
	Math::Vector2 randOffset_;

	// �A���[�����Ȃ��Ă��邩
	bool isAlarm_;
};

