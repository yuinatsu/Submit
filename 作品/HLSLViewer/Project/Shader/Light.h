#pragma once
#include "../Common/Vector3.h"

// fBNVCg
struct DirectionLight
{
	Vector3 dir;			// úËûü
	float padding1;			// lß¨
	Vector3 color;			// RGBJ[
	float padding2;			// lß¨
	Vector3 eyePos;			// _ÀW
};
// |CgCg
struct PointLight
{
	Vector3 pos;			// ÀW
	float padding1;			// lß¨
	Vector3 color;			// RGBJ[
	float padding2;			// lß¨
	float inflRange;		// e¿ÍÍ(m)
};
// X|bgCg
struct SpotLight
{
	Vector3 pos;			// ÀW
	float padding1;			// lß¨
	Vector3 color;			// RGBJ[
	float padding2;			// lß¨
	Vector3 dir;			// úËûü
	float padding3;			// lß¨
	float angle;			// úËpx
	float inflRange;		// e¿ÍÍ(m)
};