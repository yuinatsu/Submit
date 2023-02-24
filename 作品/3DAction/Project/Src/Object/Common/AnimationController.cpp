#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "AnimationController.h"

AnimationController::AnimationController(SceneManager* manager, int modelId)
{
	mSceneManager = manager;
	mModelId = modelId;

	mPlayType = -1;
	mIsLoop = false;

	mIsStop = false;
	mSwitchLoopReverse = 0.0f;
	mEndLoopSpeed = 0.0f;
	mStepEndLoopStart = 0.0f;
	mStepEndLoopEnd = 0.0f;
}

void AnimationController::Add(int type, std::string path, float speed)
{

	Animation anim;

	anim.model = MV1LoadModel(path.c_str());
	anim.animIndex = type;
	anim.speed = speed;

	if (mAnimations.count(type) == 0)
	{
		// ����ւ�
		mAnimations.emplace(type, anim);
	}
	else
	{
		// �ǉ�
		mAnimations[type].model = anim.model;
		mAnimations[type].animIndex = anim.animIndex;
		mAnimations[type].attachNo = anim.attachNo;
		mAnimations[type].totalTime = anim.totalTime;
	}

}

void AnimationController::Play(int type, bool isLoop, 
	float startStep, float endStep, bool isStop, bool isForce)
{

	if (mPlayType != type || isForce) {

		if (mPlayType != -1)
		{
			// ���f������A�j���[�V�������O��
			mPlayAnim.attachNo = MV1DetachAnim(mModelId, mPlayAnim.attachNo);
		}

		// �A�j���[�V������ʂ�ύX
		mPlayType = type;
		mPlayAnim = mAnimations[type];

		// ������
		mPlayAnim.step = startStep;

		// ���f���ɃA�j���[�V������t����
		mPlayAnim.attachNo = MV1AttachAnim(mModelId, 1, mPlayAnim.model);

		// �A�j���[�V���������Ԃ̎擾
		if (endStep > 0.0f)
		{
			mPlayAnim.totalTime = endStep;
		}
		else
		{
			mPlayAnim.totalTime = MV1GetAttachAnimTotalTime(mModelId, mPlayAnim.attachNo);
		}

		// �A�j���[�V�������[�v
		mIsLoop = isLoop;

		// �A�j���[�V�������Ȃ�
		mIsStop = isStop;

		mStepEndLoopStart = -1.0f;
		mStepEndLoopEnd = -1.0f;
		mSwitchLoopReverse = 1.0f;
	}

}

void AnimationController::Update(void)
{

	// �o�ߎ��Ԃ̎擾
	float deltaTime = mSceneManager->GetDeltaTime();

	if (!mIsStop)
	{
		// �Đ�
		mPlayAnim.step += (deltaTime * mPlayAnim.speed * mSwitchLoopReverse);

		// �A�j���[�V�����I������
		bool isEnd = false;
		if (mSwitchLoopReverse > 0.0f)
		{
			// �ʏ�Đ��̏ꍇ
			if (mPlayAnim.step > mPlayAnim.totalTime)
			{
				isEnd = true;
			}
		}
		else
		{
			// �t�Đ��̏ꍇ
			if (mPlayAnim.step < mPlayAnim.totalTime)
			{
				isEnd = true;
			}
		}

		if (isEnd)
		{
			// �A�j���[�V�������I��������
			if (mIsLoop)
			{
				// ���[�v�Đ�
				if (mStepEndLoopStart > 0.0f)
				{
					// �A�j���[�V�����I����̎w��t���[���Đ�
					mSwitchLoopReverse *= -1.0f;
					if (mSwitchLoopReverse > 0.0f)
					{
						mPlayAnim.step = mStepEndLoopStart;
						mPlayAnim.totalTime = mStepEndLoopEnd;
					}
					else
					{
						mPlayAnim.step = mStepEndLoopEnd;
						mPlayAnim.totalTime = mStepEndLoopStart;
					}
					mPlayAnim.speed = mEndLoopSpeed;
					
				}
				else
				{
					// �ʏ�̃��[�v�Đ�
					mPlayAnim.step = 0.0f;
				}
			}
			else
			{
				// ���[�v���Ȃ�
				mPlayAnim.step = mPlayAnim.totalTime;
			}

		}

	}

	// �A�j���[�V�����ݒ�
	MV1SetAttachAnimTime(mModelId, mPlayAnim.attachNo, mPlayAnim.step);

}

void AnimationController::Release(void)
{
	for (auto& anim : mAnimations)
	{
		MV1DeleteModel(anim.second.model);
	}
}

void AnimationController::SetEndLoop(float startStep, float endStep, float speed)
{
	mStepEndLoopStart = startStep;
	mStepEndLoopEnd = endStep;
	mEndLoopSpeed = speed;
}

int AnimationController::GetPlayType(void)
{
	return mPlayType;
}

bool AnimationController::IsEnd(void)
{

	bool ret = false;

	if (mIsLoop)
	{
		return ret;
	}

	if (mPlayAnim.step >= mPlayAnim.totalTime)
	{
		return true;
	}

	return ret;

}
