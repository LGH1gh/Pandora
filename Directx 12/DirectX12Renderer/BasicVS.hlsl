cbuffer cb0
{
	row_major float4x4 gMVP;
	row_major float4x4 gInvPV;
	float3 gCamPos;
	float3 gLightPos;
}


struct vs_gbuffer_in {
	float4 position : POSITION;
	float3 normal : NORMAL;

};
struct vs_gbuffer_out {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 worldPos :TEXCOORD;
};


vs_gbuffer_out VSMain(vs_gbuffer_in vIn)
{

	vs_gbuffer_out vOut;
	vOut.position = mul(vIn.position, gMVP);
	vOut.normal = vIn.normal;
	vOut.worldPos = vOut.position / vOut.position.w;
	return vOut;
}

