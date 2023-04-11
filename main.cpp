#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include "shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Function which is called each time the window is resized
    glViewport(0, 0, width, height);
}

#define BUFSIZE 10000

void processInput(GLFWwindow *window)
{
    // Process keyboard/mouse/etc inputs here
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int createVAO(const float *vertices, size_t vertices_length, const unsigned int *indices, size_t indices_length)
{
    // Get the current VAO, VBO and EBO ids
    int old_VAO = 1, old_VBO = 1, old_EBO = 1;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_VBO);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_VAO);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_EBO);

    // Create the VAO, and the buffers
    unsigned int VAO = 1, VBO = 1, EBO = 1;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the VAO and the buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy the arrays into the buffers
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length, indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices_length, vertices, GL_STATIC_DRAW);

    // Enable the attrib array to access the vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // For colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bind the VAO, VBO, and EBO to the ones before this function was called
    glBindVertexArray(old_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, old_EBO);
    glBindBuffer(GL_ARRAY_BUFFER, old_VBO);
    return VAO;
}

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello world", NULL, NULL);
    if (window == nullptr)
    {
        std::cerr << "Unable to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Check if glad is loaded
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Unable to initialize GLAD" << std::endl;
        glfwTerminate();
        return 2;
    }
    glViewport(0, 0, 800, 600);

    // Initialize the shaders
    gl::Shader shader("main", "main.vert", "main.frag");
    // Shader compilation and usage done

    /*
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    */
    // clang-format off
/*
For a pyramid
(±1, 0, 0)
( 0, ±1, 0)
( 0, 0, 1)
*/
/*
    float vertices[] = {
        // Positions        // colors
        -0.4f, -0.4f, 0.0,   1.0f, 0.0f, 0.0f,
         0.4f, -0.4f, 0.0,   0.0f, 1.0f, 0.0f, 
         0.0f,  0.4f, 0.0,   0.0f, 0.0f, 1.0f
    };
*/
    float vertices[] = {
        // Triangle 1
        0.0,1.0,0.0, 1.0f, 0.0f, 0.0f,
        1.0,0.0,0.0, 1.0f, 0.0f, 0.0f,

        -1.0,-1.0,1.0, 0.0f, 1.0f, 0.0f,
        0.0,1.0,0.0, 0.0f, 1.0f, 0.0f,

        1.0,-1.0,1.0, 0.0f, 0.0f, 1.0f,
        0.0,0.0,1.0, 0.0f, 0.0f, 1.0f,

        //Triangle 2
        0.0,1.0,0.0, 1.0f, 0.0f, 0.0f,
        1.0,0.0,0.0, 1.0f, 0.0f, 0.0f,

        1.0,-1.0,1.0,  0.0f, 1.0f, 0.0f,
        0.0,0.0,1.0,  0.0f, 1.0f, 0.0f,

        1.0,-1.0,-1.0, 0.0f, 0.0f, 1.0f,
        0.0,1.0,0.0, 0.0f, 0.0f, 1.0f,

        //Triangle 3
        0.0,1.0,0.0, 1.0f, 0.0f, 0.0f,
        1.0,0.0,0.0, 1.0f, 0.0f, 0.0f,

        1.0,-1.0,-1.0,  0.0f, 1.0f, 0.0f,
        0.0,1.0,0.0,  0.0f, 1.0f, 0.0f,

        -1.0,-1.0,-1.0, 0.0f, 0.0f, 1.0f,
        0.0,0.0,1.0,    0.0f, 0.0f, 1.0f,

        //Triangle 4
        0.0,1.0,0.0, 1.0f, 0.0f, 0.0f,
        1.0,0.0,0.0, 1.0f, 0.0f, 0.0f,

        -1.0,-1.0,-1.0,  0.0f, 1.0f, 0.0f,
        0.0,0.0,1.0,  0.0f, 1.0f, 0.0f,

        -1.0,-1.0,1.0,0.0f, 0.0f, 1.0f,
        0.0,1.0,0.0, 0.0f, 0.0f, 1.0f,

    };
    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11
    };

    // clang-format on

    unsigned int VAO1 = createVAO(vertices, sizeof(vertices), indices, sizeof(indices));
    // unsigned int VAO2 = createVAO(vertices2, sizeof(vertices2), indices2, sizeof(indices2));

    // For wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeElapsed = glfwGetTime();
        shader.use();
        shader.set_float("timeElapsed", timeElapsed);
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}