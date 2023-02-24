#pragma once


enum class ComponentID :
	unsigned int
{
	Base = 1,									// ���ׂĂ̊��
	BehaviorBase = Base | (1 << 1),				// �I�u�W�F�N�g�̓���
	Animator = Base | (1 << 2),					// �A�j���[�V����
	Transform = Base | (1 << 3),				// �g�����X�t�H�[��
	Collider = Base | (1 << 4),					// �����蔻��
	RenderBase = Base | (1 << 5),				// �`��̊��
	ModelRender = RenderBase | (1 << 6),		// model�̕`��
	Info = Base |(1 << 7),						// �I�u�W�F�N�g�̏��
	SoundSpeaker = Base | (1 << 8),				// �T�E���h�̍Đ�
	CapsuleRender = RenderBase | (1 << 9),		// �J�v�Z���̕`��
	PlayerBehavior = BehaviorBase | (1 << 10),	// �v���C���[�̓���
	MeshCollider = Collider | (1 << 11),		// ���b�V���ł̓����蔻��
	SphereCollider = Collider | (1 << 12),		// ���̂ł̓����蔻��
	CapsuleCollider = Collider | (1 << 13),		// �J�v�Z���ł̓����蔻��
	CharactorCollider = Collider | (1 << 14),	// �L�����N�^�[�̓����蔻��
	TerrainCollider = Collider | (1 << 15),		// �n�`�̓����蔻��
	EnemyBehavior = BehaviorBase | (1 << 16),	// �G�̓���
	EnemyBulletBehavior = BehaviorBase | (1 << 17),		// �G�̒e�̓���
	StageBehavior = BehaviorBase | (1 << 18),			// �X�e�[�W�̐���
	PlayerAttackBehavior = BehaviorBase | (1 << 19),	// �v���C���[�̍U��
	EffectRender = RenderBase | (1 << 20),				// �G�t�F�N�g�N���X
	EffectBehavior = BehaviorBase | (1 << 21),			// �ꕔ�G�t�F�N�g�̐���p�̃N���X
	Max = ~Base
};


