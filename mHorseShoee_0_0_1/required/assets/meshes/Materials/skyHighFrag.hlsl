#define sampler2D Sampler2D
#define sampler3D Sampler3D



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



uniform SAMPLER2D(starsTex, 0);

uniform SAMPLER2D(farFogTex, 1);


// THIS CONTAINS ALL TEH COLOR SHADERS !!! KYS

float4 mainPS(

    uniform float time,

    uniform float windSpeed,

    uniform float4 baseColor,

    uniform float4 highlightCol,
    uniform float highlightOffset,
    uniform float highlightSmoothness,

    uniform float4 middleColor,
    uniform float middleOffset,
    uniform float middleSmoothness,

    uniform float4 coreColor,
    uniform float coreOffset,
    uniform float coreSmoothness,


    uniform float bumpOffset,
    uniform float bumpRange,
    uniform float bumpHeight,

    uniform float starsOffset,

    uniform float4 farFogCol,
    uniform float farFogOffset,
    uniform float farFogSmoothness,

    float4 pos : SV_Position,
    float2 uv : TEXCOORD0

) : SV_Target
{
    //float2 cuv = float2(uv.x * (smoothstep(0, 0.5, uv.x) - smoothstep(0.5, 1, uv.x)), uv.y * (smoothstep(0, 0.5, uv.y) - smoothstep(0.5, 1, uv.y)));
    
    float bump = (smoothstep(bumpOffset - bumpRange, bumpOffset, uv.x)
        - smoothstep(bumpOffset, bumpOffset + bumpRange, uv.x)) * bumpHeight;
    
    //float high = smoothstep(0.5, 1, uv.y);
    float highOffset = smoothstep(highlightOffset - bump, highlightOffset + highlightSmoothness, uv.y);
    float mOffset = smoothstep(middleOffset - bump, middleOffset + middleSmoothness, uv.y);
    float cOffset = smoothstep(coreOffset - bump, coreOffset + coreSmoothness, uv.y);
    
    // calculate bump
    
    float4 col = lerp(baseColor, highlightCol, highOffset);
    col = lerp(col, middleColor, mOffset );
    col = lerp(col, coreColor, cOffset);
    
    
    // stars
    
    float4 star = tex2D(starsTex, uv);
    
    float sOffset = 1 - smoothstep(starsOffset, starsOffset + 0.45, uv.y);
    
    col += star * sOffset;
    
    
    
    // far fog
    
    // time calculation
    float timing = (windSpeed * 0.01) * time;
   
    
    float4 farFog1 = tex2D(farFogTex, uv * float2(2, 1) + float2(timing* -2, timing* -2));
    float4 farFog2 = tex2D(farFogTex, uv * float2(4, 2) + float2(timing, timing));
    
    float4 farFog = farFog1 * farFog2;
    
    // hardFog
    float4 posterize = floor(farFog / 0.4) * 0.4;
    float2 InMinMax = float2(0, 1);
    float2 OutMinMax = float2(0.5, 1);
    
    float reMap = OutMinMax.x + (posterize - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);
    
    
    //float fOffset = smoothstep(farFogOffset, farFogOffset + farFogSmoothness, uv.y);
    
    //farFog = lerp(0.8, 1, farFog);
    
    float cmask = smoothstep(farFogOffset, farFogOffset + farFogSmoothness, uv.y + 0.1);
    
    InMinMax = float2(0, 1);
    OutMinMax = float2(1, 2);
    float cmap = OutMinMax.x + (farFog - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);
    
    cmask = cmask * cmap;
    cmask = step(0.8, cmask);
    
    float4 reCol = lerp(col, farFogCol, reMap - 0.35);
    
    col = lerp(col, reCol, cmask);
    
    
    return col;
}
