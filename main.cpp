#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// The vertex and fragment shaders
// Vertex shader specifies how to process the vertices
// while fragment shader gives the color for each pixel.
// const char *vertexShaderSource = "#version 330 core\n"
//                                 "layout (location = 0) in vec3 aPos;\n"
//                                 "void main()\n"
//                                 "{\n"
//                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//                                 "}\0";
//
// const char *fragmentShaderSource = "#version 330 core\nout vec4 FragColor;\n\nvoid main()\n{\n    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n} \n";
char *vertexShaderSource = nullptr;
char *fragmentShaderSource = nullptr;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Bind the VAO, VBO, and EBO to the ones before this function was called
    glBindVertexArray(old_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, old_EBO);
    glBindBuffer(GL_ARRAY_BUFFER, old_VBO);
    return VAO;
}

int main()
{

    // No error handling or dynamic buffers for now
    char buffer1[BUFSIZE];
    char buffer2[BUFSIZE];
    FILE *fp;
    fp = fopen("main.vert", "r");
    size_t sz = fread(buffer1, 1, BUFSIZE - 1, fp);
    buffer1[sz] = '\0';
    fclose(fp);

    fp = fopen("main.frag", "r");
    sz = fread(buffer2, 1, BUFSIZE - 1, fp);
    buffer2[sz] = '\0';
    fclose(fp);

    vertexShaderSource = buffer1;
    fragmentShaderSource = buffer2;

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
    unsigned int vertexShader = 1;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = 1;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int success;
    char logInformation[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, logInformation);
        std::cerr << "ERROR: Vertex shader compilation failed\n"
                  << logInformation << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, logInformation);
        std::cerr << "ERROR: Fragment Shader compilation failed\n"
                  << logInformation << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, logInformation);
        std::cerr << "ERROR: Could not link shader program"
                  << logInformation << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);
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
    float vertices[] = {
        -0.4f, 0.0f, 0.0,
        0.4f, 0.0f, 0.0,
        0.0f, 0.4f, 0.0,
    };
    unsigned int indices[] = {
        0, 1, 2,
    };

    float vertices2[] = {
        -0.4f, -0.8f, 0.0,
        0.4f, -0.8f, 0.0,
        0.0f, -0.4f, 0.0,
    };
    unsigned int indices2[] = {
        0, 1, 2,
    };


    // clang-format on

    unsigned int VAO1 = createVAO(vertices, sizeof(vertices), indices, sizeof(indices));
    unsigned int VAO2 = createVAO(vertices2, sizeof(vertices2), indices2, sizeof(indices2));

    // For wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}