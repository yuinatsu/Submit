// �s�N�Z���V�F�[�_�[����
struct PSInput
{
    float4 pos : POSITION0;     // 3D��ԍ��W
    float4 vpos : POSITION1;    // 2D�s�N�Z�����W
    float4 svpos : SV_POSITION; // ��ʋ�Ԃ̃s�N�Z�����W
    float3 norm : NORMAL;       // �@��
    float4 diff : COLOR0;       // �f�B�t���[�Y�J���[
    float4 spec : COLOR1;       // �X�y�L�����J���[
    float4 uv : TEXCOORD;       // �e�N�X�`�����W
};
// �s�N�Z���V�F�[�_�[�o��
struct PSOutput
{
    float4 color : SV_Target0;  // RGB�J���[
};

PSOutput main(PSInput input)
{
    PSOutput output;            // �o�͍\����

    // �J���[�o��
    output.color = input.vpos.z / input.vpos.w;     // Z�l�
    output.color.a = 1.0f;                          // �s����(���l:1)

    return output;
}