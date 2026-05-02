#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTCoord;

uniform float myfloat;
uniform mat4 transform;
uniform mat4 proj;
uniform mat4 view;
out vec3 outColor;
out vec2 texCoord;

void main()
{
   gl_Position =  proj * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   outColor = aColor;
   texCoord = aTCoord;
}
