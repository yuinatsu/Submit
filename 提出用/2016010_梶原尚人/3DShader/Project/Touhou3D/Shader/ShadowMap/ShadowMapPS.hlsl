// ピクセルシェーダー入力
struct PSInput
{
    float4 pos : POSITION0;     // 3D空間座標
    float4 vpos : POSITION1;    // 2Dピクセル座標
    float4 svpos : SV_POSITION; // 画面空間のピクセル座標
    float3 norm : NORMAL;       // 法線
    float4 diff : COLOR0;       // ディフューズカラー
    float4 spec : COLOR1;       // スペキュラカラー
    float4 uv : TEXCOORD;       // テクスチャ座標
};
// ピクセルシェーダー出力
struct PSOutput
{
    float4 color : SV_Target0;  // RGBカラー
};

PSOutput main(PSInput input)
{
    PSOutput output;            // 出力構造体

    // カラー出力
    output.color = input.vpos.z / input.vpos.w;     // Z値基準
    output.color.a = 1.0f;                          // 不透明(α値:1)

    return output;
}