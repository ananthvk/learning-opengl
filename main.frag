#version 330 core
out vec4 FragColor;
  
in vec3 vertexColor;
in vec2 texCoord;
//uniform float timeElapsed;
uniform float mixValue;
uniform sampler2D boxTexture;
uniform sampler2D smileyTexture;

void main()
{
    //vec4 op = vec4(vertexColor, 1.0f);
    //op.x *= (sin(timeElapsed)/2.0f) + 0.5f;
    //op.y *= (sin(timeElapsed + 3*3.1415/2)/2.0f) + 0.5f;
    //op.z *= (sin(timeElapsed + 3.1415/2)/2.0f) + 0.5f;
    //vec4 col = texture(boxTexture, texCoord);
    // Mirror along x axis
    vec2 mirrored = vec2(-texCoord.x, texCoord.y);
    FragColor = mix(texture(boxTexture, texCoord), texture(smileyTexture, mirrored), mixValue);//* op;
} 
