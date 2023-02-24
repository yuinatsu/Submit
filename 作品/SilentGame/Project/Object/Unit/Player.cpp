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
	// ������
	Init(cntType);
}

Player::~Player()
{
}

bool Player::Init(CntType cntType)
{
	// �v���C���[�̃C���[�W�ƃA�j���[�V�����ǂݍ���
	AddComponent(std::make_unique<Animation>("Resource/Xml/PlayerImage.xml", "Player", "Down", *this));
	AddComponent(std::make_unique<Animation>("Resource/Xml/PlayerCrouchImage.xml", "PlayerCrouch", "Down", *this));
	AddComponent(std::make_unique<Animation>("Resource/Xml/PlayerDashImage.xml", "PlayerDash", "Down", *this));
	spMagName_ = "Player";
	oldSpMag_ = 0.0f;
	speed_ = 325.0f;
	// �R���g���[�����
	if (cntType == CntType::Key)
	{
		// �L�[�{�[�h
		controller_ = std::make_unique<Keyboard>();
	}
	if (cntType == CntType::Pad)
	{
		// �Q�[���p�b�h
		controller_ = std::make_unique<Pad>();
	}
	// �v���C���[�A�N�V�����ǂݍ���
	rapidxml::file<> attach = "Resource/Xml/PlayerAction.xml";
	stateVec_ = attach.vecData();
	stateDoc_.parse<0>(stateVec_.data());
	stateNode_ = stateDoc_.first_node("objectList");
	animNode_ = std::make_unique<animNode>();
	// �A�j���[�V�����N���X�擾
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator));
	SetAttackFlag(false);
	return true;
}

void Player::Update(float delta)
{
	delta_ = delta;
	// �L�[����
	if (!(*controller_)(delta))
	{
		return;
	}
	moveVec_ = Math::zeroVector2<float>;
	// �v���C���[�̍s������
	for (auto node = stateNode_->first_node(); node != nullptr; node = node->next_sibling())
	{
		(*animNode_)(this, node);
	}
	// �A�j���[�V�����X�V
	UpdateComponent(delta);
	// �R���g���[���[�X�V
	controller_->Update(delta);
	// �A�j���[�V�������
	if (isIdle_)
	{
		//DebugLog("�~�܂���");
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
		//DebugLog("�ς����");
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
			//DebugLog("����");
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
	// �v���C���[�\��
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng, pos_ + offset, isIdle_, spMagName_);
	//DrawLine(pos_.x + offset.x, pos_.y + offset.y, dbgTmpPos_.x + offset.x, dbgTmpPos_.y + offset.y, 0xffffff, 2);

	// �m�[�}���}�b�v
	// �萔�o�b�t�@�̊m��
	cBuff = CreateShaderConstantBuffer(sizeof(float) * 4);
	threshold = static_cast<float*>(GetBufferShaderConstantBuffer(cBuff));

	UpdateShaderConstantBuffer(cBuff);
	SetShaderConstantBuffer(cBuff, DX_SHADERTYPE_PIXEL, 0);
	//MyDrawGraph(pos_.x + offset.x,pos_.y + offset.y,);
}