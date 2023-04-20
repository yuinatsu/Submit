// 頂点シェーダー入力
struct VSInput
{
    float3 pos : POSITION;      // 座標
    float3 norm : NORMAL;       // 法線
    float4 diff : COLOR0;       // ディフューズカラー
    float4 spec : COLOR1;       // スペキュラカラー
    float4 uv0 : TEXCOORD0;     // テクスチャ座標
    float4 uv1 : TEXCOORD1;     // サブテクスチャ座標
};
// 頂点シェーダー出力
struct VSOutput
{
    float4 pos : POSITION0;     // 座標
    float4 lpos : POSITION1;    // ローカル座標
    float4 svpos : SV_POSITION; // 画面空間のピクセル座標
    float3 norm : NORMAL;       // 法線
    float4 diff : COLOR0;       // ディフューズカラー
    float4 spec : COLOR1;       // スペキュラカラー
    float4 uv : TEXCOORD;       // テクスチャ座標
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
    float4 shadowMapLightViewProjMat[3][4];       // 影マップのライトビューと射影マトリックス
    float4 texMat[3][2];                            // テクスチャマッピングの変換行列
};

// 基本パラメータ
cbuffer BufferBase : register(b1)
{
    ConstBufferBase g_base;             // グローバル変数
}
// その他のパラメータ
cbuffer BufferOtherMat : register(b2)
{
    ConstBufferOtherMat g_otherMat;     // グローバル変数
}

// ライト行列
cbuffer LightMat : register(b4)
{
    float4x4 g_lightView;       // ライトビュー変換行列
    float4x4 g_lightProj;       // ライト射影変換行列
}

VSOutput main(VSInput input)
{
    VSOutput output;                    // 出力構造体
    float4 pos = float4(input.pos, 1);  // 入力頂点座標

	// 1.ローカル座標をワールド座標に変換
    float4 worldPos;                    // ワールド座標
    worldPos.xyz = mul(pos, g_base.worldMat);
    worldPos.w = 1.0f;

	// 2.ワールド座標をビュー行列に変換
    float4 viewPos;                     // ビュー座標
    viewPos.xyz = mul(worldPos, g_base.viewMat);
    viewPos.w = 1.0f;

	// 3.ビュー座標をスクリーン座標に変換
    output.svpos = mul(viewPos, g_base.projMat);
    output.svpos.w = dot(viewPos, g_base.projMat[3]);

	// 4.従法線、接線、法線をビューベクトルに変換
    float3 worldNorm;                   // ワールド法線
    float3 viewNorm;                    // ビュー法線
    worldNorm.xyz = mul(input.norm, g_base.worldMat);
    viewNorm.xyz = mul(worldNorm, g_base.viewMat);

    // 5.UV座標の変換
    output.uv.x = dot(input.uv0, g_otherMat.texMat[0][0]);
    output.uv.y = dot(input.uv0, g_otherMat.texMat[0][1]);

    // 6.出力する値のセット
    output.pos = viewPos;
    output.norm = viewNorm;

	// 7.カメラ情報をセット
    float4 lViewPos = mul(g_lightView, worldPos);
    output.lpos = mul(g_lightProj, lViewPos);

    return output;
}
