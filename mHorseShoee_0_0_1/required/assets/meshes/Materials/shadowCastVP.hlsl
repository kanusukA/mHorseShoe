



struct VSOutput{
float4 pos : POSITION;
float4 oViewPos : TEXCOORD0;
};


VSOutput mainVS(
    float4 inPos : POSITION,

    uniform float4x4 cWorldViewProj,
    uniform float4x4 cWorldView
){

    VSOutput output = (VSOutput) 0;

    output.pos = mul(cWorldViewProj, inPos);
    output.oViewPos = mul(cWorldView, inPos).xyz;

    return output;

}

