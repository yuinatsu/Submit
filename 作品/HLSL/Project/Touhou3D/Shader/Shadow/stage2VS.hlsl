// ���_�V�F�[�_�[�̓���
struct VSInput
{
	float3 pos : POSITION;			// ���W
	float3 norm : NORMAL;			// �@��
	float4 diff : COLOR0;			// �f�B�t���[�Y�J���[
	float4 spec : COLOR1;			// �X�y�L�����J���[
	float4 uv0 : TEXCOORD0;			// �e�N�X�`�����W
	float4 uv1 : TEXCOORD1;			// �T�u�e�N�X�`�����W
};
// ���_�V�F�[�_�[�̏o��
struct VSOutput
{
	float4 diff : COLOR0;			// �f�B�t���[�Y�J���[
	float4 spec : COLOR1;			// �X�y�L�����J���[
	float4 uv : TEXCOORD0;			// �e�N�X�`�����W
	float4 lpos: POSITION;			// ���[�J�����W
	float4 pos : SV_POSITION;		// ���W
};

// ��{�p�����[�^
struct ConstBufferBase
{
	float4 antiViewportMat[4];  // Viewport�s��̋t�s��
	float4 projMat[4];          // �ˉe�ϊ��s��
	float4 viewMat[3];          // �r���[�ϊ��s��
	float4 worldMat[3];         // ���[���h�ϊ��s��
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

// ���C�g�s��
cbuffer LightMat : register(b4)
{
	float4x4 g_lightView;       // ���C�g�r���[�ϊ��s��
	float4x4 g_lightProj;       // ���C�g�ˉe�ϊ��s��
};

VSOutput main(VSInput input)
{
	VSOutput output ;							// �o�͍\����
	float4 pos = float4(input.pos, 1.0f);		// ���͒��_���W

	// 1.���[�J�����W�����[���h���W�ɕϊ�
	float4 worldPos;
	worldPos.x = dot(pos, g_base.worldMat[0]);
	worldPos.y = dot(pos, g_base.worldMat[1]);
	worldPos.z = dot(pos, g_base.worldMat[2]);
	worldPos.w = 1.0f;

	// 2.���[���h���W���r���[���W�ɕϊ�
	float4 viewPos;
	viewPos.x = dot(worldPos, g_base.viewMat[0]);
	viewPos.y = dot(worldPos, g_base.viewMat[1]);
	viewPos.z = dot(worldPos, g_base.viewMat[2]);
	viewPos.w = 1.0f;

	// 3.�r���[���W���ˉe���W�ɕϊ�
	output.pos.x = dot(viewPos, g_base.projMat[0]);
	output.pos.y = dot(viewPos, g_base.projMat[1]);
	output.pos.z = dot(viewPos, g_base.projMat[2]);
	output.pos.w = dot(viewPos, g_base.projMat[3]);

	// 4.���[�J���x�N�g�������[���h�x�N�g���ɕϊ�
	float3 worldNorm;
	worldNorm.x = dot(input.norm, g_base.worldMat[0].xyz);
	worldNorm.y = dot(input.norm, g_base.worldMat[1].xyz);
	worldNorm.z = dot(input.norm, g_base.worldMat[2].xyz);

	// 5.���[���h�x�N�g�����r���[�x�N�g���ɕϊ�
	float3 viewNorm;
	viewNorm.x = dot(worldNorm, g_base.viewMat[0].xyz);
	viewNorm.y = dot(worldNorm, g_base.viewMat[1].xyz);
	viewNorm.z = dot(worldNorm, g_base.viewMat[2].xyz);
	viewNorm = normalize(viewNorm);							// �@���𐳋K��

	// 6.�f�B�t���[�Y�J���[�A�X�y�L�����J���[�v�Z
	float4 lightLitParam;									// �s�N�Z���ɑ΂��郉�C�g�̉e�����v�Z���邽�߂̃��C�g�p�����[�^
	lightLitParam.x = dot(viewNorm, -g_common.light[0].direction);						// �@���ƃ��C�g�̋t�����x�N�g���Ƃ̓���

	float3 lightHalfVec = normalize(normalize(-viewPos.xyz) - g_common.light[0].direction);		// �n�[�t�x�N�g���̌v�Z

	lightLitParam.y = dot(lightHalfVec, viewNorm);			// �@���ƃn�[�t�x�N�g���̓���
	lightLitParam.w = g_common.material.pow;				// �X�y�L�������˗�

	float4 lightLitDest = lit(lightLitParam.x, lightLitParam.y, lightLitParam.w);				// �f�B�t���[�Y�p�x�����̃��C�g�p�����[�^�v�Z

	// �f�B�t���[�Y�J���[
	output.diff.xyz = lightLitDest.y * g_common.light[0].diff * g_common.material.diff.xyz + g_common.light[0].amb.xyz + g_common.material.ambEmissive.xyz;
	output.diff.w = g_common.material.diff.w;				// �f�t�H���g�̃}�e���A���̃f�B�t���[�Y�J���[�̃��l

	// �X�y�L�����J���[
	output.spec.xyz = lightLitDest.z * g_common.light[0].spec * g_common.material.spec.xyz;
	output.spec.w = g_common.material.spec.w;				// �f�t�H���g�̃}�e���A���̃X�y�L�����J���[�̃��l

	// 7.UV���W�̕ϊ�
	output.uv.x = dot(input.uv0, g_otherMat.texMat[0][0]);
	output.uv.y = dot(input.uv0, g_otherMat.texMat[0][1]);

	// 8.�J���������Z�b�g
	float4 lViewPos = mul(g_lightView, worldPos);		// ���[���h���W�����C�g�̃r���[���W�ɕϊ�
	output.lpos = mul(g_lightProj, lViewPos);			// ���C�g�̃r���[���W�����C�g�̎ˉe���W�ɕϊ�

	return output;
}


