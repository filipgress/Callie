#include <clpch.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Window.h>
#include <Core/Mesh.h>
#include <Graphics/Shader.h>

#include <Events/Event.h>
#include <Events/ApplicationEvent.h>
#include <Events/MouseEvent.h>

// Window dimensions
const float toRadians = M_PI / 180.0f;
const GLint WIDTH = 800, HEIGHT = 600;

unsigned int indeces[]{
    0, 1, 2,
    3, 0, 2
};

cl::Vertex verteces[]{
    cl::Vertex({-1.0f, -1.0f, 0.0f}),
    cl::Vertex({-1.0f,  1.0f, 0.0f}),
    cl::Vertex({ 1.0f,  1.0f, 0.0f}),
    cl::Vertex({ 1.0f, -1.0f, 0.0f})
};

static bool ButtonPressed(cl::Event& event){
    INFO("Mouse button pressed!")
    return true;
}
static bool Resize(cl::Event& event){
    INFO("Resize event!")
    return true;
}

static void function(cl::Event& event){
    cl::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<cl::WindowResizeEvent>(Resize);
    dispatcher.Dispatch<cl::MouseButtonPressedEvent>(ButtonPressed);
}

int main(const int argc, const char** argv){
    /* Creating Window */
    cl::Window window;
    if (window.Init() == -1)
        return -1;

    window.SetVSync(true);
    window.SetEventCallback(function);

 
    /* Initializing GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err){
        ERROR("Error:", glewGetErrorString(err));

        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    /* Setup Viewport size */
    glViewport(0, 0, window.GetWidth(), window.GetHeight());

    INFO("OpenGL Info: ")
    INFO(glGetString(GL_VERSION))
    INFO(PROJECT_NAME, "|" , PROJECT_VERSION);

    // *********************************** //
    cl::Shader shader("../res/shaders/VertexShader.shader", "../res/shaders/FragmentShader.shader");

    cl::Mesh obj(verteces, indeces, sizeof(verteces), 6);
    cl::Mesh obj2(verteces, indeces, sizeof(verteces), 6);

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)window.GetWidth()/window.GetHeight(), 0.1f, 100.0f);
    shader.Bind();
    shader.SetMat4("projection", projection);

    float triOffset = 1.0f, triIncrement = 0.005f;
    int currAngle = 0, angleIncrement = 1;
    float triScale = 0.5f, scaleIncrement = 0.005f;

    /* Loop until the user closes the window */
    while (!window.ShouldClose())
    {
        /* Render here */
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (triOffset > 3.0f)
            triIncrement = -0.005f;
        else if (triOffset < 1.0f)
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

            model = glm::translate(model, glm::vec3(-triOffset, 0.0f, -5.0f));
            model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(triScale, triScale, triScale));

            shader.SetMat4("model", model);
            obj.Draw();

            model = glm::mat4(1.0f);

            model = glm::translate(model, glm::vec3(triOffset, 0.0f, -5.0f));
            model = glm::rotate(model, -currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(triScale, triScale, triScale));

            shader.SetMat4("model", model);
            obj2.Draw();

        window.OnUpdate();
    }

    return 0;
}