#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace glpp
{
    // TODO: Make this callback passable through the constructor
    int gwidth = 0;
    int gheight = 0;
    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        // Function which is called each time the window is resized
        glViewport(0, 0, width, height);
        gwidth = width;
        gheight = height;
    }
    class Window
    {
    private:
        GLFWwindow *window;

    public:
        Window()
        {
            // Initialize GLFW
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
            gwidth = 800;
            gheight = 600;
            window = glfwCreateWindow(gwidth, gheight, "Hello world", NULL, NULL);
            if (window == nullptr)
            {
                std::cerr << "Unable to create window" << std::endl;
                glfwTerminate();
            }

            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

            // Check if glad is loaded
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cerr << "Unable to initialize GLAD" << std::endl;
                glfwTerminate();
            }
            glViewport(0, 0, 800, 600);
        }
        // Presents the window to the screen
        // and polls for new events.
        void update()
        {
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
        int getwidth()
        {
            return gwidth;
        }
        int getheight()
        {
            return gheight;
        }
        int x()
        {
            int mx, my;
            glfwGetWindowPos(window, &mx, &my);
            return mx;
        }
        int y()
        {
            int mx, my;
            glfwGetWindowPos(window, &mx, &my);
            return my;
        }
        ~Window()
        {
            glfwTerminate();
        }
        GLFWwindow *ptr()
        {
            return window;
        }
    };
};