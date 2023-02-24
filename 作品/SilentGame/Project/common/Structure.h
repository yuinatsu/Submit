#pragma once
#include <list>
#include <string>
#include<array>
#include "Math.h"
#include "MapCollMng.h"

//using namespace Math;

struct Traiangle
{
	Math::Vector2 pos1;
	Math::Vector2 pos2;
	Math::Vector2 pos3;
};

struct Squar
{
	Math::Vector2 sPos;			// ����̍��W
	Math::Vector2 ePos;			// �E��̍��W
	bool flag_;				// �m�F�p
	Squar() :sPos(0.0f, 0.0f), ePos(0.0f, 0.0f),flag_(false) {};
	Squar(const Math::Vector2& spos, const Math::Vector2& epos);
	void Update(const Math::Vector2& pos);
	void Draw(const Math::Vector2& offsetpos);
	bool Getflag(void);
	Math::Vector2 GetsPos(void);
};

struct Rect
{
	Math::Vector2 r1;
	Math::Vector2 r2;
	Math::Vector2 r3;
	Math::Vector2 r4;
	Rect() : r1(0, 0), r2(0, 0), r3(0, 0), r4(0, 0) {};
	Rect(const Math::Vector2& rr1, const Math::Vector2& rr2, const Math::Vector2& rr3, const Math::Vector2& rr4);
};

using Rectlist = std::list<Rect>;

// �`��ƃv���C���[�Ƃ̓����蔻��͑��̕��̃v���O�������Q�l�ɂ��܂����B
// ����ȊO�͎g���₷���悤�ɂ�����ŉ��ǂ��܂���
struct Fan
{
	Math::Vector2 center;				// ���S���W
	Math::Vector2 v1;					// ��`�̒[�x�N�g��
	Math::Vector2 v2;					// ��`�̒[�x�N�g��
	Math::Vector2 vv1;				// ��`�̒[�x�N�g��(�p�x�����߂邽�߂�v1�������ꏊ)
	Math::Vector2 vv2;				// ��`�̒[�x�N�g��()
	Math::Vector2 vf1;				// �ǂɓ������Ă���Ƃ��̐�`�̒[�x�N�g��
	Math::Vector2 vf2;				// �ǂɓ������Ă���Ƃ��̐�`�̒[�x�N�g��

	Rectlist reclist_;
	bool hitFlag;				// �v���C���[�Ɠ������Ă��邩
	Fan() :center(0, 0), v1(0, 0), v2(0, 0), vv1(0, 0), vv2(0, 0), vf1(0, 0), vf2(0, 0), hitFlag(false) {};
	Fan(const Math::Vector2& p, const Math::Vector2& inv1, const Math::Vector2& inv2);
	Fan(const Math::Vector2& p, float r, float angle);
	Fan(const Math::Vector2& p, const Math::Vector2& inv, float angle, ColData col1, ColData col2);
	void Checkhit(const Math::Vector2& pos);
	// �_�Ǝl�p�̓����蔻��
	bool UpdateHitRect(const Math::Vector2& pos);
	bool UpdateHitPos(const Math::Vector2& pos);
	void Draw(const Math::Vector2& offsetpos);
	float Radius(void) const;
	//�Q�̍��W����p�x�����߂�
	float GetAngle(void)const;
	float GetAngleV2(void)const;
	void AddAngle(float angle);
	// �����Ƀv���C���[�������Ă��邩��n��
	bool GetFlag(void);
};

float GetAngle2Vector(const Math::Vector2& v1, const Math::Vector2& v2);
// �������m�̓����蔻��
bool CheckhitLine(Math::Vector2 a1, Math::Vector2 a2, Math::Vector2 b1, Math::Vector2 b2);
// ��`�Ɛ����̓����蔻��
//(�v���C���[�Ƌ�`�ǂ������Ď��J�����̒��S�_�ɋ߂����œ����蔻������邩�ǂ������߂�)
bool RecthitLine(Rect rect, Math::Vector2 b1, Math::Vector2 b2,Math::Vector2 pos);


