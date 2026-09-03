struct UniformBuffer{
    // float time;
    // float windSpeed;

    float starOffset;
}
[[vk::binding(1, 0)]]
ConstantBuffer<UniformBuffer> ubo;

struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
    float2 texCoord;
};

[[vk::binding(2,0)]]
Sampler2D starTexture;


[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target {
    float2 uv = inVert.texCoord;
    float4 col = starTexture.Sample(uv);
    col.w = clamp(col.x + col.y + col.z,0.0f,1.0f) * (1 - distance(float2(0.5,0.5), uv));

    return col;

}