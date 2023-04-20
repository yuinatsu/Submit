struct VertexInput {
	float3 pos : POSITION0;//���W
	float3 norm : NORMAL;//�@��
	float4 dif : COLOR0;//�f�B�t���[�Y�J���[
	float4 spec : COLOR1;//�X�y�L�����J���[
	float4 uv0 : TEXCOORD0;//�e�N�X�`�����W
	float4 uv1 : TEXCOORD1;//�T�u�e�N�X�`�����W
	// �o���v�}�b�v
	float3 tan:TANGENT;//�ڃx�N�g��
	float3 binorm:BINORMAL;//�]�@��
	// �X�L�j���O���b�V��
	int4 BlendIndices0:BLENDINDICES0;//�{�[�������pfloat�^�萔�z��C���f�b�N�X0
	float4 BlendWeight0:BLENDWEIGHT0;//�{�[�������p�E�G�C�g�l0
};
struct VSOutput {
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float3 norm:NORMAL;
	float2 uv:TECOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
	float3 tan:TANGENT;
	float3 bin:BINORMAL;
};
// ��{�p�����[�^
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
	float4		AntiViewportM[4];		// �A���`�r���[�|�[�g�s��
	float4		ProjectionM[4];		// �r���[�@���@�v���W�F�N�V�����s��
	float4		viewM[3];				// ���[���h�@���@�r���[�s��
	float4		localM[3];		// ���[�J���@���@���[���h�s��

	float4		ToonOutLineSize;			// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource;				// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource;				// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor;			// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding;
};

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
	// ���[�J���@���@���[���h�s��
	float4		Matrix[54 * 3];
};

// ���̑��̍s��
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
	float4		ShadowMapLightViewProjectionMatrix[3][4];			// �V���h�E�}�b�v�p�̃��C�g�r���[�s��ƃ��C�g�ˉe�s�����Z��������
	float4		TextureMatrix[3][2];								// �e�N�X�`�����W����p�s��
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_VS_BASE			: register(b1)
{
	DX_D3D11_VS_CONST_BUFFER_BASE				g_Base;
};

// ���̑��̍s��
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX		: register(b2)
{
	DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX		g_OtherMatrix;
};

// �X�L�j���O���b�V���p�́@���[�J���@���@���[���h�s��
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX	: register(b3)
{
	DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX	g_LocalWorldMatrix;
};

VSOutput main(VertexInput input)
{
	VSOutput output;
	float4 pos = float4(input.pos, 1);
	float4 ILocalWorldMatrix[3];
	// �����̃t���[���̃u�����h�s��̍쐬
	ILocalWorldMatrix[0] = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 0] * input.BlendWeight0.x;
	ILocalWorldMatrix[1] = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 1] * input.BlendWeight0.x;
	ILocalWorldMatrix[2] = g_LocalWorldMatrix.Matrix[input.BlendIndices0.x + 2] * input.BlendWeight0.x;

	ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 0] * input.BlendWeight0.y;
	ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 1] * input.BlendWeight0.y;
	ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.y + 2] * input.BlendWeight0.y;

	ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 0] * input.BlendWeight0.z;
	ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 1] * input.BlendWeight0.z;
	ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.z + 2] * input.BlendWeight0.z;

	ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 0] * input.BlendWeight0.w;
	ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 1] * input.BlendWeight0.w;
	ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[input.BlendIndices0.w + 2] * input.BlendWeight0.w;

	// ���[�J�����W�����[���h���W�ɕϊ�
	float4 IWorldPos;
	IWorldPos.x = dot(pos, ILocalWorldMatrix[0]);
	IWorldPos.y = dot(pos, ILocalWorldMatrix[1]);
	IWorldPos.z = dot(pos, ILocalWorldMatrix[2]);
	IWorldPos.w = 1.0f;
	// ���[���h���W���r���[���W�ɕϊ�
	float4 IViewPos;
	IViewPos.x = dot(IWorldPos, g_Base.viewM[0]);
	IViewPos.y = dot(IWorldPos, g_Base.viewM[1]);
	IViewPos.z = dot(IWorldPos, g_Base.viewM[2]);
	IViewPos.w = 1.0f;
	//�r���[���W���ˉe���W�ɕϊ�
	output.svpos.x = dot(IViewPos, g_Base.ProjectionM[0]);
	output.svpos.y = dot(IViewPos, g_Base.ProjectionM[1]);
	output.svpos.z = dot(IViewPos, g_Base.ProjectionM[2]);
	output.svpos.w = dot(IViewPos, g_Base.ProjectionM[3]);

	// �]�@���A�ڐ��A�@�����r���[�x�N�g���ɕϊ�
	float3 IWorldNrm;
	IWorldNrm.x = dot(input.norm, ILocalWorldMatrix[0].xyz);
	IWorldNrm.y = dot(input.norm, ILocalWorldMatrix[1].xyz);
	IWorldNrm.z = dot(input.norm, ILocalWorldMatrix[2].xyz);
	float3 IWorldBin;
	IWorldBin.x = dot(input.binorm, ILocalWorldMatrix[0].xyz);
	IWorldBin.y = dot(input.binorm, ILocalWorldMatrix[1].xyz);
	IWorldBin.z = dot(input.binorm, ILocalWorldMatrix[2].xyz);
	float3 IWorldTan;
	IWorldTan.x = dot(input.tan, ILocalWorldMatrix[0].xyz);
	IWorldTan.y = dot(input.tan, ILocalWorldMatrix[1].xyz);
	IWorldTan.z = dot(input.tan, ILocalWorldMatrix[2].xyz);

	float3 IViewNrm;
	IViewNrm.x = dot(IWorldNrm, g_Base.viewM[0].xyz);
	IViewNrm.y = dot(IWorldNrm, g_Base.viewM[1].xyz);
	IViewNrm.z = dot(IWorldNrm, g_Base.viewM[2].xyz);
	float3 IViewBin;
	IViewBin.x = dot(IWorldBin, g_Base.viewM[0].xyz);
	IViewBin.y = dot(IWorldBin, g_Base.viewM[1].xyz);
	IViewBin.z = dot(IWorldBin, g_Base.viewM[2].xyz);
	float3 IViewTan;
	IViewTan.x = dot(IWorldTan, g_Base.viewM[0].xyz);
	IViewTan.y = dot(IWorldTan, g_Base.viewM[1].xyz);
	IViewTan.z = dot(IWorldTan, g_Base.viewM[2].xyz);

	output.uv.x = dot(input.uv0, g_OtherMatrix.TextureMatrix[0][0]);
	output.uv.y = dot(input.uv0, g_OtherMatrix.TextureMatrix[0][1]);

	output.pos = IViewPos.xyz;
	output.norm = IViewNrm;

	output.tan = input.tan;
	output.bin = input.binorm;
	return output;
}