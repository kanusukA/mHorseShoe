// conversion for model V3 to V4/5
#define sampler2D Sampler2D
#define sampler3D Sampler3D



struct Sampler2D
{
    Texture2D t;
    SamplerState s;
};

struct Sampler3D
{
    Texture3D t;
    SamplerState s;
};

#define SAMPLER2D(name, reg) \
    Texture2D name ## Tex : register(t ## reg);\
    SamplerState name ## State : register(s ## reg);\
    static Sampler2D name = {name ## Tex, name ## State}

#define SAMPLER3D(name, reg) \
    Texture3D name ## Tex : register(t ## reg);\
    SamplerState name ## State : register(s ## reg);\
    static Sampler3D name = {name ## Tex, name ## State}

SamplerState samplerState
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 tex3D(Sampler3D s, float3 v)
{
    return s.t.Sample(s.s, v);
}

float4 tex2D(Sampler2D s, float2 v)
{
    return s.t.SampleLevel(s.s, v, 1.0f);
}




//float4 starsTexture_ST = float4(1, 1, 1, 1);
//float4 cloudTexture_ST = float4(1, 1, 1, 1);

float pi = 3.14159265359;


uniform SAMPLER2D(farFogTex, 0);



float circle(float2 uv, float radius)
{
    float2 dist = uv - float2(0.5, 0.5);
    return 1. - smoothstep(radius - (radius * 0.01),
                         radius + (radius * 0.01),
                         dot(dist, dist) * 4.0);
    
}


float4 mainPS
(
    float4 pos : SV_Position,
    float3 uv : TEXCOORD0,

    //uniform float time,
    uniform float2 moonPos,

    uniform float moonSize,
    uniform float4 moonCoreCol,
    uniform float4 moonSmoothCol,

    uniform float moonPhaseSize,
    uniform float moonPhasePos,
    uniform float moonPhaseSmoothness
    
    //uniform float farFogOffset,
    //uniform float farFogSmoothness
   
) : SV_Target
{
    
    
    float2 cuv = float2( uv.x * (smoothstep(0, 0.5, uv.x) - smoothstep(0.5, 1, uv.x)), uv.y);
    
    float moonSDF = distance(cuv.xy, moonPos);
    
    float moonPhaseSDF = distance(cuv.xy - (float2(0.08, 0.1) * moonPhasePos), moonPos.xy);
    
    
    float moon = (1 - smoothstep(moonSize - moonPhaseSmoothness, moonSize, moonSDF)) - (1 - smoothstep(moonPhaseSize - moonPhaseSmoothness, moonPhaseSize, moonPhaseSDF));
    //moon += step(moonSDF, moonSize - 0.005) - step(moonPhaseSDF, moonSize + 0.005);
   
   float4 moonCol = lerp(moonSmoothCol, moonCoreCol, moon);
    
    //moon = saturate(moon * -worldSpaceLightPos.y);
    
    float4 col = lerp(float4(0, 0, 0, 0), moonCol, moon);
    
   
    //float4 fog = tex2D(farFogTex, cuv);
    
    //float fOffset = smoothstep(farFogOffset, farFogOffset + farFogSmoothness, uv.x);
    
   // col += fog * fOffset;
    
    return col;
    
}