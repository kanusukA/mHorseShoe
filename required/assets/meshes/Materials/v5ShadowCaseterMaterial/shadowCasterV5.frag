#version 450 core




layout (location = 0) out vec4 FragColor;

layout (location = 1) in vec2 uv;

layout (location = 2)in float depth;
  

void main()
{

 FragColor = vec4(depth,depth,depth,1);
    
}