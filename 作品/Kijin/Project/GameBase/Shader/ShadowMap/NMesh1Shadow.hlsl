struct VSInput
{
    float3 pos : POSITION; //���W
    float3 norm : NORMAL; //�@��
    float4 diff : COLOR0; //�f�B�t���[�Y�J���[
    float4 spec : COLOR1; //�X�y�L�����J���[
    float4 uv0 : TEXCOORD0; //�e�N�X�`�����W
    float4 uv1 : TEXCOORD1; //�T�u�e�N�X�`�����W
    float3 tan : TANGENT;
    float3 bin : BINORMAL;
};

struct VSOutput
{
    float4 pos : POSITION0;
    float4 vpos : POSITION1;
    float3 norm : NORMAL;
    float4 uv : TEXCOORD;
    float4 diff : COLOR0;
    float4 spec : COLOR1;
    float4 svpos : SV_POSITION;
};

// ��{�p�����[�^
struct CONST_BUFFER_BASE
{
    float4x4 AntiViewportMat; // �A���`�r���[�|�[�g�s��
    float4x4 ProjMat; // �r���[�@���@�v���W�F�N�V�����s��
    float4x3 ViewMat; // ���[���h�@���@�r���[�s��
    float4x3 LocalWorldMat; // ���[�J���@���@���[���h�s��

    float4 ToonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float DiffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float SpecularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float MulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float Padding;
};

// ���̑��̍s��
struct CONST_BUFFER_OTHERMAT
{
    float4 ShadowMapLightViewProjectionMatrix[3][4]; // �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
    float4 TextureMatrix[3][2]; // �e�N�X�`�����W����p�s��
};

// ��{�p�����[�^
cbuffer BUFFER_BASE : register(b1)
{
    CONST_BUFFER_BASE g_Base;
};

// ���̑��̍s��
cbuffer BUFFER_OTHERMAT : register(b2)
{
    CONST_BUFFER_OTHERMAT g_OtherMat;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 WorldPos;
    float4 ViewPos;
	// ���[�J�����W���Z�b�g
    float4 pos = float4(input.pos.xyz, 1);
	// ���[�J�����W�����[���h���W�ɂ���
    WorldPos.xyz = mul(pos, g_Base.LocalWorldMat);
    WorldPos.w = 1.0f;

	// ���[���h���W���r���[�s��ɂ���
    ViewPos.xyz = mul(WorldPos, g_Base.ViewMat);
    ViewPos.w = 1.0f;

	// ���[�J�����W�����[���h���W�ɂ���
    output.svpos = mul(ViewPos, g_Base.ProjMat);

	// �J�����̏����擾�ł���悤��
    output.vpos = output.svpos;
	// ���W�̃Z�b�g
    output.pos = ViewPos;
    return output;
}