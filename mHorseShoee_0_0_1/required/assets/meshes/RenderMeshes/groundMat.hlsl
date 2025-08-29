struct VertexOut{
    float4 position : POSITION;
    float2 texCoords : TEXCOORD0;


};

VertexOut mainVP(
    float4 pos : POSITION,
    float2 texCoords : TEXCOORD0,

    uniform float4x4 worldviewproj

){
    VertexOut output = (VertexOut) 0;

    output.position = mul(worldviewproj, pos);
    output.texCoords = texCoords;

    return output;
}

float4 mainFP(
    float4 pos : POSITION,
    float2 texCoords : TEXCOORD0,

    uniform float4 DiffuseColor
):SV_Target0{
    return float4(0,0,0,1);
}

