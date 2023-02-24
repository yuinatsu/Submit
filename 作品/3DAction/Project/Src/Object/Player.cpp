#include <string>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Common/SpeechBalloon.h"
#include "../Common/KeyCheck.h"
#include "Planet.h"
#include "Player.h"

Player::Player(SceneManager* manager)
{
	mSceneManager = manager;
	mResourceManager = manager->GetResourceManager();
	mGravityManager = manager->GetGravityManager();

	mAnimationController = nullptr;
	mState = STATE::NONE;
}

void Player::Init(void)
{

	// ���f���̊�{�ݒ�
	mTransform.SetModel(mResourceManager->LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	mTransform.scl = AsoUtility::VECTOR_ONE;
	mTransform.pos = { 0.0f, -30.0f, 0.0f };
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = 
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	mTransform.Update();

	// �ۉe
	mImgShadow = LoadGraph("Data/Image/Shadow.png");

	// ����
	mEffectSmoke = mResourceManager->Load(ResourceManager::SRC::FOOT_SMOKE).mHandleId;

	// ���[�v�G�t�F�N�g
	mEffectWarpOrbit = mResourceManager->Load(ResourceManager::SRC::WARP_ORBIT).mHandleId;

	// ���f���t���[��
	mFrameLeftHand = MV1SearchFrame(mTransform.modelId, "mixamorig:LeftHand");
	mFrameRightHand = MV1SearchFrame(mTransform.modelId, "mixamorig:RightHand");

	// �W�����v�n�ϐ��̏�����
	mIsJump = false;
	mStepJump = 0.0f;
	mJumpPow = AsoUtility::VECTOR_ZERO;

	// �J�v�Z���R���C�_�̐ݒ�
	mCapsule = new Capsule(&mTransform);
	mCapsule->SetRelativePosTop({ 0.0f, 110.0f, 0.0f });
	mCapsule->SetRelativePosDown({ 0.0f, 30.0f, 0.0f });
	mCapsule->SetRadius(20.0f);

	// �A�j���[�V�����̐ݒ�
	InitAnimation();


	ChangeState(STATE::PLAY);
}

void Player::InitAnimation(void)
{

	std::string path = "Data/Model/Player/";
	mAnimationController = new AnimationController(mSceneManager, mTransform.modelId);
	mAnimationController->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	mAnimationController->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);

	mAnimationController->Play((int)ANIM_TYPE::IDLE);

}

void Player::Update(void)
{

	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		UpdatePlay();
		break;
	case Player::STATE::WARP_RESERVE:
		UpdateWarpReserve();
		break;
	case Player::STATE::WARP_MOVE:
		UpdateWarpMove();
		break;
	case Player::STATE::DEAD:
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

	mTransform.Update();
	mAnimationController->Update();
	
}

void Player::UpdatePlay(void)
{
	// �ړ�����
	ProcessMove();

	// �W�����v����
	ProcessJump();

	// �X�Όv�Z
	CalcSlope();

	// �d�͂ɂ��ړ��ʌv�Z
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// �U�������������]�ɏ��X�ɉ�]���Ă���
	mTransform.quaRot = mGravityManager->GetTransform()->quaRot;
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);

	// �����G�t�F�N�g�i�����j
	EffectFootSmoke();
}

void Player::Draw(void)
{
	DrawShadow();

	// ���f���̕`��
	MV1DrawModel(mTransform.modelId);

	// �f�o�b�O�p�`��
	DrawDebug();
}

void Player::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT = 300.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	int i, j;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	int ModelHandle;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �e�𗎂Ƃ����f���̐������J��Ԃ�
	for (auto c : mColliders)
	{
		ModelHandle = c->mModelId;

		// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
		HitResDim = MV1CollCheck_Capsule(
			ModelHandle, -1, 
			mTransform.pos, VAdd(mTransform.pos, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

		// ���_�f�[�^�ŕω��������������Z�b�g
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// �|���S���̍��W�͒n�ʃ|���S���̍��W
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// �|���S���̕s�����x��ݒ肷��
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
			Vertex[0].u = (HitRes->Position[0].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// �e�|���S����`��
			DrawPolygon3D(Vertex, 1,mImgShadow, TRUE);
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);
}

void Player::DrawDebug(void)
{

	int white = 0xffffff;
	int black = 0x000000;
	int red = 0xff0000;
	int green = 0x00ff00;
	int blue = 0x0000ff;
	int yellow = 0xffff00;
	int purpl = 0x800080;

	VECTOR v;

	// �L������{���
	//-------------------------------------------------------
	// �L�������W
	v = mTransform.pos;
	DrawFormatString(20, 60, black, "�L�������W �F (%0.2f, %0.2f, %0.2f)", v.x, v.y, v.z);
	//-------------------------------------------------------

	// �Փˁi�d�́j�p�̐���
	DrawLine3D(mGravHitUp, mGravHitDown, 0x000000);

	// �W�����v��
	v = mJumpPow;
	DrawFormatString(20, 80, black, "�W�����v�� �F (%0.2f, %0.2f, %0.2)", v.x, v.y, v.z);

	// �J�v�Z���R���C�_
	mCapsule->Draw();

	// �X�Ίp
	DrawFormatString(20, 100, black, "�X�Ίp �F %0.2f", mSlopeAngleDeg);
}

void Player::Release(void)
{
	DeleteGraph(mImgShadow);

	delete mCapsule;
}

void Player::ProcessMove(void)
{
	mMovePow = AsoUtility::VECTOR_ZERO;

	// �J�����̕������擾
	auto cameraRot = mSceneManager->GetCamera()->GetQuaRotOutX();

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	double rotRad = 0.0;

	if (CheckHitKey(KEY_INPUT_W))
	{
		dir = cameraRot.GetForward();
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		dir = cameraRot.GetBack();
		rotRad = AsoUtility::Deg2RadD(180.0);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		dir = cameraRot.GetLeft();
		rotRad = AsoUtility::Deg2RadD(-90.0);
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		dir = cameraRot.GetRight();
		rotRad = AsoUtility::Deg2RadD(90.0);
	}

	if (!AsoUtility::EqualsVZero(dir))
	{
		if (!mIsJump && IsEndLanding())
		{
			if (CheckHitKey(KEY_INPUT_RSHIFT))
			{
				mSpeed = SPEED_RUN;
				mAnimationController->Play((int)ANIM_TYPE::FAST_RUN);
			}
			else
			{
				mSpeed = SPEED_MOVE;
				mAnimationController->Play((int)ANIM_TYPE::RUN);
			}
		}

		mMoveDir = dir;
		mMovePow = VScale(dir, mSpeed);

		// �U�������������]�����߂�
		SetGoalRotate(rotRad);
	}
	else
	{
		if (!mIsJump && IsEndLanding())
		{
			mAnimationController->Play((int)ANIM_TYPE::IDLE);
		}
	}

	Rotate();
}

void Player::ProcessJump(void)
{
	bool isHitKey = CheckHitKey(KEY_INPUT_BACKSLASH);
	if (isHitKey && (mIsJump || IsEndLanding()))
	{
		if (!mIsJump )
		{
			mAnimationController->Play((int)ANIM_TYPE::JUMP, false, 13.0f, 25.0f);
			mAnimationController->SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		mStepJump += mSceneManager->GetDeltaTime();
		if (mStepJump < JUMP_TIME_IN)
		{
			mJumpPow = VScale(mGravityManager->GetDirUpGravity(), JUMP_POW);
		}
		mIsJump = true;
	}

	if (!isHitKey)
	{
		mStepJump = JUMP_TIME_IN;
	}
}

void Player::SetGoalRotate(double rotRad)
{
	auto cameraRot = mSceneManager->GetCamera()->GetAngles();

	Quaternion axisY = Quaternion::AngleAxis(cameraRot.y + rotRad, AsoUtility::AXIS_Y);

	double angleDiff = Quaternion::Angle(axisY, mGoalQuaRotY);

	// �����������ŏ�����
	if (abs(angleDiff) > 5.0f)
	{
		mStepRotTime = TIME_ROT;
	}

	mGoalQuaRotY = axisY;
}

void Player::Rotate(void)
{
	mStepRotTime -= mSceneManager->GetDeltaTime();
	mPlayerRotY = Quaternion::Slerp(mPlayerRotY, mGoalQuaRotY, (TIME_ROT - mStepRotTime) / TIME_ROT);
}

void Player::CalcSlope(void)
{
	VECTOR gravityUp = mGravityManager->GetDirUpGravity();
	// �d�͂̔��Ε�������n�ʂ̖@�������Ɍ�������]�ʂ��擾
	Quaternion up2GNorQua =
		Quaternion::FromToRotation(gravityUp, mHitNormal);
	// �擾������]�̎��Ɗp�x���擾����
	float angle;
	float* anglePtr = &angle;
	VECTOR axis;
	up2GNorQua.ToAngleAxis(anglePtr, &axis);
	// 90�x�����āA�X�΃x�N�g���ւ̉�]���擾����
	Quaternion slopeQ = Quaternion::AngleAxis(
		angle + AsoUtility::Deg2RadD(90.0), axis);
	// �n�ʂ̌X�ΐ�(��)
	mSlopeDir = slopeQ.PosAxis(gravityUp);
	// �X�΂̊p�x
	mSlopeAngleDeg = AsoUtility::AngleDeg(gravityUp, mSlopeDir);
	// �X�΂ɂ��ړ�
	if (AsoUtility::SqrMagnitude(mJumpPow) == 0.0f)
	{
		float CHECK_ANGLE = 120.0f;
		if (mSlopeAngleDeg >= CHECK_ANGLE)
		{
			float diff = abs(mSlopeAngleDeg - CHECK_ANGLE);
			mSlopePow = VScale(mSlopeDir, diff / 3.0f);
			mMovePow = VAdd(mMovePow, mSlopePow);
		}
	}

}

void Player::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = mGravityManager->GetDirGravity();

	float gravityPow = mGravityManager->GetPower();

	VECTOR gravity = VScale(dirGravity, gravityPow);
	mJumpPow = VAdd(mJumpPow, gravity);

	if (VDot(dirGravity, mJumpPow) >= 0.0f)
	{
		mJumpPow = gravity;
	}
}

void Player::AddCollider(Collider* collider)
{
	mColliders.push_back(collider);
}

void Player::ClearCollider(void)
{
	mColliders.clear();
}

Transform* Player::GetTransform(void)
{
	return &mTransform;
}

void Player::UpdateWarpReserve(void)
{
	mStepWarp -= mSceneManager->GetDeltaTime();
	if (mStepWarp < 0.0f)
	{
		mTransform.quaRot = mWarpQua;
		mTransform.pos = mWarpReservePos;
		ChangeState(STATE::WARP_MOVE);
		return;
	}
	else
	{
		float t = 1.0f - (mStepWarp / mTimeWarp);
		mTransform.quaRot = Quaternion::Slerp(mReserveStartQua, mWarpQua, t);
		mTransform.pos = AsoUtility::Lerp(mReserveStartPos, mWarpReservePos, t);
	}
}

void Player::UpdateWarpMove(void)
{
	VECTOR dir = mWarpQua.GetForward();
	mTransform.pos = VAdd(mTransform.pos, VScale(dir, 30.0f));
	Stage::NAME name = mGravityManager->GetActivePlanet()->GetName();
	if (name != mPreWarpName)
	{
		// �G�t�F�N�g��~
		StopOrbitEffect();
		// �����A�j���[�V����
		mAnimationController->Play((int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);
		mAnimationController->SetEndLoop(23.0f, 25.0f, 5.0f);
		ChangeState(Player::STATE::PLAY);
		return;
	}
	mTransform.Update();
	// �G�t�F�N�g�̈ʒu�𓯊�
	SyncWarpOrbitPos();
}

Capsule* Player::GetCapsule(void)
{
	return mCapsule;
}

bool Player::IsPlay(void)
{
	return mState == STATE::PLAY;
}

bool Player::IsWarpMove(void)
{
	return mState == STATE::WARP_MOVE;
}

bool Player::IsAlive(void)
{
	return mState != STATE::DEAD;
}

void Player::StartWarp(float time, Quaternion goalRot, VECTOR goalPos)
{
	mTimeWarp = time;
	mStepWarp = time;
	mWarpQua = goalRot;
	mWarpReservePos = goalPos;
	mPreWarpName = mGravityManager->GetActivePlanet()->GetName();
	ChangeState(STATE::WARP_RESERVE);
}

void Player::StartDead(void)
{
	ChangeState(STATE::DEAD);
}

void Player::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		break;
	case Player::STATE::WARP_RESERVE:
		mJumpPow = AsoUtility::VECTOR_ZERO;
		// ���[�v�����J�n���̃v���C���[���
		mReserveStartQua = mTransform.quaRot;
		mReserveStartPos = mTransform.pos;
		mAnimationController->Play((int)Player::ANIM_TYPE::WARP_PAUSE);
		break;
	case Player::STATE::WARP_MOVE:
		// ���ʂ������Ă���͂��Ȃ̂Ń��Z�b�g
		mPlayerRotY = Quaternion();
		mGoalQuaRotY = Quaternion();
		mAnimationController->Play((int)Player::ANIM_TYPE::FLY);
		// �G�t�F�N�g�Đ�
		EffectWarpOrbit();
		break;
	case Player::STATE::DEAD:
		// �G�t�F�N�g�\���i�u���b�N�z�[���j
		// �L�������[�V�����ύX�iFALL�j
		mAnimationController->Play((int)Player::ANIM_TYPE::FALLING);
		// ���ԊǗ��i5�b�Ƃ�8�b�j
		// �V�[���J�ځi�^�C�g���j
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

}

void Player::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	mMovedPos = VAdd(mTransform.pos, mMovePow);

	// �Փˁi�J�v�Z���j
	CollisionCapsule();

	// �Փˁi�d�́j
	CollisionGravity();

	// �ړ�
	mMoveDiff = VSub(mMovedPos, mTransform.pos);
	mTransform.pos = mMovedPos;
}

void Player::CollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(&mTransform);
	trans.pos = mMovedPos;
	trans.Update();
	Capsule cap = mCapsule->Copy(&trans);

	// �J�v�Z���Ƃ̏Փ˔���
	for (auto c : mColliders)
	{

		auto hits = MV1CollCheck_Capsule(c->mModelId, -1, cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];

			// �|���S���Ƃ̕�����Փ˃`�F�b�N
			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				int pHit = HitCheck_Capsule_Triangle(cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(), hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					mMovedPos = VAdd(mMovedPos, VScale(hit.Normal, 1.0f));
					// �J�v�Z�����ړ�������
					trans.pos = mMovedPos;
					trans.Update();
					cap = mCapsule->Copy(&trans);
					continue;
				}

				break;
			}
		}

		// ���o�����Փ˔���p�̓��I�\���̂̃����������
		MV1CollResultPolyDimTerminate(hits);
	}

}

void Player::CollisionGravity(void)
{
	float checkPow = 10.0f;
	VECTOR dirGravity = mGravityManager->GetDirGravity();
	VECTOR dirUpGravity = mGravityManager->GetDirUpGravity();
	float gravityPow = mGravityManager->GetPower();

	mMovedPos = VAdd(mMovedPos, mJumpPow);
	//mMovedPos = VAdd(mMovedPos, VScale(dirGravity, gravityPow));

	mGravHitUp = VAdd(mMovedPos, VScale(dirUpGravity, gravityPow));
	mGravHitUp = VAdd(mGravHitUp, VScale(dirUpGravity, checkPow * 2.0f));
	mGravHitDown = VAdd(mMovedPos, VScale(dirGravity, checkPow));

	for (auto& c : mColliders)
	{
		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(c->mModelId, -1, mGravHitUp, mGravHitDown);
		if (hit.HitFlag > 0)
		{
			// �X�Όv�Z�p�ɏՓˏ���ۑ����Ă���
			mHitNormal = hit.Normal;
			mHitPos = hit.HitPosition;

			// �d�͕����ɖ@����n��
			//mGravityManager->SetLastHitNormal();

			// �Փ˒n�_���班����Ɉړ�
			mMovedPos = VAdd(hit.HitPosition, VScale(mGravityManager->GetDirUpGravity(), 2.0f));

			// �W�����v���Z�b�g
			mStepJump = 0.0f;
			mJumpPow = AsoUtility::VECTOR_ZERO;
			
			if (mIsJump)
			{
				// ���n���[�V����
				mAnimationController->Play((int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
			}

			mIsJump = false;
		}
	}
}

bool Player::IsEndLanding(void)
{
	bool ret = true;

	if (mAnimationController->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}

	if (mAnimationController->IsEnd())
	{
		return ret;
	}

	return false;
}

void Player::EffectFootSmoke(void)
{
	int playHandle = -1;

	float len = AsoUtility::SqrMagnitude(mMoveDiff);

	mStepFootSmoke++;
	if (mStepFootSmoke % 15 == 0 && len > 1.0f)
	{
		// �G�t�F�N�g���Đ�������
		playHandle = PlayEffekseer3DEffect(mEffectSmoke);

		// �G�t�F�N�g�̑傫��
		SetScalePlayingEffekseer3DEffect(playHandle, 5.0f, 5.0f, 5.0f);

		// �G�t�F�N�g�̈ʒu
		SetPosPlayingEffekseer3DEffect(playHandle, mTransform.pos.x, mTransform.pos.y, mTransform.pos.z);
	}
}

void Player::EffectWarpOrbit(void)
{
	// �G�t�F�N�g�Đ�
	mHandleWarpOrbitL = PlayEffekseer3DEffect(mEffectWarpOrbit);
	mHandleWarpOrbitR = PlayEffekseer3DEffect(mEffectWarpOrbit);
	// �傫��
	SetScalePlayingEffekseer3DEffect(mHandleWarpOrbitL, 10.0f, 10.0f, 10.0f);
	SetScalePlayingEffekseer3DEffect(mHandleWarpOrbitR, 10.0f, 10.0f, 10.0f);
	// �G�t�F�N�g�̈ʒu
	SyncWarpOrbitPos();
	// �G�t�F�N�g�̉�]
	VECTOR euler = mTransform.quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(mHandleWarpOrbitL, euler.x, euler.y, euler.z);
	SetRotationPlayingEffekseer3DEffect(mHandleWarpOrbitR, euler.x, euler.y, euler.z);
}

void Player::SyncWarpOrbitPos(void)
{
	VECTOR pos;
	pos = MV1GetFramePosition(mTransform.modelId, mFrameLeftHand);
	SetPosPlayingEffekseer3DEffect(mHandleWarpOrbitL, pos.x, pos.y, pos.z);
	pos = MV1GetFramePosition(mTransform.modelId, mFrameRightHand);
	SetPosPlayingEffekseer3DEffect(mHandleWarpOrbitR, pos.x, pos.y, pos.z);

}

void Player::StopOrbitEffect(void)
{
	StopEffekseer3DEffect(mHandleWarpOrbitL);
	StopEffekseer3DEffect(mHandleWarpOrbitR);
}
