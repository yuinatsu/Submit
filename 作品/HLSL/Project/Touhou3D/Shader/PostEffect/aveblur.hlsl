// �s�N�Z���V�F�[�_�[�̓���
struct PSInput
{
    float4 diff : COLOR0;       // �f�B�t���[�Y
    float4 spec : COLOR1;       // �X�y�L�����J���[
    float2 uv : TEXCOORD0;      // �e�N�X�`�����W
};
// �s�N�Z���V�F�[�_�[�̏o��
struct PSOutput
{
    float4 color : SV_TARGET0;  // �F
};

// �e�N�X�`��
SamplerState sam : register(s0);
Texture2D<float4> tex : register(t0);

PSOutput main(PSInput input)
{
    PSOutput output;					// �o�͍\����
    float4 color = float4(tex.Sample(sam, input.uv));

    float offsetU = 2.5f / 1280;        // U�l�I�t�Z�b�g
    float offsetV = 2.5f / 720;         // V�l�I�t�Z�b�g
    
    // ���σu���[
    // ��e�N�Z������w��̃e�N�Z���J���[���T���v�����O
    color += float4(tex.Sample(sam, input.uv + float2(offsetU, 0.0f)));     // �E
    color += float4(tex.Sample(sam, input.uv + float2(-offsetU, 0.0f)));    // ��
    color += float4(tex.Sample(sam, input.uv + float2(0.0f, offsetV)));     // ��
    color += float4(tex.Sample(sam, input.uv + float2(0.0f, -offsetV)));    // ��
    color += float4(tex.Sample(sam, input.uv + float2(offsetU, offsetV)));  // �E��
    color += float4(tex.Sample(sam, input.uv + float2(offsetU, -offsetV))); // �E��
    color += float4(tex.Sample(sam, input.uv + float2(-offsetU, offsetV))); // ����
    color += float4(tex.Sample(sam, input.uv + float2(-offsetU, -offsetV)));// ����
    
    // ���ϒl���Z����
    color /= 9.0f;
    
    output.color = color;
    return output;
}