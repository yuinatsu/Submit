#pragma once
#include <memory>
#include "../common/Math.h"

enum class DescriptionID
{
	LandMine,
	Decoy,
	Alcohol,
	Clock,
};

template<class T>
class MenuUICtrl;

class Controller;

// �A�C�e���̐����̕\������N���X
class ItemDescriptionWnd
{
public:
	ItemDescriptionWnd(Controller& controller,const Math::Vector2& lt, const Math::Vector2& rb);
	bool Update(float delta);
	void Draw(void);
	// �A�C�e����I�����Ă��邩���󂯎��
	bool GetFlag(void);
private:

	/// <summary> �����I�����ĂȂ����̃A�b�v�f�[�g </summary>
	/// <param name="delta"></param>
	/// <returns></returns>
	bool UpdateNon(float delta);

	/// <summary> �������\�����̎��A�b�v�f�[�g </summary>
	/// <param name="delta"></param>
	/// <returns></returns>
	bool UpdateOpen(float delta);

	/// <summary> ������������̃A�b�v�f�[�g </summary>
	/// <param name="delta"></param>
	/// <returns></returns>
	bool UpdateClose(float delta);

	/// <summary> �����I�����Ă��Ȃ��Ƃ��̕`�� </summary>
	/// <param name=""></param>
	void DrawNon(void);

	/// <summary> ������\�����̕`�� </summary>
	/// <param name=""></param>
	void DrawOpen(void);

	/// <summary> ��������Ă��鎞�̕`�� </summary>
	/// <param name=""></param>
	void DrawClose(void);

	/// <summary> �\�����J�n���� </summary>
	/// <param name="imgName"></param>
	void Start(const std::string& imgName);

	// �A�b�v�f�[�g�̊֐��|�C���^
	bool (ItemDescriptionWnd::* update_)(float);

	// �`��̊֐��|�C���^
	void (ItemDescriptionWnd::* draw_)(void);

	// ����
	Math::Vector2 lt_;

	// �E��
	Math::Vector2 rb_;

	// ���j���[���Ǘ�����N���X
	std::unique_ptr<MenuUICtrl<DescriptionID>> menuUI_;

	// ���͊֌W
	Controller& controller_;

	// �����摜�̃T�C�Y
	Math::Vector2 imgSizeHalf_;

	// �摜��
	std::string imgName_;

	// �o�ߎ���
	float time_;

	// ���肵�Ă��邩�𒲂ׂ�
	bool flag_;
};

