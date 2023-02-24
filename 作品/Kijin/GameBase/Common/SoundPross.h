#pragma once
#include <DxLib.h>
#include <cmath>
#include <string>

#include "../Scene/SceneID.h"
#include "../Scene/BaseScene.h"
#include "ScreenID.h"

namespace SoundProcess
{
	//SE�̎��
	enum class SOUNDNAME_SE
	{
		playerMove,
		playerDash,
		playerHit,
		playerAttack,
		playerAttackHit,
		playerJump,
		playerDestory,
		enemyAttack,
		enemyDestroy,
		select,
		Max
	};

	// BGM�̎��
	enum class SOUNDNAME_BGM
	{
		TitleSceneBGM,
		GameSceneBGM,
		ResultSceneBGM,
		Max
	};

	// ������
	void Init();
	void Init(SceneID nowScene);


	// ���[�h
	void Load(int loadFile, SOUNDNAME_SE name);		// SE�̃��[�h
	void Load(int loadFile, SOUNDNAME_BGM name);	// BGM�̃��[�h

	// SE�̍Đ�
	void PlayBackSound(SOUNDNAME_SE name, float volume,bool loopflag);

	// BGM�̍Đ�
	void PlayBackSound(SOUNDNAME_BGM name, float volume, bool loopflag);

	// SE���~�߂�
	void SoundStop(SOUNDNAME_SE name);

	// BGM���~�߂�
	void SoundStop(SOUNDNAME_BGM name);

	/// �~�߂Ă���SE���Đ����� ///
	/// <name> ��������SE���w�� ///
	/// <loopflag> ���[�v�����邩���w�� ///
	/// <loopTime> �Ȃ�����ĉ��b���������̎w�� ///
	void SoundPlay(SOUNDNAME_SE name, bool loopflag, float loopTime);
	
	/// �~�߂Ă���BGM���Đ����� ///
	/// <name> ��������BGM���w�� ///
	/// <loopflag> ���[�v�����邩���w�� ///
	/// <loopTime> �Ȃ�����ĉ��b���������̎w�� ///
	void SoundPlay(SOUNDNAME_BGM name, bool loopflag, float loopTime);

	// ���
	void Release();						// �i�[����Ă���T�E���h�����ׂď���
	void Release(SOUNDNAME_SE name);	// �����SE������
	void Release(SOUNDNAME_BGM name);	// �����BGM������

	// SE�̃��[�U�[���ʒ�����ݒ�
	void SetSEVolumeEntire(float UserSetVolume);

	// SE�̃��[�U�[���ʒ�����n��
	float GetSEVolumeEntire();

	// BGM�̃��[�U�[���ʒ�����ݒ�
	void SetBGMVolumeEntire(float UserSetVolume);

	// BGM�̃��[�U�[���ʒ�����n��
	float GetBGMVolumeEntire();

	void SetDefaultVolume(float defaultVol);

	float GetDefaultVolume();

	void SetVolume(float volume);

	float GetVolume();

	const int seNum = 100;			// SE�̐�
	const int bgmNum =100;			// BGM�̐�
}