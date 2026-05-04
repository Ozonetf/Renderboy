#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTCoord;
layout(location = 3) in vec3 aNormal;

uniform mat4 transform;
uniform mat4 proj;
uniform mat4 view;
uniform mat3 normalTransform;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;
void main()
{
    gl_Position = proj * view * transform * vec4(aPos, 1);
    normal = normalTransform * aNormal;
    fragPos = vec3(transform * vec4(aPos, 1));
    texCoord = aTCoord;
}
