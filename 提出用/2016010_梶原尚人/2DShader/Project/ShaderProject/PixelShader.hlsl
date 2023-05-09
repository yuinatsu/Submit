// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse	: COLOR0;		// �f�B�t���[�Y�J���[
	float4 Specular	: COLOR1;		// �X�y�L�����J���[
	float2 uv		: TEXCOORD0;	// �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color      : SV_TARGET0;		// �F
};

// �萔�o�b�t�@
cbuffer buff : register(b0) {
	float threthold;
	float angle;
}

SamplerState sam;// �T���v��
Texture2D tex : register(t0);// �e�N�X�`��
Texture2D ptn : register(t1);// �p�^�[��
Texture2D norm : register(t2);// �@���}�b�v

// 1�s�N�Z�����Ƃɓh��Ԃ��ۂɌĂ΂��
PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float discval = ptn.Sample(sam, input.uv).r;
	float yedge = pow(1.0 - saturate(abs(threthold - input.uv.y)), 5);
	if (input.uv.y + (yedge * discval) < threthold)
	{
		discard;
	}
	float3 light = float3(cos(angle), -sin(angle), 0.5);
	light = normalize(light);
	float3 normal = norm.Sample(sam, input.uv).rgb;			// nx,ny,nz
	normal = (normal * 2) - 1;
	normal = normalize(normal);
	normal.z = -normal.z;

	float ambient = 0.25f;
	float bright = saturate(saturate(dot(-light, normal)) + ambient);			// ���邳

	float4 col = tex.Sample(sam, input.uv);			// ��ʂɃe�N�X�`���o��
	output.color = col;
	if (col.a == 0.0)
	{
		discard;
	}
	float edge = saturate((1.0 - abs(input.uv.y + (yedge * discval) - threthold)));
	edge = pow(edge, 15);
	float m = 1;// fmod(input.uv.x + input.uv.y, 0.1) * 10;
	output.color.rgb = float3(0, 1, 1) * edge + (output.color.rgb * bright * m) * (1.0 - edge);
	return output;
}