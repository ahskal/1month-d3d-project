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

struct DeferredOutput
{
    float4 diffuse : SV_TARGET0;
    float4 specular : SV_TARGET1;
    float4 normal : SV_TARGET2;
    float4 emissive : SV_TARGET3;
    float4 ambient : SV_TARGET4;
	
};

PixelInput VS(VertexInput input)
{
       
    PixelInput output;
    output.Uv = input.Uv;
    output.Position = mul(input.Position, View);
    output.Position = mul(output.Position, Proj);
    return output;
}

DeferredOutput PS(PixelInput input)
{
    DeferredOutput output;
    
    output.diffuse = DiffuseMapping(input.Uv);
    
    if (! any(output.diffuse.a))
    {
    //    discard;
    }
    
	
    output.specular = float4(SpecularMapping(input.Uv), saturate(Shininess / MAX_SHININESS));
    output.diffuse.rgb += Kd.rgb * 2.0f - 1.0f; // 0.5가 디폴트
    output.diffuse = saturate(output.diffuse);

    output.diffuse.rgb *= Kd.rgb;
    output.specular.rgb *= Ks.rgb;
	
    output.normal = float4(0,0,0, 1);
    output.emissive = float4(0, 0, 0, 1);
    output.ambient = float4(Ka.rgb * output.diffuse.rgb, 1);
    
    return output;
    
   //float4 BaseColor = float4(1, 1, 1, 1);
   //if(Kd.a)
   //{
   //    BaseColor.a = TextureD.Sample(SamplerD, input.Uv).r;
   //}
   //
   //BaseColor.rgb += Kd.rgb * 2.0f - 1.0f; // 0.5가 디폴트
   //BaseColor = saturate(BaseColor);
   //
   //return BaseColor;
}