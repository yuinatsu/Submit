#pragma once
#include "Factory.h"
#include <fstream>

// 背景の空を生成するクラス
class SkyFactory :
	public Factory
{
public:
	SkyFactory(const std::string& filePath, ObjectManager& objectManager);
	ObjectID Create(ObjectID id, const Vector3& pos, const Vector3& rot = { 0.0f,0.0f,0.0f }) final;
private:
	const FactoryID GetID(void) const { return FactoryID::Sky; }
	void LoadTrans(std::ifstream& file, std::uint32_t size) final;
	void LoadModel(std::ifstream& file, std::uint32_t size) final;

	// モデルのパス
	std::string modelPath_;

	// 回転
	Vector3 rot_;

	// 拡縮
	Vector3 scale_;
};

