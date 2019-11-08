in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitan;

uniform vec3 diffuse;
uniform bool hasTexture;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

out vec4 fragColor;

void main()
{
    vec3 Kd, frag_norm;
    if (hasTexture)
    {
        Kd = texture(texture_diffuse1, uv).xyz;
        frag_norm = texture(texture_normal1, uv).xyz;
    }
    else
    {
        Kd = diffuse;
        frag_norm = normal;
    }
    
    vec3 dirlight = normalize(vec3(0.0f, 1.0f, 0.f));
    float diff = max(dot(frag_norm, dirlight), 0.f);
    fragColor = vec4(Kd * (diff + 0.1), 1.0f);
}
