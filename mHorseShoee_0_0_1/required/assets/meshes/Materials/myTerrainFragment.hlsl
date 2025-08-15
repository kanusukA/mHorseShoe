

float4 mainPS(
    float4 pos : POSITION,
    float3 uv : TEXCOORD0,
    float4 displacement : TEXCOORD1
) : SV_Target
{

    float4 white = float4(1.0f,1.0f,1.0f,1.0f);
    float4 red = float4(1.0f,0.0f,0.0f,1.0f);

    float dis = displacement.x/10;

    float4 col = lerp(white,red,dis);
    return col;

}