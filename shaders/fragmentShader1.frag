#version 330 core
out vec4 FragColor;

//in vec3 ourColor;
//in vec2 TexCoords;
//in vec3 Normal;
//in vec3 FragPos;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 Normal;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform float mixValue;
uniform vec3  viewPos;
uniform float time;

#define NB_TEXTURE_PER_TYPE 16
struct Material {
    vec3        ambient;
    vec3        diffuse;
    int         texture_diffuse_count;
    sampler2D   texture_diffuse[NB_TEXTURE_PER_TYPE];
    vec3        specular;
    int         texture_specular_count;
    sampler2D   texture_specular[NB_TEXTURE_PER_TYPE];
    int         texture_normal_count;
    sampler2D   texture_normal[NB_TEXTURE_PER_TYPE];
    int         texture_height_count;
    sampler2D   texture_height[NB_TEXTURE_PER_TYPE];
    vec3        emission;
    int         texture_emission_count;
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

vec3 GetAmbientColor() {
    if (material.texture_diffuse_count > 0)
        return texture(material.texture_diffuse[0], fs_in.TexCoords).rgb;
    return material.ambient;
}

vec3 GetDiffuseColor() {
    if (material.texture_diffuse_count > 0)
        return texture(material.texture_diffuse[0], fs_in.TexCoords).rgb;
    return material.diffuse;
}

vec3 GetSpecularColor() {
    if (material.texture_specular_count > 0)
        return texture(material.texture_specular[0], fs_in.TexCoords).rgb;
    return material.specular;
}

vec3 GetEmissionColor() {
    if (material.texture_emission_count > 0 && texture(material.texture_specular[0], fs_in.TexCoords).r == 0.0)
        return texture(material.texture_emission[0], fs_in.TexCoords + vec2(0.0, time)).rgb * mixValue;
    return material.emission;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
//    vec3 lightDir = normalize(-light.direction * fs_in.TBN);
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    //float diff = max(dot(normal, lightDir), 0.0);
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * GetAmbientColor();
    vec3 diffuse  = light.diffuse  * diff * GetDiffuseColor();
    vec3 specular = light.specular * spec * GetSpecularColor();
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    //float diff = max(dot(normal, lightDir), 0.0);
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);


    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient  * GetAmbientColor();
    vec3 diffuse  = light.diffuse  * diff * GetDiffuseColor();
    vec3 specular = light.specular * spec * GetSpecularColor();
    return ((ambient + diffuse + specular) * attenuation);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient  = light.ambient  * GetAmbientColor();

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));

    ambient  *= attenuation;

    if(theta > light.outerCutOff)
    {
        float diff = max(dot(normal, lightDir), 0.0);
        //float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse  = light.diffuse  * diff * GetDiffuseColor();

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        //vec3 halfwayDir = normalize(lightDir + viewDir);
        //float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * GetSpecularColor();

        diffuse  *= attenuation * intensity;
        specular *= attenuation * intensity;

        return (ambient + diffuse + specular);
    }
    else
        return (ambient);
}


//depth viewing
float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    // properties
    vec3 norm = normalize(fs_in.Normal);
    if (material.texture_normal_count > 0) {
        //apply texture normal normalized to normal matrix
        norm *= ((texture(material.texture_normal[0], fs_in.TexCoords).rgb * 255.0/128.0) - 1.0);
        //norm = texture(material.texture_normal[0], fs_in.TexCoords).rgb * 2 - 1;
        norm = normalize(norm);
    }

//    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 emission = GetEmissionColor();

    vec3 result = vec3(0.0f);
    // phase 1: Directional lighting
    result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
//    for(int i = 0; i < nbPointLight; i++)
//        result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, norm, fs_in.FragPos, viewDir);
//    result += CalcSpotLight(spotLight, norm, fs_in.TangentFragPos, viewDir);
    result += emission;

//    if (material.texture_normal_count > 0)
//        FragColor = vec4(norm, 1.0);
//    else
        FragColor = vec4(result, 1.0);

//    FragColor = vec4(result, 1.0);

    //depth testing
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
}
