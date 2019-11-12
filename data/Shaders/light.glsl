uniform sampler2D dir_shadowMap;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 Kd, float shadow)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // combine results
    vec3 ambient  = light.ambient  * Kd;
    vec3 diffuse  = light.diffuse  * diff * Kd;
    return (ambient + (1.0f - shadow) * diffuse);
}

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 Kd)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * Kd;
    vec3 diffuse  = light.diffuse  * diff * Kd;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    return (ambient + diffuse);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(dir_shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005; 
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

uniform DirLight dirLight;
uniform PointLight pointLight;
