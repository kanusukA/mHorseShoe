// fragment shader grass

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

float4 tex2D(Sampler2D s, float2 v) { return s.t.SampleLevel(s.s, v,1.0f); }


uniform SAMPLER2D(diffMap, 0);


//SamplerState sample : register(s1);

//INPUT
struct psInput
{
    float2 texCoord : TEXCOORD0;
};



float4 mainPS(
    psInput input
) : SV_Target
{
    //float4 color = float4(input.texCoord, 0.0f, 1.0f);
    float4 color = float4(0.23, 0.79, 0.0, 1.0);
    //printf("this");
    //float4 color = float4(1.0f,0.0f,1.0f, 1.0f);
    return color;
}