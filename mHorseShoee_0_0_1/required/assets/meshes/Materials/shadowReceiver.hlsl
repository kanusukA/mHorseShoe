

// Define outputs from vertex shader.
struct VertexVP
{
  float4 position       : POSITION;     // Vertex position in screen-space
  float4 texCoords      : TEXCOORD0;    // Vertex texture coordinates
  float3 normal : TEXCOORD1;
  float3 lightPosition : TEXCOORD2;
  float3 lightDirection : TEXCOORD3;
  float3 CamWorldPos : TEXCOORD4;
  float4 worldPos : TEXCOORD5;

};

VertexVP mainVP(
  uniform float4x4 worldviewproj,
  uniform float4x4 worldIT_mat,
  uniform float4x4 world_mat,
  uniform float3 lightdirection,
  uniform float4 lightposition,
  uniform float4 cameraWorldposition,

  float4 position : POSITION,
  float4 texCoords : TEXCOORD0,
  float4 normal : NORMAL
)
{
  VertexVP l_Out = (VertexVP) 0;
  l_Out.worldPos = mul(world_mat, position);
  l_Out.normal = normalize(mul(worldIT_mat,normal));
  l_Out.lightDirection = normalize(lightdirection);
  l_Out.lightPosition = lightposition.xyz;
  l_Out.CamWorldPos = cameraWorldposition.xyz;
  

  l_Out.position = mul(worldviewproj, position);
  l_Out.texCoords = texCoords; 

  return l_Out;
}


#define sampler2D Sampler2D

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


float4 calLight(
  float3 EyeWorldPos, // camera position in world space
  float3 WorldPos, // position in world space
  float4 DiffuseColor, // surface Color
  float SpecularIntensity,
  float4 AmbientColor,
  float4 LightColor,
  float LightPower,
  float3 LightDirection, // Normalized and in worldSpace
  float3 LightPosition, // In worldSpace
  float3 Normal, // In worldSpace
  float skyIntensity
){
  float4 SpecularColor = float4(1,1,1,1.0);

  float DiffuseFactor = dot(Normal , -LightDirection);
  float4 Color = float4(0,0,0,1.0);

  Color += AmbientColor;

  Color += float4(DiffuseColor.rgb * skyIntensity, 1.0f);

  if(DiffuseFactor > 0)
  {
    DiffuseColor = float4(DiffuseColor.rgb * LightPower * DiffuseFactor,1.0f);
    Color += DiffuseColor;

    float3 VertexToEye = normalize(WorldPos - EyeWorldPos); // Camera to Vertex(surface) direction/angle
   // float3 LightReflect = reflect(LightDirection, Normal); //angle between light and Vertex(surface)
    float3 halfwayVector = normalize(VertexToEye + LightDirection);
    float SpecularFactor = dot(Normal,VertexToEye); // How In line both angle are

    Color += float4(SpecularColor.rgb * SpecularFactor * LightPower,1.0f);

  }


  return Color;

}

float4 CalPointLight(
  float3 Normal,
  float3 WorldPos,
  float3 LightPosition, // In World Space

  float3 EyeWorldPos, // camera position in world space
  float4 DiffuseColor, // surface Color
  float SpecularIntensity,
  float4 AmbientColor,
  float4 LightColor,
  float LightPower,
  float skyIntensity
)
{
  float3 LightDirection = WorldPos - LightPosition;

  float Distance = length(LightDirection);

  LightDirection = normalize(LightDirection);

  float4 color = calLight(
    EyeWorldPos,
    WorldPos,
    DiffuseColor,
    SpecularIntensity,
    AmbientColor,
    LightColor,
    LightPower,
    LightDirection,
    LightPosition,
    Normal,
    skyIntensity
  );

  // Attenuation
//  float Attenuation = 1.0 + (0.07 * Distance) + (0.017 * Distance * Distance);

  return color;// / (Attenuation/20);

}

float4 calSpotLight(
  float3 Normal,
  float3 WorldPos,
  float3 LightPosition, // In World Space

  float3 EyeWorldPos, // camera position in world space
  float4 DiffuseColor, // surface Color
  float SpecularIntensity,
  float4 AmbientColor,
  float4 LightColor,
  float LightPower,

  float CutOff,
  float3 lightDirection, // world Space
  float skyIntensity
){

  float4 color = float4(0,0,0,1.0);

  float3 LightToPixel = normalize(WorldPos - LightPosition);

  float SpotFactor = dot(LightToPixel, lightDirection);

  if(SpotFactor > CutOff){
    color = CalPointLight(
      Normal,
      WorldPos,
      LightPosition,
      EyeWorldPos,
      DiffuseColor,
      SpecularIntensity,
      AmbientColor,
      LightColor,
      LightPower,
      skyIntensity
    );

    color = color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - CutOff));
  }

  return color;

}


float4 mainFP(

  uniform float4 DiffuseColor,
  uniform float4 AmbientColor,
  uniform float LightPower,
  uniform float4 LightColor,
  uniform float SpecularIntensity,
  uniform float skyIntensity,

  float4 position       : POSITION,    // Vertex position in screen-space
  float2 texCoords      : TEXCOORD0,    // Vertex texture coordinates
  float3 normal : TEXCOORD1,
  float3 lightPosition : TEXCOORD2,
  float3 lightDirection : TEXCOORD3,
  float3 CamWorldPos : TEXCOORD4,
  float4 worldPos : TEXCOORD5


) : SV_Target0
{

  float4 color = calSpotLight(
    normal,
    worldPos.xyz,
    lightPosition,
    CamWorldPos,
    DiffuseColor,
    SpecularIntensity,
    AmbientColor,
    LightColor,
    LightPower,
    0.3f,
    lightDirection,
    skyIntensity
  );

   
    return color;
}

