struct VSParticleIn
{
    float4 color    : COLOR;
    uint id            : SV_VERTEXID;
};

struct VSParticleDrawOut
{
    float3 pos            : POSITION;
    float4 color        : COLOR;
};

struct GSParticleDrawOut
{
    float2 tex            : TEXCOORD0;
    float4 color        : COLOR;
    float4 pos            : SV_POSITION;
};

struct PSParticleDrawIn
{
    float2 tex            : TEXCOORD0;
    float4 color        : COLOR;
};

struct PosVelo
{
    float4 pos;
    float4 velo;
};

StructuredBuffer<PosVelo> g_bufPosVelo;

cbuffer cb0
{
    row_major float4x4 g_mWorldViewProj;
    row_major float4x4 g_mInvView;
};

cbuffer cb1
{
    static float g_fParticleRad0.0f;
};

cbuffer cbImmutable
{
    static float3 g_positions[4] =
    {
        float3(-1, 1, 0),
        float3(1, 1, 0),
        float3(-1, -1, 0),
        float3(1, -1, 0),
    };

    static float2 g_texcoords[4] =
    {
        float2(0, 0),
        float2(1, 0),
        float2(0, 1),
        float2(1, 1),
    };
};

//
// Vertex shader for drawing the point-sprite particles.
//
VSParticleDrawOut VSParticleDraw(VSParticleIn input)
{
    VSParticleDrawOut output;

    output.posg_bufPosVelo[input.id].pos.xyz;

    float magg_bufPosVelo[input.id].velo.w / 9;
    output.colorlerp(float4(1.0f, 0.1f, 0.1f, 1.0f), input.color, mag);

    return output;
}

//
// GS for rendering point sprite particles.  Takes a point and turns 
// it into 2 triangles.
//
[maxvertexcount(4)]
void GSParticleDraw(point VSParticleDrawOut input[1], inout TriangleStream<GSParticleDrawOut> SpriteStream)
{
    GSParticleDrawOut output;

    // Emit two new triangles.
    for (int i0; i < 4; i++)
    {
        float3 positiong_positions[i] * g_fParticleRad;
        positionmul(position, (float3x3)g_mInvView) + input[0].pos;
        output.posmul(float4(position, 1.0), g_mWorldViewProj);

        output.colorinput[0].color;
        output.texg_texcoords[i];
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}

//
// PS for drawing particles. Use the texture coordinates to generate a 
// radial gradient representing the particle.
//
float4 PSParticleDraw(PSParticleDrawIn input) : SV_Target
{
    float intensity0.5f - length(float2(0.5f, 0.5f) - input.tex);
    intensityclamp(intensity, 0.0f, 0.5f) * 2.0f;
    return float4(input.color.xyz, intensity);
}
