#version 450 core


layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
layout (location = 8) in vec2 uv;

layout (location = 1) out vec2 texuv;

layout (location = 2) out mat3 TBN;

layout (location = 2) in vec3 p_normal;
layout (location = 14) in vec4 p_tangent;
layout (location = 15) in vec3 p_biTangent;

layout (location = 5) out vec3 TWorldViewDir; 
layout (location = 6) out vec3 worldViewPos;
layout (location = 7) out vec3 eyeViewDir; 
layout (location = 8) out vec3 out_normal;


layout(std140, binding = 0, row_major) uniform OgreUniforms {
    mat4 worldviewproj;
    mat4 worldView;
    vec4 cameraWorldposition; // MUST BE VEC 4!!!!!!
    
}; 



void main()
{

    out_normal = p_normal;

    vec3 tangent = normalize(mat3(worldView) * p_tangent.xyz);
    vec3 normal = normalize(mat3(worldView) * p_normal);
    vec3 binormal = cross(normal , tangent) * p_tangent.w;

    TBN =  transpose(mat3(tangent,binormal,normal));
    
    worldViewPos = (worldView * vec4(aPos,1.0)).xyz;

    TWorldViewDir = TBN * worldViewPos.xyz;
    TWorldViewDir = normalize(-TWorldViewDir);

    eyeViewDir = normalize(-worldViewPos);
    
    //output.Normal = normal;

    // float3 tangent = normalize(mul((float3x3)worldview,p_tangent.xyz));
    // float3 normal = normalize(mul((float3x3) worldview, Normal.xyz));
    // float3 binormal = cross(normal.xyz,tangent.xyz) * p_tangent.w;

    gl_Position = worldviewproj * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    texuv = uv;
}