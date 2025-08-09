struct VSInput
{
    float4 pos : POSITION;
    float4 uv : TEXCOORD0;
};


struct VSOutput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};


VSOutput mainVS(
    VSInput input,
    uniform float4x4 worldViewProj,
    uniform float this,
    uniform float that
    
)
{
    VSOutput output = (VSOutput) 0;
    output.pos = mul(worldViewProj, input.pos + this - that);
    output.uv = input.uv;
    
    return output;

}