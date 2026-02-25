
// PRE_DEFINED CODE FROM SAMPLE USED TO SETUP EASIER TEXTURE
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

uniform SAMPLER2D(p_ShadowMap, 0); // This is passed from the  shadow caster material
uniform SAMPLER2D(p_diffuseMap, 1);
uniform SAMPLER2D(p_normalMap, 2);
uniform SAMPLER2D(p_displacementMap,3);
uniform SAMPLER2D(p_roughnessMap, 4);



float4 mainFP(
    float4 position : POSITION,
    float2 texCoord : TEXCOORD0
) :SV_Target0
{
    float4 diffuseColor = tex2D(p_diffuseMap,texCoord);

    return diffuseColor;
}