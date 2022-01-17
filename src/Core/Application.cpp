#include <clpch.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Graphics/VertexArray.h>
#include <Graphics/IndexBuffer.h>
#include <Graphics/VertexBuffer.h>
#include <Graphics/Shader.h>

// Window dimensions
const float toRadians = M_PI / 180.0f;
const GLint WIDTH = 800, HEIGHT = 600;

int main(const int argc, const char** argv){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /** Setup GLFW window properties
     *  OpenGL version */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WIDTH, HEIGHT, PROJECT_NAME, NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Get buffer size */
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initializing GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err){
        ERROR("Error:", glewGetErrorString(err));

        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    /* Setup Viewport size */
    glViewport(0, 0, bufferWidth, bufferHeight);

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << PROJECT_NAME << " | " PROJECT_VERSION << "v\n";


    // *********************************** //
    unsigned int indeces[]{
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat verteces[]{
        -1.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };

    cl::VertexArray va;
    cl::IndexBuffer ib(indeces, 12);
    cl::VertexBuffer vb(verteces, sizeof(verteces));

    cl::VertexBufferLayout layout;
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);

    cl::Shader shader("../res/shaders/VertexShader.shader", "../res/shaders/FragmentShader.shader");

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth/bufferHeight, 0.1f, 5.0f);

    float triOffset = 0.0f, triIncrement = 0.005f;
    int currAngle = 0, angleIncrement = 1;
    float triScale = 0.5f, scaleIncrement = 0.005f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (triOffset > 0.6f)
            triIncrement = -0.005f;
        else if (triOffset < -0.6f)
            triIncrement = 0.005f;

        if (currAngle == 360)
            currAngle -= 360;

        if (triScale >= 0.7f)
            scaleIncrement = -0.005f;
        else if(triScale <= 0.3f)
            scaleIncrement = 0.005f;
            
        triOffset += triIncrement;
        currAngle += angleIncrement;
        triScale  += scaleIncrement;
        

        shader.Bind();
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(triOffset, triOffset, -2.0f));
            model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

            shader.SetMat4("model", model);
            shader.SetMat4("projection", projection);

            va.Bind();
            ib.Bind();
                glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}