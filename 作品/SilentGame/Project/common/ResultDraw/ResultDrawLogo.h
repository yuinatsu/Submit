#pragma once
#include "ResultDraw.h"
#include "../ShaderID.h"

template<class T>
class ConstantBuffer;

// リザルトシーンで結果を描画する(ロゴ)
class ResultDrawLogo :
	public ResultDraw
{
public:
	ResultDrawLogo(const std::string& imgName,float startTime, const Math::Vector2& pos , float delta,bool IsClear);
	~ResultDrawLogo();
private:

	/// <summary> 描画する </summary>
	/// <param name=""></param>
	void DrawOwn(void) final;

	// 画像のキー名
	std::string imgName_;

	bool IsClear_;
	float delta_;

	// 定数
	float t;

	std::unique_ptr<ConstantBuffer<float>> cb_;

	// ピクセルシェーダのID
	ShaderID psID_;
};

