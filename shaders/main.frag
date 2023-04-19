#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;  
 
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularity;
uniform float specularStrength;

float ambient_strength = 0.1f;



void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularity);
    vec3 specular = specularStrength * spec * lightColor;  


    float diffuse = max(dot(norm, lightDir), 0);

    vec3 ambient_light = ambient_strength * lightColor;
    vec3 resultant_color = (specular + diffuse + ambient_light) * objectColor;
    FragColor = vec4(resultant_color, 1.0f);
} 
