#include <iostream>
#include <math.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glpp/glpp.hpp"
#include "camera.h"
#include "vertices.h"

int main()
{
    glpp::Window window;

    // Create the vertices
    unsigned int VAO = 1, VBO = 1;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) / sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Load the shaders and textures
    glpp::Shader shader;
    shader.load_from_file("shaders/main.vert", "shaders/main.frag");

    glpp::Texture texture;
    texture.load_from_file("assets/container.jpg");

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window.ptr()))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetKey(window.ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window.ptr(), true);
        }
        shader.use();
        texture.activate(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        window.update();
    }
}