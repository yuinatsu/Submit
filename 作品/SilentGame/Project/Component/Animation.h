#pragma once
#include <map>
#include <vector>
#include <string>
#include "../common/AnimationData.h"
#include "Component.h"
#include "../common/Math.h"

using AnimVector = std::vector<std::pair<int, int>>;

class DrawMng;


class Animation :
	public Component
{
public:

	/// <summary> �f�[�^�t�@�C���ƃL�[�Ə����X�e�[�g�w��̃R���X�g���N�^ </summary>
	/// <param name="fname"> �A�j���[�V�����f�[�^�̃t�@�C���� </param>
	/// <param name="key"> �L�[ </param>
	/// <param name="state"> �����X�e�[�g </param>
	/// <param name="name"> �R���|�[�l���g�� </param>
	/// <param name="owner"> ������ </param>
	Animation(const std::string& fname, const std::string& key, const std::string& state, Obj& owner);
	
	~Animation();

	void Update(float delta) override;											// �X�V
	bool Draw(DrawMng& drawMng,Math::Vector2 pos, bool flag);		// �`�� Draw(���W, �T�C�Y, ����)
	bool Draw(DrawMng& drawMng, Math::Vector2 pos, bool flag, const std::string& key);		// �`�� Draw(���W, �T�C�Y, ����)
	bool Draw(DrawMng& drawMng, Math::Vector2 pos, Math::Vector2I size, std::string state);		// �`�� Draw(���W, �T�C�Y, ����)

	/// <summary> �[�x�w��̕`�� </summary>
	/// <param name="drawMng"> �`��Ǘ��p�N���X </param>
	/// <param name="pos"> �`�悷����W </param>
	/// <param name="flag"> idel </param>
	/// <returns></returns>
	bool Draw(DrawMng& drawMng, Math::Vector2 pos, float depth = 0.0f, bool flag = false);

	std::string GetKey(void);									// �L�[���擾
	std::string GetState(void);									// �X�e�[�^�X�擾
	

	/// <summary> ��Ԃ��Z�b�g </summary>
	/// <param name="state"> �X�e�[�g </param>
	void SetState(const std::string& state);

	/// <summary> ��Ԃ��Z�b�g�i��Ԃ������ł��ʂ�j </summary>
	/// <param name="state"></param>
	void SetStateSame(const std::string& state);

	void SetKey(const std::string& key);

	/// <summary> �Đ��I�����Ă��邩�H </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsEnd(void) const;

	/// <summary>
	/// �\������摜�̉��Ԗڂ���������Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���Ԗڂ� </returns>
	const int GetNowIdx(void)const;
private:

	ComponentID GetID() const override;

	// �A�j���[�V�����f�[�^�̃L�[
	std::string key_;

	// �X�e�[�g
	std::string state_;

	// ���[�v��
	int loop_;

	// �Đ�����
	float playStepTime_;

	// ���݂̍Đ��ʒu
	std::list<std::pair<int, float>>::const_iterator nowItr_;

};

