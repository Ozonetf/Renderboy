#version 330 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform sampler2D myTex;
uniform sampler2D myTex2;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0);
    // FragColor = vec4(lightColor * objectColor, 1.0);
    FragColor = (0.1 + diff) * mix(texture(myTex, texCoord), texture(myTex2, texCoord), 1);
}
