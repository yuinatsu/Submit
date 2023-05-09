// ���_�V�F�[�_�[����
struct VSInput
{
    float3 pos : POSITION;              // ���W
    float3 norm : NORMAL;               // �@��
    float4 diff : COLOR0;               // �f�B�t���[�Y�J���[
    float4 spec : COLOR1;               // �X�y�L�����J���[
    float4 uv0 : TEXCOORD0;             // �e�N�X�`�����W
    float4 uv1 : TEXCOORD1;             // �T�u�e�N�X�`�����W
	// �X�L�j���O���b�V��
    int4 blendIndices0 : BLENDINDICES0; // �{�[�������pfloat�^�萔�z��C���f�b�N�X0
    float4 blendWeight0 : BLENDWEIGHT0; // �{�[�������p�E�G�C�g�l0
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
// �X�L�j���O���b�V���p
struct ConstBufferLocalWorldMat
{
    float4 mat[54 * 3]; // ���[�J���s�񂩂烏�[���h���W�ւ̕ϊ�
};

// ��{�p�����[�^
cbuffer BufferBase : register(b1)
{
    ConstBufferBase g_base;     // �O���[�o���ϐ�
};
// ���̑��̃p�����[�^
cbuffer BufferOtherMat : register(b2)
{
    ConstBufferOtherMat g_otherMat;     // �O���[�o���ϐ�
};
// �X�L�j���O���b�V���p
cbuffer BufferLocalWorldMat : register(b3)
{
    ConstBufferLocalWorldMat g_localWorldMat;       // �O���[�o���ϐ�
};

VSOutput main(VSInput input)
{
    VSOutput output;                            // �o�͍\����
    float4 pos = float4(input.pos.xyz, 1.0f);   // ���͒��_���W

    // 1.�����̃t���[���̃u�����h�s����쐬
    float4 localWorldMat[3];
    localWorldMat[0] = g_localWorldMat.mat[input.blendIndices0.x + 0] * input.blendWeight0.x;
    localWorldMat[1] = g_localWorldMat.mat[input.blendIndices0.x + 1] * input.blendWeight0.x;
    localWorldMat[2] = g_localWorldMat.mat[input.blendIndices0.x + 2] * input.blendWeight0.x;
    
    localWorldMat[0] += g_localWorldMat.mat[input.blendIndices0.y + 0] * input.blendWeight0.y;
    localWorldMat[1] += g_localWorldMat.mat[input.blendIndices0.y + 1] * input.blendWeight0.y;
    localWorldMat[2] += g_localWorldMat.mat[input.blendIndices0.y + 2] * input.blendWeight0.y;
    
    localWorldMat[0] += g_localWorldMat.mat[input.blendIndices0.z + 0] * input.blendWeight0.z;
    localWorldMat[1] += g_localWorldMat.mat[input.blendIndices0.z + 1] * input.blendWeight0.z;
    localWorldMat[2] += g_localWorldMat.mat[input.blendIndices0.z + 2] * input.blendWeight0.z;
    
    localWorldMat[0] += g_localWorldMat.mat[input.blendIndices0.w + 0] * input.blendWeight0.w;
    localWorldMat[1] += g_localWorldMat.mat[input.blendIndices0.w + 1] * input.blendWeight0.w;
    localWorldMat[2] += g_localWorldMat.mat[input.blendIndices0.w + 2] * input.blendWeight0.w;
    
    // 2.���[�J�����W�����[���h���W�ɕϊ�
    float4 worldPos;
    worldPos.x = dot(pos, localWorldMat[0]);
    worldPos.y = dot(pos, localWorldMat[1]);
    worldPos.z = dot(pos, localWorldMat[2]);
    worldPos.w = 1.0f;
    
    // 3.���[���h���W���r���[�s��ɕϊ�
    float4 viewPos;
    viewPos.xyz = mul(worldPos, g_base.viewMat);
    viewPos.w = 1.0f;
    
    // 4.�r���[���W���X�N���[�����W�ɕϊ�
    output.svpos = mul(viewPos, g_base.projMat);

    // 5.���W�̃Z�b�g
    output.pos = viewPos;

    // 6.�󂯎���Ă����J���������擾
    output.vpos = output.svpos;

    return output;
}