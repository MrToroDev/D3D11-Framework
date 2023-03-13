namespace Lights
{
    struct Light
    {
        float3 Normal, WorldPosition, ViewPos;
        float3 Position;
        
        float Specular;
        float3 Ambient;
        float shininess;
    };

    float3 PointLight_raw(in Light light, in float3 lightColor)
    {
        const float constant = 1.0;
        const float Linear = 0.09;
        const float quadratic = 0.32;
        float distance = length(inlight.Position - inlight.WorldPosition);
        float attenuation = 1.0 / (constant + Linear * distance + quadratic * (distance * distance));
        
        float3 ambient = inlight.Ambient * lightColor;
        
        float3 norm = normalize(inlight.Normal);
        float3 lightDir = normalize(inlight.Position - inlight.WorldPosition);
    
        float diff = max(dot(norm, lightDir), 0.0);
        float3 diffuse = diff * lightColor;
        
        // specular
        float specularStrength = inlight.Specular;
        float3 viewDir = normalize(inlight.ViewPos - inlight.WorldPosition);
        float3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), inlight.shininess);
        float3 specular = specularStrength * spec * lightColor;
        
        return (ambient + diffuse + specular);
    }
}