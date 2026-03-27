#version 330 core

uniform float myfloat;

out vec4 FragColor;
in vec3 outColor;
void main()
{
   FragColor = vec4(outColor*myfloat, 0.0);
}