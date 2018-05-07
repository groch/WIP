#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform float mixValue;
uniform vec3 viewPos;
uniform float time;

#define NB_TEXTURE_PER_TYPE 8
struct Material {
    sampler2D   texture_diffuse[NB_TEXTURE_PER_TYPE];
    sampler2D   texture_specular[NB_TEXTURE_PER_TYPE];
    sampler2D   texture_normal[NB_TEXTURE_PER_TYPE];
    sampler2D   texture_height[NB_TEXTURE_PER_TYPE];
    sampler2D   texture_emission[NB_TEXTURE_PER_TYPE];
    float       shininess;
};

uniform Material material;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform int nbPointLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse[0], TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse[0], TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular[0], TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse[0], TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse[0], TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular[0], TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse[0], TexCoords));

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));

    ambient  *= attenuation;

    if(theta > light.outerCutOff)
    {
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = (diff * vec3(texture(material.texture_diffuse[0], TexCoords))) * light.diffuse;

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = vec3(texture(material.texture_specular[0], TexCoords)) * spec * light.specular;

        diffuse  *= attenuation * intensity;
        specular *= attenuation * intensity;

        return (ambient + diffuse + specular);
    }
    else
        return (ambient);
}

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 emission = vec3(0.0);
    if (texture(material.texture_specular[0], TexCoords).r == 0.0)
        emission = texture(material.texture_emission[0], TexCoords + vec2(0.0, time)).rgb * mixValue;

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < nbPointLight; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    result += emission;

    FragColor = vec4(result, 1.0);
}
