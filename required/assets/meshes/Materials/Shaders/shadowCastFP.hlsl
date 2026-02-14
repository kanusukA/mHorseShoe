


struct Vertex{
    float4 pos : POSITION;
    float3 texCoords : TEXCOORD0;
    float4 color : COLOR;
};

float4 mainPS(
    Vertex p_in,
    uniform float cFarDistance

) : SV_Target {

    float depth = length(p_in.texCoords) / cFarDistance;
    return float4(depth,depth,depth,1);

}