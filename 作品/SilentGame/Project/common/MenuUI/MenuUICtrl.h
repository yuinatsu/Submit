#pragma once
#include <concepts>
#include <unordered_map>
#include <memory>
#include <type_traits>
#include "../SoundPross.h"
#include "../../Input/Controller.h"
#include "MenuUI.h"
#include "Cursor.h"


class Controller;
class Cursor;

// ���j���[UI���R���g���[������N���X(�e���v���[�g��enum class���󂯎��z��R���Z�v�g�g����������)
template<class MenuT>
class MenuUICtrl
{
public:
	
	/// <summary> �R���X�g���N�^ </summary>
	/// <param name="controller"> �R���g���[���[ </param>
	/// <param name="eMax"> ���j���[�̍ő�l </param>
	/// <param name="curthickness"> �J�[�\���p�̉摜 </param>
	/// <param name="start"> �J�n���j���[�̈ʒu </param>
	MenuUICtrl(Controller& controller, const int eMax, float curthickness, MenuT start) :
		controller_{ controller }, eMax_{ static_cast<std::underlying_type<MenuT>::type>(eMax) }, cursor_{ curthickness }, nowMenu_{start}
	{

	}

	~MenuUICtrl()
	{

	}

	/// <summary> �A�b�v�f�[�g </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Update(float delta);

	/// <summary> �`�� </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary> ���j���[�̒ǉ� </summary>
	/// <param name="menu"> ���j���[ </param>
	/// <param name="ui"> ���j�[�N�|�C���^��MenuUI�̉E�Ӓl�Q�� </param>
	void AddMenu(MenuT menu, std::unique_ptr<MenuUI>&& ui);

	/// <summary> ���݃J�[�\�������킳���Ă��郁�j���[���擾 </summary>
	/// <param name=""></param>
	/// <returns> ���݂̃��j���[ </returns>
	const MenuT GetNowMenu(void)const;


	void SetFlag(bool flag)
	{
		flag_ = flag;
	}
	/// <summary> �w��̃��j���[�̂t�h���擾���� </summary>
	/// <param name="menu">  </param>
	/// <returns></returns>
	MenuUI& GetUI(MenuT menu);

	/// <summary> �����[�X���� </summary>
	/// <param name=""></param>
	void Relese(void)
	{
		cursor_.Relese();
		for (auto& ui : menuMap_)
		{
			ui.second->Relese();
		}
	}

	/// <summary> ���j���[�̃}�C�i�X�����ւ̈ړ���ID���Z�b�g�� </summary>
	/// <param name="id"></param>
	void SetMinusInputID(InputID id);

	/// <summary> ���j���[�̃v���X�����ւ̈ړ���ID���Z�b�g�� </summary>
	/// <param name="id"></param>
	void SetPlusInputID(InputID id);

private:

	// �}�C�i�X����InputID
	InputID minus_ = InputID::Up;

	// �v���X����InputID
	InputID plus_ = InputID::Down;

	// ���݂̃��j���[
	MenuT nowMenu_;

	// �R���g���[���[�̎Q��
	Controller& controller_;

	// �e���v���[�g�Ŏ󂯎����enum class�̍ő�l�����̌^�Ŏ���
	const int eMax_;


	bool flag_ = true;
	// ���j���[��enum clas���L�[�Ń��j���[�\���p�N���X�Ŏ���
	std::unordered_map<MenuT, std::unique_ptr<MenuUI>> menuMap_;

	// ���j���[�p�J�[�\��
	Cursor cursor_;
};

template<class MenuT>
inline void MenuUICtrl<MenuT>::Update(float delta)
{
	cursor_.Update(delta);

	if (controller_.Pressed(minus_)) // ���j���[���}�C�i�X������
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select,SoundProcess::GetSEVolumeEntire(),false);
		nowMenu_ = static_cast<MenuT>((static_cast<std::underlying_type<MenuT>::type>(nowMenu_) + (eMax_ - 1)) % eMax_);
		cursor_.ResetBlink();
	}
	if (controller_.Pressed(plus_)) // ���j���[���v���X������
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select,SoundProcess::GetSEVolumeEntire(), false);
		nowMenu_ = static_cast<MenuT>((static_cast<std::underlying_type<MenuT>::type>(nowMenu_) + 1) % eMax_);
		cursor_.ResetBlink();
	}

}

template<class MenuT>
inline void MenuUICtrl<MenuT>::Draw(void)
{
	// ���j���[��`��
	for (auto& menu : menuMap_)
	{
		menu.second->Draw();
	}

	// ���݂̃��j���[�̏�ɃJ�[�\����\������
	if (flag_)
	{
		cursor_.Draw(menuMap_[nowMenu_]->GetLT(), menuMap_[nowMenu_]->GetRB());
	}
}

template<class MenuT>
inline void MenuUICtrl<MenuT>::AddMenu(MenuT menu, std::unique_ptr<MenuUI>&& ui)
{
	// �w�肳�ꂽ���j���[��MenuUI������
	menuMap_[menu] = std::move(ui);
}

template<class MenuT>
inline const MenuT MenuUICtrl<MenuT>::GetNowMenu(void) const
{
	return nowMenu_;
}

template<class MenuT>
inline MenuUI& MenuUICtrl<MenuT>::GetUI(MenuT menu)
{
	return *menuMap_[menu];
}

template<class MenuT>
inline void MenuUICtrl<MenuT>::SetMinusInputID(InputID id)
{
	minus_ = id;
}

template<class MenuT>
inline void MenuUICtrl<MenuT>::SetPlusInputID(InputID id)
{
	plus_ = id;
}
