struct VertexOutput {
    float4 sv_position : SV_Position;
    float4 color;
    float2 texCoord;
};

struct UniformBuffer {
   // float windSpeed;
    float highlightOffset;
    float highlightSmoothness;
    float midOffset;
    float midSmoothness;
    float coreOffset;
    float coreSmoothness;
    float bumpOffset;
    float bumpRange;
    float bumpHeight;
    float4 baseCol;
    float4 highCol;
    float4 midCol;
    float4 coreCol;
};
ConstantBuffer<UniformBuffer> ubo;

[shader("fragment")]
float4 main(VertexOutput inVert) : SV_Target {

    float bump = (smoothstep(ubo.bumpOffset - ubo.bumpRange, ubo.bumpOffset, inVert.texCoord.x)
        - smoothstep(ubo.bumpOffset, ubo.bumpOffset + ubo.bumpRange, inVert.texCoord.x)) * ubo.bumpHeight;

    float highOffset = smoothstep(ubo.highlightOffset - bump, ubo.highlightOffset + ubo.highlightSmoothness, inVert.texCoord.y);
    float mOffset = smoothstep(ubo.midOffset - bump, ubo.midOffset + ubo.midSmoothness, inVert.texCoord.y);
    float cOffset = smoothstep(ubo.coreOffset - bump, ubo.coreOffset + ubo.coreSmoothness, inVert.texCoord.y);

    float4 col = lerp(ubo.baseCol, ubo.highCol, highOffset);
    col = lerp(col, ubo.midCol, mOffset);
    col = lerp(col, ubo.coreCol, cOffset);

    return col;

}