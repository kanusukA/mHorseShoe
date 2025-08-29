

struct VertexIn
{
    float4 position : POSITION;
    float4 texCoords : TEXCOORD0;
};

struct VSOutput{
    float4 pos : POSITION;
    float3 texCoords : TEXCOORD0;
    float4 color : COLOR;
};


VSOutput mainVS(
    VertexIn p_in,
    uniform float4 ambientLight,
    uniform float4x4 cWorldViewProj,
    uniform float4x4 cWorldView
){

    VSOutput output = (VSOutput) 0;

    output.pos = mul(cWorldViewProj, p_in.position);

    output.color = ambientLight;

    output.texCoords = mul(cWorldView,p_in.position).xyz;

    return output;



}

