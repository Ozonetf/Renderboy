#version 330 core

uniform float myfloat;
uniform sampler2D myTex;
uniform sampler2D myTex2;

in vec2 texCoord;
in vec3 outColor;

out vec4 FragColor;
void main()
{
   FragColor = vec4(outColor*myfloat, 0.0) + mix(texture(myTex, texCoord), texture(myTex2, texCoord), 1);
}