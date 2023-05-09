#pragma once
#include "Render.h"
#include "../../Common/SharedHandle.h"
#include "../../Common/Vector3.h"

struct effectTime
{
	float desEx;					//���S���̔����Đ�����
	float bossDesEx;				//�{�X���S���̃G�t�F�N�g�ƍĐ�����
	float sordLineVertical;			//�����؂�̍Đ�����
	float sordLineHorizontal;		//�����؂�̍Đ�����
	float sordLineThrust;			//�˂��̍Đ�����
};

typedef enum EffectIndex{
	DesEffect,						//���S���̔���(�V���[�g)
	BossDes,						//���S���̔���(�����O)
	SordLineVertical,				//�����؂�̍Đ�����
	SordLineHorizontal,				//�����؂�̍Đ�����
	SordLineThrust,					//�˂��̍Đ�����
	EffectMax,						//�ő�l
}EffectIndex_;
//
////�G�t�F�N�g�̊�{���\����
//typedef struct EffectBaseInit {
//
//	int ImageHandle;
//
//}EffectInit;
//
//typedef struct EffectInfo {
//	//	�G�t�F�N�g�̎g�p�����ǂ����̃t���O
//	bool usingEffect_;
//
//	//�G�t�F�N�g�̏I�����N�G�X�g
//	bool endRequest_;
//
//	//�G�t�F�N�g
//	EffectIndex_ effect_;
//
//	//��{���\���̂̃A�h���X
//	EffectInit* imageInit_;
//
//	// �G�t�F�N�g�ʂ̃f�[�^���i�[���郁�����̈�̃A�h���X
//	void * subDate_;
//
//}EffectInfo_;
//
//typedef struct EffectFunction
//{
//	// ���������ɌĂ΂��֐��|�C���^
//	bool (*Init)(EffectInit *information);
//
//	// ��n����ɌĂ΂��֐��|�C���^
//	void (*Terminate)(EffectInit* information);
//
//	// �쐬���ɌĂ΂��֐��|�C���^
//	bool (*Create)(EffectInfo* info_);
//
//	// �폜��ɌĂ΂��֐��|�C���^
//	void (*Delete)(EffectInfo* info_);
//
//	// ��ԑJ�ڎ��ɌĂ΂��֐��|�C���^
//	void (*Step)(EffectInfo* info_,float stepTime_);
//
//	// �`�掞�ɌĂ΂��֐��|�C���^
//	void (*Draw)(EffectInfo* info_);
//
//}EffectFunction_;

//�G�t�F�N�g�̕`��p�N���X
class EffectRender :
    public Render
{
public:
	EffectRender();
	~EffectRender();

	static constexpr ComponentID id_{ ComponentID::EffectRender };

	static constexpr int EFFECT_MAXNUM = 256;

	ComponentID GetID(void) const override
	{
		return id_;
	}
	
	SharedEffectHandle& GetHandle(void) &
	{
		return handle_;
	}
	
	bool IsEnd(void);

	void Play(void);

	void Stop(void);
protected:
	// �G�t�F�N�g�̃n���h��
	SharedEffectHandle handle_;
private:
	// �G�t�F�N�g�̃C�j�V�����C�Y
	bool Init(void);

	// ���[�h����
	void Load(const std::filesystem::path& path) final;

	// �G�t�F�N�g�̃A�b�v�f�[�g
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;

	// �G�t�F�N�g��Draw����
	void Draw(int shadowMap = -1, int buff = -1) final{}

	void SetUpDepthTex(int ps = -1, int buff = -1) final{}
	
	// �G�t�F�N�g�ɑ΂���I�����N�G�X�g
	//bool EffectEndRequest(EffectInfo* info);

	//// �G�t�F�N�g�̍쐬
	//EffectInfo* EffectCreate(EffectIndex index_);
	//
	// �G�t�F�N�g�̍폜
	//void EffectDeleate(void);

	//// �S�G�t�F�N�g�̍폜
	//void EffectDeleateAll(void);

	//// �S�G�t�F�N�g�̏�ԑJ��
	//void EffectStepAll(float stepTime);

	void UpdateEffekseer(void);

	//void LoadEffect(void);

	void End(ObjectManager& objectManager) final;

	//static EffectFunction_ functionTable[EffectMax] = {};
	/*
	static EffectInit baseInit_[EffectMax];

	static EffectInfo effectInfo_[EFFECT_MAXNUM];
*/


	//�Đ��p�v���C���O�n���h��
	int playingHandle_;
	
	//�G�t�F�N�g�̍Đ�����
	int effectTime_;

	//�G�t�F�N�g�̃��\�[�X�n���h��
	int effectResourceHandle;

	//�G�t�F�N�g�̑��Đ�����
	float timeMax_ = 0.0f;

	//�G�t�F�N�g�̑��Đ����Ԉꗗ
	effectTime effectTimeMax_ = { 120.0f, 315.0f, 60.0f, 40.0f, 40.0f };


	//�G�t�F�N�g�̕`��J�n���W
	Vector3 playPos_;

	//�G�t�F�N�g�̏I���t���O
	bool isEnd_;

	//�G�t�F�N�g����
	//effectTime effecttime_;

	std::vector<SharedEffectHandle>useShaders_;

	

};

