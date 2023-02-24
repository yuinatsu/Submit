// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse			: COLOR0;		// ディフーズカラー
	float4 Specure			: COLOR1;		// スぺキュラカラー
	float2 uv				: TEXCOORD0;	// テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 color			: SV_TARGET0;	// 色
};

// 定数バッファ
cbuffer buff : register(b0) {
	float threshold;
	float angle;
}

SamplerState sam;						// サンプラ
Texture2D tex : register(t0);			// テクスチャ
Texture2D ptn: register (t1);			// パターン
Texture2D norm: register (t2);			// 法線マップ

// 1ピクセル塗りつぶし際の呼ばれる
PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float discVal = ptn.Sample(sam, input.uv).r;		// rの値を抽出
	float yedge = pow(1.0 - saturate(abs(threshold - input.uv.y)), 5);
	if (input.uv.y + (yedge * discVal) < threshold)
	{
		discard;
	}

	// ノーマルマップ
	float3 light = float3(cos(angle), -sin(angle), 0.5);
	light = normalize(light);
	float3 normal = norm.Sample(sam, input.uv).rgb;
	normal = (normal * 2) - 1;
	normal = normalize(normal);
	normal.z = -normal.z;

	float bright = saturate(saturate(dot(-light, normal)) + 0.25);


	float4 col = tex.Sample(sam, input.uv);		//float4(input.uv,0.5, 1);		// 赤を返す
	output.color = col;
	if (col.a == 0.0)
	{
		discard;		// なかったことにする
	}

	float edge = 1.0 - saturate(abs(threshold - discVal));
	edge = pow(1.0 - saturate(abs(threshold - input.uv.y)), 3);
	edge = pow(edge, 20);
	// float m = fmod(input.uv.x + input.uv.y, 0.1) * 10;
	//output.color.rgb = 1.0 - output.color.rgb;		// 色を反転
	output.color.rgb = float3 (0, 1, 0) * edge + (output.color.rgb * bright/* * m*/) * (1.0 - edge); // float3(0,pow(edge,20), 0);
	return output;
}