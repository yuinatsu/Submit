// 頂点シェーダー入力
struct VSInput
{
    float3 pos : POSITION;              // 座標
    float3 norm : NORMAL;               // 法線
    float4 diff : COLOR0;               // ディフューズカラー
    float4 spec : COLOR1;               // スペキュラカラー
    float4 uv0 : TEXCOORD0;             // テクスチャ座標
    float4 uv1 : TEXCOORD1;             // サブテクスチャ座標
	// スキニングメッシュ
    int4 blendIndices0 : BLENDINDICES0; // ボーン初期用float型定数配列インデックス0
    float4 blendWeight0 : BLENDWEIGHT0; // ボーン処理用ウエイト値0
};
// 頂点シェーダー出力
struct VSOutput
{
    float4 pos : POSITION0;     // 3D空間座標
    float4 vpos : POSITION1;    // 2Dピクセル座標
    float4 svpos : SV_POSITION; // 画面空間のピクセル座標
    float3 norm : NORMAL;       // 法線
    float4 diff : COLOR0;       // ディフューズカラー
    float4 spec : COLOR1;       // スペキュラカラー
    float4 uv : TEXCOORD;       // テクスチャ座標
};

// 基本パラメータ
struct ConstBufferBase
{
    float4x4 antiViewportMat;	// Viewport行列の逆行列
    float4x4 projMat;			// 射影変換行列
    float4x3 viewMat;			// ビュー変換行列
    float4x3 worldMat;			// ワールド変換行列
    float4 toonOutLineSize;		// トゥーン輪郭線のサイズ
    float diffSource;			// ディフューズ光源
    float specSource;			// スペキュラ光源
    float mulSpecColor;			// スペキュラ色乗算値
    float padding;				// 詰め物
};
// その他のパラメータ
struct ConstBufferOtherMat
{
    float4 shadowMapLightViewProjMat[3][4];      // 影マップのライトビューと射影行列の乗算
    float4 texMat[3][2];                         // テクスチャマッピングの変換行列
};
// スキニングメッシュ用
struct ConstBufferLocalWorldMat
{
    float4 mat[54 * 3]; // ローカル行列からワールド座標への変換
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

VSOutput main(VSInput input)
{
    VSOutput output;                            // 出力構造体
    float4 pos = float4(input.pos.xyz, 1.0f);   // 入力頂点座標

    // 1.複数のフレームのブレンド行列を作成
    float4 localWorldMat[3];
    localWorldMat[0] = g_localWorldMat.mat[input.blendIndices0.x + 0] * input.blendWeight0.x;
    localWorldMat[1] = g_localWorldMat.mat[input.blendIndices0.x + 1] * input.blendWeight0.x;
    localWorldMat[2] = g_localWorldMat.mat[input.blendIndices0.x + 2] * input.blendWeight0.x;
    
    localWorldMat[0] += g_localWorldMat.mat[input.blendIndices0.y + 0] * input.blendWeight0.y;
    localWorldMat[1] += g_localWorldMat.mat[input.blendIndices0.y + 1] * input.blendWeight0.y;
    localWorldMat[2] += g_localWorldMat.mat[input.blendIndices0.y + 2] * input.blendWeight0.y;
    
    localWorldMat[0] += g_localWorldMat.mat[input.blendIndices0.z + 0] * input.blendWeight0.z;
    localWorldMat[1] += g_localWorldMat.mat[input.blendIndices0.z + 1] * input.blendWeight0.z;
    localWorldMat[2] += g_localWorldMat.mat[input.blendIndices0.z + 2] * input.blendWeight0.z;
    
    localWorldMat[0] += g_localWorldMat.mat[input.blendIndices0.w + 0] * input.blendWeight0.w;
    localWorldMat[1] += g_localWorldMat.mat[input.blendIndices0.w + 1] * input.blendWeight0.w;
    localWorldMat[2] += g_localWorldMat.mat[input.blendIndices0.w + 2] * input.blendWeight0.w;
    
    // 2.ローカル座標をワールド座標に変換
    float4 worldPos;
    worldPos.x = dot(pos, localWorldMat[0]);
    worldPos.y = dot(pos, localWorldMat[1]);
    worldPos.z = dot(pos, localWorldMat[2]);
    worldPos.w = 1.0f;
    
    // 3.ワールド座標をビュー行列に変換
    float4 viewPos;
    viewPos.xyz = mul(worldPos, g_base.viewMat);
    viewPos.w = 1.0f;
    
    // 4.ビュー座標をスクリーン座標に変換
    output.svpos = mul(viewPos, g_base.projMat);

    // 5.座標のセット
    output.pos = viewPos;

    // 6.受け取っていたカメラ情報も取得
    output.vpos = output.svpos;

    return output;
}