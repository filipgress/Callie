#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Core/Config.h>
#include <Core/Base.h>
#include <Core/Log.h>

#include <Events/Event.h>
#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>
#include <Events/ApplicationEvent.h>

int main(const int argc, const char** argv){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initializing GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err)
        ERROR("Error:", glewGetErrorString(err));

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << PROJECT_NAME << " | " PROJECT_VERSION << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}