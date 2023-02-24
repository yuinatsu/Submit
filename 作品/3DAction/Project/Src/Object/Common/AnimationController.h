#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{
	
public :

	struct Animation
	{
		int model;
		int attachNo;
		int animIndex;
		float speed;
		float totalTime;
		float step = 0.0f;
	};

	AnimationController(SceneManager* manager, int modelId);

	// �A�j���[�V�����ǉ�
	void Add(int type, std::string path, float speed);

	// �A�j���[�V�����Đ�
	void Play(int type, bool isLoop = true, 
		float startStep = 0.0f, float endStep = -1.0f, bool isStop = false, bool isForce = false);

	void Update(void);
	void Release(void);

	// �A�j���[�V�����I����ɌJ��Ԃ����[�v�X�e�b�v
	void SetEndLoop(float startStep, float endStep, float speed);

	// �Đ����̃A�j���[�V����
	int GetPlayType(void);

	// �Đ��I��
	bool IsEnd(void);

private :

	SceneManager* mSceneManager;

	int mModelId;
	std::map<int, Animation> mAnimations;

	int mPlayType;
	Animation mPlayAnim;

	// �A�j���[�V���������[�v���邩���Ȃ���
	bool mIsLoop;

	// �A�j���[�V�������~�߂��܂܂ɂ���
	bool mIsStop;

	// �A�j���[�V�����I����ɌJ��Ԃ����[�v�X�e�b�v
	float mStepEndLoopStart;
	float mStepEndLoopEnd;
	float mEndLoopSpeed;
	// �t�Đ�
	float mSwitchLoopReverse;

};

