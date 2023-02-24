#pragma once
#include <filesystem>
#include "Factory.h"
#include <atomic>
#include <map>

// ステージを作成するクラス
class StageFactory :
    public Factory
{
public:
	StageFactory(const std::filesystem::path& path, ObjectManager& objectManager);
private:
	const FactoryID GetID(void) const { return FactoryID::Stage; }
	
	/// <summary>
	/// ロード用関数
	/// </summary>
	/// <param name=""></param>
	void Load(void);

	/// <summary>
	/// ロード完了したか
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const bool IsLoaded(void) final;

	/// <summary>
	/// 背景のスカイドームの生成
	/// </summary>
	/// <param name="file"></param>
	void CreateSkyDome(std::ifstream& file);
	
	/// <summary>
	/// プレイヤーの生成
	/// </summary>
	/// <param name="file"></param>
	void CreatePlayer(std::ifstream& file);

	/// <summary>
	/// 敵の生成
	/// </summary>
	/// <param name="file"></param>
	void CreateEnemy(std::ifstream& file) ;

	void LoadTrans(std::ifstream& file, std::uint32_t size) final;

	void LoadModel(std::ifstream& file, std::uint32_t size) final;

	void LoadSpawn(std::ifstream& file, std::uint32_t size) final;

	// ロード完了したかのフラグ
	std::atomic_bool isLoaded_;

	// データファイルのパス
	std::filesystem::path path_;

	// ステージのオブジェクトID
	ObjectID id_;

	// 敵の数のカウント
	unsigned int enemyCount_;

	// 関数のマップ
	std::map<FactoryID, void(StageFactory::*)(std::ifstream&)> funcMap_;
};

