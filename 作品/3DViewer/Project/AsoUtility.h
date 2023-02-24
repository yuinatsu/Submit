#pragma once
#include <string>
#include <vector>
#include "DxLib.h"
static class AsoUtility
{

public:

	static std::vector <std::string> Split(std::string& line, char delimiter);

	// ���W�A��(rad)����x(deg)
	static double Rad2DegD(double rad);

	// �x(deg)���烉�W�A��(rad)
	static double Deg2RadD(double deg);

	// ���W�A��(rad)����x(deg)
	static float Rad2DegF(float rad);

	// �x(deg)���烉�W�A��(rad)
	static float Deg2RadF(float deg);

	// 0�`360�x�͈̔͂Ɏ��߂�
	static double DegIn360(double deg);

	// 0(0)�`2��(360�x)�͈̔͂Ɏ��߂�
	static double RadIn2PI(double rad);

	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	static int DirNearAroundRad(float from, float to);
	
	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	static int DirNearAroundDeg(float from, float to);
	
	// ���`���
	static double Lerp(double start, double end, double t);

	// �p�x�̐��`���
	static double LerpDeg(double start, double end, double t);

	// �p�x�̐��`���
	static VECTOR RotXZPos(VECTOR centerPos, VECTOR radiusPos, double rad);

	static double Magnitude(VECTOR v);
	static double Distance(VECTOR v1, VECTOR v2);

};

