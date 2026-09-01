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
    float starOffset;
    float4 baseColor;
    float4 highlightCol;
    float4 middleColor;
    float4 coreColor;
    
}
[[vk::binding(1, 0)]]
ConstantBuffer<UniformBuffer> ubo;

struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
    float2 texCoord;
};

// [[vk::binding(2,0)]]
// Sampler2D starTexture;


[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target {
    float2 uv = inVert.texCoord ;
    float bump = (smoothstep(ubo.bumpOffset - ubo.bumpRange, ubo.bumpOffset, uv.x)
        - smoothstep(ubo.bumpOffset, ubo.bumpOffset + ubo.bumpRange, uv.x)) * ubo.bumpHeight;

    // float high = smoothstep(0.5, 1, uv.y);
    float highOffset = smoothstep(ubo.highlightOffset - bump, ubo.highlightOffset + ubo.highlightSmoothness, uv.y);
    float mOffset = smoothstep(ubo.middleOffset - bump, ubo.middleOffset + ubo.middleSmoothness, uv.y);
    float cOffset = smoothstep(ubo.coreOffset - bump, ubo.coreOffset + ubo.coreSmoothness, uv.y);

    // calculate bump
    float4 col = lerp(ubo.baseColor, ubo.highlightCol, highOffset);
    col = lerp(col, ubo.middleColor, mOffset);
    col = lerp(col, ubo.coreColor, cOffset);

    // float4 star = starTexture.Sample(uv);
    // float sOffset = smoothstep(ubo.starOffset, ubo.starOffset + 0.45, uv.y);
    // col = star; //* sOffset;


    return col;

}