struct VertexOut{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;

};

VertexOut mainVP(
    float4 position : POSITION,
    float2 texCoord : TEXCOORD0,
    uniform float4x4 worldviewproj

){
    VertexOut output = (VertexOut) 0;

    output.position = mul(worldviewproj,position);
    output.texCoord = texCoord;

    return output;

}

