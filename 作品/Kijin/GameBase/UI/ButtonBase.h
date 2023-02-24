#pragma once
#include "UiBase.h"

class BaseScene;

// �J�[�\���ŃN���b�N�ł���Ui�̊��N���X
class ButtonBase :
	public UiBase
{
public:
	ButtonBase(const Vector2& pos);
	virtual ~ButtonBase();

	/// <summary>
	/// �����ꂽ���Ă΂��
	/// </summary>
	/// <param name="scene"> �V�[�� </param>
	virtual void Push(BaseScene& scene, Controller& controller) = 0;

	/// <summary>
	/// �J�[�\���Ɠ��������u�ԌĂ΂��
	/// </summary>
	/// <param name=""></param>
	virtual void HitCursor(void) = 0;

	/// <summary>
	/// �J�[�\���Ɠ�����Ȃ��Ȃ����u�ԌĂ΂��
	/// </summary>
	/// <param name=""></param>
	virtual void NotHitCursor(void) = 0;

	/// <summary>
	/// �J�[�\���Ɠ������Ă��邩
	/// </summary>
	/// <param name=""></param>
	/// <returns> �������Ă���Ƃ�true </returns>
	bool IsHitCursor(void) const
	{
		return isHitCursor_;
	}


	bool IsButton(void) const final
	{
		return true;
	}

	// �J�[�\���Ɠ������Ă��鎞
	bool isHitCursor_;
};

