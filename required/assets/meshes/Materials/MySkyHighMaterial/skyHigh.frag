#version 450 core

layout (location = 0) out vec4 FragColor;


layout (binding = 2) uniform sampler2D starMap;
layout (binding = 3) uniform sampler2D farFogTex;

layout (location = 1) in vec2 uv;


layout(std140,binding = 1,row_major)uniform OgreUniforms{
    float time;

    float windSpeed;

    vec4 baseColor;

    vec4 highlightCol;
    float highlightOffset;
    float highlightSmoothness;

    vec4 middleColor;
    float middleOffset;
    float middleSmoothness;

    vec4 coreColor;
    float coreOffset;
    float coreSmoothness;


    float bumpOffset;
    float bumpRange;
    float bumpHeight;

    float starsOffset;

    vec4 farFogCol;
    float farFogOffset;
    float farFogSmoothness;
};


// THIS CONTAINS ALL TEH COLOR SHADERS !!! KYS

void main()
{
    //float2 cuv = float2(uv.x * (smoothstep(0, 0.5, uv.x) - smoothstep(0.5, 1, uv.x)), uv.y * (smoothstep(0, 0.5, uv.y) - smoothstep(0.5, 1, uv.y)));
    
    float bump = (smoothstep(bumpOffset - bumpRange, bumpOffset, uv.x)
        - smoothstep(bumpOffset, bumpOffset + bumpRange, uv.x)) * bumpHeight;
    
    //float high = smoothstep(0.5, 1, uv.y);
    float highOffset = smoothstep(highlightOffset - bump, highlightOffset + highlightSmoothness, uv.y);
    float mOffset = smoothstep(middleOffset - bump, middleOffset + middleSmoothness, uv.y);
    float cOffset = smoothstep(coreOffset - bump, coreOffset + coreSmoothness, uv.y);
    
    // calculate bump
    
    vec4 col = mix(baseColor, highlightCol, highOffset);
    col = mix(col, middleColor, mOffset );
    col = mix(col, coreColor, cOffset);
    
    
    // stars
    
    vec4 star = texture(starMap, uv);
    
    float sOffset = 1 - smoothstep(starsOffset, starsOffset + 0.45, uv.y);
    
    col += star * sOffset;
    
    
    
    // far fog
    
    // time calculation
    float timing = (windSpeed * 0.01) * time;
   
    
    vec4 farFog1 = texture(farFogTex, uv * vec2(2, 1) + vec2(timing* -2, timing* -2));
    vec4 farFog2 = texture(farFogTex, uv * vec2(4, 2) + vec2(timing, timing));
    
    vec4 farFog = farFog1 * farFog2;
    
    // hardFog
    vec4 posterize = floor(farFog / 0.4) * 0.4;
    vec2 InMinMax = vec2(0, 1);
    vec2 OutMinMax = vec2(0.5, 1);
    
    float reMap = OutMinMax.x + (posterize.x - InMinMax.x) * ((OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x));
    
    
    //float fOffset = smoothstep(farFogOffset, farFogOffset + farFogSmoothness, uv.y);
    
    //farFog = max(0.8, 1, farFog);
    
    float cmask = smoothstep(farFogOffset, farFogOffset + farFogSmoothness, uv.y + 0.1);
    
    InMinMax = vec2(0, 1);
    OutMinMax = vec2(1, 2);
    float cmap = OutMinMax.x + (farFog.x - InMinMax.x) * (OutMinMax.y - OutMinMax.x) / (InMinMax.y - InMinMax.x);
    
    cmask = cmask * cmap;
    cmask = step(0.8, cmask);
    
    vec4 reCol = mix(col, farFogCol, reMap - 0.35);
    
    FragColor = mix(col, reCol, cmask);
    
}
