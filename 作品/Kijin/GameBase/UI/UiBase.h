#pragma once
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "../Common/Vector2.h"
#include "../Common/SharedHandle.h"
#include "UiID.h"
#include <filesystem>

class ObjectManager;
class Controller;

class UiBase
{
public:
	UiBase(const Vector2& pos);
	virtual ~UiBase() = default;

	virtual void Update(float delta, ObjectManager& objMng, Controller& controller) = 0;
	virtual void Draw() = 0;
	virtual UiID GetUIID(void) = 0;
	virtual void Loaded(void);

	/// <summary>
	/// �{�^���Ƃ��ċ@�\���邩(ButtonBase���p�����Ă��邩�H)
	/// </summary>
	/// <param name=""></param>
	/// <returns> �{�^���̎�true�A�����łȂ��Ƃ�false </returns>
	virtual bool IsButton(void) const
	{
		return false;
	}

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���W </returns>
	const Vector2& GetPos(void) const&
	{
		return pos_;
	}

	/// <summary>
	/// �T�C�Y�̎擾(�c��)
	/// </summary>
	/// <param name=""></param>
	/// <returns> �T�C�Y(�c��) </returns>
	const Vector2& GetSize(void) const&
	{
		return size_;
	}
protected:
	// ���W
	Vector2 pos_;

	// �T�C�Y
	Vector2 size_;
};

