
//#version 330 core

#version 420   //  layout (binding = XXX) demanda >= 420

out vec4 result;

in vec2 TexCoord;

layout (binding = 0) uniform sampler2D Texture1; // binding = 0 eh equivalente a GL_TEXTURE0.

void main()
{
    //result = texture2D(Texture1, TexCoord).rgba; 
    result = vec4(1, 0, 0, 1.0);
} 