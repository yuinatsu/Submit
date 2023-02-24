#pragma once
#include <string>
#include "../Math.h"

// ���j���[�pUI
class MenuUI
{
public:

	/// <summary> ���j���[�pUI�̃R���X�g���N�^ </summary>
	/// <param name="pos"> ���W </param>
	/// <param name="imgName"> �摜�� </param>
	/// <param name="imgNum"> �摜�̃}�X�� </param>
	/// <param name="exRate"> �摜�̊g�嗦 </param>
	MenuUI(const Math::Vector2& pos,const std::string& imgName, int imgNum = 0,float exRate = 1.0f);

	MenuUI(const MenuUI& menuUI);
	~MenuUI();

	/// <summary> �`�� </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary> ������W�̎擾 </summary>
	/// <param name=""></param>
	/// <returns> ������W </returns>
	const Math::Vector2 GetLT(void);

	/// <summary> �E�����W�̎擾 </summary>
	/// <param name=""></param>
	/// <returns> �E�����W </returns>
	const Math::Vector2 GetRB(void);

	/// <summary> ������� </summary>
	/// <param name=""></param>
	void Relese(void);
private:

	// �摜�̔ԍ�
	int imgNum_;

	// �g�嗦
	float exRate_;

	// �摜��
	std::string imgName_;

	// ���W
	Math::Vector2 pos_;

	// �摜�T�C�Y
	Math::Vector2 imgSize_;
};

