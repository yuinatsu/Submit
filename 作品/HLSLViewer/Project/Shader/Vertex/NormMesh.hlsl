// 頂点シェーダー入力
struct VSInput 
{
	float3 pos : POSITION;		// 座標
	float3 norm : NORMAL;		// 法線
	float4 diff : COLOR0;		// ディフューズカラー 
	float4 spec : COLOR1;		// スペキュラカラー
	float4 uv0 : TEXCOORD0;		// テクスチャ座標
	float4 uv1 : TEXCOORD1;		// サブテクスチャ座標
	// バンプマップ
	float3 tan : TANGENT;		// 接ベクトル
	float3 binorm : BINORMAL;	// 従法線
};
// 頂点シェーダー出力
struct VSOutput 
{
	float4 pos : POSITION;		// 座標
	float4 svpos : SV_POSITION;	// 画面空間のピクセル座標
	float3 norm : NORMAL;		// 法線
	float4 diff : COLOR0;		// ディフューズカラー
	float4 spec : COLOR1;		// スペキュラカラー
	float4 uv : TECOORD;		// テクスチャ座標
	float3 tan : TANGENT;		// 接ベクトル
	float3 binorm : BINORMAL;	// 従法線
};

// 基本パラメータ
cbuffer ConstBufferBase : register(b1)
{
	float4x4 antiViewportMat;		// Viewport行列の逆行列
	float4x4 projMat;				// 射影変換行列
	float4x3 viewMat;			// ビュー変換行列
	float4x3 worldMat;			// ワールド変換行列
	float4 toonOutLineSize;		// トゥーン輪郭線のサイズ
	float diffSource;			// ディフューズ光源
	float specSource;			// スペキュラ光源
	float mulSpecColor;			// スペキュラ色乗算値
	float padding;				// 詰め物
}

VSOutput main(VSInput input)
{
	VSOutput output;						// 出力構造体
	float4 pos = float4(input.pos, 1);		// 入力頂点座標

	pos.xyz = mul(pos, worldMat);			// ワールド変換行列適用
	output.pos = pos;						// 座標を出力に格納
	pos.xyz = mul(pos, viewMat);			// ビュー変換行列適用
	pos = mul(pos, projMat);				// プロジェクション変換行列適用

	// 入力法線を変換するためworldMat変換行列を乗算
	float3 norm = mul(input.norm, worldMat);
	float3 tan = mul(input.tan, worldMat);
	float3 binorm = mul(input.binorm, worldMat);

	output.svpos = pos;						// 変換された座標を格納

	output.uv = input.uv0;					// 入力UV0を出力に格納
	output.norm = normalize(norm);			// 正規化した法線を出力に格納
	output.tan = normalize(tan);			// 正規化した接ベクトルを出力に格納
	output.binorm = normalize(binorm);		// 正規化した法線分布ベクトルを出力に格納
	return output;
}