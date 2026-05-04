#version 330 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform sampler2D myTex;
uniform sampler2D myTex2;

out vec4 FragColor;

float specularStrength = 0.5;
float specularExp = 32;
float ambientStrength = 0.1;
void main()
{
    vec3 ambient = lightColor * ambientStrength;
    vec3 norm = normalize(normal);
    // lightDir is a vector from fragPos to lightPos
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0);
    vec3 difuse = lightColor * diff;

    // negate lightDir as we want the reflected dir to ber
    // in the same direction as the viewDir, imagine looking
    // onto the polygon/fragment
    vec3 reflected = reflect(-lightDir, norm);
    vec3 viewDir = normalize(camPos - fragPos);
    float spec = pow(max(dot(viewDir, reflected), 0.0), specularExp);
    vec3 specular = lightColor * specularStrength * spec;

    vec4 objColor = mix(texture(myTex, texCoord), texture(myTex2, texCoord), 1);
    FragColor = objColor * vec4((ambient + diff + specular), 1);
}
