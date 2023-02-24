#pragma once

/// <summary> �G�t�F�N�g�̎�� </summary>
enum class EffectName
{
	Slash,			// �a��
	Explosion,		// ����
	MagicSquare,	// ���@�w
	Particle,		// ���q
	Light,			// ��
	Up,				// �㏸
	Down,			// ���~
	Poison,			// ��
	Alarm,			// �A���[��
	Smoke,			// �y��
	Non,
	Max
};

/// <summary> �G�t�F�N�g�̎�ށi�摜�T�C�Yx120�j </summary>
enum class EffectName_x120
{
	Tornado,		// ����
	BigSlash,
	Max
};

/// <summary> �G�t�F�N�g�̎�ށi�J�����T�C�Y�S�́j </summary>
enum class EffectNameWhole
{
	Super,			// �K�E�Z
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
