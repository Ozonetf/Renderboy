#version 330 core
#define N_POINT_LIGHTS 5

struct PointLight {
    vec3 pos;
    vec3 color;
};

struct SpotLight {
    vec3 pos;
    vec3 dir;
    float innerCutOff;
    float outerCutOff;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform PointLight pointLightList[N_POINT_LIGHTS];
uniform SpotLight u_flashlight;
uniform vec3 camPos;
uniform sampler2D albedoMap;
uniform sampler2D shinenessMap;

out vec4 FragColor;

float specularStrength = 0.5;
float specularExp = 32;
float ambientStrength = 0.1;

float attenuation(float distance, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + (linear * distance) + (quadratic * (distance * distance)));
}

vec3 calculatePointLight(PointLight light, vec3 fragPos, vec3 fragNorm, vec3 viewDir)
{
    float distToLight = length(light.pos - fragPos);
    float at = attenuation(distToLight, 1, 0.09, 0.032);

    vec3 albedo = vec3(texture(albedoMap, texCoord));
    // vec3 shineness = vec3(1);
    vec3 shineness = vec3(texture(shinenessMap, texCoord));
    // lightDir is a vector from fragPos to lightPos
    vec3 lightDir = normalize(light.pos - fragPos);

    float diff = max(dot(fragNorm, lightDir), 0);

    // negate lightDir as we want the reflected dir to ber
    // in the same direction as the viewDir, imagine looking
    // onto the polygon/fragment
    vec3 reflected = reflect(-lightDir, fragNorm);
    float spec = pow(max(dot(viewDir, reflected), 0.0), specularExp);

    vec3 ambient = at * albedo * light.color * ambientStrength;
    vec3 diffuse = at * albedo * light.color * diff;
    vec3 specular = at * shineness * light.color * spec;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 finalColor = vec3(0);
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(camPos - fragPos);
    for (int i = 0; i < N_POINT_LIGHTS; ++i)
        finalColor += calculatePointLight(pointLightList[i], fragPos, norm, viewDir);

    vec3 spot2frag = normalize(u_flashlight.pos - fragPos);
    float theta = dot(spot2frag, normalize(-u_flashlight.dir));
    if (theta > u_flashlight.outerCutOff)
    {
        finalColor += vec3(1) * texture(albedoMap, texCoord).xyz;
    }
    FragColor = vec4(finalColor, 1);
}
