
uniform sampler2D diffuseTexture : register(s0);
uniform sampler2D shadowTexture0 : register(s1);
uniform sampler2D shadowTexture1 : register(s2);
uniform sampler2D shadowTexture2 : register(s3);
uniform sampler2D shadowTexture3 : register(s4);

float4 tex2D_inBranch(sampler2D samp, float2 texCoords)
{
    return tex2D(samp, texCoords, 0, 0);
}

half getShadowFactor(
    sampler2D shadowMapUnit,
    float4 lightSpacePos,
    float shadowmapSize,
    float inverseShadowmapSize,
    float fixedDepthBias,
    float gradientScaleBias,
    half shadowLightDotLN
){
    float depthAdjust = fixedDepthBias + (1.0f - abs(shadowLightDotLN)) * gradientScaleBias;
    lightSpacePos.z -= depthAdjust;

    float lightMask = (lightSpacePos.z < tex2D_inBranch(shadowMapUnit,lightSpacePos.xy).r);

    lightMask += 0.001 * (0.001*shadowmapSize + inverseShadowmapSize);

    return lightMask;

}


half getCsmShadowFactor(
    sampler2D shadowTexture0,
    sampler2D shadowTexture1,
    sampler2D shadowTexture2,
    sampler2D shadowTexture3,

    float4 lightSpacePos0,
    float4 lightSpacePos1,
    float4 lightSpacePos2,
    float4 lightSpacePos3,
    half shadowLightDotLN,

    float4 fixedDepthBias,
    float4 gradientScaleBias,
    float4 shadowMapSize
){

    half factor = 1;

    if (lightSpacePos0.x > 0.01 &&lightSpacePos0.y > 0.01 && lightSpacePos0.x < 0.99 && lightSpacePos0.y < 0.99){
        factor = getShadowFactor(shadowTexture0,lightSpacePos0,shadowMapSize.x,shadowMapSize.z,
        fixedDepthBias.x,gradientScaleBias.x,shadowLightDotLN);
    }
    else if(lightSpacePos1 > 0.01 && lightSpacePos1.y > 0.01 && lightSpacePos1.x < 0.99 && lightSpacePos1.y < 0.99){
        factor = getShadowFactor(shadowTexture1,lightSpacePos1,shadowMapSize.x,shadowMapSize.z,
        fixedDepthBias.y,gradientScaleBias.y,shadowLightDotLN);
    }
    else if (lightSpacePos2.x > 0.01 && lightSpacePos2.y > 0.01 && lightSpacePos2.x < 0.99 && lightSpacePos2 < 0.99){
        factor = getShadowFactor(shadowTexture2,lightSpacePos2,shadowMapSize.x,shadowMapSize.z,
        fixedDepthBias.z, gradientScaleBias.z, shadowLightDotLN);
    }
    else{
        factor = getShadowFactor(shadowTexture3, lightSpacePos3, shadowMapSize.x,shadowMapSize.z,
        fixedDepthBias.w, gradientScaleBias.w,shadowLightDotLN);
    }

    return factor;

}

//uniform float4 texMatrixScaleBias1;
//uniform float4 texMatrixScaleBias2;
//uniform float4 texMatrixScaleBias3;


float4 mainPS(
    float4 iUV : TEXCOORD0,
    float3 iNormal : TEXCOORD1,
    float4 iShadowLightspacePos0 : TEXCOORD2,

    uniform float4 lightDirection,
    uniform float4 ambientColor,
    uniform float shadowFadeDist,
    uniform float shadowMaxDist,

    uniform float4 fixedDepthBias,
    uniform float4 gradientScaleBias,
    uniform float4 shadowMapSize
) :SV_Target {

    float lDotN = dot(iNormal.xyz, -lightDirection.xyz);
    half3 lambert = saturate(lDotN);

    half shadow = getCsmShadowFactor(shadowTexture0,shadowTexture1,shadowTexture2,shadowTexture3,
    iShadowLightspacePos0,iShadowLightspacePos0,iShadowLightspacePos0,iShadowLightspacePos0,lDotN
    ,fixedDepthBias,gradientScaleBias,shadowMapSize);

    half weight = saturate(((iUV.z + shadowFadeDist) - shadowMaxDist) / shadowFadeDist);
    shadow = lerp (shadow,1,weight);

    lambert *= shadow;

    float4 brightness = float4(ambientColor.rgb + lambert,1);

    float4 oColor = tex2D(diffuseTexture,iUV.xy) * brightness;
    oColor.a = 1;

    return oColor;


}