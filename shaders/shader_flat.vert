#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

#define PI 3.1415926538

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 FragColor;

struct Material {
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;

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

mat3 TBN;

uniform vec3 viewPos;
uniform DirLight dirLight[20];
uniform PointLight pointLight[20];
uniform SpotLight spotLight[20];
uniform Material material;
uniform int CountdirLight;
uniform int CountpointLight;
uniform int CountspotLight;
uniform samplerCube skybox;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float DistributionGGX(vec3 N, vec3 H, float a);

void main() {
  vec3 FragPos = vec3(model * vec4(aPos, 1.0));

  vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
  vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
  vec3 N = normalize(vec3(model * vec4(normalize(aNormal), 0.0)));

  // properties
  vec3 norm = texture(material.normal, aTexCoords).rgb;
  norm = normalize(norm * 2.0 - 1.0);

  T = normalize(T - dot(T, N) * N);
  TBN = transpose(mat3(T, B, N));

  vec3 viewDir = TBN * normalize(viewPos - FragPos);

  // == =====================================================
  // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
  // For each phase, a calculate function is defined that calculates the corresponding color
  // per lamp. In the main() function we take all the calculated colors and sum them up for
  // this fragment's final color.
  // == =====================================================
  // phase 1: directional lighting
  vec3 result; // = CalcDirLight(dirLight[0], norm, viewDir);
  for (int i = 0; i < CountdirLight; i++) {
      result += CalcDirLight(dirLight[i], norm, viewDir);
  }
  // phase 2: point lights
  for(int i = 0; i < CountpointLight; i++) {
      result += CalcPointLight(pointLight[i], norm, FragPos, viewDir);
  }
  for(int i = 0; i < CountspotLight; i++) {
      result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);
  }

  vec3 I1 = normalize(FragPos - viewPos);
  vec3 R1 = reflect(I1, norm);
  result = mix(result, vec3(texture(skybox, R1).rgb), material.reflection);

  float ratio = 1.00 / 1.52;
  vec3 I2 = normalize(FragPos - viewPos);
  vec3 R2 = refract(I2, norm, ratio);
  result = mix(result, vec3(texture(skybox, R2).rgb), material.refraction);

  FragColor = vec4(result, material.d);

  gl_Position = projection * view * vec4(FragPos, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = TBN * normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.Ns);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.ambient, aTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, aTexCoords));
    vec3 specular = DistributionGGX(normal, halfwayDir, material.roughness) * light.specular * spec * vec3(texture(material.specular, aTexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = TBN * normalize(light.position - fragPos);
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
    vec3 ambient = light.ambient * vec3(texture(material.ambient, aTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, aTexCoords));
    vec3 specular = DistributionGGX(normal, halfwayDir, material.roughness) * light.specular * spec * vec3(texture(material.specular, aTexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = TBN * normalize(light.position - fragPos);
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
    vec3 ambient = light.ambient * vec3(texture(material.ambient, aTexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, aTexCoords));
    vec3 specular = DistributionGGX(normal, halfwayDir, material.roughness) * light.specular * spec * vec3(texture(material.specular, aTexCoords));
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

