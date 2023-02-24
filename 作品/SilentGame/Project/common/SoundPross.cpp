#include "SoundPross.h"

namespace SoundProcess
{
	// �{�����[���֌W
	float volume_ = 255.0f;			// �{�����[��
	float defaultSEVol_ = 200.5f;		// �S�̓I�̉��ʂ��ł����̂Ńf�t�H���g�𔼕����x�̉���
	float defaultBGMVol_ = 255.0f;

	// SE�̕ϐ�
	int se_sound[seNum];				// se�𐔂����ۑ�
	bool se_loadFlag[seNum];			// ���[�h�̃`�F�b�N
	bool se_playFlag[seNum];			// �Đ��`�F�b�N
	float se_volumeAdjustment = 1.0f;	// SE�̃��[�U�[���ʒ���
	int se_volumeSetMaxVolume[seNum];	// SE�̎w��ő剹��

	// BGM�̕ϐ�
	int bgm_sound[bgmNum];				// BGM�𐔂����ۑ�����
	bool bgm_loadFlag[bgmNum];			// ���[�h�̃`�F�b�N
	bool bgm_playFlag[bgmNum];			// �Đ��`�F�b�N
	float bgm_volumeAdjustment = 1.0f;	// BGM�̃��[�U�[���ʒ���
	int bgm_volumeSetMaxVolume[bgmNum];	// SE�̎w��ő剹��

	void Init()
	{	
		// �����ŃT�E���h�����[�h����
		Load(LoadSoundMem("Resource/Sound/enter.mp3"), SoundProcess::SOUNDNAME_SE::select);
		Load(LoadSoundMem("Resource/Sound/siren.mp3"), SoundProcess::SOUNDNAME_SE::waring);
		Load(LoadSoundMem("Resource/Sound/lost.mp3"), SoundProcess::SOUNDNAME_SE::lost);
		Load(LoadSoundMem("Resource/Sound/look.mp3"), SoundProcess::SOUNDNAME_SE::look);
		Load(LoadSoundMem("Resource/Sound/clock.mp3"), SoundProcess::SOUNDNAME_SE::clock);
		Load(LoadSoundMem("Resource/Sound/walk.mp3"), SoundProcess::SOUNDNAME_SE::walk);
		Load(LoadSoundMem("Resource/Sound/run.mp3"), SoundProcess::SOUNDNAME_SE::run);
		Load(LoadSoundMem("Resource/Sound/crouch.mp3"), SoundProcess::SOUNDNAME_SE::crouch);
		Load(LoadSoundMem("Resource/Sound/slash.mp3"), SoundProcess::SOUNDNAME_SE::slash);
		Load(LoadSoundMem("Resource/Sound/explosion.mp3"), SoundProcess::SOUNDNAME_SE::explosion);

		// BGM�̃��[�h
		Load(LoadSoundMem("Resource/Sound/TitleScene.mp3"), SoundProcess::SOUNDNAME_BGM::title);
		Load(LoadSoundMem("Resource/Sound/SelectScene.mp3"), SoundProcess::SOUNDNAME_BGM::select);
		Load(LoadSoundMem("Resource/Sound/GameScene_Contact.mp3"), SoundProcess::SOUNDNAME_BGM::alarm);
		Load(LoadSoundMem("Resource/Sound/GameScene_Normal.mp3"), SoundProcess::SOUNDNAME_BGM::stage);
		Load(LoadSoundMem("Resource/Sound/GameClear.mp3"), SoundProcess::SOUNDNAME_BGM::clear);
		Load(LoadSoundMem("Resource/Sound/GameOver.mp3"), SoundProcess::SOUNDNAME_BGM::fail);
	}

	void Load(int loadFile, SOUNDNAME_SE name)
	{
		// loadFile�Ɋi�[
		se_sound[static_cast<int>(name)] = loadFile;	// ��ޕʂɃT�E���h�𕪂���
		se_loadFlag[static_cast<int>(name)] = true;		// �t���O��true�ɂ���
	}

	void Load(int loadFile, SOUNDNAME_BGM name)
	{
		// loadFile�Ɋi�[
		bgm_sound[static_cast<int>(name)] = loadFile;	// ��ޕʂɃT�E���h�𕪂���
		bgm_loadFlag[static_cast<int>(name)] = true;	// �t���O��true�ɂ���
	}

	// SE�̍Đ�
	void PlayBackSound(SOUNDNAME_SE name, float volume, bool loopflag)
	{
		volume_ = volume;

		// �Z�b�g���ꂽvolume���i�[����
		volume_ = se_volumeAdjustment * defaultSEVol_;

		// ����SE�̍ő剹�ʂ����߂�
		se_volumeSetMaxVolume[static_cast<int>(name)] = static_cast<int>(volume_);

		// SE�̃t���O��true�ɂ���
		se_playFlag[static_cast<int>(name)] = true;

		// SE�̍Đ��t���O��true�ɂȂ��Ă����特�ʂ�^����
		if (loopflag == false)
		{
			if (se_playFlag[static_cast<int>(name)])
			{
				// ���ʂ������čĐ����Ȃ���
				ChangeVolumeSoundMem(static_cast<int>(volume_), se_sound[static_cast<int>(name)]);
				PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
			}
		}
		else
		{
			if (se_playFlag[static_cast<int>(name)])
			{
				// ���ʂ������čĐ����Ȃ���
				StopSoundMem(se_sound[static_cast<int>(name)]);
				PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_LOOP);
				ChangeVolumeSoundMem(static_cast<int>(volume_), se_sound[static_cast<int>(name)]);
			}
		}
	}

	// BGM�̍Đ�
	void PlayBackSound(SOUNDNAME_BGM name, float volume, bool loopflag)
	{
		volume_ = volume;

		// �Z�b�g���ꂽvolume���i�[����
		volume_ = bgm_volumeAdjustment * defaultBGMVol_;

		// ����BGM�̍ő剹�ʂ����߂�
		bgm_volumeSetMaxVolume[static_cast<int>(name)] = static_cast<int>(volume_);

		// BGM�̃t���O��true�ɂ���
		bgm_playFlag[static_cast<int>(name)] = true;

		// BGM�̍Đ��t���O��true�ɂȂ��Ă����特�ʂ�^����
		if (loopflag == false)
		{
			if (bgm_playFlag[static_cast<int>(name)])
			{
				// ���ʂ������čĐ����Ȃ���
				ChangeVolumeSoundMem(static_cast<int>(volume_), bgm_sound[static_cast<int>(name)]);
				PlaySoundMem(bgm_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
			}
		}
		if (loopflag == true)
		{
			if (bgm_playFlag[static_cast<int>(name)])
			{
				// ���ʂ������čĐ����Ȃ���
				StopSoundMem(bgm_sound[static_cast<int>(name)]);
				PlaySoundMem(bgm_sound[static_cast<int>(name)], DX_PLAYTYPE_LOOP);
				ChangeVolumeSoundMem(static_cast<int>(volume_), bgm_sound[static_cast<int>(name)]);
			}
		}
	}

	void SoundStop(SOUNDNAME_SE name)
	{
		// SE�̉��ʂ�0�ɂ���
		//ChangeVolumeSoundMem(vol, se_sound[static_cast<int>(name)]);
		StopSoundMem(se_sound[static_cast<int>(name)]);
	}

	void SoundStop(SOUNDNAME_BGM name)
	{
		// BGM�̉��ʂ�0�ɂ���
		//ChangeVolumeSoundMem(vol, bgm_sound[static_cast<int>(name)]);
		StopSoundMem(bgm_sound[static_cast<int>(name)]);
	}
	void SoundPlay(SOUNDNAME_SE name, bool loopflag, float loopTime)
	{
		SetLoopPosSoundMem(static_cast<int>(loopTime * 100), se_sound[static_cast<int>(name)]);
		if (!loopflag)
		{
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK , false);
		}
		else if (loopflag)
		{
			PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_LOOP,false);
		}
		ChangeVolumeSoundMem(static_cast<int>(se_volumeAdjustment * 255), se_sound[static_cast<int>(name)]);
	}
	void SoundPlay(SOUNDNAME_BGM name, bool loopflag, float loopTime)
	{
		SetLoopPosSoundMem(static_cast<int>(loopTime * 100), bgm_sound[static_cast<int>(name)]);
		ChangeVolumeSoundMem(static_cast<int>(bgm_volumeAdjustment * 255), bgm_sound[static_cast<int>(name)]);
		if (!loopflag)
		{
			PlaySoundMem(bgm_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK , false);
		}
		else if (loopflag)
		{
			PlaySoundMem(bgm_sound[static_cast<int>(name)], DX_PLAYTYPE_LOOP,false);
		}
	}
	void Release()
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
			StopSoundMem(bgm_sound[i]);
			DeleteSoundMem(bgm_sound[i]);
			bgm_sound[i] = -1;
		}
	}

	void Release(SOUNDNAME_SE name)
	{
		// SE������
		DeleteSoundMem(se_sound[static_cast<int>(name)]);
		se_sound[static_cast<int>(name)] = -1;
	}

	void Release(SOUNDNAME_BGM name)
	{
		// BGM������
		DeleteSoundMem(se_sound[static_cast<int>(name)]);
		se_sound[static_cast<int>(name)] = -1;
	}

	void SetSEVolumeEntire(float volumeEntire)
	{
		se_volumeAdjustment = volumeEntire;
	}

	float GetSEVolumeEntire()
	{
		return se_volumeAdjustment;
	}

	void SetBGMVolumeEntire(float volumeEntire)
	{
		bgm_volumeAdjustment = volumeEntire;
	}

	float GetBGMVolumeEntire()
	{
		return bgm_volumeAdjustment;
	}

	void SetDefaultVolume(float defaultVol)
	{
		defaultSEVol_ = defaultVol;
	}

	float GetDefaultVolume()
	{
		return defaultSEVol_;
	}

	void SetVolume(float volume)
	{
		volume_ = volume;
	}

	float GetVolume()
	{
		return volume_;
	}
}