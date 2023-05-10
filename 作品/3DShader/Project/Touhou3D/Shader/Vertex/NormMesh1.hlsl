// ���_�V�F�[�_�[����
struct VSInput 
{
	float3 pos : POSITION;		// ���W
	float3 norm : NORMAL;		// �@��
	float4 diff : COLOR0;		// �f�B�t���[�Y�J���[ 
	float4 spec : COLOR1;		// �X�y�L�����J���[
	float4 uv0 : TEXCOORD0;		// �e�N�X�`�����W
	float4 uv1 : TEXCOORD1;		// �T�u�e�N�X�`�����W
	// �o���v�}�b�v
	float3 tan : TANGENT;		// �ڃx�N�g��
	float3 binorm : BINORMAL;	// �]�@��
};
// ���_�V�F�[�_�[�o��
struct VSOutput 
{
	float4 pos : POSITION;		// ���W
	float4 svpos : SV_POSITION;	// ��ʋ�Ԃ̃s�N�Z�����W
	float3 norm : NORMAL;		// �@��
	float4 diff : COLOR0;		// �f�B�t���[�Y�J���[
	float4 spec : COLOR1;		// �X�y�L�����J���[
	float4 uv : TECOORD;		// �e�N�X�`�����W
	float3 tan : TANGENT;		// �ڃx�N�g��
	float3 binorm : BINORMAL;	// �]�@��
};

// ��{�p�����[�^
cbuffer ConstBufferBase : register(b1)
{
	float4x4 antiViewportMat;	// Viewport�s��̋t�s��
	float4x4 projMat;			// �ˉe�ϊ��s��
	float4x3 viewMat;			// �r���[�ϊ��s��
	float4x3 worldMat;			// ���[���h�ϊ��s��
	float4 toonOutLineSize;		// �g�D�[���֊s���̃T�C�Y
	float diffSource;			// �f�B�t���[�Y����
	float specSource;			// �X�y�L��������
	float mulSpecColor;			// �X�y�L�����F��Z�l
	float padding;				// �l�ߕ�
}

VSOutput main(VSInput input)
{
	VSOutput output;						// �o�͍\����
	float4 pos = float4(input.pos, 1);		// ���͒��_���W

	pos.xyz = mul(pos, worldMat);			// ���[���h�ϊ��s��K�p
	output.pos = pos;						// ���W���o�͂Ɋi�[
	pos.xyz = mul(pos, viewMat);			// �r���[�ϊ��s��K�p
	pos = mul(pos, projMat);				// �v���W�F�N�V�����ϊ��s��K�p

	// ���͖@����ϊ����邽��worldMat�ϊ��s�����Z
	float3 norm = mul(input.norm, worldMat);
	float3 tan = mul(input.tan, worldMat);
	float3 binorm = mul(input.binorm, worldMat);

	output.svpos = pos;						// �ϊ����ꂽ���W���i�[

	output.uv = input.uv0;					// ����UV0���o�͂Ɋi�[
	output.norm = normalize(norm);			// ���K�������@�����o�͂Ɋi�[
	output.tan = normalize(tan);			// ���K�������ڃx�N�g�����o�͂Ɋi�[
	output.binorm = normalize(binorm);		// ���K�������@�����z�x�N�g�����o�͂Ɋi�[
	return output;
}