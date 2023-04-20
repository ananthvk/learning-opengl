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
bool shouldInvertNormal = true;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Create the vertices for the lights
    unsigned int lightVAO = 1;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the same vertices for the light as they use the same model
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Load the shaders and textures
    glpp::Shader shader, lighting_shader;
    shader.load_from_file("shaders/main.vert", "shaders/main.frag");
    lighting_shader.load_from_file("shaders/lighting.vert", "shaders/lighting.frag");

    glpp::Texture texture;
    texture.load_from_file("assets/container.jpg");

    glEnable(GL_DEPTH_TEST);
    // Disable mouse cursor so that it is locked to the center of the screen
    glfwSetInputMode(window.ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.ptr(), mouse_moved);
    glfwSetScrollCallback(window.ptr(), scrolled);

    // Create the camera
    glpp::Camera camera;
    float x = 2.0f;//1.2f;
    std::vector<glm::vec3> cube_positions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(x, 0.0f, 0.0f),
        glm::vec3(0.0f, x, 0.0f),
        glm::vec3(0.0f, 0.0f, x),
        glm::vec3(x, x, 0.0f),
        glm::vec3(0.0f, x, x),
        glm::vec3(x, 0.0f, x),
        glm::vec3(x, x, x),

        glm::vec3(-x, 0.0f, 0.0f),
        glm::vec3(0.0f, -x, 0.0f),
        glm::vec3(0.0f, 0.0f, -x),
        glm::vec3(-x, -x, 0.0f),
        glm::vec3(0.0f, -x, -x),
        glm::vec3(-x, 0.0f, -x),
        glm::vec3(-x, -x, -x),

        glm::vec3(-x, x, 0.0f),
        glm::vec3(x, -x, 0.0f),

        glm::vec3(0.0f, x, -x),
        glm::vec3(0.0f, -x, x),

        glm::vec3(-x, 0.0f, x),
        glm::vec3(x, 0.0f, -x),

        glm::vec3(-x, x, x),
        glm::vec3(x, -x, x),
        glm::vec3(x, x, -x),
        glm::vec3(-x, -x, x),
        glm::vec3(x, -x, -x),
        glm::vec3(-x, x, -x),
    };

    float dt = 1.0f;
    float previousFrame = 0.0f;

    //glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 3.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::vec3 objectColor = glm::vec3(0.5f, 0.2f, 1.0f);//glm::vec3(1.0f, 0.5f, 0.31f);

    shader.use();

    while (!glfwWindowShouldClose(window.ptr()))
    {
        //lightPos.x = 5 * cos(glm::radians(20.0f) * glfwGetTime());
        //lightPos.y = 5 * sin(glm::radians(20.0f) * glfwGetTime());
        //lightPos.z = 1 * sin(glm::radians(40.0f) * glfwGetTime());
        lightColor.r = cos(glm::radians(20.0f) * glfwGetTime())/2.0f + 0.5f;
        lightColor.g = sin(glm::radians(20.0f) * glfwGetTime())/2.0f + 0.5f;
        lightColor.b = sin(glm::radians(20.0f) * glfwGetTime())/2.0f + 0.5f;
        shader.set("lightPos", lightPos);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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
        KEYPRESSED(window.ptr(), GLFW_KEY_RIGHT)
        {
            if(shouldInvertNormal){
                shouldInvertNormal = false;
            }
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_LEFT)
        {
            if(!shouldInvertNormal){
                shouldInvertNormal = true;
            }
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_DOWN)
        {
            camera.froll() -= dt * 3.5;
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_F3)
        {
            if (!camera.fps)
            {
                camera.ypos = camera.vpos().y;
                camera.fps = true;
            }
        }
        KEYPRESSED(window.ptr(), GLFW_KEY_F4)
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

        // Render lights
        glm::mat4 light_model = glm::mat4(1.0f);
        light_model = glm::translate(light_model, lightPos);
        lighting_shader.use();
        lighting_shader.set("projection", projection);
        lighting_shader.set("view", camera.view());
        lighting_shader.set("model", light_model);
        lighting_shader.set("lightColor", lightColor);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader.use();
        shader.set("view", camera.view());
        shader.set("projection", projection);
        shader.set("objectColor", objectColor);
        shader.set("lightColor", lightColor);
        shader.set("viewPos", camera.vpos());
        shader.set("inv", shouldInvertNormal);
        texture.activate(0);
        glBindVertexArray(VAO);

        for(const auto& position: cube_positions){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::rotate(model, (float)glfwGetTime() * (float)glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            shader.set("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        window.update();
        dt = glfwGetTime() - previousFrame;
        previousFrame = glfwGetTime();
    }
}