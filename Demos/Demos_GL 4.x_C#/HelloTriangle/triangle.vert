#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 TexCoord;


void main()
{
    TexCoord = uv; 
    
    gl_Position = projectionMatrix * modelMatrix * vec4(pos.x, pos.y, pos.z, 1.0);
    
    //gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}