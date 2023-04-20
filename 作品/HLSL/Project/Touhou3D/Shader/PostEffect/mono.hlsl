// �s�N�Z���V�F�[�_�[����
struct PSInput
{
    float4 diff : COLOR0;       // �f�B�t���[�Y
    float4 spec : COLOR1;       // �X�y�L�����J���[
    float2 uv : TEXCOORD0;      // �e�N�X�`�����W
};
// �s�N�Z���V�F�[�_�[�o��
struct PSOutput
{
    float4 color : SV_TARGET0; // �F
};

// �e�N�X�`��
SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);

PSOutput main(PSInput input)
{
    PSOutput output;                    // �o�͍\����
    output.color = float4(tex.Sample(sam, input.uv.xy));
    // ���m�N��
    output.color = (0.3f * output.color.x) + (0.6f * output.color.z) + (0.1f * output.color.z);
    return output;
}