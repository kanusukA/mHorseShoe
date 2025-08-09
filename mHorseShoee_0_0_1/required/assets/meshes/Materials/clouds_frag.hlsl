
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

uniform SAMPLER2D(baseTex, 0);
uniform SAMPLER2D(distortTex, 1);
uniform SAMPLER2D(secTex, 2);

float4 mainPS(
    float4 pos : SV_Position,
    float2 uv : TEXCOORD0,

    uniform float time,

    uniform float4 _cloudCol
): SV_Target
{
    
    float2 tiling = uv * 6;
    
    
    float baseCol = tex2D(baseTex, tiling - frac(time/80));
    float distort = tex2D(distortTex, (tiling) - frac(time / 20));
    float sec = tex2D(secTex, (tiling) - frac(time / 70));
    
    float4 col = lerp(sec, baseCol, distort );
    
    col = smoothstep(0.45, 0.65, col);
    
    float cloudsDark = smoothstep(0.85, 1, col);
    
    float xDis = (smoothstep(0, 0.35, uv.x) - smoothstep(0.65, 1, uv.x));
    float yDis = (smoothstep(0, 0.35, uv.y) - smoothstep(0.65, 1, uv.y));
    
    col = col * xDis * yDis;
    
    float4 cloudCol = lerp(_cloudCol, float4(0.1, 0.01, 0.06, 0.85), cloudsDark);
    
    
    
    col = lerp(float4(1, 1, 1, 0), cloudCol , col);
    
    return col;
    
}