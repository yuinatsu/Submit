#include "SoundPross.h"
#include "Debug.h"
void SoundProcess::Load(int loadFile, SOUNDNAME_SE name)
{
	// loadFile�Ɋi�[		
	se_sound[static_cast<int>(name)] = loadFile;	// ��ޕʂɃT�E���h�𕪂���
	se_loadFlag[static_cast<int>(name)] = true;		// �t���O��true�ɂ���
}

void SoundProcess::Load(int loadFile, SOUNDNAME_BGM name)
{
	// loadFile�Ɋi�[
	bgm_sound[static_cast<int>(name)] = loadFile;	// ��ޕʂɃT�E���h�𕪂���
	bgm_loadFlag[static_cast<int>(name)] = true;	// �t���O��true�ɂ���
}

void SoundProcess::Init(SceneID nowScene)
{
	if (nowScene == SceneID::Title)
	{
		// BGM�̃��[�h
		Load(LoadSoundMem(L"Resource/resource/Sound/TitleSceneBGM.wav"), SOUNDNAME_BGM::TitleSceneBGM);
		Load(LoadSoundMem(L"Resource/resource/Sound/Select.wav"), SOUNDNAME_SE::select);
		Load(LoadSoundMem(L"Resource/resource/Sound/click.mp3"), SOUNDNAME_SE::enter);
	}

	if (nowScene == SceneID::Select)
	{
		Load(LoadSoundMem(L"Resource/resource/Sound/openMenu.mp3"), SOUNDNAME_SE::openMenu);
		Load(LoadSoundMem(L"Resource/resource/Sound/cursorMove.mp3"), SOUNDNAME_SE::cursorMove);
		Load(LoadSoundMem(L"Resource/resource/Sound/click.mp3"), SOUNDNAME_SE::click);
	}

	if (nowScene == SceneID::Game)
	{
		// 3D�T�E���h�p�ɂ���
		SetCreate3DSoundFlag(true);
		Load(LoadSoundMem(L"Resource/resource/Sound/Rocket.wav"), SOUNDNAME_SE::enemyAttack);
		SetCreate3DSoundFlag(false);
		// �����܂�3D�T�E���h�Ƃ���

		// ��������2D�T�E���h
		Load(LoadSoundMem(L"Resource/resource/Sound/PlayerMoveSound.wav"), SOUNDNAME_SE::playerMove);
		Load(LoadSoundMem(L"Resource/resource/Sound/dash.wav"), SOUNDNAME_SE::playerDash);
		Load(LoadSoundMem(L"Resource/resource/Sound/afuteburner.wav"), SOUNDNAME_SE::playerAB);
		Load(LoadSoundMem(L"Resource/resource/Sound/playerHit.wav"), SOUNDNAME_SE::playerHit);
		Load(LoadSoundMem(L"Resource/resource/Sound/playerAttack.wav"), SOUNDNAME_SE::playerAttack);
		Load(LoadSoundMem(L"Resource/resource/Sound/beam.wav"), SOUNDNAME_SE::playerSpetial);
		Load(LoadSoundMem(L"Resource/resource/Sound/playerAttackHit.wav"), SOUNDNAME_SE::playerAttackHit);
		Load(LoadSoundMem(L"Resource/resource/Sound/playerJump.wav"), SOUNDNAME_SE::playerJump);
		Load(LoadSoundMem(L"Resource/resource/Sound/playerDestroy.wav"), SOUNDNAME_SE::playerDestory);
		Load(LoadSoundMem(L"Resource/resource/Sound/enemyDestroy.wav"), SOUNDNAME_SE::enemyDestroy);


		// BGM�̃��[�h
		Load(LoadSoundMem(L"Resource/resource/Sound/TutorialSceneBGM.mp3"), SOUNDNAME_BGM::TutorialSceneBGM);
		Load(LoadSoundMem(L"Resource/resource/Sound/GameSceneBGM_1.wav"), SOUNDNAME_BGM::GameSceneStage1BGM);
		Load(LoadSoundMem(L"Resource/resource/Sound/GameSceneBGM_2.mp3"), SOUNDNAME_BGM::GameSceneStage2BGM);
		// �{��BGM�ł͂Ȃ������[�v�������s������BGM�Ƃ���
	}

	if (nowScene == SceneID::Result)
	{
		Load(LoadSoundMem(L"Resource/resource/Sound/Clear.mp3"), SOUNDNAME_BGM::ResultClearBGM);
		Load(LoadSoundMem(L"Resource/resource/Sound/Fail.wav"), SOUNDNAME_BGM::ResultFailBGM);
		Load(LoadSoundMem(L"Resource/resource/Sound/Select.wav"), SOUNDNAME_SE::select);
	}
}

// SE�̍Đ�
void SoundProcess::PlayBackSound(SOUNDNAME_SE name,bool loopflag)
{
	// ���ʂ̒���
	volume_ = se_setUserVol_ * defaultSEVol_;

	// SE�̃t���O��true�ɂ���
	se_playFlag[static_cast<int>(name)] = true;

	// �Đ��\��������Đ�����
	if (!loopflag)
	{
		if (se_playFlag[static_cast<int>(name)])
		{
			// ���ʂ�ݒ肷��
			ChangeVolumeSoundMem(volume_, se_sound[static_cast<int>(name)]);
			// �Đ�����
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		// �Đ��\��������Đ�����
		if (se_playFlag[static_cast<int>(name)])
		{
			// ���ʂ�ݒ肷��
			ChangeVolumeSoundMem(volume_, se_sound[static_cast<int>(name)]);
			// �Đ�����
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_LOOP);

		}
	}
}

// BGM�̍Đ�
void SoundProcess::PlayBackSound(SOUNDNAME_BGM name, bool loopflag, bool topPotisionflag)
{
	// volume�̒���
	volume_ = bgm_seUserVol_ * defaultBGMVol_;

	// BGM�̃t���O��true�ɂ���
	bgm_playFlag[static_cast<int>(name)] = true;

	// ���[�v���邩�ǂ���
	if (!loopflag)
	{
		// �Đ��\��������Đ�����
		if (bgm_playFlag[static_cast<int>(name)])
		{
			// ���ʂ��Z�b�g
			ChangeVolumeSoundMem(volume_, bgm_sound[static_cast<int>(name)]);
			PlaySoundMem(bgm_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		// �Đ��\��������Đ�����
		if (bgm_playFlag[static_cast<int>(name)])
		{
			// ���ʂ��Z�b�g
			ChangeVolumeSoundMem(volume_, bgm_sound[static_cast<int>(name)]);
			// �Đ�����
			PlaySoundMem(bgm_sound[static_cast<int>(name)], DX_PLAYTYPE_LOOP);

		}
	}
}

void SoundProcess::SoundStop(SOUNDNAME_SE name)
{
	// SE���~�߂�
	StopSoundMem(se_sound[static_cast<int>(name)]);
}

void SoundProcess::SoundStop(SOUNDNAME_BGM name)
{
	// BGM���~�߂�
	StopSoundMem(bgm_sound[static_cast<int>(name)]);
}

void SoundProcess::Release()
{
	// SE������
	for (int i = 0; i != seNum; ++i)
	{
		if (!se_loadFlag[i])
		{
			continue;
		}
		DeleteSoundMem(se_sound[i]);
		se_sound[i] = -1;
	}

	// BGM������
	for (int i = 0; i != bgmNum; ++i)
	{
		if (!bgm_loadFlag[i])
		{
			continue;
		}
#ifndef DEBUG
		StopSoundMem(bgm_sound[i]);
#endif
		DeleteSoundMem(bgm_sound[i]);
		bgm_sound[i] = -1;
	}
}

void SoundProcess::Release(SOUNDNAME_SE name)
{
	DeleteSoundMem(se_sound[static_cast<int>(name)]);
}

void SoundProcess::Release(SOUNDNAME_BGM name)
{
	DeleteSoundMem(bgm_sound[static_cast<int>(name)]);
}

void SoundProcess::SetSEVolumeUserSet(float volumeEntire)
{
	se_setUserVol_ = volumeEntire;
}

int SoundProcess::GetSEVolumeUserSet()
{
	return se_setUserVol_;
}

void SoundProcess::SetBGMVolumeUserSet(float volumeEntire)
{
	bgm_seUserVol_ = volumeEntire;
}

int SoundProcess::GetBGMVolumeUserSet()
{
	return bgm_seUserVol_;
}

int SoundProcess::GetDefaultSEVolume()
{
	return defaultSEVol_;
}

int SoundProcess::GetDefaultBGMVolume()
{
	return defaultBGMVol_;
}

SoundProcess::SoundProcess()
{
	// SE�̃f�t�H���g����
	defaultSEVol_ = 255.0f;

	// BGM�̃f�t�H���g����
	defaultBGMVol_ = 255.0f;

}

SoundProcess::~SoundProcess()
{
}
