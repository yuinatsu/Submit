#include "SoundPross.h"
#include "Debug.h"

namespace SoundProcess
{
	// ボリューム関係
	float volume_ = 255.0f;				// ボリューム
	float defaultSEVol_ = 200.5f;		// 全体的の音量がでかいのでデフォルトを半分程度の音に
	float defaultBGMVol_ = 150.5f;

	// SEの変数
	int se_sound[seNum];				// seを数だけ保存
	bool se_loadFlag[seNum];			// ロードのチェック
	bool se_playFlag[seNum];			// 再生チェック
	float se_volumeAdjustment = 1.0f;	// SEのユーザー音量調整
	int se_volumeSetMaxVolume[seNum];	// SEの指定最大音量

	// BGMの変数
	int bgm_sound[bgmNum];				// BGMを数だけ保存する
	bool bgm_loadFlag[bgmNum];			// ロードのチェック
	bool bgm_playFlag[bgmNum];			// 再生チェック
	float bgm_volumeAdjustment = 1.0f;	// BGMのユーザー音量調整
	int bgm_volumeSetMaxVolume[bgmNum];	// SEの指定最大音量

	void Init()
	{
	}

	void Init(SceneID nowScene)
	{
		if (nowScene == SceneID::Title)
		{
			// BGMのロード
			Load(LoadSoundMem(L"Resource/resource/Sound/TitleSceneBGM.wav"), SoundProcess::SOUNDNAME_BGM::TitleSceneBGM);
			Load(LoadSoundMem(L"Resource/resource/Sound/Select.wav"), SoundProcess::SOUNDNAME_SE::select);
		}

		if (nowScene == SceneID::Game)
		{
			// 3Dサウンド用にする
			SetCreate3DSoundFlag(true);
			Load(LoadSoundMem(L"Resource/resource/Sound/Rocket.wav"), SoundProcess::SOUNDNAME_SE::enemyAttack);
			SetCreate3DSoundFlag(false);
			// ここまで3Dサウンドとする
			
			// ここから2Dサウンド
			Load(LoadSoundMem(L"Resource/resource/Sound/PlayerMoveSound.wav"), SoundProcess::SOUNDNAME_SE::playerMove);
			Load(LoadSoundMem(L"Resource/resource/Sound/dash.wav"), SoundProcess::SOUNDNAME_SE::playerDash);
			Load(LoadSoundMem(L"Resource/resource/Sound/playerHit.wav"), SoundProcess::SOUNDNAME_SE::playerHit);
			Load(LoadSoundMem(L"Resource/resource/Sound/playerAttack.wav"), SoundProcess::SOUNDNAME_SE::playerAttack);
			Load(LoadSoundMem(L"Resource/resource/Sound/playerAttackHit.wav"), SoundProcess::SOUNDNAME_SE::playerAttackHit);
			Load(LoadSoundMem(L"Resource/resource/Sound/playerJump.wav"), SoundProcess::SOUNDNAME_SE::playerJump);
			Load(LoadSoundMem(L"Resource/resource/Sound/playerDestroy.wav"), SoundProcess::SOUNDNAME_SE::playerDestory);
			Load(LoadSoundMem(L"Resource/resource/Sound/enemyDestroy.wav"), SoundProcess::SOUNDNAME_SE::enemyDestroy);
			// BGMのロード
			Load(LoadSoundMem(L"Resource/resource/Sound/GameSceneBGM_1.wav"), SoundProcess::SOUNDNAME_BGM::GameSceneBGM);
		}

		if (nowScene == SceneID::Result)
		{
			Load(LoadSoundMem(L"Resource/resource/Sound/ResultBGM.wav"), SoundProcess::SOUNDNAME_BGM::ResultSceneBGM);
			Load(LoadSoundMem(L"Resource/resource/Sound/Select.wav"), SoundProcess::SOUNDNAME_SE::select);
		}
	}

	void Load(int loadFile, SOUNDNAME_SE name)
	{
		// loadFileに格納
		se_sound[static_cast<int>(name)] = loadFile;	// 種類別にサウンドを分ける
		se_loadFlag[static_cast<int>(name)] = true;		// フラグをtrueにする
	}

	void Load(int loadFile, SOUNDNAME_BGM name)
	{
		// loadFileに格納
		bgm_sound[static_cast<int>(name)] = loadFile;	// 種類別にサウンドを分ける
		bgm_loadFlag[static_cast<int>(name)] = true;	// フラグをtrueにする
	}

	// SEの再生
	void PlayBackSound(SOUNDNAME_SE name, float volume, bool loopflag)
	{
		volume_ = volume;

		// セットされたvolumeを格納する
		volume_ = se_volumeAdjustment * defaultSEVol_;

		// 流すSEの最大音量を決める
		se_volumeSetMaxVolume[static_cast<int>(name)] = static_cast<int>(volume_);

		// SEのフラグをtrueにする
		se_playFlag[static_cast<int>(name)] = true;

		// SEの再生フラグがtrueになっていたら音量を与える
		if (loopflag == false)
		{
			if (se_playFlag[static_cast<int>(name)])
			{
				// 音量を消して再生しなおす
				ChangeVolumeSoundMem(static_cast<int>(volume_), se_sound[static_cast<int>(name)]);
				PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
			}
		}
		else
		{
			if (se_playFlag[static_cast<int>(name)])
			{
				// 音量を消して再生しなおす
				StopSoundMem(se_sound[static_cast<int>(name)]);
				PlaySoundMem(se_sound[static_cast<int>(name)], DX_PLAYTYPE_LOOP);
				ChangeVolumeSoundMem(static_cast<int>(volume_), se_sound[static_cast<int>(name)]);
			}
		}
	}

	// BGMの再生
	void PlayBackSound(SOUNDNAME_BGM name, float volume, bool loopflag)
	{
		volume_ = volume;

		// セットされたvolumeを格納する
		volume_ = bgm_volumeAdjustment * defaultBGMVol_;

		// 流すBGMの最大音量を決める
		bgm_volumeSetMaxVolume[static_cast<int>(name)] = static_cast<int>(volume_);

		// BGMのフラグをtrueにする
		bgm_playFlag[static_cast<int>(name)] = true;

		// BGMの再生フラグがtrueになっていたら音量を与える
		if (loopflag == false)
		{
			if (bgm_playFlag[static_cast<int>(name)])
			{
				// 音量を消して再生しなおす
				ChangeVolumeSoundMem(static_cast<int>(volume_), bgm_sound[static_cast<int>(name)]);
				PlaySoundMem(bgm_sound[static_cast<int>(name)], DX_PLAYTYPE_BACK);
			}
		}
		if (loopflag == true)
		{
			if (bgm_playFlag[static_cast<int>(name)])
			{
				// 音量を消して再生しなおす
				StopSoundMem(bgm_sound[static_cast<int>(name)]);
				PlaySoundMem(bgm_sound[static_cast<int>(name)], DX_PLAYTYPE_LOOP);
				ChangeVolumeSoundMem(static_cast<int>(volume_), bgm_sound[static_cast<int>(name)]);
			}
		}
	}

	void SoundStop(SOUNDNAME_SE name)
	{
		// SEの音量を0にする
		//ChangeVolumeSoundMem(vol, se_sound[static_cast<int>(name)]);
		StopSoundMem(se_sound[static_cast<int>(name)]);
	}

	void SoundStop(SOUNDNAME_BGM name)
	{
		// BGMの音量を0にする
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
		// SEを消す
		for (int i = 0; i != seNum; ++i)
		{
			if (!se_loadFlag[i])
			{
				continue;
			}
			DeleteSoundMem(se_sound[i]);
			se_sound[i] = -1;
		}

		// BGMを消す
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
		// SEを消す
		DeleteSoundMem(se_sound[static_cast<int>(name)]);
		se_sound[static_cast<int>(name)] = -1;
	}

	void Release(SOUNDNAME_BGM name)
	{
		// BGMを消す
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