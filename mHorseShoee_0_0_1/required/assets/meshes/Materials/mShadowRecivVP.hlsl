


struct VSOutput {
 float4 oPos : POSITION;
 float4 oUV : TEXCOORD0;
 float3 oNormal : TEXCOORD1;
 float4 oShadowLightspacePos0 : TEXCOORD2;
};


VSOutput mainVS(
    float4 iPos : POSITION,
    float3 iNormal : NORMAL,
    float4 iTexCoord0 : TEXCOORD0,
    float4 iColor : COLOR0,

    uniform float4x4 worldViewProj,
    uniform float4x4 worldMat,
    uniform float4x4 texMatrix0

){

    VSOutput output = (VSOutput) 0;

    output.oPos = mul(worldViewProj, iPos);
    output.oUV = iTexCoord0;
    output.oNormal = normalize(mul((float3x3)worldMat, iNormal));

    float4 worldPos = mul(worldMat, iPos);
    output.oShadowLightspacePos0 = mul(texMatrix0, worldPos);

    output.oUV.z = output.oPos.z;
    
    return output;

}