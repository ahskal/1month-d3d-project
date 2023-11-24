#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
	float2 Uv : UV0;
};
struct PixelInput
{
    float4 Position : SV_POSITION;
	float2 Uv : UV0;
};

PixelInput VS(VertexInput input)
{
   
    PixelInput output;
	output.Uv = input.Uv;
    //  o           =  i X W
    output.Position = mul(input.Position, World);
    return output;
}

Texture2D diffuseTexture : register(t10);
Texture2D specularTexture : register(t11);
Texture2D normalTexture : register(t12);
Texture2D posTexture : register(t13);
Texture2D emissiveTexture : register(t14);
Texture2D ambientTexture : register(t15);

SamplerState SamplerDefault
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 PS(PixelInput input) : SV_TARGET
{
	float4 BaseColor = diffuseTexture.Sample(SamplerDefault, input.Uv);
	
    if (BaseColor.a == 0)
    {
        discard;
    }
	float3 Normal = normalTexture.Sample(SamplerDefault, input.Uv).xyz;
	float3 Specular = specularTexture.Sample(SamplerDefault, input.Uv).xyz;
	float3 wPosition = posTexture.Sample(SamplerDefault, input.Uv).xyz;
	float3 emissive = emissiveTexture.Sample(SamplerDefault, input.Uv).xyz;
	float3 ambient = ambientTexture.Sample(SamplerDefault, input.Uv).xyz;
	
	
	float4 Result = float4(DeferredDirLighting(BaseColor.rgb, Specular,
    Normal, wPosition),
    BaseColor.a);
	
	//광원 수만큼
	//반복

	for (int i = 0; i < lights[0].Size; i++)
	{
        [flatten]
		if (!lights[i].isActive)
			continue;
        
        [flatten]
		if (lights[i].Type == 0)
		{
			Result.rgb += DeferredPointLighting(BaseColor.rgb, Specular,
			Normal, wPosition, i);
		}
		else if (lights[i].Type == 1)
		{
			Result.rgb += DeferredSpotLighting(BaseColor.rgb, Specular,
            Normal, wPosition, i);
		}
	}
	
	Result.rgb += emissive;
	Result.rgb += ambient;
		return Result;
}