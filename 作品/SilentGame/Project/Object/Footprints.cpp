#include "../Resource/Tmx/TmxObj.h"
#include "../common/ImageMng.h"
#include "Footprints.h"

// Vector2をDxLibのVECTORに変換する
VECTOR Vector2ToDxVec(const Math::Vector2& vec)
{
	return VGet(vec.x, vec.y, 0.0f);
}


Footprints::Footprints(std::vector<Math::Vector2I>& route, std::shared_ptr<TmxObj>& tmxObj)
{
	auto& tileSize = tmxObj->GetTileSize();
	Math::Vector2 posA;
	Math::Vector2 posB;
	vert_.resize(route.size() * 4);
	ind_.resize(route.size() * 6);
	num_ = 0;
	for (int i = 0; i < static_cast<int>(route.size()); i++)
	{
		if (i >= static_cast<int>(route.size()) - 1)
		{
			posA = static_cast<Math::Vector2>((route[i] * tileSize) + (tileSize / 2));
			posB = static_cast<Math::Vector2>((route[0] * tileSize) + (tileSize / 2));
		}
		else
		{
			posA = static_cast<Math::Vector2>((route[i] * tileSize) + (tileSize/2));
			posB = static_cast<Math::Vector2>((route[i + 1] * tileSize) + (tileSize / 2));
		}
		SetVertexAndIndex(posA, posB);
	}
	lpImageMng.GetID("Resource/image/Footprints.png", "Footprints");
}

Footprints::~Footprints()
{
}

void Footprints::Draw(const Math::Vector2& offset)
{
	// アドレスモードを変更
	SetTextureAddressMode(DX_TEXADDRESS_WRAP);
	
	// 全頂点にカメラのオフセットを計算
	auto mat = MGetTranslate(VGet(offset.x, offset.y, 0.0f));
	for (auto& v : vert_)
	{
		v.pos = VTransform(v.pos, mat);
	}

	// 描画
	DrawPolygonIndexed2D(vert_.data(), static_cast<int>(vert_.size()), ind_.data(), num_ * 2, lpImageMng.GetID("Footprints")[0], true);
	
	// もとに戻す
	mat = MGetTranslate(VGet(-offset.x, -offset.y, 0.0f));
	for (auto& v : vert_)
	{
		v.pos = VTransform(v.pos, mat);
	}

	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);
}

void Footprints::SetVertexAndIndex(const Math::Vector2& start, const Math::Vector2& end)
{
	int startVertIdx = num_ * 4;
	int startIndIdx = num_ * 6;

	auto vecA = end - start;
	vecA.Normalize();
	vecA.Rota(Math::Deg2Rad(-90.0f));

	auto vecB = end - start;
	vecB.Normalize();
	vecB.Rota(Math::Deg2Rad(90.0f));

	Math::Vector2 startBack;
	//startBack.Normalize();

	auto endBack = end - start;
	endBack.Normalize();
	
	// 左上
	vert_[startVertIdx].pos = Vector2ToDxVec(start + ((vecB + startBack) * 16.0f));
	vert_[startVertIdx].u = 0.0f;
	vert_[startVertIdx].v = 0.0f;
	vert_[startVertIdx].rhw = 1.0f;
	vert_[startVertIdx].dif = GetColorU8(255, 255, 255, 255);
	ind_[startIndIdx] = startVertIdx;

	// 右上
	startVertIdx++;
	vert_[startVertIdx].pos = Vector2ToDxVec(start + ((vecA + startBack) * 16.0f));
	vert_[startVertIdx].u = 1.0f;
	vert_[startVertIdx].v = 0.0f;
	vert_[startVertIdx].rhw = 1.0f;
	vert_[startVertIdx].dif = GetColorU8(255, 255, 255, 255);
	ind_[startIndIdx + 1] = startVertIdx;
	ind_[startIndIdx + 3] = startVertIdx;

	// 左下
	startVertIdx++;
	vert_[startVertIdx].pos = Vector2ToDxVec(end + ((vecB + endBack) * 16.0f));
	vert_[startVertIdx].u = 0.0f;
	vert_[startVertIdx].v = std::abs((end - start).Magnitude()) /32.0f;
	vert_[startVertIdx].rhw = 1.0f;
	vert_[startVertIdx].dif = GetColorU8(255, 255, 255, 255);
	ind_[startIndIdx + 2] = startVertIdx;
	ind_[startIndIdx + 5] = startVertIdx;

	// 右下
	startVertIdx++;
	vert_[startVertIdx].pos = Vector2ToDxVec(end + ((vecA + endBack) * 16.0f ));
	vert_[startVertIdx].u = 1.0f;
	vert_[startVertIdx].v = std::abs((end - start).Magnitude()) / 32.0f;
	vert_[startVertIdx].rhw = 1.0f;
	vert_[startVertIdx].dif = GetColorU8(255, 255, 255, 255);
	ind_[startIndIdx + 4] = startVertIdx;
	
	num_++;
}
