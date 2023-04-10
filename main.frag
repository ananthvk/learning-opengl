#version 330 core
out vec4 FragColor;
  
in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)  
uniform float timeElapsed;

void main()
{
    vec4 op = vec4(vertexColor, 1.0f);
    op.x *= (sin(timeElapsed)/2.0f) + 0.5f;
    op.y *= (sin(timeElapsed + 3*3.1415/2)/2.0f) + 0.5f;
    op.z *= (sin(timeElapsed + 3.1415/2)/2.0f) + 0.5f;
    FragColor = op;
} 
