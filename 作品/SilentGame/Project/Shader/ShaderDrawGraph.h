#pragma once
#include <DxLib.h>
#include <array>
#include <cmath>
#include <vector>

using namespace std;

inline void MyDrawGraph(int x, int y, int firstHandle,int psHandle)
{
	int width;
	int hight;
	array<VERTEX2DSHADER, 4> vertex;

	GetGraphSize(firstHandle, &width, &hight);

	for (auto& v : vertex)
	{
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	// 左上
	vertex[0].rhw = 1.0f;
	vertex[0].pos.x = static_cast<float>(x);
	vertex[0].pos.y = static_cast<float>(y);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;

	// 右上
	vertex[1].rhw = 1.0f;
	vertex[1].pos.x = static_cast<float>(x + width);
	vertex[1].pos.y = static_cast<float>(y);
	vertex[1].u = 1.0f;
	vertex[1].v = 0.0f;

	// 左下
	vertex[2].rhw = 1.0f;
	vertex[2].pos.x = static_cast<float>(x);
	vertex[2].pos.y = static_cast<float>(y + hight);
	vertex[2].u = 0.0f;
	vertex[2].v = 1.0f;

	// 右下
	vertex[3].rhw = 1.0;
	vertex[3].pos.x = static_cast<float>(x + width);
	vertex[3].pos.y = static_cast<float>(y + hight);
	vertex[3].u = 1.0f;
	vertex[3].v = 1.0f;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUsePixelShader(psHandle);
	SetUseTextureToShader(0, firstHandle);
	DrawPrimitive2DToShader(vertex.data(), static_cast<int>(vertex.size()), DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

inline void MyDrawGraph(int x, int y, int firstHandle)
{
	int width;
	int hight;
	array<VERTEX2DSHADER, 4> vertex;

	GetGraphSize(firstHandle, &width, &hight);

	for (auto& v : vertex)
	{
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	// 左上
	vertex[0].rhw = 1.0f;
	vertex[0].pos.x = static_cast<float>(x);
	vertex[0].pos.y = static_cast<float>(y);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;

	// 右上
	vertex[1].rhw = 1.0f;
	vertex[1].pos.x = static_cast<float>(x + width);
	vertex[1].pos.y = static_cast<float>(y);
	vertex[1].u = 1.0f;
	vertex[1].v = 0.0f;

	// 左下
	vertex[2].rhw = 1.0;
	vertex[2].pos.x = static_cast<float>(x);
	vertex[2].pos.y = static_cast<float>(y + hight);
	vertex[2].u = 0.0f;
	vertex[2].v = 1.0f;

	// 右下
	vertex[3].rhw = 1.0;
	vertex[3].pos.x = static_cast<float>(x + width);
	vertex[3].pos.y = static_cast<float>(y + hight);
	vertex[3].u = 1.0f;
	vertex[3].v = 1.0f;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUseTextureToShader(0, firstHandle);
	DrawPrimitive2DToShader(vertex.data(), static_cast<int>(vertex.size()), DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

// テスト用のMyDrawRotaGraph
inline void MyDrawRotaGraph(int x, int y, double rate, double angle, int firstHandle)
{
	int width;
	int hight;
	array<VERTEX2DSHADER, 4> vertex;
	GetGraphSize(firstHandle, &width, &hight);

	for (auto& v : vertex)
	{
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	// 左上
	vertex[0].rhw = 1.0f;
	vertex[0].pos.x = static_cast<float>(x * rate);
	vertex[0].pos.y = static_cast<float>(y * rate);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;

	// 右上
	vertex[1].rhw = 1.0f;
	vertex[1].pos.x = static_cast<float>((x + width) * rate);
	vertex[1].pos.y = static_cast<float>(y * rate);
	vertex[1].u = 1.0f;
	vertex[1].v = 0.0f;

	// 左下
	vertex[2].rhw = 1.0f;
	vertex[2].pos.x = static_cast<float>(x * rate);
	vertex[2].pos.y = static_cast<float>((y + hight) * rate);
	vertex[2].u = 0.0f;
	vertex[2].v = 1.0f;

	// 右下
	vertex[3].rhw = 1.0f;
	vertex[3].pos.x = static_cast<float>((x + width) * rate);
	vertex[3].pos.y = static_cast<float>((y + hight) * rate);
	vertex[3].u = 1.0f;
	vertex[3].v = 1.0f;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUseTextureToShader(0, firstHandle);
	DrawPrimitive2DToShader(vertex.data(), static_cast<int>(vertex.size()), DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

inline void MyDrawRotaGraph(int x, int y, double rate, double angle, int firstHandle, int psHandle)
{
	int width;
	int hight;
	array<VERTEX2DSHADER, 4> vertex;
	GetGraphSize(firstHandle, &width, &hight);

	for (auto& v : vertex)
	{
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	// 左上
	vertex[0].rhw = 1.0f;
	vertex[0].pos.x = static_cast<float>(x * rate);
	vertex[0].pos.y = static_cast<float>(y * rate);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;

	// 右上
	vertex[1].rhw = 1.0;
	vertex[1].pos.x = static_cast<float>((x + width) * rate);
	vertex[1].pos.y = static_cast<float>(y * rate);
	vertex[1].u = 1.0f;
	vertex[1].v = 0.0f;

	// 左下
	vertex[2].rhw = 1.0;
	vertex[2].pos.x = static_cast<float>(x * rate);
	vertex[2].pos.y = static_cast<float>((y + hight) * rate);
	vertex[2].u = 0.0f;
	vertex[2].v = 1.0f;

	// 右下
	vertex[3].rhw = 1.0;
	vertex[3].pos.x = static_cast<float>((x + width) * rate);
	vertex[3].pos.y = static_cast<float>((y + hight) * rate);
	vertex[3].u = 1.0f;
	vertex[3].v = 1.0f;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetUsePixelShader(psHandle);
	SetUseTextureToShader(0, firstHandle);
	DrawPrimitive2DToShader(vertex.data(), static_cast<int>(vertex.size()), DX_PRIMTYPE_TRIANGLESTRIP);
}
