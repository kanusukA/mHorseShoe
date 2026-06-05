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
    vec3 lightdirectionArray[5];
};

layout (location = 1) in vec2 uv;


layout (location = 2) in mat3 TBN;

layout (location = 5) in vec3 TWorldViewDir; 
layout (location = 6) in vec3 worldViewPos;
layout (location = 7) in vec3 eyeViewDir; 


layout (location = 8) in vec3 p_normal;


const vec3 mainLightDir = vec3(1.8,1,4); // The direction is already fliped for easier calculation!

void main(){

    vec3 pos_normal = (p_normal + 1.0)/2;// THE NORMAL VALUE HERE RANGES FROM 0-1, Which is easier to calculate light.

    vec2 texCoords = vec2( uv.x , 1- uv.y);

    vec3 diffuseColor = texture(diffuseMap,uv).xyz;

    float mainLight = dot(pos_normal,normalize(mainLightDir));

    vec3 viewDir = normalize(-worldViewPos);

    vec3 normal = normalize((texture(normalMap,texCoords).xyz * 2.0f) - 1.0f);

    vec3 crossNormal = cross(p_normal,normal);

    vec3 lightToWorld = normalize(worldViewPos - lightpositionArray[0].xyz);

    float diffuseFactor = clamp(dot(crossNormal,  normalize(mainLightDir) ),0.0,1.0);

    float SpotFactor = dot(lightToWorld,lightdirectionArray[0].xyz);

    // texCoords = float2(texCoords.x, 1-texCoords.y);

    FragColor = vec4(vec3(diffuseFactor),1.0);

   // FragColor = vec4(diffuseColor * mainLight * diffuseFactor * 2,1.0);  //vec4(diffuseColor * TBN[2] ,1.0);

//    float3 LightToWorld = normalize(worldviewPos - lightpositionArray[i].xyz); //set for directional light

//    float Diffusefactor = clamp(  dot(Normal,-lightdirectionArray[i].xyz),0.0,1.0); // Light to normal Cos0

//         color += float4(diffuseColor * Diffusefactor * LightPower,1.0f);

    // vec3 viewDir = TBN * normalize(viewPos - fragPos);

    // vec3 Normal = normalize(((texture(normalMap,uv).xyz * 2.0) - 1.0)) * TBN;
    // vec4 diffuse = texture(diffuseMap, uv * scale);

    // float combined_light = 0;
    
    
    //     vec3 lightDir = normalize((lightpositionArray[0] - fragPos)) * TBN;

    //     vec3 reflected_ray = reflect(-lightDir,Normal);

    //     combined_light += dot(reflected_ray,viewDir);
    //     // merg this!!!!!!!!!!
    //     // how lightDir and view Dirctions are equal from each other
    

   // FragColor = diffuse * clamp(combined_light,0.0,1.0);

}