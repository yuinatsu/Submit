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

cbuffer DofArea : register(b6)
{
    float DepthStart;
    float DepthEnd;
    float DepthScope;
}


SamplerState sam : register(s0);
SamplerState depthsam : register(s1);
SamplerState bokesam1 : register(s2);
SamplerState bokesam2 : register(s3);

Texture2D<float4> tex : register(t0);
Texture2D<float4> depth : register(t1);
Texture2D<float4> boke1 : register(t2);
Texture2D<float4> boke2 : register(t3);

PSOutput main(PSInput input)
{
    PSOutput output;
    float fade;
    float4 color1, color2;
    float blend;
    // �[�x�l���擾����
    float d = depth.Sample(sam,input.uv);
    
    // �t�H�[�J�X��񂩂�ڂ₯�����Z�o
    if ( d < DepthStart)
    {
        fade = 1.0f - d / DepthStart;
    }
    else if (d < DepthEnd)
    {
        fade = 0.0f;
    }
    else
    {
        fade = (d - DepthEnd) / (1.0f - DepthEnd);
    }
    // �ڂ₯������F���Z�o
    if (fade < 0.5f)
    {
        color1 = tex.Sample(sam, input.uv);
        color2 = boke1.Sample(sam, input.uv);
        blend = fade / 0.5f;
    }
    else
    {
        color1 = boke1.Sample(sam, input.uv);
        color2 = boke2.Sample(sam, input.uv);
        blend = (fade - 0.5f) / 0.5f;
    }
    output.color = lerp(color1,color2,blend);
    return output;
}
