#version 450 core

layout (location = 0) out vec4 FragColor;


layout (binding = 2) uniform sampler2D p_ShadowMap;
layout (binding = 3) uniform sampler2D p_diffuseMap;
layout (binding = 4) uniform sampler2D p_normalMap;
layout (binding = 5) uniform sampler2D p_displacementMap;
layout (binding = 6) uniform sampler2D p_roughnessMap;

layout (location = 1) in vec2 uv;


void main(){

    FragColor = texture(p_diffuseMap,uv);


}