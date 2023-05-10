struct VSInput
{
    float3 pos : POSITION; //���W
    float3 norm : NORMAL; //�@��
    float4 diff : COLOR0; //�f�B�t���[�Y�J���[
    float4 spec : COLOR1; //�X�y�L�����J���[
    float4 uv0 : TEXCOORD0; //�e�N�X�`�����W
    float4 uv1 : TEXCOORD1; //�T�u�e�N�X�`�����W
    float3 tan : TANGENT; //�ڃx�N�g��
    float3 bin : BINORMAL; //�]�@��
};

struct VSOutput
{
    float4 pos : POSITION0;
    float4 lpos : POSITION1;
    float3 norm : NORMAL0;
    float3 cnorm : NORMAL1;
    float4 uv : TEXCOORD;
    float4 diff : COLOR0;
    float4 spec : COLOR1;
    float3 tan : TANGENT;
    float3 bin : BINORMAL;
    float4 svpos : SV_POSITION;
};

// ��{�p�����[�^
struct CONST_BUFFER_BASE
{
    float4 AntiViewportMat[4]; // �A���`�r���[�|�[�g�s��
    float4 ProjMat[4]; // �r���[�@���@�v���W�F�N�V�����s��
    float4 ViewMat[3]; // ���[���h�@���@�r���[�s��
    float4 LocalWorldMat[3]; // ���[�J���@���@���[���h�s��

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

cbuffer LIGHT_VIEW : register(b4)
{
    float4x4 g_lightView;
    float4x4 g_lightProjection;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 pos = float4(input.pos, 1);
    float4 WorldPos;
    float4 ViewPos;

	// ���[�J�����W�����[���h���W�ɂ���
	
   WorldPos.x = dot(pos, g_Base.LocalWorldMat[0]);
    WorldPos.y = dot(pos, g_Base.LocalWorldMat[1]);
    WorldPos.z = dot(pos, g_Base.LocalWorldMat[2]);
    WorldPos.w = 1.0f;

	// ���[���h���W���r���[�s��ɂ���
    ViewPos.x = dot(WorldPos, g_Base.ViewMat[0]);
    ViewPos.y = dot(WorldPos, g_Base.ViewMat[1]);
    ViewPos.z = dot(WorldPos, g_Base.ViewMat[2]);
    ViewPos.w = 1.0f;

	// ���[�J�����W�����[���h���W�ɂ���
    output.svpos.x = dot(ViewPos, g_Base.ProjMat[0]);
    output.svpos.y = dot(ViewPos, g_Base.ProjMat[1]);
    output.svpos.z = dot(ViewPos, g_Base.ProjMat[2]);
    output.svpos.w = dot(ViewPos, g_Base.ProjMat[3]);

	// �]�@���A�ڐ��A�@�����r���[�x�N�g���ɕϊ�
    float3 WorldNrm;
    float3 WorldTan;
    float3 WorldBin;
    
    float3 ViewNrm;
    float3 ViewTan;
    float3 ViewBin;
	
    // �@��
    WorldNrm.x = dot(input.norm, g_Base.LocalWorldMat[0].xyz);
    WorldNrm.y = dot(input.norm, g_Base.LocalWorldMat[1].xyz);
    WorldNrm.z = dot(input.norm, g_Base.LocalWorldMat[2].xyz);
    
    ViewNrm.x = dot(WorldNrm, g_Base.ViewMat[0].xyz);
    ViewNrm.y = dot(WorldNrm, g_Base.ViewMat[1].xyz);
    ViewNrm.z = dot(WorldNrm, g_Base.ViewMat[2].xyz);
    // �]�@��
    WorldTan.x = dot(input.tan, g_Base.LocalWorldMat[0].xyz);
    WorldTan.y = dot(input.tan, g_Base.LocalWorldMat[1].xyz);
    WorldTan.z = dot(input.tan, g_Base.LocalWorldMat[2].xyz);
    
    ViewTan.x = dot(WorldTan, g_Base.ViewMat[0].xyz);
    ViewTan.y = dot(WorldTan, g_Base.ViewMat[1].xyz);
    ViewTan.z = dot(WorldTan, g_Base.ViewMat[2].xyz);
    // �ڐ�
    WorldBin.x = dot(input.bin, g_Base.LocalWorldMat[0].xyz);
    WorldBin.y = dot(input.bin, g_Base.LocalWorldMat[1].xyz);
    WorldBin.z = dot(input.bin, g_Base.LocalWorldMat[2].xyz);
    
    ViewBin.x = dot(WorldBin, g_Base.ViewMat[0].xyz);
    ViewBin.y = dot(WorldBin, g_Base.ViewMat[1].xyz);
    ViewBin.z = dot(WorldBin, g_Base.ViewMat[2].xyz);

    output.uv.x = dot(input.uv0, g_OtherMat.TextureMatrix[0][0]);
    output.uv.y = dot(input.uv0, g_OtherMat.TextureMatrix[0][1]);

    output.pos = ViewPos;
    output.norm = ViewNrm;
    output.tan = ViewTan;
    output.bin = ViewBin;
    
	// �J���������Z�b�g
    float4 LviewPos = mul(g_lightView, WorldPos);
    output.lpos = mul(g_lightProjection, LviewPos);

    return output;
}