// �@���t�������o�[�g�g�U����
// �s�N�Z���V�F�[�_�[����
struct PSInput 
{
	float4 svpos : SV_POSITION;		// �ŏI�I�ȍ��W
	float3 pos : POSITION;			// ���W
	float3 norm : NORMAL;			// �@��
	float4 uv : TECOORD;			// �e�N�X�`�����W
	float4 diff : COLOR0;			// �f�B�t���[�Y�J���[
	float4 spec : COLOR1;			// �X�y�L�����J���[
	float3 tan : TANGENT;			// �ڃx�N�g��
	float3 bin : BINORMAL;			// �]�@��
};
// �s�N�Z���V�F�[�_�[�o��
struct PSOutput
{
	float4 color : SV_TARGET0;		// �F
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
	float3 dir;								// ���˕���
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

// �萔�o�b�t�@��{�p�����[�^
struct ConstBufferBase
{
	float4 factorColor;			// �ŏI�I�ȐF�ŏ�Z�����F
	float mulAlphaColor;		// �ŏI�I�ȃA���t�@�ŏ�Z�����A���t�@�l( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float alphaTestRef;			// �A���t�@�e�X�g�̎Q�ƒl
	float2 padding1;			// �l�ߕ�
	int alphaTestCmpMode;		// �A���t�@ �e�X�g�̔�r���[�h
	int3 padding2;				// �l�ߕ�
	float4 ignoreTexColor;		// �e�N�X�`���f�[�^�𖳎�����Ƃ��Ɏg�p�����F
};

// �V�F�[�_�[���ʃp�����[�^
cbuffer BufferCommon : register(b0)
{
	ConstBufferCommon g_common;
};

// ��{�p�����[�^
cbuffer BufferBase : register(b1)
{
	ConstBufferBase g_base;
};

SamplerState sam : register(s0);		// �e�N�X�`���̃T���v�����
SamplerState normMap : register(s1);	// �@���}�b�v�e�N�X�`���̃T���v�����
SamplerState specMap : register(s2);	// ���ʔ��˃}�b�v�e�N�X�`���̃T���v�����
sampler toon : register(s3);			// �g�D�[���e�N�X�`���̃T���v���[���
Texture2D<float4> tex : register(t0);		// 2D�e�N�X�`�� 
Texture2D<float4> normTex : register(t1);	// 2D�@���}�b�v
Texture2D<float4> specTex : register(t2);	// 2D���ʔ��˃}�b�v�e�N�X�`��
Texture2D<float4> toonTex : register(t3);	// 2D�g�D�[���e�N�X�`��

PSOutput main(PSInput input) : SV_TARGET
{
	PSOutput output;
	// 1.���K��
	float3 norm = normalize(input.norm);		// �@��
	float3 tan = normalize(input.tan);			// �ڃx�N�g��
	float3 bin = normalize(input.bin);			// �]�@��

	// 2.�r���[�x�N�g���v�Z
	float3 base;
	base.x = dot(tan, -input.pos.xyz);		// ���_���W���王�_�ւ̃x�N�g����ڒ��Ԃɓ��e
	base.y = dot(bin, -input.pos.xyz);
	base.z = dot(norm, -input.pos.xyz);
	float3 eye = normalize(base);			// �r���[�x�N�g���𐳋K��

	// 3.�@���}�b�v���T���v�����O���Č��݂̃s�N�Z���̖@�����擾
	float3 normal = (normTex.Sample(normMap, input.uv.xy) - float3(0.5f, 0.5f, 0.5f)) * 2.0f;

	// 4.�f�B���N�V���i�����C�g����
	base = g_common.light[0].dir;			// ���C�g�����x�N�g���̌v�Z
	float3 lightDir;						// ���C�g�x�N�g����ڒn��Ԃɕϊ�
	lightDir.x = dot(tan, base);
	lightDir.y = dot(bin, base);
	lightDir.z = dot(norm, base);

	// 5.�@���ƌ��̕����̊p�x��p���Ċg�U�����v�Z
	float diffAngleGen = saturate(dot(normal, -lightDir));		// �f�B�t���[�Y�p�x�������v�Z(��������)
	float3 totalDiff = float3(0.0f, 0.0f, 0.0f);
	totalDiff += g_common.light[0].diff * g_common.material.diff.xyz * diffAngleGen + g_common.light[0].amb;
	
	// 6.�X�y�L�����J���[�v�Z
	base = normalize(eye - lightDir);		// �n�[�t�x�N�g���̌v�Z
	// �n�[�t�x�N�g���Ɩ@���ŃX�y�L�����[���v�Z
	float4 temp = pow(max(0.0f, dot(normal, base)), g_common.material.pow);
	float3 texSpec = specTex.Sample(specMap, input.uv);
	float3 totalSpec = float3(0.0f, 0.0f, 0.0f);
	totalSpec += temp * g_common.light[0].spec * texSpec;

	// 7.�o�̓J���[�v�Z
	totalDiff += g_common.material.ambEmissive.xyz;		// �A���r�G���g�ƃG�~�b�V�u���f�B�t���[�Y�ɉ��Z
	float3 specColor = totalSpec * g_common.material.spec.xyz;		// �X�y�L�����̃��C�e�B���O�ɃJ���[����Z����
	// �f�B�t���[�Y�e�N�X�`�����T���v�����O���ăf�B�t���[�Y���C�e�B���O�Ə�Z����
	float4 texDiffColor = tex.Sample(sam, input.uv);
	output.color.rgb = texDiffColor.rgb * totalDiff + specColor;
	// �A���t�@�l�ɌW���J���[�A���t�@����Z
	output.color.a = texDiffColor.a * g_common.material.diff.a * g_base.factorColor.a;

	return output;
}