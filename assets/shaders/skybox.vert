#version 430 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

layout(std140, binding = 0) uniform u_camera
{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 skyboxViewMat;

void main()
{
    TexCoords = aPos;
    vec4 pos = proj * skyboxViewMat * vec4(aPos, 1.0);
    // using xyww so that when calculatintng the pixel depth, the w component
    // is always 1 thus enabling depth testing for skyboxes
    gl_Position = pos.xyww;
}
