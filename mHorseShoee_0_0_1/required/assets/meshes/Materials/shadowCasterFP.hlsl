

float4 mainPS(
    float4 oPos : POSITION,
    float4 iViewPos : TEXCOORD0,

    uniform float cFarDistance

) : SV_Target {

    float depth = length(iViewPos) / cFarDistance;
    return float4(depth,depth,depth,1);

}