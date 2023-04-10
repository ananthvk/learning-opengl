#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  
uniform float timeElapsed;

void main()
{
    FragColor = vec4((cos(timeElapsed)/2.0f) + 0.5f, (sin(timeElapsed)/2.0f) + 0.5f, 0.0f, 1.0f);
} 
