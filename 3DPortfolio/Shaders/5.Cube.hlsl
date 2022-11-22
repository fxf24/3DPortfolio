#include "Common.hlsl"

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
    float4 Normal : NORMAL0;
    float Weights : WEIGHTS0;
};
struct PixelInput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float4 Normal : NORMAL0;
    float3 wPosition : POSITION0;
    float Weights : WEIGHTS0;
    float4 vPosition : POSITION1;
};




PixelInput VS(VertexInput input)
{
   
    PixelInput output;
    output.Uv = input.Uv;
    //  o           =  i X W
    output.Position = mul(input.Position, World);
    output.vPosition = mul(output.Position, ShadowVP);
    output.wPosition = output.Position.xyz;
    output.Position = mul(output.Position, ViewProj);
    input.Normal.w = 0.0f;
    output.Normal = mul(input.Normal, World);
    output.Weights = input.Weights;
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    
    float4 BaseColor = TextureD.Sample(SamplerD, input.Uv);
   
    float4 BaseColor2 = TextureN.Sample(SamplerN, input.Uv);
    BaseColor = BaseColor * input.Weights 
    + BaseColor2 * (1 - input.Weights);
    
    
    float3 Normal = normalize(input.Normal);
    
    BaseColor = Lighting(BaseColor, input.Uv, Normal, input.wPosition);
    
    BaseColor = AddShadow(BaseColor, input.vPosition);
    
    return BaseColor;
}