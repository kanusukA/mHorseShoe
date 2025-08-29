struct VertexOut{
    float4 position : POSITION;
    float2 texCoords : TEXCOORD0;
};

VertexOut mainVS(
    float4 pos : POSITION,
    float2 texCoords : TEXCOORD0,
    uniform float4x4 worldviewproj
){
    VertexOut output = (VertexOut) 0;

    output.position = mul(worldviewproj, pos);

    output.texCoords = texCoords;

    return output;


}



float4 mainPS(
    float2 texCoords : TEXCOORD0,
    uniform float4 diffuse
):SV_Target0{

    return diffuse;
}