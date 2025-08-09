




float4 mainPS(
    float4 pos : POSITION,
    float3 uv : TEXCOORD0,
    uniform float cFarDist
) : SV_Target
{
    float depth = length(uv) / cFarDist;
    return float4(depth, depth, depth, 1);
}