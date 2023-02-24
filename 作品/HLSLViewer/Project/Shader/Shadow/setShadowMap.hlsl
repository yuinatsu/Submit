// ピクセルシェーダーの入力
struct PSInput
{
	float4 diff : COLOR0;       // ディフューズカラー
	float4 spec : COLOR1;       // スペキュラカラー
	float4 uv : TEXCOORD0;		// テクスチャ座標
	float4 lpos : POSITION;		// ローカル座標
};
// ピクセルシェーダーの出力
struct PSOutput
{
	float4 color : SV_TARGET0;	// 色
};

// 基本パラメータ
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

// 基本パラメータ
cbuffer BufferBase : register(b1)
{
	ConstBufferBase g_base;
} ;

// ディフューズマップテクスチャ
SamplerState sam : register(s0);
Texture2D tex : register(t0);

// テクスチャ(ハードシャドウ)
SamplerState depth : register(s1);		// 深度バッファ
Texture2D depthtex : register(t1);		// 深度バッファ

PSOutput main(PSInput input)
{
	PSOutput output;					// 出力構造体

	// テクスチャカラーの読み込み
	float4 texDiffColor = tex.Sample(sam, input.uv.xy);

	// 出力色
	float4 defaultOutput = input.diff * texDiffColor + input.spec;

	// 出力α値
	defaultOutput.a = input.diff.a * texDiffColor.a * g_base.factorColor.a;
	output.color = defaultOutput;

	// UV座標空間に変換
	float2 shadowMapUV = input.lpos.xy / input.lpos.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;
	// Z値を計算(ライトビュースクリーン)
	float lposZ = input.lpos.z / input.lpos.w;
	// UV座標を使ってシャドウマップから影をサンプリング
	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
	{
		// 深度バッファ
        float shadowMapZ = depthtex.Sample(depth, shadowMapUV).r;
        if (lposZ > shadowMapZ + 0.005f)
        {
			// 遮断状態
            output.color.xyz *= 0.5f;
        }
	}
	return output;
}
