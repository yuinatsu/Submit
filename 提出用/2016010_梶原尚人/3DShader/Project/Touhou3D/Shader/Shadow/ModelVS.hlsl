// ���_�V�F�[�_�[�̓���
struct VSInput
{
	float3 pos : POSITION;					// ���W
	float3 norm : NORMAL0;					// �@��
	float4 diff : COLOR0;					// �f�B�t���[�Y�J���[
	float4 spec : COLOR1;					// �X�y�L�����J���[
	float4 uv0 : TEXCOORD0;					// �e�N�X�`�����W
	float4 uv1 : TEXCOORD1;					// �T�u�e�N�X�`�����W
	// �X�L�j���O���b�V��
	int4 blendIndices0 : BLENDINDICES0;		// �X�L�j���O�����p Float�^�萔�z��C���f�b�N�X
	float4 blendWeight0 : BLENDWEIGHT0;     // �X�L�j���O�����p�E�G�C�g�l
};
// ���_�V�F�[�_�[�̏o��
struct VSOutput
{
	float4 diff : COLOR0;					// �f�B�t���[�Y�J���[
	float4 spec : COLOR1;					// �X�y�L�����J���[
	float4 uv : TEXCOORD0;					// �e�N�X�`�����W
	float4 lpos : POSITION;					// ���[�J�����W
	float4 pos : SV_POSITION;				// ���W
};

// ��{�p�����[�^
struct ConstBufferBase
{
	float4x4 antiViewportMat;   // Viewport�s��̋t�s��
	float4x4 projMat;           // �ˉe�ϊ��s��
	float4x3 viewMat;           // �r���[�ϊ��s��
	float4x3 worldMat;          // ���[���h�ϊ��s��
	float4 toonOutLineSize;     // �g�D�[���֊s���̃T�C�Y
	float diffSource;           // �f�B�t���[�Y����
	float specSource;           // �X�y�L��������
	float mulSpecColor;         // �X�y�L�����F��Z�l
	float padding;              // �l�ߕ�
};
// ���̑��̃p�����[�^
struct ConstBufferOtherMat
{
	float4 shadowMapLightViewProjMat[3][4];       // �e�}�b�v�̃��C�g�r���[�Ǝˉe�s��̏�Z
	float4 texMat[3][2];                          // �e�N�X�`���}�b�s���O�̕ϊ��s��
};
// �X�L�j���O���b�V���p
struct ConstBufferLocalWorldMat
{
	float4 mat[54 * 3]; // ���[�J���s�񂩂烏�[���h���W�ւ̕ϊ�
};

// �}�e���A���p�����[�^
struct ConstMaterial
{
	float4 diff;							// �f�B�t���[�Y�J���[
	float4 spec;							// �X�y�L�����J���[
	float4 ambEmissive;						// �A���r�G���g/�G�~�b�V�u�J���[
	float pow;								// �X�y�L�����̋���
	float typeParam0;						// �}�e���A���^�C�v�p�����[�^0
	float typeParam1;						// �}�e���A���^�C�v�p�����[�^1
	float typeParam2;						// �}�e���A���^�C�v�p�����[�^2
};
// �t�H�O�p�����[�^
struct ConstFog
{
	float linearAdd;						// ���`���Z
	float linearDiv;						// �t�H�O�v�Z�̋����͈�
	float density;							// ���x
	float e;								// ���R�ΐ��̒�(�w���֐��I�ȃt�H�O�v�Z�̂��߂̊�l)
	float4 color;							// �F
};
// ���C�g�p�����[�^
struct ConstLight
{
	int type;								// ���C�g�̎��(0:�|�C���g���C�g�A1:�X�|�b�g���C�g�A2:�f�B���N�V�������C�g)
	int3 padding1;							// �l�ߕ�
	float3 pos;								// ���W
	float rangePow2;						// ���C�g�̋�����2��
	float3 direction;						// ���˕���
	float fallOut;							// ���C�g�̌�����(�X�|�b�g���C�g�p)
	float3 diff;							// �f�B�t���[�Y�J���[
	float spotParam0;						// �X�|�b�g���C�g�p�p�����[�^0
	float3 spec;							// �X�y�L�����J���[
	float spotParam1;						// �X�|�b�g���C�g�p�p�����[�^1
	float4 amb;								// �A���r�G���g�J���[(����)
	float att0;								// ���C�g�̌����W��0(Attenuation)
	float att1;								// ���C�g�̌����W��1
	float att2;								// ���C�g�̌����W��2
	float padding2;							// �l�ߕ�
};
// �V�F�[�_�[���ʃp�����[�^
struct ConstBufferCommon
{
	ConstLight light[6];		// ���C�g
	ConstMaterial material;		// �}�e���A��
	ConstFog fog;				// �t�H�O
};

// �V�F�[�_�[���ʃp�����[�^
cbuffer BufferCommon : register(b0)
{
	ConstBufferCommon g_common;       // �O���[�o���ϐ�
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

// ���C�g�s��
cbuffer LightMat : register(b4)
{
	float4x4 g_lightView;       // ���C�g�r���[�ϊ��s��
	float4x4 g_lightProj;       // ���C�g�ˉe�ϊ��s��
};

VSOutput main(VSInput input)
{
	VSOutput output;					// �o�͍\����
	float4 pos = float4(input.pos, 1);  // ���͒��_���W

	// 1.�����̃t���[���̃u�����h�s��̍쐬
	float4 localWorldMatrix[3];		// ���[���h�ϊ�
	localWorldMatrix[0] = g_localWorldMat.mat[input.blendIndices0.x + 0] * input.blendWeight0.x;
	localWorldMatrix[1] = g_localWorldMat.mat[input.blendIndices0.x + 1] * input.blendWeight0.x;
	localWorldMatrix[2] = g_localWorldMat.mat[input.blendIndices0.x + 2] * input.blendWeight0.x;

	localWorldMatrix[0] += g_localWorldMat.mat[input.blendIndices0.y + 0] * input.blendWeight0.y;
	localWorldMatrix[1] += g_localWorldMat.mat[input.blendIndices0.y + 1] * input.blendWeight0.y;
	localWorldMatrix[2] += g_localWorldMat.mat[input.blendIndices0.y + 2] * input.blendWeight0.y;

	localWorldMatrix[0] += g_localWorldMat.mat[input.blendIndices0.z + 0] * input.blendWeight0.z;
	localWorldMatrix[1] += g_localWorldMat.mat[input.blendIndices0.z + 1] * input.blendWeight0.z;
	localWorldMatrix[2] += g_localWorldMat.mat[input.blendIndices0.z + 2] * input.blendWeight0.z;

	localWorldMatrix[0] += g_localWorldMat.mat[input.blendIndices0.w + 0] * input.blendWeight0.w;
	localWorldMatrix[1] += g_localWorldMat.mat[input.blendIndices0.w + 1] * input.blendWeight0.w;
	localWorldMatrix[2] += g_localWorldMat.mat[input.blendIndices0.w + 2] * input.blendWeight0.w;

	// 2.���[�J�����W�����[���h���W�ɕϊ�
	float4 worldPos;
	worldPos.x = dot(pos, localWorldMatrix[0]);
	worldPos.y = dot(pos, localWorldMatrix[1]);
	worldPos.z = dot(pos, localWorldMatrix[2]);
	worldPos.w = 1.0f ;

	// 3.���[���h���W���r���[���W�ɕϊ�
	float4 viewPos = float4(mul(worldPos, g_base.viewMat), 1.0f);

	// 4.�r���[���W���ˉe���W�ɕϊ�
	output.pos = mul(viewPos, g_base.projMat);

	// 5.���[�J���x�N�g�������[���h�x�N�g���ɕϊ�
	float3 worldNorm;					// ���[���h�@��
	worldNorm.x = dot(input.norm, localWorldMatrix[0].xyz);
	worldNorm.y = dot(input.norm, localWorldMatrix[1].xyz);
	worldNorm.z = dot(input.norm, localWorldMatrix[2].xyz);

	// 6.���[���h�x�N�g�����r���[�x�N�g���ɕϊ�
	float3 viewNorm = mul(worldNorm, g_base.viewMat);
	viewNorm = normalize(viewNorm);		// �@���𐳋K��

	// 7.�f�B�t���[�Y�J���[�A�X�y�L�����J���[�v�Z
	float4 lightLitParam;			// �s�N�Z���ɑ΂��郉�C�g�̉e�����v�Z���邽�߂̃��C�g�p�����[�^
	lightLitParam.x = dot(viewNorm, -g_common.light[0].direction);		// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓���

	float3 lightHalfVec = normalize(normalize(-viewPos.xyz) - g_common.light[0].direction);	// �n�[�t�x�N�g���̌v�Z

	lightLitParam.y = dot(lightHalfVec, viewNorm);		// �@���ƃn�[�t�x�N�g���̓���
	lightLitParam.w = g_common.material.pow;			// �X�y�L�������˗�

	float4 lightLitDest = lit(lightLitParam.x, lightLitParam.y, lightLitParam.w);			// �f�B�t���[�Y�p�x�����̃��C�g�p�����[�^�v�Z

	// �f�B�t���[�Y�J���[
	output.diff.xyz = lightLitDest.y * g_common.light[0].diff * g_common.material.diff.xyz + g_common.light[0].amb.xyz + g_common.material.ambEmissive.xyz;
	output.diff.w = g_common.material.diff.w;			// �f�t�H���g�̃}�e���A���̃f�B�t���[�Y�J���[�̃��l

	// �X�y�L�����J���[
	output.spec.xyz = lightLitDest.z * g_common.light[0].spec * g_common.material.spec.xyz;
	output.spec.w = g_common.material.spec.w;			// �f�t�H���g�̃}�e���A���̃X�y�L�����J���[�̃��l

	// 8.UV���W�̕ϊ�
	output.uv.x = dot(input.uv0, g_otherMat.texMat[0][0]);
	output.uv.y = dot(input.uv0, g_otherMat.texMat[0][1]);

	// 9.�J���������Z�b�g
	float4 lViewPos = mul(g_lightView, worldPos);			// ���[���h���W�����C�g�̃r���[���W�ɕϊ�
	output.lpos = mul(g_lightProj, lViewPos);				// ���C�g�̃r���[���W�����C�g�̎ˉe���W�ɕϊ�

	return output;
}


