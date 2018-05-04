#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform float mixValue;
uniform vec3 viewPos;
uniform float time;

struct Material {
    sampler2D   diffuse;
    sampler2D   specular;
    sampler2D   emission;
    float       shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoords))) * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    vec3 emission = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0) {
        emission = texture(material.emission, TexCoords + vec2(0.0, time)).rgb;
        emission = emission * mixValue;
    }

    FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue);// * vec4(ourColor, 1.0);
}
