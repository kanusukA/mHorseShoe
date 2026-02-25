float4 mainFP(
    float2 texCoords : TEXCOORD0,
    float depth : TEXCOORD1
) :SV_Target0
{
    return float4(depth,depth,depth,1.0); //passing depth for shadow Map
}