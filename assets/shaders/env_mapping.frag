#version 430 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

layout(std140, binding = 0) uniform u_camera
{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(fragPos - cam_pos);
    vec3 R = reflect(I, normalize(normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
