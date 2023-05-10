// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 Diffuse	: COLOR0;		// ディフューズカラー
	float4 Specular	: COLOR1;		// スペキュラカラー
	float2 uv		: TEXCOORD0;	// テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 color      : SV_TARGET0;		// 色
};

SamplerState sam;// サンプラ
Texture2D tex : register(t0);// テクスチャ
Texture2D norm : register(t1);// テクスチャ
cbuffer ConstantBuffer : register(b0) {
	float threthold;
	float angle;
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float2 n = norm.Sample(sam, input.uv).xy;
	n = n * 2 - 1;// -1~1

	float m = 0.5;// 実装内容によって数値が異なる		水面揺れ:0.05、ひび割れ:0.5、弾痕:0.1
	float2 ygm = n.xy * m;
	//ygm = float2(cos(angle) * ygm.x + sin(angle) * ygm.y, -sin(angle) * ygm.x + cos(angle) * ygm.y);// 水面揺れ
	output.color = tex.Sample(sam, input.uv + ygm);
	return output;
}