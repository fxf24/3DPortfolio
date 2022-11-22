#include "Common.hlsl"


struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
	//정점변환때만 쓰이는 멤버
    float4 Indices : INDICES0;
    float4 Weights : WEIGHTS0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float3 wPosition : POSITION0;
    float2 Uv : UV0;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float4 vPosition : POSITION1;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    //World = offset * World;
    
    
     [branch]
    if (input.Weights.x)
        output.Position = mul(input.Position, SkinWorld(input.Indices, input.Weights));
    else
        output.Position = mul(input.Position, World);
    
    output.wPosition = output.Position;
    output.vPosition = mul(output.Position, ShadowVP);
    output.Position = mul(output.Position, ViewProj);
    output.Uv = input.Uv;
    output.Normal = mul(input.Normal, (float3x3) World);
    output.Tangent = mul(input.Tangent, (float3x3) World);
    output.Binormal = cross(output.Normal, output.Tangent);
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 BaseColor = DiffuseMapping(input.Uv);
    
    float3 Normal = NormalMapping(input.Tangent, input.Binormal, input.Normal, input.Uv);
    
    BaseColor = Lighting(BaseColor, input.Uv, Normal, input.wPosition);
    BaseColor = AddShadow(BaseColor, input.vPosition);
    return BaseColor;
}