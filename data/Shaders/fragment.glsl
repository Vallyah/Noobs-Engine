in vec3 world_pos;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitan;
in vec4 pos_dirlightspace;

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
        frag_norm = normalize(normal);
    }
    else
    {
        Kd = diffuse;
        frag_norm = normalize(normal);
    }
    float shadow = DirShadowCalculation(pos_dirlightspace);
    vec3 result = CalcDirLight(dirLight, frag_norm, Kd, shadow);
    shadow = PointShadowCalculation(pointLight, world_pos, frag_norm);
    result += CalcPointLight(pointLight, frag_norm, world_pos, Kd, shadow);
    fragColor = vec4(result, 1.0f);
}
