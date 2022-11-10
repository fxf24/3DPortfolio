#include "Common.hlsl"

cbuffer Blur : register(b10)
{
    int     count;
    float   width;
    float   height;
    int     select;
    float4  blendColor;
    float2  center;
    float   radius;
    float   padding;
};


static const float2 arr[8] =
{
    float2(-1, -1), float2(0, -1), float2(1, -1),
    float2(-1, 0), float2(1, 0),
    float2(-1, 1), float2(0, 1), float2(1, 1)
};

static const float weights[13] =
{
    0.0561f, 0.1353f, 0.2780f, 0.4868f, 0.7261f, 0.9231f,
    1.0f,
    0.9231f, 0.7261f, 0.4868f, 0.2780f, 0.1353f, 0.0561f
};

float4 CrossBlur(float2 uv)
{
    float4 color = 0;
    
    for (int i = 0; i < count; i++)
    {
        float divX = (1 + i) / width;
        float divY = (1 + i) / height;
        
        color += TextureD.Sample(SamplerD, float2(uv.x + divX, uv.y));
        color += TextureD.Sample(SamplerD, float2(uv.x - divX, uv.y));
        color += TextureD.Sample(SamplerD, float2(uv.x, uv.y + divY));
        color += TextureD.Sample(SamplerD, float2(uv.x, uv.y - divY));
        //color += TextureD.Sample(SamplerD, float2(uv.x , uv.y));
        //color += TextureD.Sample(SamplerD, float2(uv.x , uv.y));
        //color += TextureD.Sample(SamplerD, float2(uv.x, uv.y ));
        //color += TextureD.Sample(SamplerD, float2(uv.x, uv.y ));
    }

    color /= count * 4;
    
    return color;
}



float4 OctaBlur(float2 uv)
{
    float4 color = 0;
    
    for (int i = 0; i < count; i++)
    {
        float divX = (1 + i) / width;
        float divY = (1 + i) / height;
        
        for (int j = 0; j < 8; j++)
        {
            float x = arr[j].x * divX + uv.x;
            float y = arr[j].y * divY + uv.y;
            
            color += TextureD.Sample(SamplerD, float2(x, y));
        }
    }

    color /= count * 8;
    
    return color;
}



float4 GaussianBlur(float2 uv)
{
    float divX = 1.0f / width;
    float divY = 1.0f / height;
    
    float sum = 0;
    float4 color = 0;
    
    for (int i = -6; i <= 6; i++)
    {
        float2 temp = uv + float2(divX * i * count, 0);
        color += weights[6 + i] * TextureD.Sample(SamplerD, temp);
        
        temp = uv + float2(0, divY * i * count);
        color += weights[6 + i] * TextureD.Sample(SamplerD, temp);
        
        sum += weights[6 + i] * 2;
    }

    color /= sum;
    
    return color;
}


float4 Mosaic(float2 uv)
{
    int x = (int) (uv.x * count);
    int y = (int) (uv.y * count);
    
    float2 temp;
    temp.x = (float) x / count;
    temp.y = (float) y / count;
    
    return TextureD.Sample(SamplerD, temp);
}

//width = radius;
//height = amount;
float4 RadialBlur(float2 uv)
{
    float2 radiusUV = uv - float2(0.5f, 0.5f);
    float r = length(radiusUV);
    radiusUV /= r;
    
    r = saturate(2 * r / width);
    
    float2 delta = -radiusUV * r * r * height / count;
    
    float4 color = 0;
    
    for (int i = 0; i < count; i++)
    {
        color += TextureD.Sample(SamplerD, uv);
        uv += delta;
    }
    
    color /= count;

    return color;
}

float4 Outline(float2 uv)
{
    float sum = 0;
    
    for (int i = 1; i <= count; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            float2 temp = float2(1.0f / width * i, 1.0f / height * i);
            temp *= arr[j];
            
            float4 albedo = TextureD.Sample(SamplerD, uv + temp);
            
            sum += albedo.a;
        }
    }

    sum /= count * 8;
    
    if (sum > 0.0f && sum < 1.0f)
        return TextureD.Sample(SamplerD, uv) * 0.9f;
    
    return TextureD.Sample(SamplerD, uv);
}
float4 Water(float2 uv)
{
    uv.x = uv.x + (sin(uv.x * count) * 0.03);
    uv.y = uv.y + (sin(uv.y * count) * 0.03);
    
    float4 Color = TextureD.Sample(SamplerD, uv);
    
    return Color;
    
}


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
    
    float4 TextureColor;
    TextureColor = TextureD.Sample(SamplerD, input.Uv);
    //TextureColor.x = 1.0f - TextureColor.x;
    
    //if (TextureColor.x)
    //{
    //    return float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
    
    //TextureColor.x *= 100.0f;
    //return float4(TextureColor.x, TextureColor.x, TextureColor.x, 1.0f);
    
    if (select == 0)
        TextureColor = TextureD.Sample(SamplerD, input.Uv);
    else if (select == 1)
        TextureColor = CrossBlur(input.Uv);
    else if (select == 2)
        TextureColor = OctaBlur(input.Uv);
    else if (select == 3)
        TextureColor = GaussianBlur(input.Uv);
    else if (select == 4)
        TextureColor = Mosaic(input.Uv);
    else if (select == 5)
        TextureColor = RadialBlur(input.Uv);
    else if (select == 6)
        TextureColor = Outline(input.Uv);
    else if (select == 7)
        TextureColor = Water(input.Uv);
    
    // 0 ~ 1 -> 0 ~ 2 -> -1 ~ 1
    float3 blend = blendColor.rgb * 2.0f - 1.0f;
    
    TextureColor.rgb += blend.rgb;
    
    float2 Distance = input.Position.xy - center.xy;
    float Dis = length(Distance);
    
    if(Dis <radius)
    {
      
        return TextureColor;
        //return float4(TextureColor.rgb * (1.0f - Dis / radius), 1);
    }
    
    return float4(0, 0, 0, 1);
}