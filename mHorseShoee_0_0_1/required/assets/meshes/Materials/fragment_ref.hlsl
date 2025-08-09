sampler Tex0: register(s0);

float4 main(float4 texCoord0: TEXCOORD0): COLOR0
{
    return tex2D(Tex0, texCoord0);
}


