
struct VSInput
{
    float4 pos : POSITION;
    float4 uv : TEXCOORD0;
   // uint vertID : SV_VertexID;
};


struct VSOutput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};


//static const float4 positions[4] = {
//    {-0.5, 0.0,  0.5, 1.0},
//    { 0.5, 0.0,  0.5, 1.0},
//    {-0.5, 0.0, -0.5, 1.0},
//    { 0.5, 0.0, -0.5, 1.0}
//};


VSOutput mainVS(
    VSInput input,
    uniform float4x4 worldViewProj
    //uniform float gridSize

){
    VSOutput output = (VSOutput) 0;

    //output.pos = positions[input.vertID];
    //input.pos.xz *= 100;
    output.pos = mul(worldViewProj, input.pos);
    

    output.uv = input.uv;


    return output;
}