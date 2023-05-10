// ランバート拡散反射
// ピクセルシェーダー入力
struct PSInput 
{
	float4 svpos : SV_POSITION;		// 最終的な座標
	float3 pos : POSITION;			// 座標
	float3 norm : NORMAL;			// 法線
	float4 uv : TECOORD;			// テクスチャ座標
	float4 diff : COLOR0;			// ディフューズカラー
	float4 spec : COLOR1;			// スペキュラカラー
};

// テクスチャ
SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);

// ディレクションライト用の定数バッファ
cbuffer DirectionLight : register(b0)
{
	float3 lightDir;		// 方向
	float3 lightColor;		// 色
}

float4 main(PSInput input) : SV_TARGET
{
	float inner = dot(input.norm,lightDir);			// ピクセルの法線とライトの方向の内積
	inner *= -1.0f;			// 内積の結果に-1を乗算
	if (inner < 0.0f)
	{
		// 光の強さが負の場合は0にする
		inner = 0.0f;
	}
	float3 diff = lightColor * inner;				// ライトの色
	float4 color = tex.Sample(sam, input.uv);		// サンプラからテクスチャの色を取り出す
	color.xyz *= diff;								// テクスチャの色とfiffを乗算
	return color;
}