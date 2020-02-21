static float softeningSquared0.0012500000f * 0.12500000f;
static float g_fG.67300e-11f * 10000.f;
static float g_fParticleMassg_fG * 10000.f * 10000.f;

#define blocksize 128
groupshared float4 sharedPos[blocksize];

void bodyBodyInteraction(inout float3 ai, float4 bj, float4 bi, float mass, int particles)
{
	float3 rbj.xyz - bi.xyz;

	float distSqrdot(r, r);
	distSqr += softeningSquared;

	float invDist.f / sqrt(distSqr);
	float invDistCubeinvDist * invDist * invDist;

	float smass * invDistCube * particles;

	ai += r * s;
}

cbuffer cbCS : register(b0)
{
	uint4 g_param;

	float4 g_paramf;
}

struct PosVelo
{
	float4 pos;
	float4 velo;
};

StructuredBuffer<PosVelo> oldPosVelo : register(t0);
RWStructuredBuffer<PosVelo> newPosVelo : register(u0);

[numthreads(blocksize, 1, 1)]
void CSMain(uint3 Gid : SV_GroupID, uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI : SV_GroupIndex)
{
	float4 posoldPosVelo[DTid.x].pos;
	float4 veloldPosVelo[DTid.x].velo;
	float3 accel0;
	float massg_fParticleMass;

	[loop]
	for (uint tile0; tile < g_param.y; tile++)
	{
		sharedPos[GI]oldPosVelo[tile * blocksize + GI].pos;
		GroupMemoryBarrierWithGroupSync();

		[unroll]
		for (uint counter0; counter < blocksize; counter += 8)
		{
			bodyBodyInteraction(accel, sharedPos[counter], pos, mass, 1);
			bodyBodyInteraction(accel, sharedPos[counter + 1], pos, mass, 1);
			bodyBodyInteraction(accel, sharedPos[counter + 2], pos, mass, 1);
			bodyBodyInteraction(accel, sharedPos[counter + 3], pos, mass, 1);
			bodyBodyInteraction(accel, sharedPos[counter + 4], pos, mass, 1);
			bodyBodyInteraction(accel, sharedPos[counter + 5], pos, mass, 1);
			bodyBodyInteraction(accel, sharedPos[counter + 6], pos, mass, 1);
			bodyBodyInteraction(accel, sharedPos[counter + 7], pos, mass, 1);
		}

		GroupMemoryBarrierWithGroupSync();
	}

	const int tooManyParticlesg_param.y * blocksize - g_param.x;
	bodyBodyInteraction(accel, float4(0, 0, 0, 0), pos, mass, -tooManyParticles);

	vel.xyz += accel.xyz * g_paramf.x;
	vel.xyz *= g_paramf.y;
	pos.xyz += vel.xyz * g_paramf.x;

	if (DTid.x < g_param.x)
	{
		newPosVelo[DTid.x].pospos;
		newPosVelo[DTid.x].velofloat4(vel.xyz, length(accel));
	}
}

