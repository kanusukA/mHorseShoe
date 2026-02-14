//SKIP

#define PI 3.14159265359


struct Sampler2D
{
    Texture2D t;
    SamplerState s;
};


#define SAMPLER2D(name, reg) \
    Texture2D name ## Tex : register(t ## reg);\
    SamplerState name ## State : register(s ## reg);\
    static Sampler2D name = {name ## Tex, name ## State}

SamplerState samplerState
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 tex2D(Sampler2D s, float2 v)
{
    return s.t.SampleLevel(s.s, v, 1.0f);
}

uniform SAMPLER2D(p_ShadowMap, 0);
uniform SAMPLER2D(p_diffuseMap, 1);
uniform SAMPLER2D(p_normalMap, 2);
uniform SAMPLER2D(p_displacementMap,3);
uniform SAMPLER2D(p_roughnessMap, 4);

struct VertexOut{
    float4 position : POSITION;
    float2 texCoords : TEXCOORD0;
    float3 EyeViewDirection : TEXCOORD1;
    float3 worldviewPos : TEXCOORD3;

    float3x3 TBN: TEXCOORD4;

    float3 tangentWorldViewDir : TEXCOORD7;
    
};

VertexOut mainVP(
    float4 position : POSITION,
    float2 texCoords : TEXCOORD0,
    float4 p_tangent : TANGENT,
    float3 Normal : NORMAL,

    uniform float4x4 worldviewproj,
    //uniform float4x4 worldviewIT,
    uniform float3 lightdirection,
    uniform float4x4 worldview,
    uniform float3 lightposition

    

){
    VertexOut output = (VertexOut) 0;

    float3 tangent = normalize(mul((float3x3)worldview,p_tangent.xyz));
    float3 normal = normalize(mul((float3x3) worldview, Normal.xyz));
    float3 binormal = cross(normal.xyz,tangent.xyz) * p_tangent.w;

    

    

    output.TBN = float3x3(tangent,binormal,normal);

    output.worldviewPos = mul(worldview ,position).xyz;

    output.tangentWorldViewDir = mul(output.TBN,output.worldviewPos.xyz);

    output.tangentWorldViewDir = normalize(-output.tangentWorldViewDir);
    
    //output.Normal = normal;

    output.EyeViewDirection = normalize(-output.worldviewPos);

    output.position = mul(worldviewproj,position);
    output.texCoords = texCoords;
    return output;
}



// NDF
float DistributionGGX(
    float3 surfaceNormal,
    float3 halfwayVector,
    float alpha
){
    float NdotH = max(dot(surfaceNormal,halfwayVector),0.0);
    float alphamale = alpha * alpha;
    float d = (NdotH * NdotH) * (alphamale - 1.0) + 1.0;
    d = PI * d * d;
    return alphamale / max(d,1e-6);
}

float SchlickApprox (float dotx, float k){
    return dotx / (dotx * (1.0 - k) + k);
}

float smithfunc(float3 lightDir, float3 viewDir, float3 Normal, float roughness){

    float NdotV = max(dot(Normal, viewDir), 0.0);
    float NdotL = max(dot(Normal, lightDir), 0.0);

    float k = 1.0 - roughness;
    k = (k *k) / 8.0;

    float g1NdotV = SchlickApprox(NdotV, k);
    float g1NdotL = SchlickApprox(NdotL, k);

    return g1NdotL * g1NdotV;

    
}

float Fersnelrefective(float3 viewDir, float3 halfwayVec, float3 F0){
    return F0 + (1.0 - F0) * pow(1.0-dot(viewDir,halfwayVec),5.0);
}

// Parallax Mapping
float2 parallaxMapping(float2 texCoords, float3 viewDir){
    const float numLayers = 25;

    float layerDepth = 1.0/ numLayers;

    float currentLayerDepth = 0.0;

    float2 p = (viewDir.xy / viewDir.z) * 0.06; // SCALE
    float2 deltaTexCoords = p / numLayers;

    float2 currentTexCoords = texCoords;
    float currentDepthMapValue = tex2D(p_displacementMap,currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue){

        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = tex2D(p_displacementMap,currentTexCoords);
        currentLayerDepth += layerDepth;

    }

    return currentTexCoords;

}


float4 mainFP(
    float4 position : POSITION,
    float2 texCoords : TEXCOORD0,
    float3 EyeViewDirection : TEXCOORD1,
    float3 worldviewPos : TEXCOORD3,

    float3x3 TBN: TEXCOORD4,

    float3 tangentWorldViewDir : TEXCOORD7,
    
    
    uniform float numLight,
    uniform float4 lightdirectionArray[5],
    uniform float4 lightpositionArray[5],
    uniform float4x4 worldviewIT,

//!SKIP
    //uniform float4 DiffuseColor,
    uniform float LightPower,
    uniform float skyIntensity,
    uniform float roughness,
    uniform float metallic,

    uniform int flipParaX,
    uniform int flipParaY

    //uniform float parallaxScale

    
    ):SV_Target0
    {

    
   // float3 tangentView =  normalize((viewDir.x * tangent.xyz) + (viewDir.y * binormal) + (viewDir.z * p_normal));

    if(flipParaX > 0){
        texCoords = float2(1-texCoords.x, texCoords.y);
    }
    if(flipParaY > 0){
        texCoords = float2(texCoords.x, 1-texCoords.y);
    }

    texCoords = parallaxMapping(texCoords,tangentWorldViewDir);

    

    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0){
        discard;
    }



    //worldviewPos = mul(TBN,worldviewPos);

    float3 viewDir = normalize(-worldviewPos);

    float3 diffuseColor = tex2D(p_diffuseMap,texCoords).rgb;
    //float roughness = tex2D(p_roughnessMap,texCoords).r;

    float3 Normal = normalize((tex2D(p_normalMap,texCoords).xyz * 2.0) - 1.0f);
   // bumpMap = normalize(bumpMap);

    //float3 Normal = normalize((bumpMap.x * tangent.xyz) + (bumpMap.y * binormal) + (bumpMap.z * p_normal));
    //float3 Normal = normalize(float3(0,1,0));

    //spotFactor
    
    float4 SpecularColor = float4(1,1,1,1);

    float4 color = float4(0,0,0,1.0);

    for (int i = 0; i < numLight; i++){

        //float3 lightPosition = mul(TBN,lightpositionArray[i].xyz);
        //float3 lightdirection = mul(TBN, lightdirectionArray[i].xyz);

        float3 LightToWorld = normalize(worldviewPos - lightpositionArray[i].xyz); //set for directional light
        float SpotFactor = dot(LightToWorld, lightdirectionArray[i].xyz);
        
        // Diffuse factor

        float Diffusefactor = clamp(  dot(Normal,-lightdirectionArray[i].xyz),0.0,1.0); // Light to normal Cos0

        color += float4(diffuseColor * Diffusefactor * LightPower,1.0f);

        //Specular

        float3 halfwayVector = normalize(-LightToWorld + viewDir); //fix this

        float3 F0 = lerp(float3(0.4,0.4,0.4),diffuseColor.rgb,metallic);

        //float Specularfactor =  smoothstep(0.8,1.0,dot(Normal,halfwayVector));

        float NDFGGX = DistributionGGX(Normal,halfwayVector,roughness);
        float masking = smithfunc(-LightToWorld,viewDir,Normal, roughness);
        float3 fresnel = Fersnelrefective(viewDir,halfwayVector,F0);

        color += float4(SpecularColor.rgb * NDFGGX * masking * fresnel, 1.0f);
        //color += float4(SpecularColor.rgb * Specularfactor, 1.0f);


        color = color * smoothstep(0.5,1,SpotFactor);
    
    }

    color += float4(diffuseColor.rgb * skyIntensity,1.0f);

   // color = float4(Normal.xyz,1.0);
  // color = tex2D(p_displacementMap,texCoords);

    return color;



}
