#pragma once
#include <DxLib.h>
#include "BaseScene.h"
#include <vector>
#include "../Common/SharedHandle.h"

struct LIGHT_MAT
{
	MATRIX view;
	MATRIX proj;
};

// ライトカメラの位置
constexpr Vector3 camPos{ 500.0f, 800.0f, -3000.0f };
// ライトカメラの注視点
constexpr Vector3 camTar{ 0.0f, 0.0f, -1000.0f };
// ライトカメラの正射影の表示範囲
constexpr float offsetOrtho = 12000.0f;
// ライトカメラの手前の距離と奥の距離
constexpr float offsetNear = 0.001f;
constexpr float offsetFar = 13000.0f;

class GameScene :
    public BaseScene
{
public:
	GameScene();

	void SetResult(ResultAttribute result)
	{
		result_ = result;
	}
private:
	const SceneID GetID(void) const
	{
		return SceneID::Game;
	}

	/// <summary>
	/// リザルトシーンへの生成関数
	/// </summary>
	/// <param name="own"></param>
	/// <returns></returns>
	SceneUptr MakeResultFunc(SceneUptr own);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta"></param>
	void Update(float delta, Controller& controller) final;

	/// <summary>
	/// シーンの描画
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void) final;

	void SetupShadowMap(void);

	void SetupPostEffect(void);

	void SetPostEffect(bool flag,int x,int y,int img,int Postps);

	/// <summary>
	/// ロード完了しているかを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool IsLoaded(void) final;

	/// <summary>
	/// ロード完了時の処理
	/// </summary>
	/// <param name=""></param>
	void Loaded(Controller& controller);

	// プレイヤーの座標を取得
	Vector3 pos_;

	// シャドウマップ
	int shadowMap_;
	LIGHT_MAT* lightMat_;
	LIGHT_MAT lightM_;
	int cbuffer_;

	// ポストエフェクト
	int PostPS_;

	// ポストエフェクト用
	int PostTex_;
	// 使用する頂点シェーダ
	std::vector<SharedShaderHandle> useShaders_;

	ResultAttribute result_;
};

