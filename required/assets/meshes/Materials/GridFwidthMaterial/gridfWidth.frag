#version 450 core


layout (location = 0) out vec4 FragColor;

layout (location = 1) in vec2 uv;
  
layout(std140,binding = 1, row_major) uniform OgreUniforms{
    vec4 color;
}; 

void main()
{
    // vec2 gridUV = uv * scale;

    // vec2 grid = abs(fract(gridUV - 0.5) - 0.5) / fwidth(gridUV);
    // float lin = min(grid.x,grid.y);

    // float color = 1 - min(lin,1);

    // FragColor = vec4(1, 1, 1, color) * smoothstep(radius, 1, (1 - distance(vec2(0.5, 0.5), uv)));
    FragColor = color;

} 