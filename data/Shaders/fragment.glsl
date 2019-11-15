in vec3 world_pos;
in vec3 te_normal;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform vec3 viewPos;

#include "light.glsl"

out vec4 fragColor;

void main()
{
    vec3 Kd = diffuse;
    vec3 Ks = specular;
    vec3 Ka = ambient;
    vec3 frag_norm = normalize(te_normal);
    vec3 viewDir = normalize(viewPos - world_pos);
    
    vec3 color = CalcDirLight(dirLight, frag_norm, viewDir, Ka, Kd, Ks, shininess);
    
    fragColor = vec4(color, 1.0f);
}
