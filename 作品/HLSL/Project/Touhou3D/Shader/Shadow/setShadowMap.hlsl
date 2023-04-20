// �s�N�Z���V�F�[�_�[�̓���
struct PSInput
{
	float4 diff : COLOR0;       // �f�B�t���[�Y�J���[
	float4 spec : COLOR1;       // �X�y�L�����J���[
	float4 uv : TEXCOORD0;		// �e�N�X�`�����W
	float4 lpos : POSITION;		// ���[�J�����W
};
// �s�N�Z���V�F�[�_�[�̏o��
struct PSOutput
{
	float4 color : SV_TARGET0;	// �F
};

// ��{�p�����[�^
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

// ��{�p�����[�^
cbuffer BufferBase : register(b1)
{
	ConstBufferBase g_base;
} ;

// �f�B�t���[�Y�}�b�v�e�N�X�`��
SamplerState sam : register(s0);
Texture2D tex : register(t0);

// �e�N�X�`��(�n�[�h�V���h�E)
SamplerState depth : register(s1);		// �[�x�o�b�t�@
Texture2D depthtex : register(t1);		// �[�x�o�b�t�@

PSOutput main(PSInput input)
{
	PSOutput output;					// �o�͍\����

	// �e�N�X�`���J���[�̓ǂݍ���
	float4 texDiffColor = tex.Sample(sam, input.uv.xy);

	// �o�͐F
	float4 defaultOutput = input.diff * texDiffColor + input.spec;

	// �o�̓��l
	defaultOutput.a = input.diff.a * texDiffColor.a * g_base.factorColor.a;
	output.color = defaultOutput;

	// UV���W��Ԃɕϊ�
	float2 shadowMapUV = input.lpos.xy / input.lpos.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;
	// Z�l���v�Z(���C�g�r���[�X�N���[��)
	float lposZ = input.lpos.z / input.lpos.w;
	// UV���W���g���ăV���h�E�}�b�v����e���T���v�����O
	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
	{
		// �[�x�o�b�t�@
        float shadowMapZ = depthtex.Sample(depth, shadowMapUV).r;
        if (lposZ > shadowMapZ + 0.005f)
        {
			// �Ւf���
            output.color.xyz *= 0.5f;
        }
	}
	return output;
}
