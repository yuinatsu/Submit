struct PSInput {
	float4 svpos:SV_POSITION;
	float4 pos:POSITION;
	float3 norm:NORMAL;
	float4 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
};

// ピクセルシェーダーの出力
struct PSOutput
{
	float4 color0           : SV_TARGET0;	// 色
};

// マテリアルパラメータ
struct DX_D3D11_CONST_MATERIAL
{
	float4		diffuse;				// ディフューズカラー
	float4		specular;				// スペキュラカラー
	float4		ambient_Emissive;		// マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー

	float		power;					// スペキュラの強さ
	float		typeParam0;			// マテリアルタイプパラメータ0
	float		typeParam1;			// マテリアルタイプパラメータ1
	float		typeParam2;			// マテリアルタイプパラメータ2
};

// フォグパラメータ
struct DX_D3D11_VS_CONST_FOG
{
	float		linearAdd;				// フォグ用パラメータ end / ( end - start )
	float		linearDiv;				// フォグ用パラメータ -1  / ( end - start )
	float		density;				// フォグ用パラメータ density
	float		e;						// フォグ用パラメータ 自然対数の低

	float4		color;					// カラー
};

// ライトパラメータ
struct DX_D3D11_CONST_LIGHT
{
	int			type;					// ライトタイプ( DX_LIGHTTYPE_POINT など )
	int3		padding1;				// パディング１

	float3		pos;				// 座標( ビュー空間 )
	float		rangePow2;				// 有効距離の２乗

	float3		dir;				// 方向( ビュー空間 )
	float		fallOff;				// スポットライト用FallOff

	float3		diffuse;				// ディフューズカラー
	float		spotParam0;			// スポットライト用パラメータ０( cos( Phi / 2.0f ) )

	float3		specular;				// スペキュラカラー
	float		spotParam1;			// スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		ambient;				// アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

	float		attenuation0;			// 距離による減衰処理用パラメータ０
	float		attenuation1;			// 距離による減衰処理用パラメータ１
	float		attenuation2;			// 距離による減衰処理用パラメータ２
	float		padding2;				// パディング２
};

// ピクセルシェーダー・頂点シェーダー共通パラメータ
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		light[6];			// ライトパラメータ
	DX_D3D11_CONST_MATERIAL		material;				// マテリアルパラメータ
	DX_D3D11_VS_CONST_FOG		fog;					// フォグパラメータ
};

// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		factorColor;			// アルファ値等

	float		mulAlphaColor;			// カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
	float		alphaTestRef;			// アルファテストで使用する比較値
	float2		padding1;

	int			alphaTestCmpMode;		// アルファテスト比較モード( DX_CMP_NEVER など )
	int3		padding2;

	float4		ignoreTextureColor;	// テクスチャカラー無視処理用カラー
};

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register(b0)
{
	DX_D3D11_CONST_BUFFER_COMMON		g_Common;
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

SamplerState sam:register(s0);
SamplerState normmap:register(s1);
SamplerState specmap:register(s2);
sampler Toon:register(s3);
Texture2D<float4> tex:register(t0);
Texture2D<float4> normtex:register(t1);
Texture2D<float4> spectex:register(t2);
Texture2D<float4> toon:register(t3);

PSOutput main(PSInput input) : SV_TARGET
{
	PSOutput output;
	// 法線を正規化
	float3 VNrm = normalize(input.norm);
	float3 V_to_Eye = normalize(-input.pos);
	// 法線の 0〜1 の値を -1.0〜1.0 に変換する
	// バンプマップがある場合
	//float3 Normal = (normtex.Sample(normmap, PSInput.uv.xy) - float3(0.5f, 0.5f, 0.5f)) * 2.0f;

	// ない場合
	float3 Normal = VNrm;
	// ディフューズカラーとスペキュラカラーの蓄積値を初期化
	float3 TotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	float3 TotalSpecular = float3(0.0f, 0.0f, 0.0f);
	
	// ディレクショナルライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
	// ライト方向ベクトルの計算
	float3 lLightDir;
	lLightDir = g_Common.light[0].dir;
	// トーンの処理
	float p = dot(input.norm * -1.0f, lLightDir);
	p = p * 0.5f + 0.5f;
	p = p * p;
	float4 col = toon.Sample(Toon, float2(p, 0.0f));

	// ライトのベクトルを接地空間に変換
	// DiffuseAngleGen = ディフューズ角度減衰率計算(減衰無し)
	float DiffuseAngleGen = saturate(dot(Normal, -lLightDir));
	// ディフューズカラー蓄積値 += ライトのディフューズカラー * マテリアルのディフューズカラー * ディフューズカラー角度減衰率 + ライトのアンビエントカラーとマテリアルのアンビエントカラーを乗算したもの 
	TotalDiffuse += g_Common.light[0].diffuse * g_Common.material.diffuse.xyz * DiffuseAngleGen *col + g_Common.light[0].ambient;
	// スペキュラカラー計算
	// ハーフベクトルの計算
	float3 TempF3 = normalize(V_to_Eye - lLightDir);
	float4 Temp = pow(max(0.0f, dot(Normal, TempF3)), g_Common.material.power);

	// スペキュラカラー蓄積値 += Temp * ライトのスペキュラカラー
	TotalSpecular += Temp * g_Common.light[0].specular * col;
	// ディレクショナルライトの処理 +++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )
	
	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )
	// TotalDiffuse = ライトディフューズカラー蓄積値 + ( マテリアルのアンビエントカラーとグローバルアンビエントカラーを乗算したものとマテリアルエミッシブカラーを加算したもの )
	TotalDiffuse += g_Common.material.ambient_Emissive.xyz;
	// SpecularColor = ライトのスペキュラカラー蓄積値 * マテリアルのスペキュラカラー
	float3 SpecularColor = TotalSpecular * g_Common.material.specular.xyz;

	// 出力カラー = TotalDiffuse * SpecularColor
	output.color0.rgb = TotalDiffuse + SpecularColor;
	// アルファ値 = マテリアルのディフューズアルファ * 不透明度
	output.color0.a = g_Common.material.diffuse.a * g_Base.factorColor.a;
	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )	
	output.color0 *= col;
	return output;
}