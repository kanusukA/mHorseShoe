
#define mod(x,y) ((x) - (y) * floor((x)/(y)))


float4 mainPS(
    //uniform float gridSize,
    float4 pos : SV_Position,
    float2 uv : TEXCOORD0,

    uniform float3 cameraPos,

    uniform float scale,
    uniform float radius


) : SV_Target
{

    float2 gridUV = uv * scale;

    float2 grid = abs(frac(gridUV - 0.5) - 0.5) / fwidth(gridUV);
    float lin = min(grid.x,grid.y);

    float color = 1 - min(lin,1);

    return float4(1, 1, 1, color) * smoothstep(radius, 1, (1 - distance((0.5, 0.5), uv)));

    
    
}