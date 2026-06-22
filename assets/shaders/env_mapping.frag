#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(fragPos - camPos);
    vec3 R = reflect(I, normalize(normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
