	#pragma once
#include <DxLib.h>
#include <array>
#include "../Scene/SceneID.h"

#define lpSooundPross SoundProcess::GetInstance()

//SE�̎��
enum class SOUNDNAME_SE
{
	playerMove,
	playerDash,
	playerAB,
	playerHit,
	playerAttack,
	playerSpetial,
	playerAttackHit,
	playerJump,
	playerDestory,
	enemyAttack,
	enemyDestroy,
	select,
	openMenu,
	cursorMove,
	click,
	enter,
	Max
};

// BGM�̎��
enum class SOUNDNAME_BGM
{
	TitleSceneBGM,
	TutorialSceneBGM,
	GameSceneStage1BGM,
	GameSceneStage2BGM,
	ResultClearBGM,
	ResultFailBGM,
	Max
};

constexpr int seNum = static_cast<int>(SOUNDNAME_SE::Max) + 1;				// SE�̐�
constexpr int bgmNum = static_cast<int>(SOUNDNAME_BGM::Max) + 1;			// BGM�̐�

class SoundProcess
{
public:

	static SoundProcess& GetInstance(void)
	{
		static SoundProcess s_Instance;
		return s_Instance;
	}

	// ������
	void Init(SceneID nowScene);

	// ���[�h
	void Load(int loadFile, SOUNDNAME_SE name);		// SE�̃��[�h
	void Load(int loadFile, SOUNDNAME_BGM name);	// BGM�̃��[�h

	/// <summary>
	/// SE�̍Đ�
	/// </summary>
	/// <param name="name">SE�̎��</param>
	/// <param name="loopflag">���[�v���邩�ǂ���</param>
	void PlayBackSound(SOUNDNAME_SE name, bool loopflag);

	/// <summary>
	/// BGM�̍Đ�
	/// </summary>
	/// <param name="name">BGM�̎��</param>
	/// <param name="loopflag">���[�v���邩�ǂ���</param>
	/// <param name="topPotisionflag">�r������Đ����邩�ǂ���</param>
	void PlayBackSound(SOUNDNAME_BGM name,bool loopflag, bool topPotisionflag);

	/// <summary>
	/// SE���~�߂�
	/// </summary>
	/// <param name="name">���O</param>
	void SoundStop(SOUNDNAME_SE name);

	/// <summary>
	/// BGM���~�߂�
	/// </summary>
	/// <param name="name">���O</param>
	void SoundStop(SOUNDNAME_BGM name);

	// ���
	void Release();
	void Release(SOUNDNAME_SE name);
	void Release(SOUNDNAME_BGM name);

	// SE�̃��[�U�[���ʒ�����ݒ�
	void SetSEVolumeUserSet(float UserSetVolume);

	// SE�̃��[�U�[���ʒ�����n��
	int GetSEVolumeUserSet();

	// BGM�̃��[�U�[���ʒ�����ݒ�
	void SetBGMVolumeUserSet(float UserSetVolume);

	// BGM�̃��[�U�[���ʒ�����n��
	int GetBGMVolumeUserSet();

	// SE�̃f�t�H���g���ʂ�n��
	int GetDefaultSEVolume();

	// BGM�̃f�t�H���g���ʂ�n��
	int GetDefaultBGMVolume();

private:
	SoundProcess();
	~SoundProcess();

	// �{�����[���֌W
	int volume_;									// �{�����[��
	int defaultSEVol_;								// �f�t�H���g��SE�̃{�����[��
	int defaultBGMVol_ ;							// �f�t�H���g��BGM�̃{�����[��

	// SE�̕ϐ�
	std::array<int, seNum> se_sound;				// se�𐔂����ۑ�
	std::array<bool, seNum> se_loadFlag;			// ���[�h�̃`�F�b�N
	std::array<bool, seNum> se_playFlag;			// �Đ��`�F�b�N
	float se_setUserVol_;							// SE�̃��[�U�[���ʒ���

	// BGM�̕ϐ�
	std::array<int, bgmNum> bgm_sound;				// BGM�𐔂����ۑ�����
	std::array<bool, seNum> bgm_loadFlag;			// ���[�h�̃`�F�b�N
	std::array<bool, seNum> bgm_playFlag;			// �Đ��`�F�b�N
	float bgm_seUserVol_;							// BGM�̃��[�U�[���ʒ���
};