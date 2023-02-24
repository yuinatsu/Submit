// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 pos : SV_POSITION;				// �o�͍��W
	float4 diffuse			: COLOR0;		// �f�B�t�[�Y�J���[
	float2 uv				: TEXCOORD0;	// �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color			: SV_TARGET0;	// �F
};

// �T���v��
SamplerState sam;

// �e�N�X�`��
Texture2D<float4> tex : register(t0);

// ���G�t�F�N�g�p�e�N�X�`��
Texture2D<float4> fog : register(t1);

cbuffer Effect : register(b0)
{
	float2 centPos;			// ���S���W
	float radius;			// ���a
	float redVal;			// 
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	// ���G�t�F�N�g���擾
	float4 fogColor = fog.Sample(sam,input.uv);

	// �e�N�X�`���̎擾
	float4 texColor = tex.Sample(sam, input.uv);

	// ���������
	float l = distance(centPos, input.pos.xy);

	// ���K��
	l /= radius;

	// �����l0.2����l1�ŃN�����v
	float tmp = clamp(l, 0.2, 1);

	// ���̒l�ɏ�Z����
	fogColor = fogColor * tmp;
	
	// redVal�̒l�ɍ��킹�ăe�N�X�`������e�N�X�`���ɐԐF����Z�����l����`���
	texColor = lerp(texColor, texColor * float4(1, 0.2, 0.2, 1), redVal);

	// �e�N�X�`���̐F�Ɩ��̐F�𖶂̃A���t�@�l�Ő��`���
	output.color.rgb = lerp(texColor.rgb, fogColor.rgb, fogColor.a);
	output.color.a = 1.0f;
	return output;
}