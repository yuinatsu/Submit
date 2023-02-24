#pragma once
#include <DxLib.h>
#include <cmath>
#include <string>

namespace SoundProcess
{
	//SEの種類
	enum class SOUNDNAME_SE
	{
		select,
		waring,
		look,
		lost,
		clock,
		walk,
		run,
		crouch,
		slash,
		explosion,
	};

	// BGMの種類
	enum class SOUNDNAME_BGM
	{
		title,
		select,
		stage,
		alarm,
		clear,
		fail,
		none
	};

	// 初期化
	void Init();

	// ロード
	void Load(int loadFile, SOUNDNAME_SE name);		// SEのロード
	void Load(int loadFile, SOUNDNAME_BGM name);	// BGMのロード

	// SEの再生
	void PlayBackSound(SOUNDNAME_SE name, float volume,bool loopflag);

	// BGMの再生
	void PlayBackSound(SOUNDNAME_BGM name, float volume, bool loopflag);

	// SEを止める
	void SoundStop(SOUNDNAME_SE name);

	// BGMを止める
	void SoundStop(SOUNDNAME_BGM name);

	/// 止めていたSEを再生する ///
	/// <name> 流したいSEを指定 ///
	/// <loopflag> ループをするかを指定 ///
	/// <loopTime> 曲が流れて何秒立ったかの指定 ///
	void SoundPlay(SOUNDNAME_SE name, bool loopflag, float loopTime);
	
	/// 止めていたBGMを再生する ///
	/// <name> 流したいBGMを指定 ///
	/// <loopflag> ループをするかを指定 ///
	/// <loopTime> 曲が流れて何秒立ったかの指定 ///
	void SoundPlay(SOUNDNAME_BGM name, bool loopflag, float loopTime);

	// 解放
	void Release();						// 格納されているサウンドをすべて消す
	void Release(SOUNDNAME_SE name);	// 特定のSEを消す
	void Release(SOUNDNAME_BGM name);	// 特定のBGMを消す

	// SEのユーザー音量調整を設定
	void SetSEVolumeEntire(float UserSetVolume);

	// SEのユーザー音量調整を渡す
	float GetSEVolumeEntire();

	// BGMのユーザー音量調整を設定
	void SetBGMVolumeEntire(float UserSetVolume);

	// BGMのユーザー音量調整を渡す
	float GetBGMVolumeEntire();

	void SetDefaultVolume(float defaultVol);

	float GetDefaultVolume();

	void SetVolume(float volume);

	float GetVolume();

	const int seNum = 11;			// SEの数
	const int bgmNum = 7;			// BGMの数
}