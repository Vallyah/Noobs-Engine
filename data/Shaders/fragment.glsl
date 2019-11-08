in vec3 world_pos;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitan;

uniform vec3 diffuse;
uniform bool hasTexture;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

#include "light.glsl"

out vec4 fragColor;

void main()
{
    vec3 Kd, frag_norm;
    if (hasTexture)
    {
        Kd = texture(texture_diffuse1, uv).xyz;
        //frag_norm = texture(texture_normal1, uv).xyz;
        //wooden tower's normal map is bad for my usage
        frag_norm = normalize(normal);
    }
    else
    {
        Kd = diffuse;
        frag_norm = normalize(normal);
    }
    vec3 result = CalcDirLight(dirLight, frag_norm, Kd);
    result += CalcPointLight(pointLight, frag_norm, world_pos, Kd);
//     vec3 dirlight = normalize(vec3(0.0f, 1.0f, 0.f));
//     float diff = max(dot(frag_norm, dirlight), 0.f);
    fragColor = vec4(result, 1.0f);
}
