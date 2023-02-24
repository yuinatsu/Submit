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
	virtual ~AnimationState(){}

	// 
	virtual AnimationState* Update(AnimationState* state,float delta) = 0;

	/// <summary>
	/// �Đ����p����������
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	virtual void Init(AnimationState* start, AnimationState* end) = 0;

	/// <summary>
	/// �Đ����Ԃ����X�^�[�g����
	/// </summary>
	/// <param name=""></param>
	void ReStart(void);

	/// <summary>
	/// �A�j���[�V�������A�^�b�`����
	/// </summary>
	/// <param name=""></param>
	void Attach(void);

	/// <summary>
	/// �A�^�b�`�����C���f�b�N�X���擾����
	/// </summary>
	/// <param name=""></param>
	int GetAttachIdx(void);

	/// <summary>
	/// �A�j���[�V�������f�^�b�`����
	/// </summary>
	void Detach();

	/// <summary>
	/// ���f���̃n���h�����Z�b�g
	/// </summary>
	/// <param name="handle"></param>
	void SetHandle(int handle);

	/// <summary>
	/// ���f���̃n���h�����擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const int GetHandle(void) const;

	/// <summary>
	/// �A�j���[�V�����̃Z�b�g�A�b�v������
	/// </summary>
	/// <param name="idx"></param>
	void SetUpAnim(int idx);

	/// <summary>
	/// �A�j���[�V�����C���f�b�N�X���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetAnimIdx(void);

	
	/// <summary>
	/// �u�����h���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual bool IsBlend(void) = 0;

	/// <summary>
	/// ���[�v���s�����̃t���O���Z�b�g
	/// </summary>
	/// <param name="flag"> �t���O </param>
	void SetLoopFlag(bool flag)
	{
		isLoop_ = flag;
	}

	/// <summary>
	/// �Đ����Ԃ̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> �Đ����� </returns>
	virtual float GetPlayTime(void) { return playTime_; }

	/// <summary>
	/// ���ߕ��Đ����Ԃ̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���ߕ��Đ����� </returns>
	virtual float GetPlayTimeOver(void) { return playTimeOver_; }

	/// <summary>
	/// ���Đ�����(�ŏ�����Ō�܂ł�)���擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���Đ����� </returns>
	virtual float GetTotalTime(void) { return totalTime_; }

	virtual Vector3 GetMovePosition(void) { return position_; }
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

	Vector3 playerPos_;
	Vector3 position_;
	Vector3 prevPosition_;
	Vector3 nowPosition_;

	int moveAnimFrameIndex_;
};

// �ʏ�Đ����̏��
class NormalState :
	public AnimationState
{
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	void Init(AnimationState* start, AnimationState* end) final;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="state"></param>
	/// <param name="delta"></param>
	/// <returns></returns>
	AnimationState* Update(AnimationState* state, float delta) final;
	
	/// <summary>
	/// �u�����h���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns> �u�����h�̎�true�����łȂ��Ƃ�false </returns>
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

	/// <summary>
	/// �u�����h���Ԃ̃Z�b�g
	/// </summary>
	/// <param name="blendTime"> �u�����h���� </param>
	void SetBlendTime(const float blendTime)
	{
		blendTime_ = blendTime;
	}
private:

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	void Init(AnimationState* start, AnimationState* end) final;

	/// <summary>
	/// �u�����h���ǂ���
	/// </summary>
	/// <param name=""></param>
	/// <returns> �u�����h�̎�true�����łȂ��Ƃ�false </returns>
	bool IsBlend(void)
	{
		return true;
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="state"></param>
	/// <param name="delta"></param>
	/// <returns></returns>
	AnimationState* Update(AnimationState* state, float delta) final;

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

	/// <summary>
	/// idx�̃A�j���[�V�����̂ֈڍs����
	/// </summary>
	/// <param name="idx"></param>
	void SetNextAnimation(int idx, float blendTime = 1.0f);

	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	void Begin(ObjectManager& objectManager) final;

	/// <summary>
	/// �A�j���[�V������ǉ�����
	/// </summary>
	/// <param name="anitIdx"> ���f���̃A�j���[�V�����̃C���f�b�N�X </param>
	/// <param name="isLoop"> ���[�v���邩�H </param>
	void AddAnimation(int anitIdx, bool isLoop = true);

	AnimationState* GetAnimState(void) { return nowState_; }
private:
	// ���݂̃X�e�[�g�N���X
	AnimationState* nowState_;

	// �u�����h���Ɏg�p����X�e�[�g�N���X
	std::unique_ptr<BlendState> blendState_;

	// �eindex���ƂɃA�j���[�V������Ԃ̃N���X��ێ�
	std::map<int,std::unique_ptr<AnimationState>> animStates_;

	// �`��N���X
	ComponentHandle<ModelRender> render_;

};



