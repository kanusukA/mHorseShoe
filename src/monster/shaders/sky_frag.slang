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
    float farFogOffset;
    float farFogSmoothness;
    float windSpeed;
    float4 baseColor;
    float4 highlightCol;
    float4 middleColor;
    float4 coreColor;
    float4 farFogColor;
    
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
Sampler2D fogTexture;

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

    // fog

    // float timing = (ubo.windSpeed * 0.01) * pConst.time;
    float4 farfog1 = fogTexture.Sample(uv * float2(2, 1) + float2(pConst.time * -2));
    float4 farfog2 = fogTexture.Sample(uv * float2(4, 2) + float2(pConst.time));
    float4 farfog = farfog1 * farfog2;

    float4 posterize = floor(farfog / 0.4) * 0.4;
    float2 InMinMax = float2(0, 1);
    float2 OutMinMax = float2(0.5, 1);

    float reMap = OutMinMax.x + (posterize.x - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);

    float cmask = smoothstep(ubo.farFogOffset, ubo.farFogOffset + ubo.farFogSmoothness, (uv.y + 1) - (uv.y * 2));

    InMinMax = float2(0, 1);
    OutMinMax = float2(1, 2);
    float cmap = OutMinMax.x + (farfog.x - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);

    cmask = cmask * cmap;
    cmask = step(0.8, cmask);

    float4 reCol = lerp(col, ubo.farFogColor, reMap - 0.35);

    col = lerp(col, reCol, cmask);


    return col;

}