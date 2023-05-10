struct PSInput {
	float4 pos:POSITION0;
    float4 lpos:POSITION1;
	float3 norm:NORMAL0;
    float3 cnorm:NORMAL1;
	float2 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
    float4 svpos : SV_POSITION;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PSOutput
{
    float4 color0 : SV_TARGET0; // �F
};

// �}�e���A���p�����[�^
struct CONST_MATERIAL
{
    float4 diffuse; // �f�B�t���[�Y�J���[
    float4 specular; // �X�y�L�����J���[
    float4 ambient_Emissive; // �}�e���A���G�~�b�V�u�J���[ + �}�e���A���A���r�G���g�J���[ * �O���[�o���A���r�G���g�J���[

    float power; // �X�y�L�����̋���
    float typeParam0; // �}�e���A���^�C�v�p�����[�^0
    float typeParam1; // �}�e���A���^�C�v�p�����[�^1
    float typeParam2; // �}�e���A���^�C�v�p�����[�^2
};

// �t�H�O�p�����[�^
struct CONST_FOG
{
    float linearAdd; // �t�H�O�p�p�����[�^ end / ( end - start )
    float linearDiv; // �t�H�O�p�p�����[�^ -1  / ( end - start )
    float density; // �t�H�O�p�p�����[�^ density
    float e; // �t�H�O�p�p�����[�^ ���R�ΐ��̒�

    float4 color; // �J���[
};

// ���C�g�p�����[�^
struct CONST_LIGHT
{
    int type; // ���C�g�^�C�v( DX_LIGHTTYPE_POINT �Ȃ� )
    int3 padding1; // �p�f�B���O�P

    float3 position; // ���W( �r���[��� )
    float rangePow2; // �L�������̂Q��

    float3 direction; // ����( �r���[��� )
    float fallOff; // �X�|�b�g���C�g�pFallOff

    float3 diffuse; // �f�B�t���[�Y�J���[
    float spotParam0; // �X�|�b�g���C�g�p�p�����[�^�O( cos( Phi / 2.0f ) )

    float3 specular; // �X�y�L�����J���[
    float spotParam1; // �X�|�b�g���C�g�p�p�����[�^�P( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

    float4 ambient; // �A���r�G���g�J���[�ƃ}�e���A���̃A���r�G���g�J���[����Z��������

    float attenuation0; // �����ɂ�錸�������p�p�����[�^�O
    float attenuation1; // �����ɂ�錸�������p�p�����[�^�P
    float attenuation2; // �����ɂ�錸�������p�p�����[�^�Q
    float padding2; // �p�f�B���O�Q
};

// �s�N�Z���V�F�[�_�[�E���_�V�F�[�_�[���ʃp�����[�^
struct CONST_BUFFER_COMMON
{
    CONST_LIGHT light[6]; // ���C�g�p�����[�^
    CONST_MATERIAL material; // �}�e���A���p�����[�^
    CONST_FOG fog; // �t�H�O�p�����[�^
};

// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct CONST_BUFFER_BASE
{
    float4 factorColor; // �A���t�@�l��

    float mulAlphaColor; // �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
    float alphaTestRef; // �A���t�@�e�X�g�Ŏg�p�����r�l
    float2 padding1;

    int alphaTestCmpMode; // �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
    int3 padding2;

    float4 ignoreTextureColor; // �e�N�X�`���J���[���������p�J���[
};

// ���_�V�F�[�_�[�E�s�N�Z���V�F�[�_�[���ʃp�����[�^
cbuffer cbD3D11_CONST_BUFFER_COMMON : register(b0)
{
    CONST_BUFFER_COMMON g_Common;
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
{
    CONST_BUFFER_BASE g_Base;
};

SamplerState texsam : register(s0);
SamplerState normsam : register(s1);
SamplerState specsam : register(s2);
SamplerState depthsam : register(s3);
SamplerComparisonState depthsmp : register(s3);
//SamplerComparisonState depthsmp  		// �[�x�o�b�t�@�e�N�X�`��
//{
//	// sampler state
//    Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
//    MaxAnisotropy = 1;
//    AddressU = MIRROR;
//    AddressV = MIRROR;
	
//	// sampler conmparison state
//    ComparisonFunc = GREATER;
//};

Texture2D<float4> tex : register(t0);
Texture2D<float4> norm : register(t1);
Texture2D<float4> spec : register(t2);
Texture2D<float4> depthtex : register(t3);


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
    // �@����0�`1�̒l��-1.0f�`1.0f�ɕϊ�����
    float3 Normal = (norm.Sample(normsam, input.uv.xy).xyz - float3(0.5f, 0.5f, 0.5f)) * 2.0f;
    // �f�B�t���[�Y�J���[�ƃX�y�L�����J���[�̒~�ϒl��������
    float4 TotalDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 TotalSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // ���C�g�����x�N�g���̌v�Z
    TempF3 = g_Common.light[0].direction;
    // ���C�g�̃x�N�g����ڒn��Ԃɕϊ�
    float3 lLightDir;
    lLightDir.x = dot(VTan, TempF3);
    lLightDir.y = dot(VBin, TempF3);
    lLightDir.z = dot(VNrm, TempF3);
    lLightDir = normalize(lLightDir);
    // �f�B�t���[�Y�p�x�������v�Z
    float DiffuseAngleGen = saturate(dot(Normal, -lLightDir));
    // �f�B�t���[�Y�J���[�ɒ~�ς���
    TotalDiffuse.xyz += (g_Common.light[0].diffuse * 2) * g_Common.material.diffuse.xyz * DiffuseAngleGen + g_Common.light[0].ambient.xyz;
    // �X�y�L�����J���[�v�Z�ƃn�[�t�x�N�g���̌v�Z
    TempF3 = normalize(V_to_Eye - lLightDir);
    float4 Temp = pow(max(0.0f, dot(Normal, TempF3)), g_Common.material.power);
    float3 TextureSpecular = spec.Sample(specsam, input.uv.xy);
    TotalSpecular.xyz += Temp.xyz * g_Common.light[0].specular * TextureSpecular;
    
    TotalDiffuse += g_Common.material.ambient_Emissive;
    float3 SpecularColor = TotalSpecular.xyz * g_Common.material.specular.xyz;
    
    float3 dirLig = TotalDiffuse.xyz + SpecularColor;
	
	// �������C�g�̌v�Z
    // �@���ƌ��̓��˕����Ɉˑ����郊���̋��������߂�
    float power1 = 1.0f - max(0.0f, dot(g_Common.light[0].direction, input.norm));
	// �@���Ǝ����̕����Ɉˑ����郊���̋��������߂�
    float power2 = 1.0f - max(0.0f, input.cnorm.z * -1.0f);
	// �ŏI�I�ȃ����̋��������߂�
    float limPower = power1 * power2;
	// pow()���g�p���āA�����̕ω����w���֐��I�ɂ���
    limPower = pow(limPower, 0.8f);
	// dirLig�Ƀ������C�g�̔��ˌ������Z����
	// �܂��A�������C�g�̃J���[���v�Z����
    float3 limColor = limPower * g_Common.light[0].ambient;
	// �ŏI�I�Ȕ��ˌ��Ƀ����̔��ˌ������Z����
    dirLig += limColor;

    
    float4 TextureDiffuseColor = tex.Sample(texsam, input.uv);
    // �A���t�@�l�ȊO�̃J���[���o��
    output.color0.rgb = (TextureDiffuseColor.xyz * g_Common.material.diffuse.xyz) * dirLig;
    // �A���t�@�l�̏o��
    output.color0.a = TextureDiffuseColor.a * g_Common.material.diffuse.a * g_Base.factorColor.a;
    
    float2 shadowUV;
    // �[�x�e�N�X�`���̍��W���Z�o
    shadowUV.x = (input.lpos.x + 1.0f) * 0.5f;
    // y�͏㉺���]���Ȃ��Ƃ����Ȃ�
    shadowUV.y = 1.0f - (input.lpos.y + 1.0f) * 0.5f;
	// �}�b�n�o���h���N�����Ȃ��悤�ɂ��邽��
    input.lpos.z -= 0.005f;
    
    // ���͂̃f�[�^�Ɛ[�x�e�N�X�`���̐[�x���擾
    float comp = 0;
    float U = 1.0f / 2048;
    float V = 1.0f / 2048;
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(0, 0)).r, 0.0f) * 1500 - 0.5f);
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(U, 0)).r, 0.0f) * 1500 - 0.5f);
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(-U, 0)).r, 0.0f) * 1500 - 0.5f);
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(0, V)).r, 0.0f) * 1500 - 0.5f);
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(0, -V)).r, 0.0f) * 1500 - 0.5f);
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(U, V)).r, 0.0f) * 1500 - 0.5f);
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(-U, V)).r, 0.0f) * 1500 - 0.5f);
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(U, -V)).r, 0.0f) * 1500 - 0.5f);
    comp += saturate(max(input.lpos.z - depthtex.Sample(depthsam, shadowUV + float2(-U, -V)).r, 0.0f) * 1500 - 0.5f);
	
    // �o�������̂̕��ς��擾
    comp = 1 - saturate(comp / 9);
    
	// ���̂܂ܓ����ƍ��������̂ŁA�����������߂�
    output.color0.xyz *= comp / 2.0f + 0.2;

    return output;
}