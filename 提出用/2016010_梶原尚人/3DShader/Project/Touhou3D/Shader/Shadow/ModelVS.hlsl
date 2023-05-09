// 頂点シェーダーの入力
struct VSInput
{
	float3 pos : POSITION;					// 座標
	float3 norm : NORMAL0;					// 法線
	float4 diff : COLOR0;					// ディフューズカラー
	float4 spec : COLOR1;					// スペキュラカラー
	float4 uv0 : TEXCOORD0;					// テクスチャ座標
	float4 uv1 : TEXCOORD1;					// サブテクスチャ座標
	// スキニングメッシュ
	int4 blendIndices0 : BLENDINDICES0;		// スキニング処理用 Float型定数配列インデックス
	float4 blendWeight0 : BLENDWEIGHT0;     // スキニング処理用ウエイト値
};
// 頂点シェーダーの出力
struct VSOutput
{
	float4 diff : COLOR0;					// ディフューズカラー
	float4 spec : COLOR1;					// スペキュラカラー
	float4 uv : TEXCOORD0;					// テクスチャ座標
	float4 lpos : POSITION;					// ローカル座標
	float4 pos : SV_POSITION;				// 座標
};

// 基本パラメータ
struct ConstBufferBase
{
	float4x4 antiViewportMat;   // Viewport行列の逆行列
	float4x4 projMat;           // 射影変換行列
	float4x3 viewMat;           // ビュー変換行列
	float4x3 worldMat;          // ワールド変換行列
	float4 toonOutLineSize;     // トゥーン輪郭線のサイズ
	float diffSource;           // ディフューズ光源
	float specSource;           // スペキュラ光源
	float mulSpecColor;         // スペキュラ色乗算値
	float padding;              // 詰め物
};
// その他のパラメータ
struct ConstBufferOtherMat
{
	float4 shadowMapLightViewProjMat[3][4];       // 影マップのライトビューと射影行列の乗算
	float4 texMat[3][2];                          // テクスチャマッピングの変換行列
};
// スキニングメッシュ用
struct ConstBufferLocalWorldMat
{
	float4 mat[54 * 3]; // ローカル行列からワールド座標への変換
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
	float3 direction;						// 放射方向
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

// シェーダー共通パラメータ
cbuffer BufferCommon : register(b0)
{
	ConstBufferCommon g_common;       // グローバル変数
};
// 基本パラメータ
cbuffer BufferBase : register(b1)
{
	ConstBufferBase g_base;     // グローバル変数
};
// その他のパラメータ
cbuffer BufferOtherMat : register(b2)
{
	ConstBufferOtherMat g_otherMat;     // グローバル変数
};
// スキニングメッシュ用
cbuffer BufferLocalWorldMat : register(b3)
{
	ConstBufferLocalWorldMat g_localWorldMat;       // グローバル変数
};

// ライト行列
cbuffer LightMat : register(b4)
{
	float4x4 g_lightView;       // ライトビュー変換行列
	float4x4 g_lightProj;       // ライト射影変換行列
};

VSOutput main(VSInput input)
{
	VSOutput output;					// 出力構造体
	float4 pos = float4(input.pos, 1);  // 入力頂点座標

	// 1.複数のフレームのブレンド行列の作成
	float4 localWorldMatrix[3];		// ワールド変換
	localWorldMatrix[0] = g_localWorldMat.mat[input.blendIndices0.x + 0] * input.blendWeight0.x;
	localWorldMatrix[1] = g_localWorldMat.mat[input.blendIndices0.x + 1] * input.blendWeight0.x;
	localWorldMatrix[2] = g_localWorldMat.mat[input.blendIndices0.x + 2] * input.blendWeight0.x;

	localWorldMatrix[0] += g_localWorldMat.mat[input.blendIndices0.y + 0] * input.blendWeight0.y;
	localWorldMatrix[1] += g_localWorldMat.mat[input.blendIndices0.y + 1] * input.blendWeight0.y;
	localWorldMatrix[2] += g_localWorldMat.mat[input.blendIndices0.y + 2] * input.blendWeight0.y;

	localWorldMatrix[0] += g_localWorldMat.mat[input.blendIndices0.z + 0] * input.blendWeight0.z;
	localWorldMatrix[1] += g_localWorldMat.mat[input.blendIndices0.z + 1] * input.blendWeight0.z;
	localWorldMatrix[2] += g_localWorldMat.mat[input.blendIndices0.z + 2] * input.blendWeight0.z;

	localWorldMatrix[0] += g_localWorldMat.mat[input.blendIndices0.w + 0] * input.blendWeight0.w;
	localWorldMatrix[1] += g_localWorldMat.mat[input.blendIndices0.w + 1] * input.blendWeight0.w;
	localWorldMatrix[2] += g_localWorldMat.mat[input.blendIndices0.w + 2] * input.blendWeight0.w;

	// 2.ローカル座標をワールド座標に変換
	float4 worldPos;
	worldPos.x = dot(pos, localWorldMatrix[0]);
	worldPos.y = dot(pos, localWorldMatrix[1]);
	worldPos.z = dot(pos, localWorldMatrix[2]);
	worldPos.w = 1.0f ;

	// 3.ワールド座標をビュー座標に変換
	float4 viewPos = float4(mul(worldPos, g_base.viewMat), 1.0f);

	// 4.ビュー座標を射影座標に変換
	output.pos = mul(viewPos, g_base.projMat);

	// 5.ローカルベクトルをワールドベクトルに変換
	float3 worldNorm;					// ワールド法線
	worldNorm.x = dot(input.norm, localWorldMatrix[0].xyz);
	worldNorm.y = dot(input.norm, localWorldMatrix[1].xyz);
	worldNorm.z = dot(input.norm, localWorldMatrix[2].xyz);

	// 6.ワールドベクトルをビューベクトルに変換
	float3 viewNorm = mul(worldNorm, g_base.viewMat);
	viewNorm = normalize(viewNorm);		// 法線を正規化

	// 7.ディフューズカラー、スペキュラカラー計算
	float4 lightLitParam;			// ピクセルに対するライトの影響を計算するためのライトパラメータ
	lightLitParam.x = dot(viewNorm, -g_common.light[0].direction);		// 法線とライトの逆方向ベクトルとの内積

	float3 lightHalfVec = normalize(normalize(-viewPos.xyz) - g_common.light[0].direction);	// ハーフベクトルの計算

	lightLitParam.y = dot(lightHalfVec, viewNorm);		// 法線とハーフベクトルの内積
	lightLitParam.w = g_common.material.pow;			// スペキュラ反射率

	float4 lightLitDest = lit(lightLitParam.x, lightLitParam.y, lightLitParam.w);			// ディフューズ角度減衰のライトパラメータ計算

	// ディフューズカラー
	output.diff.xyz = lightLitDest.y * g_common.light[0].diff * g_common.material.diff.xyz + g_common.light[0].amb.xyz + g_common.material.ambEmissive.xyz;
	output.diff.w = g_common.material.diff.w;			// デフォルトのマテリアルのディフューズカラーのα値

	// スペキュラカラー
	output.spec.xyz = lightLitDest.z * g_common.light[0].spec * g_common.material.spec.xyz;
	output.spec.w = g_common.material.spec.w;			// デフォルトのマテリアルのスペキュラカラーのα値

	// 8.UV座標の変換
	output.uv.x = dot(input.uv0, g_otherMat.texMat[0][0]);
	output.uv.y = dot(input.uv0, g_otherMat.texMat[0][1]);

	// 9.カメラ情報をセット
	float4 lViewPos = mul(g_lightView, worldPos);			// ワールド座標をライトのビュー座標に変換
	output.lpos = mul(g_lightProj, lViewPos);				// ライトのビュー座標をライトの射影座標に変換

	return output;
}


