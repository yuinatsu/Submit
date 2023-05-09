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

// 基本パラメータ
cbuffer BufferBase : register(b1)
{
    ConstBufferBase g_base;		// グローバル変数
};
// その他のパラメータ
cbuffer BufferOtherMat : register(b2)
{
    ConstBufferOtherMat g_otherMat;		// グローバル変数
};

VSOutput main(VSInput input)
{
    VSOutput output;						    // 出力構造体
    float4 pos = float4(input.pos, 1.0f);		// 入力頂点座標

    // 1.ローカル座標をワールド座標に変換
    float4 worldPos;
    worldPos.xyz = mul(pos, g_base.worldMat);
    worldPos.w = 1.0f;

    // 2.ワールド座標をビュー座標に変換
    float4 viewPos;
    viewPos.xyz = mul(worldPos, g_base.viewMat);
    viewPos.w = 1.0f;

    // 3.ビュー座標をスクリーン座標に変換
    output.svpos = mul(viewPos, g_base.projMat);

    // 4.出力する値のセット
    output.pos = viewPos;
    
    // 5.カメラ情報をセット
    output.vpos = output.svpos;

    return output;
}