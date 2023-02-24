// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 pos : SV_POSITION;				// 出力座標
	float4 diffuse			: COLOR0;		// ディフーズカラー
	float2 uv				: TEXCOORD0;	// テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 color			: SV_TARGET0;	// 色
};

// サンプラ
SamplerState sam;

// テクスチャ
Texture2D<float4> tex : register(t0);

// 霧エフェクト用テクスチャ
Texture2D<float4> fog : register(t1);

cbuffer Effect : register(b0)
{
	float2 centPos;			// 中心座標
	float radius;			// 半径
	float redVal;			// 
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	// 霧エフェクトを取得
	float4 fogColor = fog.Sample(sam,input.uv);

	// テクスチャの取得
	float4 texColor = tex.Sample(sam, input.uv);

	// 距離を取る
	float l = distance(centPos, input.pos.xy);

	// 正規化
	l /= radius;

	// 下限値0.2上限値1でクランプ
	float tmp = clamp(l, 0.2, 1);

	// 霧の値に乗算する
	fogColor = fogColor * tmp;
	
	// redValの値に合わせてテクスチャからテクスチャに赤色を乗算した値を線形補間
	texColor = lerp(texColor, texColor * float4(1, 0.2, 0.2, 1), redVal);

	// テクスチャの色と霧の色を霧のアルファ値で線形補間
	output.color.rgb = lerp(texColor.rgb, fogColor.rgb, fogColor.a);
	output.color.a = 1.0f;
	return output;
}