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
uniform float heightScale;

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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};
uniform SpotLight spotLight;

vec3 GetAmbientColor(vec2 texCoords) {
    if (material.texture_diffuse_count > 0)
        return texture(material.texture_diffuse[0], texCoords).rgb;
    return material.ambient;
}

vec3 GetDiffuseColor(vec2 texCoords) {
    if (material.texture_diffuse_count > 0)
        return texture(material.texture_diffuse[0], texCoords).rgb;
    return material.diffuse;
}

vec3 GetSpecularColor(vec2 texCoords) {
    if (material.texture_specular_count > 0)
        return texture(material.texture_specular[0], texCoords).rgb;
    return material.specular;
}

vec3 GetEmissionColor(vec2 texCoords) {
    if (material.texture_emission_count > 0 && texture(material.texture_specular[0], fs_in.TexCoords).r == 0.0)
        return texture(material.texture_emission[0], texCoords + vec2(0.0, time)).rgb * mixValue;
    return material.emission;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec2 texCoords) {
    vec3 lightDir = normalize(-light.direction * fs_in.TBN);
//    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    //float diff = max(dot(normal, lightDir), 0.0);
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * GetAmbientColor(texCoords);
    vec3 diffuse  = light.diffuse  * diff * GetDiffuseColor(texCoords);
    vec3 specular = light.specular * spec * GetSpecularColor(texCoords);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords)
{
    vec3 lightDir = normalize(light.position * fs_in.TBN - fragPos);
    //vec3 lightDir = normalize(light.position - fragPos);

    // attenuation
    float distance    = length(light.position * fs_in.TBN - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));

    // diffuse shading
    //float diff = max(dot(normal, lightDir), 0.0);
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//    vec3 halfwayDir = normalize(lightDir + viewDir);
//    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = light.ambient  * GetAmbientColor(texCoords);
    vec3 diffuse  = light.diffuse  * diff * GetDiffuseColor(texCoords);
    vec3 specular = light.specular * spec * GetSpecularColor(texCoords);
    return ((ambient + diffuse + specular) * attenuation);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoords) {
    //vec3 lightDir = normalize(light.position - fragPos);
    vec3 lightDir = normalize(fs_in.TBN * light.position - fragPos);

//    float theta = dot(lightDir, normalize(-light.direction));
//    float theta = dot(lightDir, normalize((-light.direction) * fs_in.TBN));
    float theta = dot(normalize(light.position - fs_in.FragPos), normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient  = light.ambient  * GetAmbientColor(texCoords);

    //float distance    = length(light.position - fs_in.FragPos);
    float distance    = length(light.position * fs_in.TBN - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));

    if(theta > light.outerCutOff)
    {
        //float diff = max(dot(normal, lightDir), 0.0);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse  = light.diffuse  * diff * GetDiffuseColor(texCoords);

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        //vec3 halfwayDir = normalize(lightDir + viewDir);
        //float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * GetSpecularColor(texCoords);

        return ((ambient + diffuse + specular) * attenuation * intensity);
    }
    else
        return (ambient * attenuation);
}


//depth viewing
float near = 0.1;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(material.texture_height[0], currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords += deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(material.texture_height[0], currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords - deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(material.texture_height[0], prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

    vec2 texCoords = fs_in.TexCoords;
    if (material.texture_height_count > 0) {
        texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);
        if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
            discard;
    }


    vec3 norm = fs_in.Normal;
    if (material.texture_normal_count > 0) {
        //apply texture normal normalized to normal matrix
        norm = normalize(texture(material.texture_normal[0], texCoords).rgb * 255.0/128.0 - 1.0);
    }

    vec3 result = vec3(0.0f);
    // phase 1: Directional lighting
    result = CalcDirLight(dirLight, norm, viewDir, texCoords);
    // phase 2: Point lights
    for(int i = 0; i < nbPointLight; i++)
        result += CalcPointLight(pointLights[i], norm, fs_in.TangentFragPos, viewDir, texCoords);
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, norm, fs_in.TangentFragPos, viewDir, texCoords);
    result += GetEmissionColor(texCoords);

    FragColor = vec4(result, 1.0);
}
