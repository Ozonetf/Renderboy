#version 330 core
#define N_POINT_LIGHTS 5

float near = 0.01;
float far = 100;

out vec4 FragColor;
float normalizedDepth(float ndcZ)
{
    float z = ndcZ * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float nz = normalizedDepth(gl_FragCoord.z);
    FragColor = vec4(vec3(nz), 1);
}
