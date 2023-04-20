struct VSInput
{
    float3 pos : POSITION0;
    float4 spos : POSITION1;
    float3 norm : NORMAL;
    float3 tan : TANGEN0;
    float3 binorm : BINORMAL0;
    float4 diff : COLOR0;
    float4 spec : COLOR1;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

//struct VSOutput
//{
//    float4 dif : COLOR0; // �f�B�t���[�Y�J���[
//    float2 uv : TEXCOORD0; // �e�N�X�`�����W
//    float4 svpos : SV_POSITION; // ���W( �v���W�F�N�V������� )
//};

struct VSOutput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float4 tex0 : TEXCOORD1;
    float4 tex1 : TEXCOORD2;
    float4 tex2 : TEXCOORD3;
    float4 tex3 : TEXCOORD4;
    float4 tex4 : TEXCOORD5;
    float4 tex5 : TEXCOORD6;
    float4 tex6 : TEXCOORD7;
    float4 tex7 : TEXCOORD8;
    float4 svpos : SV_POSITION;
};

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

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
struct CONST_BUFFER_LOCALWORLDMAT
{
    float4 Mat[54 * 3]; // ���[�J���@���@���[���h�s��
};

// ��{�p�����[�^
cbuffer BUFFER_BASE : register(b1)
{
    CONST_BUFFER_BASE g_Base;
};

cbuffer ScrSize : register(b5)
{
    float2 size;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 WorldPos;
    float4 ViewPos;
    
    // ���W�ϊ�
    float4 pos = float4(input.pos, 1);
    WorldPos.x = dot(pos, g_Base.LocalWorldMat[0]);
    WorldPos.y = dot(pos, g_Base.LocalWorldMat[1]);
    WorldPos.z = dot(pos, g_Base.LocalWorldMat[2]);
    WorldPos.w = 1.0f;
    
    //ViewPos.xyz = mul(WorldPos, g_Base.ViewMat);
    ViewPos.x = dot(WorldPos, g_Base.ViewMat[0]);
    ViewPos.y = dot(WorldPos, g_Base.ViewMat[1]);
    ViewPos.z = dot(WorldPos, g_Base.ViewMat[2]);
    ViewPos.w = 1.0f;
    
    //output.svpos = mul(ViewPos, g_Base.ProjMat);
    output.svpos.x = dot(ViewPos, g_Base.ProjMat[0]);
    output.svpos.y = dot(ViewPos, g_Base.ProjMat[1]);
    output.svpos.z = dot(ViewPos, g_Base.ProjMat[2]);
    output.svpos.w = dot(ViewPos, g_Base.ProjMat[3]);
    
    output.uv = input.uv0;
    
    // �e�N�X�`���T�C�Y��ϊ�
    float2 texSize = float2(0.0f,0.0f);
    float level;
    // ��e�N�Z����UV���W���L�^
    float2 tex = input.uv0; 
    // ��e�N�Z������U���W��+1�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex0.xy = float2(1.0f / size.x, 0.0f);
    
    // ��e�N�Z������U���W��+3�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex1.xy = float2(3.0f / size.x, 0.0f);
    
    // ��e�N�Z������U���W��+5�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex2.xy = float2(5.0f / size.x, 0.0f);
    
    // ��e�N�Z������U���W��+7�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex3.xy = float2(7.0f / size.x, 0.0f);
    
    // ��e�N�Z������U���W��+9�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex4.xy = float2(9.0f / size.x, 0.0f);
    
    // ��e�N�Z������U���W��+11�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex5.xy = float2(11.0f / size.x, 0.0f);
    
    // ��e�N�Z������U���W��+13�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex6.xy = float2(13.0f / size.x, 0.0f);
    
    // ��e�N�Z������U���W��+15�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex7.xy = float2(15.0f / size.x, 0.0f);
    
    // �I�t�Z�b�g��-1�������ă}�C�i�X�����̃I�t�Z�b�g���v�Z����
    output.tex0.zw = output.tex0.xy * -1.0f;
    output.tex1.zw = output.tex1.xy * -1.0f;
    output.tex2.zw = output.tex2.xy * -1.0f;
    output.tex3.zw = output.tex3.xy * -1.0f;
    output.tex4.zw = output.tex4.xy * -1.0f;
    output.tex5.zw = output.tex5.xy * -1.0f;
    output.tex6.zw = output.tex6.xy * -1.0f;
    output.tex7.zw = output.tex7.xy * -1.0f;
    // �I�t�Z�b�g�Ɋ�e�N�Z����UV���W�𑫂��Z���āA���ۂɃT���v�����O����UV���W�ɕϊ�
    output.tex0 += float4(tex, tex);
    output.tex1 += float4(tex, tex);
    output.tex2 += float4(tex, tex);
    output.tex3 += float4(tex, tex);
    output.tex4 += float4(tex, tex);
    output.tex5 += float4(tex, tex);
    output.tex6 += float4(tex, tex);
    output.tex7 += float4(tex, tex);
	return output;
}