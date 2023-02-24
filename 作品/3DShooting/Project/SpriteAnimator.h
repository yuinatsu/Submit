#pragma once
#include "DxLib.h"
#include "ResourceManager.h"
class SceneManager;

class SpriteAnimator
{

public:

	static constexpr int CNT_ANIM = 7;
	static constexpr float SPEED_ANIM = 8.0f;

	enum class STATE
	{
		NONE,
		PLAY,
		END
	};

	// �R���X�g���N�^�EInit�ECreate�̋�ʂ���₱�����̂ŁA
	// Init����
	SpriteAnimator(SceneManager* manager, ResourceManager::SRC src, int size, float speed);

	void Create(VECTOR pos);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void);
	void SetPos(VECTOR pos);

	float GetZLen(void);
	void SetZLen(float len);

	bool IsEnd(void);

private:

	SceneManager* mSceneManager;

	// ���
	STATE mState;

	// �摜�n���h��
	int* mImages;

	// �Đ��C���f�b�N�X
	int mIdxAnim;

	// ���W
	VECTOR mPos;

	// �p�x
	VECTOR mAngles;

	// �傫��
	float mSize;

	// �A�j���[�V�����p�X�V�X�e�b�v
	float mStepAnim;

	// �J���������Z����
	float mZLen;

	// ���A�j���[�V������
	int mCntTotalAnim;

	// �A�j���[�V�����X�s�[�h
	float mSpeedAnim;

	// ��ԑJ��
	void ChangeState(STATE state);

};
