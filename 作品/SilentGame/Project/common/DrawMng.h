#pragma once
#include <string>
#include <deque>
#include <functional>
#include "Math.h"


// �`��p�f�[�^
struct DrawData
{
	Math::Vector2 pos;		// �`����W
	float depth;			// �[�x�Ɋւ���l(0.0f����O1.0f�����Ƃ��ĕ`�揇�����߂�̂Ɏg��)
	float rot;				// ��]�p�x
	int handle;				// �摜�n���h��
};

// �`�揇�Ԃ��Ǘ�����N���X
class DrawMng
{
public:
	DrawMng(float maxPosY);
	~DrawMng();

	/// <summary> �����ɕ`�悷�� </summary>
	/// <param name="pos"> ������W </param>
	/// <param name="handle"> �摜�̃n���h�� </param>
	void Draw(const Math::Vector2& pos,int handle);

	/// <summary> ���S��ɕ`�悷�� </summary>
	/// <param name="pos"> ���S���W </param>
	/// <param name="rot"> ��]�p�x </param>
	/// <param name="handle"> �摜�̃n���h�� </param>
	void Draw(const Math::Vector2& pos, float rot, int handle);

	/// <summary> ���S��ɐ[�x���w�肵�ĕ`�悷�� </summary>
	/// <param name="pos"> ���S���W </param>
	/// <param name="depth"> �[�x(0.0f����O1.0f����) </param>
	/// <param name="rot"> ��]�p�x </param>
	/// <param name="handle"> �摜�̃n���h�� </param>
	void Draw(const Math::Vector2& pos, float depth, float rot, int handle);

	/// <summary> �L���[�ɂ��߂Ă����`�揈�������s���� </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// �`��̏������e��function�Ɛ[�x���w�肵�ĕ`��L���[�ɓ����
	/// </summary>
	/// <param name="func"> �`�揈����function </param>
	/// <param name="depth"> �`�揇�����߂邽�߂̐[�x(0.0f����O1.0f����) </param>
	void Draw(std::function<void(void)>&& func, float depth);

	/// <summary> y���W�̈ʒu����`�揇�����߂�[�x�����߂� </summary>
	/// <param name="y"></param>
	/// <returns></returns>
	const float GetPosY2Depth(const float y) const { return 1.0f - (y / maxPosY_); }
private:

	using DrawFunc = std::function<void(void)>;

	// �`�揈�����f�[�^�ɂ��ăf�L���[�Ŏ���
	std::deque<std::pair<DrawFunc, float>> drawQueue_;

	
	

	// �������ׂ��L���[�̐�
	int queueCount_;

	// Y���W�̍ő�l
	const float maxPosY_;

};

