#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool inv;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ambient_strength = 0.1f;
float diffuse_strength = 1.0f;
float specular_strength = 0.5f;
int specularity = 32;
vec3 Normal;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Calculate the normal matrix for non uniform transforms
    if(inv)
        Normal = mat3(transpose(inverse(model))) * aNormal;  
    else
        Normal = aNormal;

    vec3 FragPos = vec3(model * vec4(aPos, 1.0f));
    // Calculate the ambient lighting
    vec3 ambient = ambient_strength * lightColor;

    // Calculate diffuse lighting
    vec3 normal = normalize(Normal);
    // Direction is from the object to the light
    vec3 direction = normalize(lightPos - FragPos);
    float diffuse_value = max(dot(direction, normal), 0);
    vec3 diffuse = diffuse_strength * diffuse_value * lightColor;

    // Calculate specular lighting
    vec3 view_direction = normalize(viewPos - FragPos);
    // We negate the direction because reflect requires a vector from
    // the source to the point, but direction is from the point on the 
    // surface to the light.
    vec3 reflected = reflect(-direction, normal);
    float specular_value = max(dot(view_direction, reflected), 0.0f);
    specular_value = pow(specular_value, specularity);
    vec3 specular = specular_strength * specular_value * lightColor;

    vec3 resultant_color = (ambient + diffuse + specular) * objectColor;
    Color = resultant_color;
}