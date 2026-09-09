struct UniformBuffer{
    // float time;
    // float windSpeed;
    float starOffset;
    float starSmoothness;
    float3 fogCol1;
    float3 fogCol2;
}
[[vk::binding(1, 0)]]
ConstantBuffer<UniformBuffer> ubo;

struct PushConstants {
    float time;
}
[[vk::push_constant]] PushConstants pConst;


struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
    float2 texCoord;
};

[[vk::binding(2,0)]]
Sampler2D starTexture;

[[vk::binding(3, 0)]]
Sampler2D fogTexture;


[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target {
    float2 uv = inVert.texCoord;
    float4 col = starTexture.Sample(uv);
    col.w = clamp(col.x + col.y + col.z, 0.0f, 1.0f);
    col += float4(ubo.fogCol1, 1.0f ) * (fogTexture.Sample(uv + float2(pConst.time)).x * 0.9);
    col += float4(ubo.fogCol2, 1.0f) * (fogTexture.Sample(uv + float2(fract(pConst.time * 1.5))).x * 0.7);
    col *= smoothstep(ubo.starOffset, ubo.starOffset + ubo.starSmoothness, 1 - distance(float2(0.5, 0.5), uv));
    

    return col;

}