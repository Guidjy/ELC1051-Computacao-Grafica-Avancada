
//Author: Cesar Tadeu Pozzer

//#version 120 //GLSL version 1.5
//#extension GL_EXT_geometry_shader4 : enable

#version 120


uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

void main()
{
   gl_Position = projectionMatrix * modelViewMatrix * gl_Vertex;
}
