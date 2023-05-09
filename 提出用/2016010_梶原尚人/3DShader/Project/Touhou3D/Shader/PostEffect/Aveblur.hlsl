// ピクセルシェーダーの入力
struct PSInput
{
    float4 diff : COLOR0;       // ディフューズ
    float4 spec : COLOR1;       // スペキュラカラー
    float2 uv : TEXCOORD0;      // テクスチャ座標
};
// ピクセルシェーダーの出力
struct PSOutput
{
    float4 color : SV_TARGET0;  // 色
};

// テクスチャ
SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);

PSOutput main(PSInput input)
{
    PSOutput output;					// 出力構造体
    float4 color = float4(tex.Sample(sam, input.uv));

    float offsetU = 2.5f / 1280;        // U値オフセット
    float offsetV = 2.5f / 720;         // V値オフセット
    
    // 平均ブラー
    // 基準テクセルから指定のテクセルカラーをサンプリング
    color += float4(tex.Sample(sam, input.uv + float2(offsetU, 0.0f)));     // 右
    color += float4(tex.Sample(sam, input.uv + float2(-offsetU, 0.0f)));    // 左
    color += float4(tex.Sample(sam, input.uv + float2(0.0f, offsetV)));     // 下
    color += float4(tex.Sample(sam, input.uv + float2(0.0f, -offsetV)));    // 上
    color += float4(tex.Sample(sam, input.uv + float2(offsetU, offsetV)));  // 右下
    color += float4(tex.Sample(sam, input.uv + float2(offsetU, -offsetV))); // 右上
    color += float4(tex.Sample(sam, input.uv + float2(-offsetU, offsetV))); // 左下
    color += float4(tex.Sample(sam, input.uv + float2(-offsetU, -offsetV)));// 左上
    
    // 平均値除算する
    color /= 9.0f;
    
    output.color = color;
    return output;
}