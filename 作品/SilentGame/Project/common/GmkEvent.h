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
	Alert,		// �Ď��J�����Ƃ��Ɍ����������ɃT�C�������Ȃ�^�C�v
	Clear,		// �N���A���ɂȂ�T�C����
	Non,		// ���ɉ����N���Ă��Ȃ��Ƃ�
};


class GmkEvent
{
	using GmkFuncMap = std::unordered_map < EventType, std::function<void(float)>>;
public:
	GmkEvent(std::shared_ptr< ResultData>& resultData);
	~GmkEvent();
	
	/// <summary> �w��̃C�x���g���J�n���� </summary>
	/// <param name="pos"> ���W </param>
	/// <param name="type"> �C�x���g�̎�� </param>
	void StartEvent(const Math::Vector2& pos,EventType type);

	void StartAlert();
	void EndAlert();

	/// <summary> �C�x���g�̍X�V���� </summary>
	/// <param name="delta">  �f���^�^�C�� </param>
	void Update(float delta);

	void Draw(void);

	// �x�����̕`��Ȃ�
	void MainDraw(void);

	// ���݂̃^�C�v���擾
	EventType GetNowEventType(void);

	/// <summary> ObjManager���Z�b�g���� </summary>
	/// <param name="objMng"> ObjManager��weak_ptr�� </param>
	void SetObjMng(std::weak_ptr<ObjManager> objMng) { objMng_ = objMng; }

	const float Color(void) const { return sColor_; }

	void SetCollFunc(std::function<void(void)>&& startFunc, std::function<void(void)>&& endFunc);
private:
	// �A���[�g�̍X�V�ƕ`��
	void UpdateAlert(float delta);
	
	// �M�~�b�N�C�x���g���Ƃ̍X�V�ƕ`�揈����state�Ő؂�ւ���p��
	GmkFuncMap gmkFunc_;

	// �o�ߎ���
	float gmkStepTime_;

	// ���݂̃C�x���g�̎��
	EventType nowType_;

	// �������̕`��P�x�̒���
	bool flag_;
	float sColor_;
	bool scFlag_;

	std::weak_ptr<ObjManager> objMng_;

	std::shared_ptr< ResultData> resultData_;

	// �A���[�g�J�n���ɌĂ΂��function
	std::list<std::function<void(void)>> startCollFunc_;

	// �A���[�g�I�����ɌĂ΂��function
	std::list<std::function<void(void)>> endCollFunc_;
};

