#include <DxLib.h>
#include <cmath>
#include <vector>
#include "Math.h"
#include "Structure.h"

namespace {
	VECTOR V2V(const Math::Vector2& v)
	{
		return VGet(v.x, v.y, 0);
	}
}

Fan::Fan(const Math::Vector2& p, const Math::Vector2& inv1, const Math::Vector2& inv2) :
	center(p), v1(inv1), v2(inv2), vv1(inv1), vv2(inv2),vf1(inv1),vf2(inv2), hitFlag(false)
{
}
Fan::Fan(const Math::Vector2& p, float r, float angle):center(p),hitFlag(false)
{
	v1 = Math::Vector2(r, 0);
	v2 = Math::Vector2(r * cos(angle),r * sin(angle));
	vv1 = v1;
	vv2 = v2;
}

Fan::Fan(const Math::Vector2& p, const Math::Vector2& inv, float angle, ColData col1, ColData col2) :
	center(p), v1(inv),hitFlag(false)
{
	v2 = Math::RotateMat(angle) * v1;
	vv1 = v1;
	vv2 = v2;
	for (auto c : col1)
	{
		Math::Vector2 sPos = c.first;
		Math::Vector2 ePos = c.first + c.second;
		reclist_.emplace_back(sPos, Math::Vector2{ ePos.x,sPos.y }, Math::Vector2{ sPos.x,ePos.y }, ePos);
	}
	for (auto c : col2)
	{
		Math::Vector2 sPos = c.first;
		Math::Vector2 ePos = c.first + c.second;
		reclist_.emplace_back(sPos, Math::Vector2{ ePos.x,sPos.y }, Math::Vector2{ sPos.x,ePos.y }, ePos);
	}
}

void Fan::Checkhit(const Math::Vector2& pos)
{
	float diff = (pos - center).Magnitude();
	// 半径外にいるかどうかを調べる
	if (diff > Radius() + 16)
	{
		hitFlag = false;
		return;
	}
	// 中心点が扇形のv1とv2の間に入っていれば確定当たり
	auto v = pos - center;
	auto betAngle = GetAngleV2();
	if (Math::GetAngle2Vector(v1, v) < betAngle) 
	{
		hitFlag = true;
		return;
	}
	// それぞれの線分との距離を測る
	// v1
	if ((v - v1 * Math::Clamp(Dot(v, v1) / v1.SqMagnitude(),0.0f,1.0f)).Magnitude() <= 16)
	{
		hitFlag = true;
		return;
	}
	// v2
	if ((v - v2 * Math::Clamp(Dot(v, v2) / v2.SqMagnitude(), 0.0f, 1.0f)).Magnitude() <= 16)
	{
		hitFlag = true;
		return;
	}
	hitFlag = false;
	return;
}

bool Fan::UpdateHitRect(const Math::Vector2& pos)
{
	for (const auto& r : reclist_)
	{
		if (RecthitLine(r, center, v1 + center, pos))
		{
			hitFlag = false;
			return true;
		}
		if (RecthitLine(r, center, v2 + center, pos))
		{
			hitFlag = false;
			return true;
		}
	}
	return false;
}

bool Fan::UpdateHitPos(const Math::Vector2& pos)
{
	Checkhit(pos);
	if (hitFlag)
	{
		return true;
	}
	return false;
}

void Fan::Draw( const Math::Vector2& offsetpos)
{
	auto color = 0xff0000;
	constexpr float min_angle = DX_PI_F / 36.0f;	// だいたい５度
	float angle = Math::GetAngle2Vector(v1, v2);

	int triangles_num = static_cast<int>(ceil(angle / min_angle));
	std::vector<VERTEX2D> v(3 * triangles_num);

	for (auto& e : v)
	{
		e.rhw = 1.0f;
		e.dif = GetColorU8(color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff, 255);
		e.u = 0;
		e.v = 0;
	}
	auto vstart = v1;
	auto vend = Math::RotateMat(min_angle) * v1;
	for (int i = 0; i < triangles_num; i++)
	{
		if (i < triangles_num - 1)
		{
			v[i * 3 + 0].pos = V2V(center + offsetpos);
			v[i * 3 + 1].pos = V2V(center + offsetpos + vstart);
			v[i * 3 + 2].pos = V2V(center + offsetpos + vend);
		}
		else
		{
			v[i * 3 + 0].pos = V2V(center + offsetpos);
			v[i * 3 + 1].pos = V2V(center + offsetpos + vstart);
			v[i * 3 + 2].pos = V2V(center + offsetpos + v2);
		}
		vstart = vend;
		vend = Math::RotateMat(min_angle) * vstart;
	}
	// 2Dのポリゴンを描画
	DrawPrimitive2D(v.data(), static_cast<int>(v.size()), DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, true);
	
}

float Fan::Radius(void) const
{
	return v1.Magnitude();
}

float Fan::GetAngle(void) const
{
	return Math::GetAngle2Vector(vv1,vv2);
}

float Fan::GetAngleV2(void) const
{
	return Math::GetAngle2Vector(v1, v2);
}

void Fan::AddAngle(float angle)
{
	auto vf1 = v1;
	auto vf2 = v2;
	float tmpanglev1 = Math::GetAngle2Vector(v2, v1);
	float tmpAnglev2 = Math::GetAngle2Vector(v1, v2);
	v1 = Math::RotateMat(tmpanglev1 + angle) * vf2;
	v2 = Math::RotateMat(tmpAnglev2 + angle) * vf1;

	float tmpangle = Math::GetAngle2Vector(vv1, vv2);
	vv2 = Math::RotateMat(tmpangle + angle) * vv1;
}

bool Fan::GetFlag(void)
{
	return hitFlag;
}

float GetAngle2Vector(const Math::Vector2& v1, const Math::Vector2& v2)
{
	auto tmpv1 = v1.Normalized();
	auto tmpv2 = v2.Normalized();
	float dot = Dot(tmpv1, tmpv2);
	float cross = Cross(tmpv1, tmpv2);
	auto angle = atan2(cross, dot);
	if (angle < 0)
	{
		angle += DX_PI_F * 2.0f;
	}
	return angle;
}

bool CheckhitLine(Math::Vector2 a1, Math::Vector2 a2, Math::Vector2 b1, Math::Vector2 b2)
{
	auto f = [](Math::Vector2& v1, Math::Vector2& v2, Math::Vector2& v3)
	{
		float s = (v1.x * v2.y) - (v2.x * v1.y);
		float t = (v1.x * v3.y) - (v3.x * v1.y);
		if (s * t > 0)
		{
			return false;
		}
		return true;
	};
	auto v1 = a2 - a1;
	auto v2 = b1 - a1;
	auto v3 = b2 - a1;
	auto flag1 = f(v1, v2, v3);
	v1 = b2 - b1;
	v2 = a1 - b1;
	v3 = a2 - b1;
	auto flag2 = f(v1, v2, v3);
	if (flag1 && flag2)
	{
		return true;
	}
	return false;
}
/// <summary>
/// 矩形と線分の当たり判定
/// (プレイヤーと壁どちらが近いかで当たり判定をやるかどうか決める)
/// </summary>
/// <param name="rect">矩形</param>
/// <param name="b1">線分の始点</param>
/// <param name="b2">線分の終点</param>
/// <param name="pos">プレイヤーの座標</param>
/// <returns></returns>
bool RecthitLine(Rect rect, Math::Vector2 b1, Math::Vector2 b2, Math::Vector2 pos)
{
	if (CheckhitLine(rect.r1, rect.r2, b1, b2)) { return true; }
	if (CheckhitLine(rect.r2, rect.r4, b1, b2)) { return true; }
	if (CheckhitLine(rect.r4, rect.r3, b1, b2)) { return true; }
	if (CheckhitLine(rect.r3, rect.r1, b1, b2)) { return true; }
	return false;
}



//Math::Vector2 operator*(const Math::Matrix& mat, const Math::Vector2& vec)
//{
//	return MultipleVec(mat,vec);
//}

Rect::Rect(const Math::Vector2& rr1, const Math::Vector2& rr2, const Math::Vector2& rr3, const Math::Vector2& rr4)
{
	r1 = rr1;
	r2 = rr2;
	r3 = rr3;
	r4 = rr4;
}

Squar::Squar(const Math::Vector2& spos, const Math::Vector2& epos)
{
	flag_ = false;
	sPos = spos;
	ePos = epos;
}

void Squar::Update(const Math::Vector2& pos)
{
	flag_ = false;
	auto vec = ePos - sPos;
	auto center = sPos + Math::Vector2(vec.x / 2, vec.y / 2);
	auto saX = abs(pos.x - center.x);
	auto saY = abs(pos.y - center.y);
	if (saX <= 32 && saY <= 32)
	{
		flag_ = true;
	}
}

void Squar::Draw(const Math::Vector2& offsetpos)
{
	auto color = 0x00ff00;
	if (flag_)
	{
		color = 0x00ffff;
	}
	DrawBox(static_cast<int>(sPos.x + offsetpos.x), static_cast<int>(sPos.y + offsetpos.y), static_cast<int>(ePos.x + offsetpos.x), static_cast<int>(ePos.y + offsetpos.y), color, true);
}

bool Squar::Getflag(void)
{
	return flag_;
}

Math::Vector2 Squar::GetsPos(void)
{
	return sPos;
}
