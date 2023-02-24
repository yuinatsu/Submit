struct PSInput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float4 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PSOutput
{
	float4 color0           : SV_TARGET0;	// �F
};

// �f�B���N�V�������C�g�p�̒萔�o�b�t�@
//cbuffer DirectionLightCb : register(b1)
//{
//	float3 ligDirection;	//���C�g�̕���
//	float3 ligColor;		//���C�g�̃J���[

// �}�e���A���p�����[�^
struct DX_D3D11_CONST_MATERIAL
{
	float4		diffuse;				// �f�B�t���[�Y�J���[
	float4		specular;				// �X�y�L�����J���[
	float4		ambient_Emissive;		// �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

	float		power;					// �X�y�L�����̋���
	float		typeParam0;			// �}�e���A���^�C�v�p�����[�^0
	float		typeParam1;			// �}�e���A���^�C�v�p�����[�^1
	float		typeParam2;			// �}�e���A���^�C�v�p�����[�^2
};

// �t�H�O�p�����[�^
struct DX_D3D11_VS_CONST_FOG
{
	float		linearAdd;				// �t�H�O�p�p�����[�^ end / ( end - start )
	float		linearDiv;				// �t�H�O�p�p�����[�^ -1  / ( end - start )
	float		density;				// �t�H�O�p�p�����[�^ density
	float		e;						// �t�H�O�p�p�����[�^ ���R�ΐ��̒�

	float4		color;					// �J���[
};

// ���C�g�p�����[�^
struct DX_D3D11_CONST_LIGHT
{
	int			type;					// ���C�g�^�C�v( DX_LIGHTTYPE_POINT �Ȃ� )
	int3		padding1;				// �p�f�B���O�P

	float3		pos;				// ���W( �r���[��� )
	float		rangePow2;				// �L�������̂Q��

	float3		dir;				// ����( �r���[��� )
	float		fallOff;				// �X�|�b�g���C�g�pFallOff

	float3		diffuse;				// �f�B�t���[�Y�J���[
	float		spotParam0;			// �X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )

	float3		specular;				// �X�y�L�����J���[
	float		spotParam1;			// �X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

	float4		ambient;				// �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������

	float		attenuation0;			// �����ɂ�錸�������p�p�����[�^�O
	float		attenuation1;			// �����ɂ�錸�������p�p�����[�^�P
	float		attenuation2;			// �����ɂ�錸�������p�p�����[�^�Q
	float		padding2;				// �p�f�B���O�Q
};

// �s�N�Z���V�F�[�_�[�E���_�V�F�[�_�[���ʃp�����[�^
struct DX_D3D11_CONST_BUFFER_COMMON
{
	DX_D3D11_CONST_LIGHT		light[6];			// ���C�g�p�����[�^
	DX_D3D11_CONST_MATERIAL		material;				// �}�e���A���p�����[�^
	DX_D3D11_VS_CONST_FOG		fog;					// �t�H�O�p�����[�^
};

// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		factorColor;			// �A���t�@�l��

	float		mulAlphaColor;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		alphaTestRef;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		padding1;

	int			alphaTestCmpMode;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		padding2;

	float4		ignoreTextureColor;	// �e�N�X�`���J���[���������p�J���[
};

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON					: register(b0)
{
	DX_D3D11_CONST_BUFFER_COMMON		g_Common;
};

// ��{�p�����[�^
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
	// �ڐ��E�]�@���E�@���𐳋K��
	float3 VNrm = normalize(input.norm);
	float3 VTan = normalize(input.tan);
	float3 VBin = normalize(input.bin);

	// ���_���W���王�_�ւ̃x�N�g����ڒ��Ԃɓ��e�����㐳�K�����ĕۑ�
	float3 TempF3;
	TempF3.x = dot(VTan, -input.pos.xyz);
	TempF3.y = dot(VBin, -input.pos.xyz);
	TempF3.z = dot(VNrm, -input.pos.xyz);
	float3 V_to_Eye = normalize(TempF3);
	// �@���� 0�`1 �̒l�� -1.0�`1.0 �ɕϊ�����
	// �o���v�}�b�v���Ȃ��̂�
	float3 Normal = (normtex.Sample(normmap, input.uv.xy) - float3(0.5f, 0.5f, 0.5f)) * 2.0f;

	// �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl��������
	float3 TotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	float3 TotalSpecular = float3(0.0f, 0.0f, 0.0f);

	// �f�B���N�V���i�����C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
	// ���C�g�����x�N�g���̌v�Z
	TempF3 = g_Common.light[0].dir;
	// ���C�g�̃x�N�g����ڒn��Ԃɕϊ�
	float3 lLightDir;
	lLightDir.x = dot(VTan, TempF3);
	lLightDir.y = dot(VBin, TempF3);
	lLightDir.z = dot(VNrm, TempF3);
	// �g�[���̏���
	float p = dot(input.norm * -1.0f, g_Common.light[0].dir);
	p = p * 0.5f + 0.5f;
	p = p * p;
	float4 col = toon.Sample(Toon, float2(p, 0.0f));
	//output.color0 = col;
	//return output;

	// DiffuseAngleGen = �f�B�t���[�Y�p�x�������v�Z(��������)
	float DiffuseAngleGen = saturate(dot(Normal, -lLightDir));
	// �f�B�t���[�Y�J���[�~�ϒl += ���C�g�̃f�B�t���[�Y�J���[ * �}�e���A���̃f�B�t���[�Y�J���[ * �f�B�t���[�Y�J���[�p�x������ + ���C�g�̃A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z�������� 
	TotalDiffuse += g_Common.light[0].diffuse * g_Common.material.diffuse.xyz * col* DiffuseAngleGen + g_Common.light[0].ambient;
	// �X�y�L�����J���[�v�Z
	// �n�[�t�x�N�g���̌v�Z
	TempF3 = normalize(V_to_Eye - lLightDir);
	// Temp = pow( max( 0.0f, N * H ), g_Common.Material.Power )
	float4 Temp = pow(max(0.0f, dot(Normal, TempF3)), g_Common.material.power);
	float3 TextureSpecular = spectex.Sample(specmap, input.uv);
	// �X�y�L�����J���[�~�ϒl += Temp * ���C�g�̃X�y�L�����J���[
	TotalSpecular += Temp * g_Common.light[0].specular * TextureSpecular * col;
	// �f�B���N�V���i�����C�g�̏��� +++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )
	// TotalDiffuse = ���C�g�f�B�t���[�Y�J���[�~�ϒl + ( �}�e���A���̃A���r�G���g�J���[�ƃO���[�o���A���r�G���g�J���[����Z�������̂ƃ}�e���A���G�~�b�V�u�J���[�����Z�������� )
	TotalDiffuse += g_Common.material.ambient_Emissive.xyz;
	// SpecularColor = �}�e���A���̃X�y�L�����J���[
	float3 SpecularColor = TotalSpecular * g_Common.material.specular.xyz;
	// �o�̓J���[ = �e�N�X�`���J���[
	float4 TextureDiffuseColor = tex.Sample(sam, input.uv);
	output.color0.rgb = TextureDiffuseColor.rgb * TotalDiffuse + SpecularColor;
	// �A���t�@�l = �e�N�X�`���A���t�@ * �}�e���A���̃f�B�t���[�Y�A���t�@ * �s�����x
	output.color0.a = TextureDiffuseColor.a * g_Common.material.diffuse.a * g_Base.factorColor.a;
	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )
	
	return output;
	//float p = dot(input.norm * -1.0f, g_Common.light[0].dir);
	//p = p * 0.5f + 0.5f;
	//p = p * p;
	//float4 Col = toon.Sample(Toon, float2(p, 0.0f));
	//output.color0 = Col * tex.Sample(sam, input.uv);
	//return output;
}