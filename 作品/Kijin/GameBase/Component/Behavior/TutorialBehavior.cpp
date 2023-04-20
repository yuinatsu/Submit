#include "TutorialBehavior.h"
#include "../../Object/ObjectManager.h"
#include "../Transform/Transform.h"
#include "../Behavior/PlayerBehavior.h"
#include "../Info/ObjectInfo.h"
#include "../Behavior/EnemyBehavior.h"
#include "../Render/BillboardRender.h"
#include "../ComponentPool.h"
#include "../../Scene/BaseScene.h"
#include "../../Common/Debug.h"
#include "../../Common/SoundPross.h"

// ���\��������Ƃ��̓G����̍����I�t�Z�b�g
constexpr float arrowOffsetY{ 300.0f };

TutorialBehavior::TutorialBehavior() :
	step_{0.0f}, nextUpdate_{nullptr}, update_{nullptr}, waitTime_{1.0f}
{
	nextType_ = nowType_ = TutorialType::Move;
}

void TutorialBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	step_ += delta;
	UpdateEnemyPosList(objectManager);
	(this->*update_)(scene, objectManager, delta, controller);
}

void TutorialBehavior::Begin(ObjectManager& objectManager)
{
	auto player = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());

	if (player.IsActive())
	{
		playerStartPos_ = player->GetPos();
	}
	nextType_ = nowType_ = TutorialType::Move;
	nextUpdate_ = &TutorialBehavior::UpdateMove;
	update_ = &TutorialBehavior::UpdateMove;
	step_ = 0.0f;
}

void TutorialBehavior::UpdateWait(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (step_ >= waitTime_)
	{
		// ��莞�Ԍo������nextUpdate���ڂ�
		update_ = nextUpdate_;
		step_ = 0.0f;
		nowType_ = nextType_;
	}
}

void TutorialBehavior::UpdateMove(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	auto player = objectManager.GetComponent<Transform>(objectManager.GetPlayerID());
	if (controller.GetLeftInput().SqMagnitude() >= 1.0f)
	{
		// ����
		nextUpdate_ = &TutorialBehavior::UpdateView;
		update_ = &TutorialBehavior::UpdateWait;
		step_ = 0.0f;
		waitTime_ = 2.0f;
		nextType_ = TutorialType::View;
	}
}

void TutorialBehavior::UpdateView(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (step_ >= 2.0f)
	{
		// ����
		nextUpdate_ = &TutorialBehavior::UpdateJump;
		update_ = &TutorialBehavior::UpdateWait;
		step_ = 0.0f;
		waitTime_ = 2.0f;
		nextType_ = TutorialType::Jump;
	}
}

void TutorialBehavior::UpdateJump(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (controller.Press(InputID::Jump))
	{
		// ����
		nextUpdate_ = &TutorialBehavior::UpdateAttack;
		update_ = &TutorialBehavior::UpdateWait;
		step_ = 0.0f;
		nextType_ = TutorialType::Attack;
	}
}

void TutorialBehavior::UpdateAttack(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (controller.Pressed(InputID::Attack))
	{
		// ����
		nextUpdate_ = &TutorialBehavior::UpdateEnemySpawn;
		update_ = &TutorialBehavior::UpdateWait;
		step_ = 0.0f;
		nextType_ = TutorialType::Attack;
		
	}
}

void TutorialBehavior::UpdateEnemySpawn(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	nextUpdate_ = &TutorialBehavior::UpdateEnemy;
	update_ = &TutorialBehavior::UpdateEnemy;
	nextType_ = TutorialType::Enemy;
	nowType_ = TutorialType::Enemy;
	step_ = 0.0f;

	// �G�𐶐�
	enemyID_ = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, zeroVector3<float>);
	objectManager.GetComponent<EnemyBehavior>(enemyID_)->Tutorial();
	objectManager.GetComponent<EnemyBehavior>(enemyID_)->SetStageID(ownerId_);
	objectManager.Begin(enemyID_);
	objectManager.Begin(objectManager.GetComponent<EnemyBehavior>(enemyID_)->GetlaserSightID());
	enemyPosList_.emplace(*enemyID_, zeroVector2<float>);

	// ���𐶐�
	arrow_ = objectManager.MakeObjectID();
	objectManager.AddComponent(objectManager.GetPool().Pop<BillboardRender>(), arrow_);
	objectManager.AddComponent(objectManager.GetPool().Pop<Transform>(), arrow_);
	objectManager.AddComponent(objectManager.GetPool().Pop<ObjectInfo>(), arrow_);
	objectManager.GetComponent<Transform>(arrow_)->Pos() = objectManager.GetComponent<Transform>(enemyID_)->GetPos() + (upVector3<float> *arrowOffsetY);
	objectManager.GetComponent<Render>(arrow_)->Load("Resource/resource/TutorialArrow.png");
	objectManager.Begin(arrow_);
}

void TutorialBehavior::UpdateEnemy(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (!objectManager.IsActive(enemyID_))
	{
		// �G���|���ꂽ��
		objectManager.GetComponent<ObjectInfo>(arrow_)->Destory();
		nextUpdate_ = &TutorialBehavior::UpdateSkillMess;
		update_ = &TutorialBehavior::UpdateSkillMess;
		nextType_ = TutorialType::SkillMess;
		nowType_ = TutorialType::SkillMess;
		step_ = 0.0f;
	}
	else
	{
		// ���̍��W��G�Ɠ���������
		objectManager.GetComponent<Transform>(arrow_)->Pos() = objectManager.GetComponent<Transform>(enemyID_)->GetPos() + (upVector3<float> *arrowOffsetY);
	}
}

void TutorialBehavior::UpdateEnemySpawnGage(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	// �G���X�|�[��������
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			SpawnEnemy(objectManager, { static_cast<float>(x) * 150.0f, 0.0f,static_cast<float>(y) * 150.0f });
		}
	}

	nextUpdate_ = &TutorialBehavior::UpdateGageSkill;
	update_ = &TutorialBehavior::UpdateGageSkill;
}

void TutorialBehavior::SpawnEnemy(ObjectManager& objectManager, const Vector3& pos)
{
	auto id = objectManager.CreateFromFactory(FactoryID::Enemy, ownerId_, pos);

	// �`���[�g���A����Ԃɂ���
	objectManager.GetComponent<EnemyBehavior>(id)->Tutorial();

	// �X�e�[�W�̃I�u�W�F�N�gID���Z�b�g����
	objectManager.GetComponent<EnemyBehavior>(id)->SetStageID(ownerId_);

	objectManager.Begin(id);
	objectManager.Begin(objectManager.GetComponent<EnemyBehavior>(id)->GetlaserSightID());

	// ���X�g�Ɋi�[����
	enemyPosList_.emplace(*id, Vector2{pos.x, pos.z});
}

void TutorialBehavior::UpdateGageSkill(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	auto player = objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID());
	if (player.IsActive())
	{
		const auto& [val,range] = player->GetGaugeValue(UiID::Skill);
		if (val >= range.second)
		{
			// �X�L�������܂�����X�L���̎g�p�̐����ֈȍ~
			nextUpdate_ = &TutorialBehavior::UpdateSkill;
			update_ = &TutorialBehavior::UpdateSkill;
			nextType_ = TutorialType::Skill;
			nowType_ = TutorialType::Skill;
		}
	}
}

void TutorialBehavior::UpdateSkillMess(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (step_ >= 5.0f)
	{
		nextUpdate_ = &TutorialBehavior::UpdateEnemySpawnGage;
		update_ = &TutorialBehavior::UpdateEnemySpawnGage;
		nextType_ = TutorialType::SkillChargeMess;
		nowType_ = TutorialType::SkillChargeMess;
		step_ = 0.0f;
		auto player = objectManager.GetComponent<PlayerBehavior>(objectManager.GetPlayerID());
	}
}

void TutorialBehavior::UpdateSkill(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (controller.Pressed(InputID::Skil))
	{
		// �X�L���g�p��
		nextUpdate_ = &TutorialBehavior::UpdateEnd;
		update_ = &TutorialBehavior::UpdateWait;
		waitTime_ = 3.0f;
		nextType_ = TutorialType::End;
		step_ = 0.0f;
	}
}

void TutorialBehavior::UpdateEnd(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (step_ >= 5.0f)
	{
		// 5�b�o������Z���N�g�֖߂�
		scene.ChangeSceneID(SceneID::Select);

		// �T�E���h�̍폜
		lpSooundPross.Release(SOUNDNAME_BGM::GameSceneStage1BGM);
		lpSooundPross.Release(SOUNDNAME_SE::playerMove);
		lpSooundPross.Release(SOUNDNAME_SE::playerAB);
	}
}
