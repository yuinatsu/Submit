struct PS_INPUT
{
	float4 Diffuse : COLOR0;		// 出力座標
	float4 Specular : COLOR1;		// ディフューズカラー
	float2 uv : TEXCOORD0;			// テクスチャ座標
};

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
};

cbuffer buff : register(b0)
{
	float thershold;
}
// サンプラ
SamplerState sam;

// テクスチャ
Texture2D<float4> tex : register(t0);
Texture2D<float4> ptn : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float discaval = ptn.Sample(sam, input.uv).r;
	float yedge = pow(0.0 + saturate(abs(thershold + input.uv.x)), 10);
	if (input.uv.x - (discaval * yedge) > thershold)
	{
		discard;
	}
	float4 col = tex.Sample(sam, input.uv);
	output.color = col;
	if (col.a == 0.0)
	{
		discard;
	}
	float edge = 1.0 - saturate(abs(thershold - discaval));
	//edge = saturate((1.0 - abs(input.uv.x + (yedge * discaval) - thershold)));
	edge = pow(edge, 10);
	output.color.rgb = float3(0, 0, 1) * edge + (output.color.rgb) * (1.0 - edge);
	return output;

	// まあまあいい
	//PS_OUTPUT output;
	//float discaval = ptn.Sample(sam, input.uv).r;
	//float yedge = pow(0.0 + saturate(abs(thershold + input.uv.x)), 5);
	//if (input.uv.x - (discaval * yedge) > thershold)
	//{
	//	discard;
	//}
	//float4 col = tex.Sample(sam, input.uv);
	//output.color = col;
	//if (col.a == 0.0)
	//{
	//	discard;
	//}
	//float edge = 1.0 + saturate(abs(thershold - discaval));
	//edge = saturate((1.0 - abs(input.uv.x + (yedge * discaval) - thershold)));
	//edge = pow(edge, 15);
	//output.color.rgb = float3(0, 0, 1) * edge + (output.color.rgb) * (1.0 - edge);
	//return output;
}