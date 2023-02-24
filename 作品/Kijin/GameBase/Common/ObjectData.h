#pragma once

enum class DataType
{
	Transform,			// 座標回転拡縮データ
	Collider,			// 当たり判定データ
	Model,				// モデルデータのパス
	Spawn,				// スポーンさせる座標とデータパス
	Spawner,			// スポナーの位置
	Other				// そのほかのパラメータ用
};

struct ObjectDataHeader
{
	char sig[4];
	unsigned int check;
	unsigned int version;
};

struct DataHeader
{
	DataType type;
	std::uint32_t size;
};

enum class ColDataType
{
	Box,
	Capsule,
	Sphere,
	Mesh
};
