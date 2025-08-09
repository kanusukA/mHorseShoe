#version 460

#include <OgreUnifiedShader.h>


layout(binding = 0, row_major) uniform OgreUniforms {
    
    mat4 worldViewProj;
    mat4 modelView;
    float time;
    float noise[];
    
};

//OGRE_UNIFORMS(
//    float time;
//    mat4 worldViewProj;
//    mat4 modelView;
    
//)

//layout(binding = 0) uniform image2D noise;




layout(location = 0) in vec4 vertex;
layout(location = 2) in vec3 normal;

layout(location = 8) in vec4 uv0;

layout(location = 0) out vec2 oUv0;




vec4 worldSize = vec4(5,1,5,1);
vec4 windSpeed = vec4(1,1,1,1);

vec4 vertex_m = vec4(0,0,0,0);

layout(location = 1) out vec2 sp;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

//layout(set = 0,binding = 0) uniform sampler2D heightTex;

void main(){

    // 1) add trig functions - a) texture ?? b) pure math
    // 2) normalize positions
    // 3) set min vert height  
    // 4) animation

    vec4 worldPos =   modelView * vertex;

    vertex_m = vertex;

    vec2 samplePos = worldPos.xz/worldSize.xz;
    samplePos += (time / 20) * windSpeed.xz;
    //sp = samplePos;

    float asolote = sin(fract(time/20) * 90);
    float uns = abs(asolote);

   // vertex_m.x -= (worldPos.y * 0.5) * asolote;
    //float t = fract(time/20) + (fract(time/20) - 1);
    //vertex_m.y -= ((worldPos.y * (uns + (uns * -0.5))) * worldPos.y) ; //figure this
    float randy = (rand(vec2(int(worldPos.x),int(worldPos.z))) * 0.3) + 0.3;
    
    // // lean towards rotated direction
    vertex_m.x -= (worldPos.y * normal.x) ;
    vertex_m.z -= (worldPos.y * normal.z) ;
   
    
    //int height = int((worldPos.x + 1) * (worldPos.z + 1));
    vertex_m.y -= ((worldPos.y * (0.2)) * (worldPos.y * randy));
    //vertex_m.y -= (3) * (worldPos.y / 8);

    // normalizing to World space
    // perform vertex skewing prior to this step
    vec4 pos = worldViewProj * vertex_m ;

    
    // curve y
    //float t = worldPos.y / 10;
    //float m = (1 - t);

    //pos.y -=  m * m * 0 + 2 * m * t * (-10) + t * t * (-5);
    //pos.y += (worldPos.y * 0.2) * worldPos.y;

    //pos.x += sin(fract(sp.x) * 180);
    //pos.z += cos(fract(sp.y) * 180);

    gl_Position = pos;

    oUv0 = uv0.xy;

}

