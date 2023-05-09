struct PSInput
{
    float4 diff : COLOR0; // �f�B�t���[�Y
    float4 spec : COLOR1; // �X�y�L�����J���[
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
};

struct PSOutput
{
    float4 color : SV_TARGET0; // �F
};

cbuffer Magnification:register(b6)
{
    float mag;
}

SamplerState texsam : register(s0);
Texture2D<float4> tex : register(t0);
PSOutput main(PSInput input)
{
    PSOutput output;
    // �e�N�X�`���̃J���[���擾
    output.color = float4(tex.Sample(texsam, input.uv.xy));
    float mag_ = 0.0f;
    float r, g, b;
    // ���񂾂񃂃m�N���ɂȂ��Ă����悤�ɏ���������
    r = abs((1.2f- mag)) *  output.color.x + (0.587f * mag) * output.color.y + (0.114f * mag) * output.color.z;
    g = abs((1.5f- mag)) * output.color.y + (0.299f * mag) * output.color.x + (0.114f * mag) * output.color.z;
    b = abs((1.1f- mag)) * output.color.z + (0.299f * mag) * output.color.x + (0.587f * mag) * output.color.y;
    output.color.x = r;
    output.color.y = g;
    output.color.z = b;
    return output;
}