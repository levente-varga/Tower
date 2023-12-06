struct VS_OUT
{
    float4 color : Color;
    float4 position : SV_Position;
};

cbuffer ConstantBuffer
{
    matrix transform;
};

VS_OUT main(float3 position : Position, float4 color : Color)
{
    VS_OUT output;
    
    output.position = mul(float4(position.x, position.y, position.z, 1), transform);
    output.color = color;
    
    return output;
}