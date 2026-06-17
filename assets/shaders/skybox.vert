#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    // using xyww so that when calculatintng the pixel depth, the w component
    // is always 1 thus enabling depth testing for skyboxes
    gl_Position = pos.xyww;
}
