struct PSInput {
	float4 pos:POSITION0;
	float4 vpos:POSITION1;
	float3 norm:NORMAL;
	float4 uv:TEXCOORD;
	float4 diff:COLOR0;
	float4 spec:COLOR1;
	float4 svpos:SV_POSITION;
};

struct PSOutput {
	float4 color:SV_TARGET0;
};

PSOutput main(PSInput input)
{
	PSOutput output;
	// Z�l��F�Ƃ��ďo��
	output.color = input.vpos.z / input.vpos.w;
	// �����ɂȂ�Ȃ��悤�ɃA���t�@�͕K��1
	output.color.a = 1.0f;
	return output;
}