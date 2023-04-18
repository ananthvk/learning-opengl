#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glpp/glpp.hpp"
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

    // Create the camera
    glpp::Camera camera;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    std::vector<glm::vec3> cube_positions;
    cube_positions.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
    cube_positions.emplace_back(glm::vec3(2.0f, 5.0f, -15.0f));
    cube_positions.emplace_back(glm::vec3(-1.5f, -2.2f, -2.5f));
    cube_positions.emplace_back(glm::vec3(-3.8f, -2.0f, -12.3f));
    cube_positions.emplace_back(glm::vec3(2.4f, -0.4f, -3.5f));
    cube_positions.emplace_back(glm::vec3(-1.7f, 3.0f, -7.5f));
    cube_positions.emplace_back(glm::vec3(1.3f, -2.0f, -2.5f));
    cube_positions.emplace_back(glm::vec3(1.5f, 2.0f, -2.5f));
    cube_positions.emplace_back(glm::vec3(1.5f, 0.2f, -1.5f));
    cube_positions.emplace_back(glm::vec3(-1.3f, 1.0f, -1.5f));

    std::vector<float> rotation_speed;
    //std::srand(time(NULL));
    std::srand(1007);
    for (const auto &model : cube_positions)
    {
        rotation_speed.emplace_back((std::rand() % 120) - 60);
    }

    std::vector<glm::vec3> axis_of_rotation;
    for (const auto &model : cube_positions)
    {
        float x = ((std::rand() % 2 * 1000) - 1000) / 1000.0f;
        float y = ((std::rand() % 2 * 1000) - 1000) / 1000.0f;
        float z = ((std::rand() % 2 * 1000) - 1000) / 1000.0f;
        axis_of_rotation.emplace_back(glm::vec3(x, y, z));
    }

    while (!glfwWindowShouldClose(window.ptr()))
    {

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetKey(window.ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window.ptr(), true);
        }
        float r = 8;
        camera.vpos().x = r * cos((float)glfwGetTime());
        camera.vpos().z = r * sin((float)glfwGetTime());
        shader.use();
        shader.set("view", camera.view());
        shader.set("projection", projection);

        texture.activate(0);
        glBindVertexArray(VAO);
        for (size_t i = 0; i < cube_positions.size(); ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(rotation_speed[i]), axis_of_rotation[i]);
            shader.set("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        window.update();
    }
}