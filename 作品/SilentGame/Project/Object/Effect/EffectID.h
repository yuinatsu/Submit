#pragma once

/// <summary> エフェクトの種類 </summary>
enum class EffectName
{
	Slash,			// 斬撃
	Explosion,		// 爆発
	MagicSquare,	// 魔法陣
	Particle,		// 粒子
	Light,			// 光
	Up,				// 上昇
	Down,			// 下降
	Poison,			// 毒
	Alarm,			// アラーム
	Smoke,			// 土煙
	Non,
	Max
};

/// <summary> エフェクトの種類（画像サイズx120） </summary>
enum class EffectName_x120
{
	Tornado,		// 竜巻
	BigSlash,
	Max
};

/// <summary> エフェクトの種類（カメラサイズ全体） </summary>
enum class EffectNameWhole
{
	Super,			// 必殺技
	Max
};

static EffectName begin(EffectName) { return EffectName::Slash; }
static EffectName end(EffectName) { return EffectName::Max; }
static EffectName operator++(EffectName& state) { return (state = static_cast<EffectName>(std::underlying_type<EffectName>::type(state) + 1)); }
static EffectName operator*(const EffectName& state) { return state; }

static EffectName_x120 begin(EffectName_x120) { return EffectName_x120::Tornado; }
static EffectName_x120 end(EffectName_x120) { return EffectName_x120::Max; }
static EffectName_x120 operator++(EffectName_x120& state) { return (state = static_cast<EffectName_x120>(std::underlying_type<EffectName_x120>::type(state) + 1)); }
static EffectName_x120 operator*(const EffectName_x120& state) { return state; }

static EffectNameWhole begin(EffectNameWhole) { return EffectNameWhole::Super; }
static EffectNameWhole end(EffectNameWhole) { return EffectNameWhole::Max; }
static EffectNameWhole operator++(EffectNameWhole& state) { return (state = static_cast<EffectNameWhole>(std::underlying_type<EffectNameWhole>::type(state) + 1)); }
static EffectNameWhole operator*(const EffectNameWhole& state) { return state; }
