#include <DxLib.h>
#include "PEManager.h"
#include "PEBase.h"
#include "PEType/NoPE.h"
#include "PEType/MonoPE.h"
#include "PEType/AveblurPE.h"
#include "PEType/GaussPE.h"
#include "PEType/DofPE.h"
#include "PEType/VolfogPE.h"
#include "PEType/ColortoMono.h"

#include "../Common/Debug.h"

PEManager::PEManager()
{
	// �K�v�ȃV�F�[�_��ǂݍ���
	ShaderLoad();
	// �|�X�g�G�t�F�N�g�̃t���O�����ׂ�false�ɂ���
	FlagSetFalse();
	// �񓯊���������x����(�񓯊����ɂ܂킷�ƃt���[�Y���邱�Ƃ�����)
	SetUseASyncLoadFlag(false);
	// �|�X�g�G�t�F�N�g�̏����ݒ���s��
	PostEffect_[0].first = std::make_shared<NoPE>(-1, pos_, rate_);
	PostEffect_[1].first = std::make_shared<MonoPE>(postPS_[0], pos_, rate_);
	PostEffect_[2].first = std::make_shared<AveblurPE>(postPS_[1], pos_, rate_);
	PostEffect_[3].first = std::make_shared<GaussPE>(postPS_[2], pos_, rate_);
	PostEffect_[4].first = std::make_shared<DofPE>(postPS_[3], pos_, rate_);
	PostEffect_[5].first = std::make_shared<VolfogPE>(postPS_[4], pos_, rate_);
	PostEffect_[6].first = std::make_shared<ColortoMono>(postPS_[5], pos_, rate_);
	// �t���O�Ǘ��p�̕ϐ���������
	usePE_.fill(false);
	// ���ׂĂ̏������I������̂ŁA�񓯊�������true�ɂ���
	SetUseASyncLoadFlag(true);
}

PEManager::~PEManager()
{
	// �g�����V�F�[�_������������
	for (auto& ps : postPS_)
	{
		DeleteShader(ps);
	}
}

void PEManager::Update(float delta)
{
#ifdef _DEBUG
	// �f�o�b�O�p�|�X�g�G�t�F�N�g�������Ǝg�p�ł��邩�m�F����p
	CheckPostEffect(PEID::Default,KEY_INPUT_0);
	CheckPostEffect(PEID::Mono, KEY_INPUT_4);
	CheckPostEffect(PEID::Aveblur, KEY_INPUT_5);
	CheckPostEffect(PEID::Gaussian, KEY_INPUT_6);
	CheckPostEffect(PEID::Dof, KEY_INPUT_7);
	CheckPostEffect(PEID::VolFog, KEY_INPUT_8);
	CheckPostEffect(PEID::CtoM, KEY_INPUT_9);
#endif
	// �ꕔ�̃|�X�g�G�t�F�N�g��Update���K�v�Ȃ̂ł��̃|�X�g�G�t�F�N�g��true�Ȃ�܂킷
	for (const auto& post : PostEffect_)
	{
		if (post.second)
		{
			post.first->Update(delta);
		}
	}
}

void PEManager::Draw(int beforeScr, int afterScr, int depth,int skyScr, int redScr)
{
	// �|�X�g�G�t�F�N�g���g�p���Ă��邩�𒲂ׂ�
	for (const auto& post : PostEffect_)
	{
		// ���̃t���O��true���𒲂ׂ�
		if (post.second)
		{
			// ��������ꍇ���l��Draw�����Ă��̏���beforeScr�Ɋi�[����
			beforeScr = post.first->Draw(beforeScr,afterScr,depth,skyScr,redScr);
		}
	}
	// ���C���̃X�N���[���ɐ؂�ւ�
	SetDrawScreen(afterScr);
	ClsDrawScreen();
	// �|�X�g�G�t�F�N�g��K�p�������̂�`�悷��
	DrawGraph(0, 0, beforeScr, true);
}

void PEManager::SetBuffer(int buff)
{
	// �V�F�[�_�Ŏg���o�b�t�@���i�[����
	for (auto post : PostEffect_)
	{
		post.first->SetBuffer(buff);
	}
}

void PEManager::SetFlag(PEID id, bool flag)
{
	for (auto& post : PostEffect_)
	{
		// post.first��id����v���邩���ׂ�
		if (post.first->GetPEID() == id)
		{
			// ture��������flag���i�[����
			post.second = flag;
		}
	}
	/*auto itr = std::find_if(PostEffect_.begin(), PostEffect_.end(), [id](std::pair<std::shared_ptr<PEBase>, bool>& p) { return id == p.first->GetPEID(); });
	if (itr != PostEffect_.end())
	{
		itr->second = flag;
	}*/
}

bool PEManager::GetFlag(PEID id)
{
	// PostEffect_�̒��g�̐������܂킷
	for (const auto& post : PostEffect_)
	{
		// id��post.first������PEID���𒲂ׂ�
		if (post.first->GetPEID() == id)
		{
			// �����ꍇ�̓Z�b�g����Ă���Flag��n��
			return post.second;
		}
	}
	return false;
}

void PEManager::CheckPostEffect(PEID id, int hitkey)
{
	// �L�[�������ꂽ�����擾����
	if (CheckHitKey(hitkey))
	{
		// id�ɑΉ�����t���O��false���ǂ������ׂ�
		if (!usePE_[static_cast<size_t>(id)])
		{
			// false�������̂�true�ɂ���
			usePE_[static_cast<size_t>(id)] = true;
			// ����id�̃t���O���擾����
			bool ownFlag = GetFlag(id);
			if (ownFlag)
			{
				// ture�������ꍇ��false��
				SetFlag(id, false);
			}
			else
			{
				// false�������ꍇ��true�ɂ���
				SetFlag(id, true);
			}
		}
	}
	// �L�[��������Ă��Ȃ��Ńt���O��true�������ꍇ��
	else if(usePE_[static_cast<size_t>(id)])
	{
		// false�ɕς���
		usePE_[static_cast<size_t>(id)] = false;
	}
}

void PEManager::ShaderLoad(void)
{
	// �g�p����V�F�[�_���i�[����
	postPS_[0] = LoadPixelShader(L"Resource/resource/Shader/PostEffect/mono.pso");
	postPS_[1] = LoadPixelShader(L"Resource/resource/Shader/PostEffect/aveblur.pso");
	postPS_[2] = LoadPixelShader(L"Resource/resource/Shader/PostEffect/gauss.pso");
	postPS_[3] = LoadPixelShader(L"Resource/resource/Shader/PostEffect/Dof/dof.pso");
	postPS_[4] = LoadPixelShader(L"Resource/resource/Shader/PostEffect/volFog.pso");
	postPS_[5] = LoadPixelShader(L"Resource/resource/Shader/PostEffect/baw.pso");
}

void PEManager::FlagSetFalse(void)
{
	// ���ׂẴt���O��false�ɂ���
	for (auto& post : PostEffect_)
	{
		post.second = false;
	}
}
