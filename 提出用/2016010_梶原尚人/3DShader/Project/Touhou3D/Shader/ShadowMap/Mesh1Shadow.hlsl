// ���_�V�F�[�_�[����
struct VSInput
{
    float3 pos : POSITION;      // ���W
    float3 norm : NORMAL;       // �@��
    float4 diff : COLOR0;       // �f�B�t���[�Y�J���[
    float4 spec : COLOR1;       // �X�y�L�����J���[
    float4 uv0 : TEXCOORD0;     // �e�N�X�`�����W
    float4 uv1 : TEXCOORD1;     // �T�u�e�N�X�`�����W
};
// ���_�V�F�[�_�[�o��
struct VSOutput
{
    float4 pos : POSITION0;     // 3D��ԍ��W
    float4 vpos : POSITION1;    // 2D�s�N�Z�����W
    float4 svpos : SV_POSITION; // ��ʋ�Ԃ̃s�N�Z�����W
    float3 norm : NORMAL;       // �@��
    float4 diff : COLOR0;       // �f�B�t���[�Y�J���[
    float4 spec : COLOR1;       // �X�y�L�����J���[
    float4 uv : TEXCOORD;       // �e�N�X�`�����W
};

// ��{�p�����[�^
struct ConstBufferBase
{
    float4x4 antiViewportMat;	// Viewport�s��̋t�s��
    float4x4 projMat;			// �ˉe�ϊ��s��
    float4x3 viewMat;			// �r���[�ϊ��s��
    float4x3 worldMat;			// ���[���h�ϊ��s��
    float4 toonOutLineSize;		// �g�D�[���֊s���̃T�C�Y
    float diffSource;			// �f�B�t���[�Y����
    float specSource;			// �X�y�L��������
    float mulSpecColor;			// �X�y�L�����F��Z�l
    float padding;				// �l�ߕ�
};
// ���̑��̃p�����[�^
struct ConstBufferOtherMat
{
    float4 shadowMapLightViewProjMat[3][4];      // �e�}�b�v�̃��C�g�r���[�Ǝˉe�s��̏�Z
    float4 texMat[3][2];                         // �e�N�X�`���}�b�s���O�̕ϊ��s��
};

// ��{�p�����[�^
cbuffer BufferBase : register(b1)
{
    ConstBufferBase g_base;		// �O���[�o���ϐ�
};
// ���̑��̃p�����[�^
cbuffer BufferOtherMat : register(b2)
{
    ConstBufferOtherMat g_otherMat;		// �O���[�o���ϐ�
};

VSOutput main(VSInput input)
{
    VSOutput output;						    // �o�͍\����
    float4 pos = float4(input.pos, 1.0f);		// ���͒��_���W

    // 1.���[�J�����W�����[���h���W�ɕϊ�
    float4 worldPos;
    worldPos.xyz = mul(pos, g_base.worldMat);
    worldPos.w = 1.0f;

    // 2.���[���h���W���r���[���W�ɕϊ�
    float4 viewPos;
    viewPos.xyz = mul(worldPos, g_base.viewMat);
    viewPos.w = 1.0f;

    // 3.�r���[���W���X�N���[�����W�ɕϊ�
    output.svpos = mul(viewPos, g_base.projMat);

    // 4.�o�͂���l�̃Z�b�g
    output.pos = viewPos;
    
    // 5.�J���������Z�b�g
    output.vpos = output.svpos;

    return output;
}