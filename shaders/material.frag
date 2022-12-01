 #version 410 core

#define PI 3.1415926538

out vec4 FragColor;

struct Material {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Ns;
    float d;
    float roughness;
    float refraction;
    float reflection;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform DirLight dirLight[20];
uniform PointLight pointLight[20];
uniform SpotLight spotLight[20];
uniform Material material;
uniform int CountdirLight;
uniform int CountpointLight;
uniform int CountspotLight;
uniform samplerCube skybox;

uniform float eta = 0.66;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float DistributionGGX(vec3 N, vec3 H, float a);

void main() {
//    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result;
    for (int i = 0; i < CountdirLight; i++) {
        result += CalcDirLight(dirLight[i], norm, viewDir);
    }
    // phase 2: point lights
    for(int i = 0; i < CountpointLight; i++)
        result += CalcPointLight(pointLight[i], norm, FragPos, viewDir);
    // phase 3: spot light
    for(int i = 0; i < CountspotLight; i++) {
        result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);
    }

    vec3 I1 = normalize(FragPos - viewPos);
    vec3 R1 = reflect(I1, normalize(Normal));
    result = mix(result, vec3(texture(skybox, R1).rgb), material.reflection);

    float ratio = 1.00 / 1.52;
    vec3 I2 = normalize(FragPos - viewPos);
    vec3 R2 = refract(I2, normalize(Normal), ratio);
    result = mix(result, vec3(texture(skybox, R2).rgb), material.refraction);

    FragColor = vec4(result, material.d);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Ns);
    // combine results
    vec3 ambient = light.ambient * material.Ka ;
    vec3 diffuse = light.diffuse * diff * material.Kd;
    vec3 specular = DistributionGGX(normal, halfwayDir, material.roughness) * light.specular * spec * material.Ks;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Ns);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * material.Ka ;
    vec3 diffuse = light.diffuse * diff * material.Kd;
    vec3 specular = DistributionGGX(normal, halfwayDir, material.roughness) * light.specular * spec * material.Ks;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Ns);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.Ka ;
    vec3 diffuse = light.diffuse * diff * material.Kd;
    vec3 specular = DistributionGGX(normal, halfwayDir, material.roughness) * light.specular * spec * material.Ks;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

float DistributionGGX(vec3 N, vec3 H, float a) {
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;

    return nom / denom;
}
