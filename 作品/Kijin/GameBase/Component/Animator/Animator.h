#pragma once
#include "../ComponentBase.h"
#include <memory>
#include <map>
#include "../ComponentHandle.h"
#include "../Render/ModelRender.h"

// �A�j���[�V�����̍Đ���Ԃ�\���N���X(���f���̃n���h���͕���������int�Ŏ����Ă�)
class AnimationState
{
public:
	virtual ~AnimationState() {}
	virtual AnimationState* Update(AnimationState* state, ObjectManager& objectManager,float delta) = 0;

	// �Đ����p���������� 
	virtual void Init(AnimationState* start, AnimationState* end) = 0;

	// �Đ��I�����Ă��邩 
	virtual bool IsEnd(void) = 0;

	// �Đ����Ԃ����X�^�[�g 
	void ReStart(void);

	// �A�j���[�V�������A�^�b�` 
	void Attach(void);

	// �A�^�b�`�����C���f�b�N�X���擾 
	int GetAttachIdx(void);

	// �A�j���[�V�������f�^�b�` 
	void Detach();

	// ���f���̃n���h�����Z�b�g 
	void SetHandle(int handle);

	// ���f���̃n���h�����擾 
	const int GetHandle(void) const;

	// �A�j���[�V�����̃Z�b�g�A�b�v 
	void SetUpAnim(int idx);

	// �A�j���[�V�����C���f�b�N�X���擾 
	int GetAnimIdx(void);

	
	// �u�����h���ǂ��� 
	virtual bool IsBlend(void) = 0;

	// ���[�v���s�����̃t���O���Z�b�g 
	void SetLoopFlag(bool flag)
	{
		isLoop_ = flag;
	}

	// �Đ����Ԃ̎擾 
	virtual float GetPlayTime(void) { return playTime_; }

	// ���ߕ��Đ����Ԃ̎擾 
	virtual float GetPlayTimeOver(void) { return playTimeOver_; }

	// ���Đ�����(�ŏ�����Ō�܂ł�)���擾 
	virtual float GetTotalTime(void) { return totalTime_; }

	virtual Vector3 GetMovePosition(void) { return pos_; }

protected:

	// �n���h��
	int handle_{-1};

	// �A�^�b�`�����Ƃ��̃C���f�b�N�X
	int attachIdx_{-1};

	// �A�j���[�V�����̃C���f�b�N�X
	int animIdx_{-1};

	// ���[�v���邩�H
	bool isLoop_{false};

	// ���Đ�����
	float totalTime_{0.0f};

	// �Đ�����
	float playTime_{0.0f};

	// �Đ����Ԓ��ߕ�
	float playTimeOver_{ 0.0f };

	Vector3 pos_;
	Vector3 prePos_;
	Vector3 nowPos_;

	int moveAnimFrameIndex_;


};

// �ʏ�Đ����̏��
class NormalState :
	public AnimationState
{
	// ���������� 
	void Init(AnimationState* start, AnimationState* end) final;

	// �X�V���� 
	AnimationState* Update(AnimationState* state, ObjectManager& objectManager, float delta) final;
	
	bool IsEnd(void) final;

	// �u�����h���ǂ��� 
	bool IsBlend(void)
	{
		return false;
	}
};


// �u�����h�Đ����̏��
class BlendState :
	public AnimationState
{
public:

	// �u�����h���Ԃ̃Z�b�g 
	void SetBlendTime(const float blendTime)
	{
		blendTime_ = blendTime;
	}
private:

	// ���������� 
	void Init(AnimationState* start, AnimationState* end) final;

	// �u�����h���ǂ��� 
	bool IsBlend(void)
	{
		return true;
	}

	// �X�V���� 
	AnimationState* Update(AnimationState* state, ObjectManager& objectManager, float delta) final;

	bool IsEnd(void) final;

	float GetTotalTime(void) final { return end_->GetTotalTime(); }
	float GetPlayTime(void) final { return end_->GetPlayTime(); }

	// �u�����h�J�n���̏��
	AnimationState* start_{nullptr};

	// �u�����h�I�����̏��
	AnimationState* end_{nullptr};

	// �u�����h���鎞��
	float blendTime_{1.0f};
};


class Animator :
    public ComponentBase
{
public:
	ComponentID GetID(void) const override
	{
		return id_;
	}
	SetID(ComponentID::Animator, ComponentID::Animator)

	// idx�̃A�j���[�V�����̂ֈڍs 
	void SetNextAnimation(int idx, float blendTime = 1.0f);

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	void Begin(ObjectManager& objectManager) final;

	// �A�j���[�V������ǉ� 
	void AddAnimation(int anitIdx, bool isLoop = true);

	// �A�j���[�V�����̏�Ԃ�\���N���X���擾 
	AnimationState* GetAnimState(void) { return nowState_; }
private:
	// ���݂̃X�e�[�g�N���X
	AnimationState* nowState_{nullptr};

	// �u�����h���Ɏg�p����X�e�[�g�N���X
	std::unique_ptr<BlendState> blendState_;

	// �eindex���ƂɃA�j���[�V������Ԃ̃N���X��ێ�
	std::map<int,std::unique_ptr<AnimationState>> animStates_;

	// �`��N���X
	ComponentHandle<ModelRender> render_;

};



