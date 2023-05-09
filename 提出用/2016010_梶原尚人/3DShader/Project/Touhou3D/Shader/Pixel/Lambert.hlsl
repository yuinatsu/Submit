// �����o�[�g�g�U����
// �s�N�Z���V�F�[�_�[����
struct PSInput 
{
	float4 svpos : SV_POSITION;		// �ŏI�I�ȍ��W
	float3 pos : POSITION;			// ���W
	float3 norm : NORMAL;			// �@��
	float4 uv : TECOORD;			// �e�N�X�`�����W
	float4 diff : COLOR0;			// �f�B�t���[�Y�J���[
	float4 spec : COLOR1;			// �X�y�L�����J���[
};

// �e�N�X�`��
SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);

// �f�B���N�V�������C�g�p�̒萔�o�b�t�@
cbuffer DirectionLight : register(b0)
{
	float3 lightDir;		// ����
	float3 lightColor;		// �F
}

float4 main(PSInput input) : SV_TARGET
{
	float inner = dot(input.norm,lightDir);			// �s�N�Z���̖@���ƃ��C�g�̕����̓���
	inner *= -1.0f;			// ���ς̌��ʂ�-1����Z
	if (inner < 0.0f)
	{
		// ���̋��������̏ꍇ��0�ɂ���
		inner = 0.0f;
	}
	float3 diff = lightColor * inner;				// ���C�g�̐F
	float4 color = tex.Sample(sam, input.uv);		// �T���v������e�N�X�`���̐F�����o��
	color.xyz *= diff;								// �e�N�X�`���̐F��fiff����Z
	return color;
}