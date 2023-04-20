// ピクセルシェーダー入力
struct PSInput
{
    float4 diff : COLOR0;       // ディフューズ
    float4 spec : COLOR1;       // スペキュラカラー
    float2 uv : TEXCOORD0;      // テクスチャ座標
};
// ピクセルシェーダー出力
struct PSOutput
{
    float4 color : SV_TARGET0; // 色
};

// テクスチャ
SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);

PSOutput main(PSInput input)
{
    PSOutput output;                    // 出力構造体
    output.color = float4(tex.Sample(sam, input.uv.xy));
    // モノクロ
    output.color = (0.3f * output.color.x) + (0.6f * output.color.z) + (0.1f * output.color.z);
    return output;
}