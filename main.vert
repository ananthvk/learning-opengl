#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aCol;
  
out vec3 vertexColor; // specify a color output to the fragment shader
uniform float timeElapsed;

void main()
{
    float t = mod(timeElapsed, (2*3.1415));
    vec4 pos = vec4(aPos, 1.0);
    // Translate first, then rotate
    // [ 1 0 x ]
    // [ 0 1 y ]
    // [ 0 0 1 ]
    //pos.x = aPos.x + 0 + aPos.x;
    //pos.y = 0 + aPos.y + aPos.y;
    //pos.z = 0 + 0 + aPos.z;
    pos.x += sin(timeElapsed)/2 + 0.5;
    pos.y += sin(timeElapsed + 3.1415/2)/2 + 0.5;
    pos.z += sin(timeElapsed + 3*3.1415/2)/2 + 0.5;

    // Rotate the coordinates
    pos.x = pos.x * cos(t) - pos.y * sin(t);
    pos.y = pos.x * sin(t) + pos.y * cos(t);

    gl_Position = pos;
    vertexColor = aCol;//vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
}