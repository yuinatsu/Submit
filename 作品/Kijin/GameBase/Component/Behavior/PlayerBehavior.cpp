#include "../../Scene/BaseScene.h"
#include "PlayerBehavior.h"
#include "../../Common/Input/InputType/Keyboard.h"
#include "../../Common/Input/InputType/Pad.h"
#include "../../Common/Debug.h"
#include "../../Common/Math.h"
#include "../../Common/Camera.h"
#include "../Collider/CharactorCollider.h"
#include "../Info/ObjectInfo.h"
#include "EnemyBehavior.h"
#include "PlayerAttackBehavior.h"

PlayerBehavior::PlayerBehavior(CntType cnt)
{
	Init(cnt);
}

bool PlayerBehavior::Init(CntType cnt)
{
	// 移動
	dodgeTimer_ = 0.0f;
	noDashTime_ = 0.0f;
	isStaminaLoss_ = false;
	// 回転
	angles_ = Vector3{ Deg2Rad(0.0f),Deg2Rad(dir_.up),Deg2Rad(0.0f) };
	// 攻撃
	isAttack_ = false;
	isDodge_ = false;
	combo_ = 0;
	useSkill_ = false;
	// アニメーション
	anim_.push_back(std::pair(AnimIndex::Idle, animtime_.idle));
	anim_.push_back(std::pair(AnimIndex::ForwardMove, animtime_.move));
	anim_.push_back(std::pair(AnimIndex::Attack1, animtime_.attack1));
	anim_.push_back(std::pair(AnimIndex::Attack2, animtime_.attack2));
	anim_.push_back(std::pair(AnimIndex::Attack3, animtime_.attack3));
	// ゲージ
	gauge_.emplace(UiID::Hp, std::pair(200.0f, std::pair(0.0f, 200.0f)));
	gauge_.emplace(UiID::Skill, std::pair(0.0f, std::pair(0.0f, 200.0f)));
	gauge_.emplace(UiID::Stamina, std::pair(200.0f, std::pair(0.0f, 200.0f)));
	atkAnim_ = 0;
	// サウンド
	for (int i = 0; i < static_cast<int>(SoundProcess::SOUNDNAME_SE::Max); i++)
	{
		sound_.push_back(std::pair(static_cast<SoundProcess::SOUNDNAME_SE>(i), false));
	}

	return true;
}

void PlayerBehavior::Begin(ObjectManager& objectManager)
{
	// Y座標地面固定
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	//transform_->Pos().y = 10.0f;
	// モデル情報
	auto modelrender = objectManager.GetComponent<ModelRender>(ownerId_);
	auto& handle = modelrender->GetHandle();
	// アニメーション
	animation_ = objectManager.GetComponent<Animator>(ownerId_);
	for (int i = 0; i <= 13; i++)
	{
		animation_->AddAnimation(i, false);
	}
	// 初期アニメーションセット
	animation_->SetNextAnimation(static_cast<int>(AnimIndex::Idle));
	// 当たり判定
	collider_ = objectManager.GetComponent<CharactorCollider>(ownerId_);
	collider_->SetHitFunc(std::bind(&PlayerBehavior::OnHit, this, std::placeholders::_1, std::placeholders::_2));
	// 音関係
	SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::playerMove, SoundProcess::GetVolume(), true);
}

void PlayerBehavior::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	// 操作制御
	auto speed = ProcessDash(controller,delta);
	collider_->SetSpeed(speed);
	auto rot = ProcessMove(scene, controller, speed);
	ProcessRotate(scene,controller, rot);
	ProcessJump(controller);
	ProcessAttack(controller,objectManager);
	ProcessStamina(controller);
	if (isStaminaLoss_)
	{
		CoolTimer(delta);
	}
	ProcessSkill(controller);
	// アニメーション制御
	Animation(controller);
	// 音関係制御
	Sound(controller);
}

float PlayerBehavior::ProcessMove(BaseScene& scene, Controller& controller, float speed)
{
	auto rot = controller.GetInputVec().GetAngle();
	Camera& camera = scene.GetCamera();
	// 移動
	if (controller.GetInputVec().SqMagnitude() > 0.0f)
	{
		transform_->Pos().x -= sinf(camera.GetAngle().x + rot) * speed;
		transform_->Pos().z -= cosf(camera.GetAngle().x + rot) * speed;
	}
	auto animMove = animation_->GetAnimState()->GetMovePosition();
	transform_->Pos().x -= sinf(camera.GetAngle().x + angles_.x) * animMove.x;
	transform_->Pos().z -= cosf(camera.GetAngle().x + angles_.x) * animMove.x;
	collider_->SetSpeed(movedis_.walk);
	return rot;
}

float PlayerBehavior::ProcessDash(Controller& controller, const float& delta)
{
	auto speed = movedis_.walk;
	if (isStaminaLoss_)
	{
		return speed;
	}
	// 回避制御
	isDodge_ = controller.Pressed(InputID::Dash) ? true
		: (controller.Released(InputID::Dash) || dodgeTimer_ > DodgeLimit) ? false
		: isDodge_;// 何もしない
	// ダッシュ制御
	if (controller.Press(InputID::Dash) && (controller.GetInputVec().SqMagnitude() > 0.0f))
	{
		sound_.at(static_cast<int>(SoundProcess::SOUNDNAME_SE::playerDash)).second = true;
		speed = isDodge_ ? movedis_.dodge : movedis_.dash;
		(dodgeTimer_ >= 0.0f && isDodge_) ? dodgeTimer_ += delta : dodgeTimer_ = 0.0f;
	}
	return speed;
}

void PlayerBehavior::ProcessRotate(BaseScene& scene, Controller& controller, float rot)
{
	// モデルの角度を設定
	transform_->SetRotFromEulerRot(angles_);
	if (!(controller.GetInputVec().SqMagnitude() >0.0f))
	{
		return;
	}

	// 移動方向にキャラクターの角度を徐々に変える
	float radUnitAnglesY = angles_.y;
	Camera& camera = scene.GetCamera();
	float radMoveAnglesY = RadIn2PI(camera.GetAngle().x + rot);

	// 回転量が少ない方の回転方向を取得する（時計回り：１、反時計回り：ー１）
	float aroundDir = DirNearAroundRad(radUnitAnglesY, radMoveAnglesY);

	// true:しきい値以内に合わせる false:回転量を加える
	float diff = abs(radMoveAnglesY - radUnitAnglesY);
	angles_.y = (diff < 0.1f) ? radMoveAnglesY : angles_.y + (SpeedRotRad * aroundDir);

	// sngles_.yも0~360度以内に収める
	angles_.y = RadIn2PI(angles_.y);
	collider_->SetMoveDir(-transform_->GetForward());
}


void PlayerBehavior::ProcessJump(Controller& controller)
{
	// ジャンプ制御
	auto isJump = (controller.Press(InputID::Jump) && !isStaminaLoss_) ? 1
		: !collider_->IsGround() ? -1
		: 0;
	if (isJump)
	{
		if ((transform_->Pos().y > 0.0f && isJump == -1) || (transform_->Pos().y < 1000.0f && isJump == 1))
		{
			jump_.vel += jump_.acc * isJump;
			collider_->SetGravityPow(jump_.vel);
		}
		collider_->SetGroundFlag(false);
		transform_->Pos().y = Clamp(transform_->Pos().y + transform_->GetUp().y * jump_.vel, 0.0f, 1000.0f);
	}
	else
	{
		jump_.vel = 0.0f;
	}
}


void PlayerBehavior::ProcessAttack(Controller& controller, ObjectManager& objectManager)
{
	if(controller.Pressed(InputID::Attack) || controller.Pressed(InputID::Skil))
	{
		// コンボ攻撃
		auto animPlayTime = animation_->GetAnimState()->GetPlayTime();
		auto animTotalTime = animation_->GetAnimState()->GetTotalTime();
		if (animPlayTime >= animTotalTime)
		{
			auto id = objectManager.CreateFromFactory(FactoryID::PlayerAttack, ownerId_, objectManager.GetComponent<Transform>(ownerId_)->GetPos());
			objectManager.Begin(id);
			auto playerAtk = objectManager.GetComponent<PlayerAttackBehavior>(id);
			playerAtk->SetComboNum(atkAnim_ % AtkAnimMax);
			if (!atkAnim_)
			{
				playerAtk->SetAttackStartTime(1.0f);
				playerAtk->SetAttackTime((animTotalTime - 30.0f) / 30.0f);
			}
			else
			{
				playerAtk->SetAttackTime(animTotalTime / 30.0f);
			}
			atkAnim_++;
			isAttack_ = true;
			sound_.at(static_cast<int>(SoundProcess::SOUNDNAME_SE::playerAttack)).second = true;
		}
	}
}

void PlayerBehavior::ProcessStamina(Controller& controller)
{
	auto& value = gauge_.at(UiID::Stamina);
	auto& [min, max] = value.second;
	auto dashFlag = (controller.Press(InputID::Dash) && controller.GetInputVec().SqMagnitude() > 0.0f && !isStaminaLoss_);
	// ダッシュ
	value.first = (dashFlag && isDodge_) ? value.first - RiseValue	// 回避
		: dashFlag ? value.first - RiseValue * 0.5								// ダッシュ
		: value.first;								// 回復
	// ジャンプ
	auto jumpFlag = (controller.Press(InputID::Jump) && !isStaminaLoss_);
	value.first = (jumpFlag && jump_.vel >= 15.0f) ? value.first - RiseValue	// ジャンプ加速後
		: (jumpFlag && jump_.vel < 15.0f) ? value.first - RiseValue * 0.5								// ジャンプ加速中
		: value.first;								// 回復

	if ((!controller.Press(InputID::Jump) && !(controller.Press(InputID::Dash) && (controller.GetInputVec().SqMagnitude() > 0.0f))) || isStaminaLoss_)
	{
		value.first += RiseValue * 0.5f;
	}
	value.first = Clamp(value.first, min, max);
	// スタミナ切れ
	if (value.first <= min)
	{
		isStaminaLoss_ = true;
	}
}

void PlayerBehavior::CoolTimer(float& delta)
{
	// クールタイム
	noDashTime_ += delta;
	if (noDashTime_ >= CoolTime)
	{
		isStaminaLoss_ = false;
		noDashTime_ = 0.0f;
	}
}

void PlayerBehavior::ProcessSkill(Controller& controller)
{
	if (controller.Pressed(InputID::Skil) && useSkill_)
	{
		// 必殺スキル
		//DebugLog("スキル発動");
		// ゲージリセット
		gauge_.at(UiID::Skill).first = 0.0f;
		useSkill_ = false;
	}
}

void PlayerBehavior::RiseSkillValue(void)
{
	auto& value = gauge_.at(UiID::Skill);
	auto& [min, max] = value.second;
	// スキルゲージ上昇
	value.first = Clamp(value.first + RiseValue * 30.0f, min, max);
	// ゲージMAX
	if (value.first >= max)
	{
		useSkill_ = true;
	}
}

std::pair<float, std::pair<float, float>> PlayerBehavior::GetGaugeValue(UiID id) const
{
	return gauge_.at(id);
}


void PlayerBehavior::Animation(Controller& controller)
{
	// コンボ攻撃
	auto index = AnimIndex::Idle;
	if (isAttack_)
	{
		// 攻撃アニメーション
		index = (atkAnim_ % AtkAnimMax == 1) ? AnimIndex::Attack1
			: (atkAnim_ % AtkAnimMax == 2) ? AnimIndex::Attack2
			: (atkAnim_ % AtkAnimMax == 0) ? AnimIndex::Attack3
			: index;
		// 入力受付時間内に入力が無ければコンボ終了
		auto animPlayTime = animation_->GetAnimState()->GetPlayTime();
		auto animTotalTime = animation_->GetAnimState()->GetTotalTime();
		if (animPlayTime >= animTotalTime && animTotalTime != 0.0f)
		{
			auto animPlayTimeOver = animation_->GetAnimState()->GetPlayTimeOver();
			if (animPlayTimeOver >= animTotalTime)
			{
				atkAnim_ = 0;
				combo_ = 0;
				isAttack_ = false;
			}
		}
	}
	else
	{
		index = (controller.GetInputVec().SqMagnitude() > 0.0f) ? AnimIndex::ForwardMove		// 移動
			: (controller.IsNeutral()) ? AnimIndex::Idle		// 立ち
			: index;// 何もしない
	}
	for (auto &tmp : anim_)
	{
		auto& [idx, time] = tmp;
		// indexに対応したアニメーションを入れる
		if (index == idx)
		{
			animation_->SetNextAnimation(static_cast<int>(idx), time);
		}
	}
}


void PlayerBehavior::Sound(Controller& controller)
{
	// 音制御
	for (auto& s : sound_)
	{
		if (s.second)
		{
			SoundProcess::PlayBackSound(s.first, SoundProcess::GetVolume(), false);
			s.second = false;
		}
	}
}

void PlayerBehavior::OnHit(Collider& col, ObjectManager& objectManager)
{
	auto atr = objectManager.GetComponent<ObjectInfo>(col.GetOwnerID())->GetAttribute();
	if (atr == ObjectAttribute::EnemyAttack)
	{
		// HP減らす
		auto power = objectManager.GetComponent<EnemyBehavior>(objectManager.GetEnemyID())->GetEnemyPower();
		auto& value = gauge_.at(UiID::Hp);
		auto& [min, max] = value.second;
		// HP消費
		value.first = Clamp(value.first - power, min, max);
		// HP全損
		if (value.first <= 0.0f)
		{
			// プレイヤー消滅処理
			objectManager.GetComponent<ObjectInfo>(objectManager.GetPlayerID())->Destory();
			sound_.at(static_cast<int>(SoundProcess::SOUNDNAME_SE::playerDestory)).second = true;
		}
		else
		{
			sound_.at(static_cast<int>(SoundProcess::SOUNDNAME_SE::playerHit)).second = true;
		}
	}
}
