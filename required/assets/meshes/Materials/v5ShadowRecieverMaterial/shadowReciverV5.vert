#version 450 core


layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
layout (location = 1) in vec2 uv;

layout (location = 1) out vec2 texuv;


layout(std140, binding = 0, row_major) uniform OgreUniforms {
    mat4 worldviewproj;
    
}; 



void main()
{
    gl_Position = worldviewproj * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    texuv = uv;
}