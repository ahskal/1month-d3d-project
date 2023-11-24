#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
};

struct CHullOutput
{
    float edgeTessFactor[4] : SV_TessFactor;
    float insideTessFactor[2] : SV_InsideTessFactor;
};

struct HullOutput
{
    float4 Position : POSITION;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
};


#define NUM_CONTROL_POINTS 4

cbuffer EdgeInfo : register(b10)
{
    float4 edges;
    float2 inside;
	float2 _padding;
}



VertexInput VS(VertexInput input)
{
    return input;
}

CHullOutput CHS(InputPatch<VertexInput, NUM_CONTROL_POINTS> input)
{
    CHullOutput output;

	output.edgeTessFactor[0] = edges.x;
	output.edgeTessFactor[1] = edges.y;
	output.edgeTessFactor[2] = edges.z;
	output.edgeTessFactor[3] = edges.w;
	output.insideTessFactor[0] = inside.x;
	output.insideTessFactor[1] = inside.y;


    return output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHS")]
HullOutput HS(InputPatch<VertexInput, NUM_CONTROL_POINTS> input,
	uint i : SV_OutputControlPointID)
{
    HullOutput output;

    output.Position = input[i].Position;

    return output;
}


[domain("quad")]
PixelInput DS(CHullOutput input, float2 uv : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
    PixelInput output;

	float4 v1 = lerp(patch[0].Position, patch[2].Position, uv.x);
	float4 v2 = lerp(patch[1].Position, patch[3].Position, uv.x);
	
	float4 pos = lerp(v1, v2, 1 - uv.y);
	output.Position = float4(pos.xyz, 1.0f);

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return float4(1, 1, 0, 1);
}
