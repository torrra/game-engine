#version 450 core

uniform sampler2D texture1;
uniform vec3 viewPos; 
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 normal;
in vec3 tangent;
in vec3 biTangent;
in vec2 texCoord;
in vec3 fragPos;

out vec4 fragColor;

void main()
{
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec3 result = (ambient + diffuse + specular);

    fragColor = texture(texture1, texCoord) * vec4(result, 1.0);
    //fragColor = objectColor *  vec4(result, 1.0);
} 