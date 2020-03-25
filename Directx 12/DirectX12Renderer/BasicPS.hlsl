struct vs_gbuffer_out {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 worldPos :TEXCOORD;
};

struct ps_output
{
	float3 albedo : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 specgloss : SV_TARGET2;
};

ps_output PSMain(vs_gbuffer_out pIn) : SV_TARGET
{
	ps_output output;
	output.albedo = float3(1.0f, 0.0f, 0.0f);
	output.normal = float4(pIn.normal, 1.0f);


	output.specgloss = float4(0.5,0.5,0.5,0.6);

	return output;
}