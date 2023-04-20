#pragma once


enum class ComponentID :
	unsigned long long
{
	Base = 1ull,									// ���ׂĂ̊��
	Info = Base | (1ull << 1),						// �I�u�W�F�N�g�̏��
	Transform = Base | (1ull << 2),				// �g�����X�t�H�[��
	RenderBase = Base | (1ull << 3),				// �`��̊��
	Animator = Base | (1ull << 4),					// �A�j���[�V����
	Collider = Base | (1ull << 5),					// �����蔻��
	BehaviorBase = Base | (1ull << 6),				// �I�u�W�F�N�g�̓���
	ModelRender = RenderBase | (1ull << 7),		// model�̕`��
	SoundSpeaker = Base | (1ull << 8),				// �T�E���h�̍Đ�
	CapsuleRender = RenderBase | (1ull << 9),		// �J�v�Z���̕`��
	PlayerBehavior = BehaviorBase | (1ull << 10),	// �v���C���[�̓���
	MeshCollider = Collider | (1ull << 11),		// ���b�V���ł̓����蔻��
	SphereCollider = Collider | (1ull << 12),		// ���̂ł̓����蔻��
	CapsuleCollider = Collider | (1ull << 13),		// �J�v�Z���ł̓����蔻��
	CharactorCollider = Collider | (1ull << 14),	// �L�����N�^�[�̓����蔻��
	TerrainCollider = Collider | (1ull << 15),		// �n�`�̓����蔻��
	EnemyBehavior = BehaviorBase | (1ull << 16),	// �G�̓���
	EnemyBulletBehavior = BehaviorBase | (1ull << 17),		// �G�̒e�̓���
	StageBehavior = BehaviorBase | (1ull << 18),			// �X�e�[�W�̐���
	PlayerAttackBehavior = BehaviorBase | (1ull << 19),	// �v���C���[�̍U��
	EffectRender = RenderBase | (1ull << 20),				// �G�t�F�N�g�̕`��
	EffectBehavior = BehaviorBase | (1ull << 21),			// �G�t�F�N�g�̐���
	PlayerSkillBehavior = BehaviorBase | (1ull << 22),		// �v���C���[�̕K�E�Z
	TutorialBehavior = BehaviorBase | (1ull << 23),		// �`���[�g���A������p
	BillBoardRender = RenderBase | (1ull << 24),			// �r���{�[�h�`��p
	ThrusterBehavior = BehaviorBase | (1ull << 25),		// �v���C���[�̃X���X�^�[�p
	BeemEffectBehavior = BehaviorBase | (1ull << 26),		// �r�[���G�t�F�N�g����p
	CameraBehavior = BehaviorBase | (1ull << 27),			// �J����
	SkyBehavior = BehaviorBase | (1ull << 28),				// �X�J�C�h�[���̐���p
	LineRender = RenderBase | (1ull << 29),					// ���̕`��p
	EnemyLaserSightBehavior  = BehaviorBase | (1ull << 30),		// �G�̍U�����ɏo�鎋��
	SandSmokeDiffusionBehavior = BehaviorBase | (1ull << 31),	// �����i�L����j
	SandSmokeBehavior = BehaviorBase| (1ull << 32),				// ����
	SandSmokeSlowMoveBehavior = BehaviorBase | (1ull << 33),				// ����
	EnemyAttackBehavior = BehaviorBase | (1ull << 34),						// �G�̋ߐڍU������p
	reserve1 = (1ull << 35),
	reserve2 = (1ull << 36),
	reserve3 = (1ull << 37),
	reserve4 = (1ull << 38),
	Max = ~Base
};


