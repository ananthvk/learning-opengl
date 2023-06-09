#include <iostream>
#include <math.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_img.h"
#include "shader.h"

float global_mix = 0.2f;
float rotateSpeed = 1.0f;
float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = -3.0f;
float fovAngle = 45.0f;
float vfx = 800.0f;
float vfy = 600.0f;

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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        global_mix = std::min(global_mix + 0.01f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        global_mix = std::max(global_mix - 0.01f, 0.0f);
    }
    float mvspeed = 0.5f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraZ += mvspeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraX += mvspeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraZ -= mvspeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraX -= mvspeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraY -= mvspeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        cameraY += mvspeed;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        fovAngle += mvspeed * 10;
        std::cout << "fovAngle=" << fovAngle << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        fovAngle -= mvspeed * 10;
        std::cout << "fovAngle=" << fovAngle << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        vfx += mvspeed * 10;
        std::cout << "vfx=" << vfx << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        vfx -= mvspeed * 10;
        std::cout << "vfx=" << vfx << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        vfy += mvspeed * 10;
        std::cout << "vfy=" << vfy << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        vfy -= mvspeed * 10;
        std::cout << "vfy=" << vfy << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        std::cout << "RESET" << std::endl;
        global_mix = 0.2f;
        rotateSpeed = 1.0f;
        cameraX = 0.0f;
        cameraY = 0.0f;
        cameraZ = -3.0f;
        fovAngle = 45.0f;
        vfx = 800.0f;
        vfy = 600.0f;
    }
}

unsigned int createVAO(const float *vertices, size_t vertices_length)
{
    // Get the current VAO, VBO and EBO ids
    int old_VAO = 1, old_VBO = 1, old_EBO = 1;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_VBO);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_VAO);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_EBO);

    // Create the VAO, and the buffers
    unsigned int VAO = 1, VBO = 1; //, EBO = 1;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    //  Bind the VAO and the buffers
    glBindVertexArray(VAO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy the arrays into the buffers
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length, indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertices_length, vertices, GL_STATIC_DRAW);

    // Enable the attrib array to access the vertices
    // Vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0));
    // Colors
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    // Textures
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    // Enable the attribute pointers
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // glEnableVertexAttribArray(2);

    // Bind the VAO, VBO, and EBO to the ones before this function was called
    glBindVertexArray(old_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, old_EBO);
    glBindBuffer(GL_ARRAY_BUFFER, old_VBO);
    return VAO;
}

int main()
{
    stbi_set_flip_vertically_on_load(true);
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
    float i = 1.0f;
    /*
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   i, i,   // top right
         0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   i, 0.0f,   // bottom right
         -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
         -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, i    // top left 
        // 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f, // top right
        // 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f, // bottom right
        //-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f, // bottom left
        //-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f  // top left 

    };
    */

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


    // Load the texture
    int width, height, nchannels;
    unsigned char * data = stbi_load("container.jpg", &width, &height, &nchannels, 0);
    if(!data){
        std::cerr<<"Unable to load texture. Check if file exists"<<std::endl;
        glfwTerminate();
        return 3;
    }
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // Load the smiley texture
    glActiveTexture(GL_TEXTURE1);
    data = stbi_load("face.png", &width, &height, &nchannels, 0);
    if(!data){
        std::cerr<<"Unable to load texture. Check if file exists"<<std::endl;
        glfwTerminate();
        return 3;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    unsigned int smileyTexture;
    glGenTextures(1, &smileyTexture);
    glBindTexture(GL_TEXTURE_2D, smileyTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    // clang-format on

    unsigned int VAO1 = createVAO(vertices, sizeof(vertices));
    // unsigned int VAO2 = createVAO(vertices2, sizeof(vertices2), indices2, sizeof(indices2));

    // For wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shader.use();
    // glUniform1i(glGetUniformLocation(shader.getid(), "boxTexture"), 0);
    shader.set_int("boxTexture", 0);
    shader.set_int("smileyTexture", 1);

    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeElapsed = glfwGetTime();
        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, smileyTexture);

        // shader.set_float("timeElapsed", timeElapsed);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);

        // glm::mat4 model = glm::mat4(1.0f);
        //  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, (float)(glfwGetTime() * rotateSpeed) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::translate(view, glm::vec3(cameraX, cameraY, cameraZ));
        glm::mat4 projection;
        // projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        projection = glm::perspective(glm::radians(fovAngle), vfx / vfy, 0.1f, 100.0f);

        unsigned int viewLocation = glGetUniformLocation(shader.getid(), "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLocation = glGetUniformLocation(shader.getid(), "projection");
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        shader.set_float("mixValue", global_mix);
        glBindVertexArray(VAO1);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            // Make every 3rd object rotate
            if (i % 3 == 0)
            {
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                model = glm::rotate(model, (float)(glfwGetTime() * rotateSpeed) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            unsigned int modelLocation = glGetUniformLocation(shader.getid(), "model");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO1);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}