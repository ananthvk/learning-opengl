#version 330 core
out vec4 FragColor;
  
//in vec2 texCoord;
//uniform sampler2D boxTexture;

uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
    //FragColor = texture(boxTexture, texCoord);
    FragColor = vec4(objectColor*lightColor, 1.0f);
} 
