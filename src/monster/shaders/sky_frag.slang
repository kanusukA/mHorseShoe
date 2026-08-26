struct UniformBuffer{
    // float time;
    // float windSpeed;
    float highlightOffset;
    float highlightSmoothness;
    float middleOffset;
    float middleSmoothness;
    float coreOffset;
    float coreSmoothness;
    float bumpOffset;
    float bumpRange;
    float bumpHeight;
    float4 baseColor;
    float4 highlightCol;
    float4 middleColor;
    float4 coreColor;
}
[[vk::binding(2, 0)]]
ConstantBuffer<UniformBuffer> ubo;

struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
    float2 texCoord;
};

Sampler2D texture;


[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target {
    float bump = (smoothstep(ubo.bumpOffset - ubo.bumpRange, ubo.bumpOffset, inVert.texCoord.x)
        - smoothstep(ubo.bumpOffset, ubo.bumpOffset + ubo.bumpRange, inVert.texCoord.x)) * ubo.bumpHeight;

    // float high = smoothstep(0.5, 1, uv.y);
    float highOffset = smoothstep(ubo.highlightOffset - bump, ubo.highlightOffset + ubo.highlightSmoothness, inVert.texCoord.y);
    float mOffset = smoothstep(ubo.middleOffset - bump, ubo.middleOffset + ubo.middleSmoothness, inVert.texCoord.y);
    float cOffset = smoothstep(ubo.coreOffset - bump, ubo.coreOffset + ubo.coreSmoothness, inVert.texCoord.y);

    // calculate bump

    float4 col = lerp(ubo.baseColor, ubo.highlightCol, highOffset);
    col = lerp(col, ubo.middleColor, mOffset);
    col = lerp(col, ubo.coreColor, cOffset);

    return col;

}