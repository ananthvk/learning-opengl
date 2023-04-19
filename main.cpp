#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glpp/glpp.hpp"
#include "vertices.h"

double prevx = 400 / 2;
double fov = 45.0f; // In radians
double prevy = 600 / 2;
double xmov = 0.0f;
double ymov = 0.0f;
double sensitivity = 0.1f;
bool fullscreen = false;
int windowed_width = 800;
int windowed_height = 600;
int windowed_x = 0;
int windowed_y = 0;

void mouse_moved(GLFWwindow *window, double xpos, double ypos)
{
    xmov = (xpos - prevx) * sensitivity;
    ymov = (prevy - ypos) * sensitivity; // reversed y since for screen, y starts from top of screen

    prevx = xpos;
    prevy = ypos;
}
void scrolled(GLFWwindow *window, double xo, double yo)
{
    fov -= (float)yo;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

#define KEYPRESSED(win, key) if (glfwGetKey(win, key) == GLFW_PRESS)
int main()
{
    glpp::Window window;

    windowed_width = window.getwidth();
    windowed_height = window.getheight();
    windowed_x = window.x();
    windowed_y = window.y();

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
    // Disable mouse cursor so that it is locked to the center of the screen
    glfwSetInputMode(window.ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.ptr(), mouse_moved);
    glfwSetScrollCallback(window.ptr(), scrolled);

    // Create the camera
    glpp::Camera camera;

    std::vector<glm::vec3> cube_positions;
    for (int i = 0; i < 100; i++)
    {
        float x = ((std::rand() % 2000) - 1000) / 40.0f;
        float y = ((std::rand() % 2000) - 1000) / 40.0f;
        float z = ((std::rand() % 2000) - 1000) / 40.0f;
        cube_positions.emplace_back(glm::vec3(x, y, z));
    }
    /*
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
    */

    std::vector<float> rotation_speed;
    // std::srand(time(NULL));
    std::srand(1007);
    for (const auto &model : cube_positions)
    {
        rotation_speed.emplace_back((std::rand() % 120) - 60);
    }

    std::vector<glm::vec3> axis_of_rotation;
    for (const auto &model : cube_positions)
    {
        float x = ((std::rand() % 2000) - 1000) / 1000.0f;
        float y = ((std::rand() % 2000) - 1000) / 1000.0f;
        float z = ((std::rand() % 2000) - 1000) / 1000.0f;
        axis_of_rotation.emplace_back(glm::vec3(x, y, z));
    }

    float dt = 1.0f;
    float previousFrame = 0.0f;

    while (!glfwWindowShouldClose(window.ptr()))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        KEYPRESSED(window.ptr(), GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(window.ptr(), true);
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_W)
        {
            camera.move(glpp::Camera::Direction::FORWARD, dt);
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_A)
        {
            camera.move(glpp::Camera::Direction::LEFT, dt);
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_S)
        {
            camera.move(glpp::Camera::Direction::BACKWARD, dt);
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_D)
        {
            camera.move(glpp::Camera::Direction::RIGHT, dt);
        }
        // TODO: Temporary
        KEYPRESSED(window.ptr(), GLFW_KEY_UP)
        {
            camera.froll() += dt * 3.5;
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_DOWN)
        {
            camera.froll() -= dt * 3.5;
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_F4)
        {
            if (!camera.fps)
            {
                camera.ypos = camera.vpos().y;
                camera.fps = true;
            }
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_F5)
        {
            if (camera.fps)
            {
                camera.fps = false;
            }
        }

        KEYPRESSED(window.ptr(), GLFW_KEY_F5)
        {
            if (!camera.custom_lookat)
            {
                camera.custom_lookat = true;
            }
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_F6)
        {
            if (camera.custom_lookat)
            {
                camera.custom_lookat = false;
            }
        }

        KEYPRESSED(window.ptr(), GLFW_KEY_F8)
        {
            glfwSetInputMode(window.ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_F9)
        {
            glfwSetInputMode(window.ptr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_F12)
        {
            if (!fullscreen)
            {
                windowed_width = window.getwidth();
                windowed_height = window.getheight();
                windowed_x = window.x();
                windowed_y = window.y();
                GLFWmonitor *monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode *mode = glfwGetVideoMode(monitor);
                glfwWindowHint(GLFW_DECORATED, NULL);
                glfwSetWindowMonitor(window.ptr(), monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                fullscreen = true;
            }
        }

        KEYPRESSED(window.ptr(), GLFW_KEY_F11)
        {
            if (fullscreen)
            {
                glfwSetWindowMonitor(window.ptr(), NULL, windowed_x, windowed_y, windowed_width, windowed_height, GLFW_DONT_CARE);
                fullscreen = false;
            }
        }
        glm::mat4 projection = glm::perspective(glm::radians((float)fov), (float)window.getwidth() / (float)window.getheight(), 0.1f, 100.0f);

        float yaw = camera.fyaw();
        float pitch = camera.fpitch();

        camera.fyaw() += xmov;
        camera.fpitch() = std::min(89.0f, std::max(-89.0f, (float)(pitch + ymov)));
        // std::cout<<xmov<<" "<<ymov<<std::endl;
        xmov = 0;
        ymov = 0;

        camera.calculate_face();
        camera.calculate_target_from_face();

        shader.use();
        shader.set("view", camera.view());
        shader.set("projection", projection);

        texture.activate(0);
        glBindVertexArray(VAO);

        // glm::mat4 marker_model = glm::mat4(1.0f);
        // marker_model = glm::scale(marker_model, glm::vec3(0.1, 0.1, 0.1));
        // marker_model = glm::translate(marker_model, camera.vtarget() + camera.vface());
        // shader.set("model", marker_model);
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        for (size_t i = 0; i < cube_positions.size(); ++i)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(rotation_speed[i]), axis_of_rotation[i]);
            shader.set("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        window.update();
        dt = glfwGetTime() - previousFrame;
        previousFrame = glfwGetTime();
    }
}