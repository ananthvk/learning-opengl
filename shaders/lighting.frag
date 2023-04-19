#version 330 core
out vec4 FragColor;
  
//in vec2 texCoord;
//uniform sampler2D boxTexture;

uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
} 
