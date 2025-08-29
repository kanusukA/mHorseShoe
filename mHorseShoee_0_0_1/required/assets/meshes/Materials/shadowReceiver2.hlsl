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

struct VertexOut{
    float4 position : POSITION;
    float2 texCoords : TEXCOORD0;


    float3 EyeViewDirection : TEXCOORD1;
    float3 Normal : TEXCOORD2;
    float3 lightdirection : TEXCOORD3;
    float3 lightposition: TEXCOORD4;
    float3 worldviewPos : TEXCOORD5;
};

VertexOut mainVP(
    float4 position : POSITION,
    float2 texCoords : TEXCOORD0,

    float4 normal : NORMAL,

    uniform float4x4 worldviewproj,
    uniform float4x4 worldviewIT,
    uniform float3 lightdirection,
    uniform float4x4 worldview,
    uniform float3 lightposition

){
    VertexOut output = (VertexOut) 0;


    output.Normal = normalize(mul((float3x3)worldviewIT ,normal.xyz)).xyz;

    output.lightdirection = lightdirection;
    output.lightposition = lightposition;
    output.worldviewPos = mul(worldview ,position).xyz;

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
    float alphamale = alpha *alpha;
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


float4 mainFP(
    float4 position : POSITION,
    float2 texCoords : TEXCOORD0,
    float3 EyeViewDirection : TEXCOORD1,
    float3 Normal : TEXCOORD2,
    float3 lightdirection : TEXCOORD3,
    float3 lightposition : TEXCOORD4,
    float3 worldviewPos : TEXCOORD5,
//!SKIP
    uniform float4 DiffuseColor,
    uniform float LightPower,
    uniform float skyIntensity,
    uniform float roughness,
    uniform float metallic 
    
    ):SV_Target0
    
    {

    //spotFactor
    float4 color = float4(0,0,0,1.0);
    float4 SpecularColor = float4(1,1,1,1);

    float3 LightToWorld = normalize(worldviewPos - lightposition); //set for directional light
    float SpotFactor = dot(LightToWorld, lightdirection);

    

    // Diffuse factor

    float Diffusefactor = clamp(  dot(Normal,-lightdirection),0.0,1.0); // Light to normal Cos0

    color += float4(DiffuseColor.rgb * Diffusefactor * LightPower,1.0f);

    //Specular

    float3 viewDir = normalize(-worldviewPos);

    float3 halfwayVector = normalize(-LightToWorld + viewDir); //fix this

    float3 F0 = lerp(float3(0.4,0.4,0.4),DiffuseColor.rgb,metallic);

    //float Specularfactor =  smoothstep(0.8,1.0,dot(Normal,halfwayVector));

    float NDFGGX = DistributionGGX(Normal,halfwayVector,roughness);
    float masking = smithfunc(-LightToWorld,viewDir,Normal, roughness);
    float3 fresnel = Fersnelrefective(viewDir,halfwayVector,F0);

    color += float4(SpecularColor.rgb * NDFGGX * masking * fresnel, 1.0f);
    //color += float4(SpecularColor.rgb * Specularfactor, 1.0f);


    color = color * smoothstep(0.5,1,SpotFactor);

    color += float4(DiffuseColor.rgb * skyIntensity,1.0f);

    

    return color;



}
