#pragma once
#include <functional>
#include "Obj.h"

class TmxObj;

// �}�b�v�̑����p�̃I�u�W�F�N�g
class Decoration :
	public Obj
{
public:

	/// <summary> �����蔻�薳���A�j���[�V��������̃R���X�g���N�^ </summary>
	/// <param name="fname"> �A�j���[�V�����t�@�C���� </param>
	/// <param name="state"> �A�j���[�V�����̃X�e�[�g�� </param>
	/// <param name="key"> �A�j���[�V�����̃L�[ </param>
	/// <param name="pos"> ���W </param>
	Decoration(const std::string& fname, const std::string& state, const std::string& key, const Math::Vector2& pos);

	/// <summary> �����蔻�肠��A�j���[�V��������̃R���X�g���N�^ </summary>
	/// <param name="fname"> �A�j���[�V�����̃t�@�C���� </param>
	/// <param name="state"> �A�j���[�V�����̃X�e�[�g�� </param>
	/// <param name="key"> �A�j���[�V�����̃L�[ </param>
	/// <param name="r"> �����蔻��̃L�[ </param>
	/// <param name="pos"> ���W </param>
	Decoration(const std::string& fname, const std::string& state, const std::string& key, std::shared_ptr<TmxObj>& tmxObj, float r, const Math::Vector2& pos);

	/// <summary> �����蔻�薳���A�j���[�V���������̃R���X�g���N�^ </summary>
	/// <param name="fname"> �t�@�C���� </param>
	/// <param name="key"> �摜�̃L�[ </param>
	/// <param name="divSize"> �����T�C�Y </param>
	/// <param name="divNum"> ������ </param>
	/// <param name="idx"> �摜�̕�����̃}�X�� </param>
	/// <param name="pos"> ���W </param>
	Decoration(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx, const Math::Vector2& pos);

	/// <summary> �����蔻�肠��A�j���[�V���������̃R���X�g���N�^ </summary>
	/// <param name="fname"> �t�@�C���� </param>
	/// <param name="key"> �摜�̃L�[ </param>
	/// <param name="divSize"> �����T�C�Y </param>
	/// <param name="divNum"> ������ </param>
	/// <param name="idx"> �摜�̕�����̃}�X�� </param>
	/// <param name="r"> �����蔻��̔��a </param>
	/// <param name="pos"> ���W </param>
	Decoration(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx, std::shared_ptr<TmxObj>& tmxObj, float r, const Math::Vector2& pos);

	~Decoration();
private:

	/// <summary> �A�j���[�V�����̏��������� </summary>
	/// <param name="fname"></param>
	/// <param name="state"></param>
	/// <param name="key"></param>
	/// <returns></returns>
	bool InitAnim(const std::string& fname, const std::string& state, const std::string& key);

	/// <summary>
	/// �����蔻��̏�����
	/// </summary>
	/// <param name="tmxObj"> tmx </param>
	/// <param name="r"> �����蔻��̔��a </param>
	/// <returns></returns>
	bool InitCollision(std::shared_ptr<TmxObj>& tmxObj, float r);

	/// <summary>
	/// �摜�̏���������
	/// </summary>
	/// <param name="fname"> �t�@�C���l�[�� </param>
	/// <param name="key"> �L�[ </param>
	/// <param name="divSize"> �����T�C�Y </param>
	/// <param name="divNum"> ������ </param>
	/// <param name="idx"> �\������摜�̔Ԗ� </param>
	/// <returns></returns>
	bool InitImage(const std::string& fname, const std::string& key, const Math::Vector2I& divSize, const Math::Vector2I& divNum, int idx);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="offset"> �I�t�Z�b�g </param>
	/// <param name="drawMng"> �`��Ǘ��N���X </param>
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) final;


	ObjID GetObjID() final { return ObjID::Decoration; }
	std::function<void(const Math::Vector2&, DrawMng&)> drawFunc_;
};

