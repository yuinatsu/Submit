#include "Player.h"
#include "state/PlayerState.h"
#include "../../common/AnimMng.h"
#include "../../Component/Animation.h"
#include "../../common/ImageMng.h"
#include "../../Input/Pad.h"
#include "../../Input/Keyboard.h"
#include "../../common/rapidxml.hpp"
#include "../../common/rapidxml_utils.hpp"
#include "../../common/Debug.h"
#include "../../common/SoundPross.h"
#include "../../Shader/ShaderDrawGraph.h"
Player::Player(CntType cntType, std::shared_ptr<TmxObj> tmx, Math::Vector2 pos):
	Unit{pos,tmx}
{
	// 初期化
	Init(cntType);
}

Player::~Player()
{
}

bool Player::Init(CntType cntType)
{
	// プレイヤーのイメージとアニメーション読み込み
	AddComponent(std::make_unique<Animation>("Resource/Xml/PlayerImage.xml", "Player", "Down", *this));
	AddComponent(std::make_unique<Animation>("Resource/Xml/PlayerCrouchImage.xml", "PlayerCrouch", "Down", *this));
	AddComponent(std::make_unique<Animation>("Resource/Xml/PlayerDashImage.xml", "PlayerDash", "Down", *this));
	spMagName_ = "Player";
	oldSpMag_ = 0.0f;
	speed_ = 325.0f;
	// コントローラ情報
	if (cntType == CntType::Key)
	{
		// キーボード
		controller_ = std::make_unique<Keyboard>();
	}
	if (cntType == CntType::Pad)
	{
		// ゲームパッド
		controller_ = std::make_unique<Pad>();
	}
	// プレイヤーアクション読み込み
	rapidxml::file<> attach = "Resource/Xml/PlayerAction.xml";
	stateVec_ = attach.vecData();
	stateDoc_.parse<0>(stateVec_.data());
	stateNode_ = stateDoc_.first_node("objectList");
	animNode_ = std::make_unique<animNode>();
	// アニメーションクラス取得
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator));
	SetAttackFlag(false);
	return true;
}

void Player::Update(float delta)
{
	delta_ = delta;
	// キー処理
	if (!(*controller_)(delta))
	{
		return;
	}
	moveVec_ = Math::zeroVector2<float>;
	// プレイヤーの行動処理
	for (auto node = stateNode_->first_node(); node != nullptr; node = node->next_sibling())
	{
		(*animNode_)(this, node);
	}
	// アニメーション更新
	UpdateComponent(delta);
	// コントローラー更新
	controller_->Update(delta);
	// アニメーション状態
	if (isIdle_)
	{
		//DebugLog("止まった");
		if (oldSpMag_ == 0.4f)
		{
			//SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::crouch);
		}
		else if (oldSpMag_ == 0.8f)
		{
			//SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::run);
		}
		else
		{
			SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::walk);
		}
		oldSpMag_ = 0.0f;
	}
	else if (oldSpMag_ != spMag_ && spMag_ != 0.0f)
	{
		//DebugLog("変わった");
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::walk);
		//SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::run);
		//SoundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::crouch);
		if (spMag_ == 0.4f)
		{
			//SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::crouch, SoundProcess::GetVolume(), true);
			spMagName_ = "PlayerCrouch";
		}
		else if (spMag_ == 0.8f)
		{
			//DebugLog("走る");
			//SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::run, SoundProcess::GetVolume(), true);
			spMagName_ = "PlayerDash";
		}
		else
		{
			SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::walk, SoundProcess::GetSEVolumeEntire(), true);
			spMagName_ = "Player";
		}
		oldSpMag_ = spMag_;
	}
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).SetKey(spMagName_);
}

void Player::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	// プレイヤー表示
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng, pos_ + offset, isIdle_, spMagName_);
	//DrawLine(pos_.x + offset.x, pos_.y + offset.y, dbgTmpPos_.x + offset.x, dbgTmpPos_.y + offset.y, 0xffffff, 2);

	// ノーマルマップ
	// 定数バッファの確保
	cBuff = CreateShaderConstantBuffer(sizeof(float) * 4);
	threshold = static_cast<float*>(GetBufferShaderConstantBuffer(cBuff));

	UpdateShaderConstantBuffer(cBuff);
	SetShaderConstantBuffer(cBuff, DX_SHADERTYPE_PIXEL, 0);
	//MyDrawGraph(pos_.x + offset.x,pos_.y + offset.y,);
}