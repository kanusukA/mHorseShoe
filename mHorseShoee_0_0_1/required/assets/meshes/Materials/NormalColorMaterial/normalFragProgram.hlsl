
float4 mainPS(
    //uniform float gridSize,
    float4 pos : SV_Position,
    float2 uv : TEXCOORD0,

    uniform float4 color

) : SV_Target
{
    return float4(uv.x,uv.y,0.0,1.0);
}