in vec3 world_pos;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitan;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform bool hasTexture;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform vec3 viewPos;

#include "light.glsl"

out vec4 fragColor;

void main()
{
    vec3 Ka, Kd, Ks, frag_norm;
    if (hasTexture)
    {
        Kd = texture(texture_diffuse1, uv).xyz;
        //frag_norm = texture(texture_normal1, uv).xyz;
        frag_norm = normalize(normal);
    }
    else
    {
        Kd = diffuse;
        Ks = specular;
        Ka = ambient;
        frag_norm = normalize(normal);
    }
    vec3 viewDir = normalize(viewPos - world_pos);
    vec3 result = CalcDirLight(dirLight, frag_norm, viewDir, Ka, Kd, Ks, shininess);
    fragColor = vec4(result, 1.0f);
}
