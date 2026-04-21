#version 460 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
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

uniform Material material;
uniform Light light;

void main() {
    vec3 lightDirection = normalize(light.position - FragPos);
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //    ambient *= attenuation;

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * (texture(material.diffuse, TexCoords)).rgb;
    diffuse *= attenuation * intensity;

    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    specular *= attenuation * intensity;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}