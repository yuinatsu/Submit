// 入力
struct PSInput
{
	float4 svpos : SV_POSITION;		// 最終的な座標
	float3 pos : POSITION;			// 座標
	float3 norm : NORMAL;			// 法線
	float4 uv : TECOORD;			// テクスチャ座標
	float4 diff : COLOR0;			// ディフューズカラー
	float4 spec : COLOR1;			// スペキュラカラー
};
// 出力
struct PSOutput
{
	float4 color : SV_TARGET0;		// 色
};

// マテリアルパラメータ
struct ConstMaterial
{
	float4 diff;							// ディフューズカラー
	float4 spec;							// スペキュラカラー
	float4 ambEmissive;						// アンビエント/エミッシブカラー
	float pow;								// スペキュラの強さ
	float typeParam0;						// マテリアルタイプパラメータ0
	float typeParam1;						// マテリアルタイプパラメータ1
	float typeParam2;						// マテリアルタイプパラメータ2
};

// フォグパラメータ
struct ConstFog
{
	float linearAdd;						// 線形加算
	float linearDiv;						// フォグ計算の距離範囲
	float density;							// 密度
	float e;								// 自然対数の底(指数関数的なフォグ計算のための基準値)
	float4 color;							// 色
};

// ライトパラメータ
struct ConstLight
{
	int type;								// ライトの種類(0:ポイントライト、1:スポットライト、2:ディレクションライト)
	int3 padding1;							// 詰め物
	float3 pos;								// 座標
	float rangePow2;						// ライトの距離の2乗
	float3 dir;								// 放射方向
	float fallOut;							// ライトの減衰率(スポットライト用)
	float3 diff;							// ディフューズカラー
	float spotParam0;						// スポットライト用パラメータ0
	float3 spec;							// スペキュラカラー
	float spotParam1;						// スポットライト用パラメータ1
	float4 amb;								// アンビエントカラー(環境光)
	float att0;								// ライトの減衰係数0(Attenuation)
	float att1;								// ライトの減衰係数1
	float att2;								// ライトの減衰係数2
	float padding2;							// 詰め物
};

// シェーダー共通パラメータ
struct ConstBufferCommon
{
	ConstLight light[6];		// ライト
	ConstMaterial material;		// マテリアル
	ConstFog fog;				// フォグ
};

// 定数バッファ基本パラメータ
struct ConstBufferBase
{
	float4 factorColor;			// 最終的な色で乗算される色
	float mulAlphaColor;		// 最終的なアルファで乗算されるアルファ値( 0.0f:乗算しない  1.0f:乗算する )
	float alphaTestRef;			// アルファテストの参照値
	float2 padding1;			// 詰め物
	int alphaTestCmpMode;		// アルファ テストの比較モード
	int3 padding2;				// 詰め物
	float4 ignoreTexColor;		// テクスチャデータを無視するときに使用される色
};

// シェーダー共通パラメータ
cbuffer BufferCommon : register(b0)
{
	ConstBufferCommon g_common;
};

// 基本パラメータ
cbuffer BufferBase : register(b1)
{
	ConstBufferBase g_base;
};

SamplerState sam : register(s0);		// テクスチャのサンプラ状態
SamplerState normMap : register(s1);	// 法線マップテクスチャのサンプラ状態
SamplerState specMap : register(s2);	// 鏡面反射マップテクスチャのサンプラ状態
sampler toon : register(s3);			// トゥーンテクスチャのサンプラー状態
Texture2D<float4> tex : register(t0);		// 2Dテクスチャ 
Texture2D<float4> normTex : register(t1);	// 2D法線マップ
Texture2D<float4> specTex : register(t2);	// 2D鏡面反射マップテクスチャ
Texture2D<float4> toonTex : register(t3);	// 2Dトゥーンテクスチャ

PSOutput main(PSInput input) : SV_TARGET
{
	PSOutput output;

	// 1.正規化
	float3 norm = normalize(input.norm);		// 法線

	// 2.ビューベクトル計算
	float3 eye = normalize(-input.pos);

	// 3.現在のピクセルの法線を取得
	float3 normal = norm;

	// 4.ディレクショナルライト処理
	float3 lightDir;
	lightDir = g_common.light[0].dir;

	// 5.トーンの処理
	float p = dot(input.norm * -1.0f, lightDir);
	p = p * 0.5f + 0.5f;
	p = p * p;
	float4 col = toonTex.Sample(toon, float2(p, 0.0f));

	// 6.法線と光の方向の角度を用いて拡散光を計算
	float diffAngleGen = saturate(dot(normal, -lightDir));		// ディフューズ角度減衰率計算(減衰無し)
	float3 totalDiff = float3(0.0f, 0.0f, 0.0f);
	totalDiff += g_common.light[0].diff * g_common.material.diff.xyz * diffAngleGen * col + g_common.light[0].amb;

	// 7.スペキュラカラー計算
	float3 base = normalize(eye - lightDir);		// ハーフベクトルの計算
	// ハーフベクトルと法線でスペキュラーを計算
	float4 temp = pow(max(0.0f, dot(normal, base)), g_common.material.pow);
	float3 totalSpec = float3(0.0f, 0.0f, 0.0f);
	totalSpec += temp * g_common.light[0].spec * col;

	// 8.出力カラー計算
	totalDiff += g_common.material.ambEmissive.xyz;
	float3 specColor = totalSpec * g_common.material.spec.xyz;

	// 9.ディフューズカラーとスペキュラカラーの合成
	output.color.rgb = totalDiff + specColor;

	// 10.アルファ値に係数カラーアルファを乗算
	output.color.a = g_common.material.diff.a * g_base.factorColor.a;
	output.color *= col;

	return output;
}