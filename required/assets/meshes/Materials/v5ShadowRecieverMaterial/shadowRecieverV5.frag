#version 450 core

layout (location = 0) out vec4 FragColor;

//uniform sampler2D shadowMap;
layout (binding = 2) uniform sampler2D shadowMap;
layout (binding = 3) uniform sampler2D diffuseMap;
layout (binding = 4) uniform sampler2D normalMap;

// layout (binding = 2) uniform sampler2D p_ShadowMap;
// layout (binding = 3) uniform sampler2D p_diffuseMap;
// layout (binding = 4) uniform sampler2D p_normalMap;
// layout (binding = 5) uniform sampler2D p_displacementMap;
// layout (binding = 6) uniform sampler2D p_roughnessMap;

layout(std140,binding = 1,row_major)uniform OgreUniforms{
    float scale;
    vec3 lightpositionArray[5];
};

layout (location = 1) in vec2 uv;


layout (location = 2) in mat3 TBN;

layout (location = 5) in vec3 TfragPos; // Tangent Frag Position
layout (location = 6) in vec3 TviewPos; // Tangent View Position




void main(){

    vec3 viewDir = normalize(-TviewPos);

    vec3 Normal = normalize(((texture(normalMap,uv).xyz * 2.0) - 1.0) * TBN);
    vec4 diffuse = texture(diffuseMap, uv * scale);
    for (int i = 0; i < 5 ; i++){
        vec3 lightPos = normalize(lightpositionArray[i] * TBN);
        // merg this!!!!!!!!!!
        // how lightDir and view Dirctions are equal from each other
    }

    FragColor = texture(diffuseMap, uv * scale);

}