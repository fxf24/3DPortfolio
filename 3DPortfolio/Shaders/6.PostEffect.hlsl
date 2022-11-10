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

float4 PS(PixelInput input) : SV_TARGET
{
    float2 UV = input.Uv;
    //UV.r = floor(UV.r);
    //UV.g = floor(UV.g);
    
    //UV /= 100.0f;
    
    //            -1 ~ 1
    UV.y = UV.y + sin(UV.y * 0.03);
    
    
    float4 BaseColor = DiffuseMapping(UV);
    //float avg = BaseColor.r + BaseColor.g + BaseColor.b;
    //avg /= 3.0f;
    //float3 temp = avg;
    ////세피아
    //BaseColor.r = dot(temp.rgb, float3(0.393f, 0.769f, 0.189f));
    //BaseColor.g = dot(temp.rgb, float3(0.349f, 0.686f, 0.168f));
    //BaseColor.b = dot(temp.rgb, float3(0.272f, 0.534f, 0.131f));
    
     //흑과 백으로 나누기
    //if (any(BaseColor.rgb))
    //{
    //    BaseColor.r = 1.0f;
    //    BaseColor.g = 1.0f;
    //    BaseColor.b = 1.0f;
    //}
    //else
    //{
    //    BaseColor.r = 0.0f;
    //    BaseColor.g = 0.0f;
    //    BaseColor.b = 0.0f;
    //}
    
    // 0.33 -> 3.3 -> 3 -> 0.3
    
    //BaseColor.r *= 100.0f;
    //BaseColor.g *= 100.0f;
    //BaseColor.b *= 100.0f;
    
    //BaseColor.r = floor(BaseColor.r);
    //BaseColor.g = floor(BaseColor.g);
    //BaseColor.b = floor(BaseColor.b);
    
    //BaseColor.r /= 100.0f;
    //BaseColor.g /= 100.0f;
    //BaseColor.b /= 100.0f;
       
    //모노
    //BaseColor.rgb = BaseColor.r * 0.3f + BaseColor.g * 0.59f + BaseColor.b * 0.11f;
    //BaseColor.rgb = BaseColor.r * 0.33f + BaseColor.g * 0.33f + BaseColor.b * 0.33f;
    
    //색 반전
    //BaseColor.rgb = (1.0f - BaseColor.rgb);
    return BaseColor;
}