struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 Ka, vec3 Kd, vec3 Ks, float shininess)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.ambient  * Ka;
    vec3 diffuse  = light.diffuse  * diff * Kd;
    vec3 specular = light.specular * spec * Ks;
    return (ambient + diffuse + specular);
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

uniform DirLight dirLight;
uniform PointLight pointLight;
