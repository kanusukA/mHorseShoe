

struct VSInput
{
    float4 pos : POSITION;
    float4 uv : TEXCOORD;
};


struct VSOutput
{
    float4 pos : SV_Position;
    float3 uv : TEXCOORD0;
};


VSOutput mainVS(
    VSInput input,
    uniform float4x4 worldViewProj,
    uniform float4x4 worldView
)
{
    VSOutput output = (VSOutput) 0;
    output.pos = mul(worldViewProj, input.pos);
    output.uv = mul(worldView, input.pos).xyz;
    
    return output;
}
