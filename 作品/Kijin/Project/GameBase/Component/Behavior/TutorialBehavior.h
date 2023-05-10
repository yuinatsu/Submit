#pragma once
#include "StageBehavior.h"
#include "../../Common/Vector3.h"

#include "../../Common/SharedHandle.h"

// �`���[�g���A���̎��
enum class TutorialType
{
	Move,				// �ړ�
	View,				// ���_
	Jump,				// �W�����v
	Dash,				// �_�b�V��
	Attack,				// �U��
	Enemy,				// �G
	SkillMess,			// �X�L���̐���
	SkillChargeMess,	// �X�L���𗭂߂邽�߂̃X�|�[��
	SkillCharge,		// �X�L���𗭂߂�
	Skill,				// �X�L���̎g�p
	End,				// �I��
	Max
};


class TutorialBehavior :
    public StageBehavior
{
public:
	TutorialBehavior();
	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::TutorialBehavior };

	const TutorialType GetNowType(void) { return nowType_; }
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Begin(ObjectManager& objectManager) final;

	/// <summary>
	/// �e�A�N�V�����̊Ԃ̑҂����ԗp�A�b�v�f�[�g
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateWait(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �ړ��`���[�g���A���p�A�b�v�f�[�g
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateMove(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// ���_����̃`���[�g���A���p
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateView(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �W�����v�`���[�g���A���p�A�b�v�f�[�g
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateJump(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �U���`���[�g���A���p
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateAttack(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �G��|���`���[�g���A���̓G�̃X�|�[�������p
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateEnemySpawn(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �G��|���`���[�g���A���p
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateEnemy(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �X�L���𗭂߂邽�߂̓G���X�|�[��������p
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateEnemySpawnGage(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �G���X�|�[��������
	/// </summary>
	/// <param name="objectManager"></param>
	/// <param name="pos"></param>
	void SpawnEnemy(ObjectManager& objectManager, const Vector3& pos);

	/// <summary>
	/// �X�L���𗭂߂�p
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateGageSkill(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �X�L���̐���
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateSkillMess(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �X�L���`���[�g���A���p
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateSkill(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	/// <summary>
	/// �`���[�g���A���I�����̍X�V
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="objectManager"></param>
	/// <param name="delta"></param>
	/// <param name="controller"></param>
	void UpdateEnd(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller);

	// �o�ߎ���
	float step_;

	// �҂���
	float waitTime_;

	// ���ꂼ��̃A�N�V�����̊J�n���̃v���C���[�̍��W
	Vector3 playerStartPos_;

	// �X�V����
	void (TutorialBehavior::* update_)(BaseScene&, ObjectManager&, float, Controller&);

	// ���̍X�V����
	void (TutorialBehavior::* nextUpdate_)(BaseScene&, ObjectManager&, float, Controller&);

	// ���݂̃`���[�g���A���^�C�v
	TutorialType nowType_;

	// ���̃`���[�g���A���^�C�v
	TutorialType nextType_;

	// �G��ID
	ObjectID enemyID_;

	// ����ID
	ObjectID arrow_;
};

