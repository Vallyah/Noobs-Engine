in vec3 normal;
uniform vec3 diffuse;
out vec4 color;
void main()
{
    vec3 dirlight = normalize(vec3(-0.5, -0.5, -0.f));
    float diff = max(dot(normal, dirlight), 0.f);
    color = vec4(diffuse * (diff + 0.1), 1.0f);
}
