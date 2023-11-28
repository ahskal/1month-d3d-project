#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Mov : MOVE0;
    float2 Size : SIZE0;
};
struct PixelInput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    
    float4 wPosition : POSITION0;
};

struct DeferredOutput
{
    float4 diffuse : SV_TARGET0;
    float4 specular : SV_TARGET1;
    float4 normal : SV_TARGET2;
    float4 emissive : SV_TARGET3;
    float4 ambient : SV_TARGET4;
	
};

static const float2 TEXCOORD[4] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f)
};


VertexInput VS(VertexInput input)
{
    VertexInput output;
    output.Size = input.Size;
    output.Mov = input.Mov;
    
    // 뷰 행렬과 월드 행렬의 순서 수정
    output.Position = mul(input.Position, View);
    output.Position = mul(output.Position, World);
    
    return output;
}

[maxvertexcount(4)]
void GS(point VertexInput input[1], inout TriangleStream<PixelInput> output)
{
    float4 vertices[4];
    
    // 왼 아래
    vertices[0] = float4(input[0].Position.xy + input[0].Mov, input[0].Position.z, 1.0f);
    // 왼쪽 위
    vertices[1] = float4(vertices[0].x, vertices[0].y + input[0].Size.y, input[0].Position.z, 1.0f);
    // 오른 아래
    vertices[2] = float4(vertices[0].x + input[0].Size.x, vertices[0].y, input[0].Position.z, 1.0f);
    // 오른 위
    vertices[3] = float4(vertices[0].x + input[0].Size.x, vertices[0].y + input[0].Size.y, input[0].Position.z, 1.0f);
    
    PixelInput pixelInput;
    
    [unroll(4)]
    for (int i = 0; i < 4; i++)
    {
        // 월드에서 다시 NDC로 변환
        pixelInput.Position = mul(vertices[i], GSProj);
        pixelInput.Uv = TEXCOORD[i];
        output.Append(pixelInput);
    }
}

DeferredOutput PS(PixelInput input)
{
    DeferredOutput output;
    
    // Diffuse 및 Specular 매핑 함수 호출
 //   output.diffuse = DiffuseMapping(input.Uv);
 //   output.specular = float4(SpecularMapping(input.Uv), saturate(Shininess / MAX_SHININESS));
 //   output.diffuse.rgb *= Kd.rgb;
 //   output.specular.rgb *= Ks.rgb;
    
    //// Normal 및 Emissive를 상수에서 동적으로 설정하도록 변경할 수 있습니다.
    //// 적절한 텍스처 매핑이나 계산을 통해 값을 설정하세요.
    //output.normal = float4(0, 0, 1, 0); // 예: 기본값을 (0, 0, 1)로 설정
    //output.emissive = float4(0, 0, 0, 1);
    //
    //output.ambient = float4(Ka.rgb * output.diffuse.rgb, 1);
    
    return output;
}