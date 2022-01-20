#include <clpch.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Core/Window.h>
#include <Core/Camera.h>
#include <Core/Mesh.h>
#include <Graphics/Shader.h>
#include <Core/Line.h>

#include <Events/Event.h>
#include <Events/ApplicationEvent.h>
#include <Events/MouseEvent.h>


const float toRadians = M_PI / 180.0f;

unsigned int indeces[]{
    0, 1, 2,
    2, 1, 3,
    
    0, 1, 5,
    5, 4, 0,

    1, 3, 5,
    5, 3, 7,

    2, 3, 7,
    7, 6, 2,
    
    0, 4, 2,
    2, 4, 6,

    4, 5, 6,
    6, 5, 7
 };

cl::Vertex verteces[] = {
    cl::Vertex({ -1.0f, -1.0f,  1.0f }),
    cl::Vertex({  1.0f, -1.0f,  1.0f }),
    cl::Vertex({ -1.0f, -1.0f, -1.0f }),
    cl::Vertex({  1.0f, -1.0f, -1.0f }),
    cl::Vertex({ -1.0f,  1.0f,  1.0f }),
    cl::Vertex({  1.0f,  1.0f,  1.0f }),
    cl::Vertex({ -1.0f,  1.0f, -1.0f }),
    cl::Vertex({  1.0f,  1.0f, -1.0f }),
};

cl::Window window;
cl::Camera camera;

static bool MouseScrolledEvent(cl::MouseScrolledEvent e){
    camera.Zoom(e.GetYOffset()*0.05f);
    return true;
}

bool mouse0 = false;
int mouseX=0, mouseY=0;
int pressedX=0, pressedY=0;
int deltaX=0, deltaY=0;
static bool MouseMovedEvent(cl::MouseMovedEvent e){
    mouseX = e.GetX();
    mouseY = e.GetY();

    if (mouse0){
        camera.RotateAroundDefault(-(float)(deltaX + mouseX - pressedX)/window.GetWidth()*4, -(float)(deltaY + mouseY - pressedY)/window.GetHeight()*4);
        return true;
    }
    return false;
}

static bool MouseButtonPressedEvent(cl::MouseButtonPressedEvent e){
    pressedX = mouseX;
    pressedY = mouseY;

    if (e.GetMouseButton() == 0){
        mouse0 = true;
        return true;
    } 
    return false;
}

static bool MouseButtonReleasedEvent(cl::MouseButtonReleasedEvent e){
    if (e.GetMouseButton() == 0){
        mouse0 = false;
        deltaX += (mouseX - pressedX);
        deltaY += (mouseY - pressedY);

        INFO(deltaX);
        return true;
    } 
    return false;
}

static bool Resize(cl::Event& event){
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
    return true;
}

static void EventHandler(cl::Event& event){
    cl::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<cl::WindowResizeEvent>(Resize);

    dispatcher.Dispatch<cl::MouseMovedEvent>(MouseMovedEvent);
    dispatcher.Dispatch<cl::MouseButtonPressedEvent>(MouseButtonPressedEvent);
    dispatcher.Dispatch<cl::MouseButtonReleasedEvent>(MouseButtonReleasedEvent);

    dispatcher.Dispatch<cl::MouseScrolledEvent>(MouseScrolledEvent);
    // dispatcher.Dispatch<cl::MouseButtonPressedEvent>(ButtonPressed);
}

int main(const int argc, const char** argv){
    /* Creating Window */
    if (window.Init() == -1)
        return -1;

    window.SetVSync(true);
    window.SetEventCallback(EventHandler);

 
    /* Initializing GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err){
        ERROR("Error:", glewGetErrorString(err));

        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    // *********************************** //
    INFO("OpenGL Info: ")
    INFO(glGetString(GL_VERSION))
    INFO(PROJECT_NAME, "|" , PROJECT_VERSION);

    cl::Shader shader("../res/shaders/VertexShader.shader", "../res/shaders/FragmentShader.shader");

    /* Grid */
    cl::Line grid[] = {
        cl::Line({ -3.0f,  0.0f, -3.0f }, { 3.0f, 0.0f, -3.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({ -3.0f,  0.0f, -2.0f }, { 3.0f, 0.0f, -2.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({ -3.0f,  0.0f, -1.0f }, { 3.0f, 0.0f, -1.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({ -5.0f,  0.0f,  0.0f }, { 5.0f, 0.0f,  0.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({ -3.0f,  0.0f,  1.0f }, { 3.0f, 0.0f,  1.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({ -3.0f,  0.0f,  2.0f }, { 3.0f, 0.0f,  2.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({ -3.0f,  0.0f,  3.0f }, { 3.0f, 0.0f,  3.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),

        cl::Line({ -3.0f,  0.0f, -3.0f }, {-3.0f,  0.0f, 3.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({ -2.0f,  0.0f, -3.0f }, {-2.0f,  0.0f, 3.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({ -1.0f,  0.0f, -3.0f }, {-1.0f,  0.0f, 3.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({  0.0f,  0.0f, -5.0f }, { 0.0f,  0.0f, 5.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({  1.0f,  0.0f, -3.0f }, { 1.0f,  0.0f, 3.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({  2.0f,  0.0f, -3.0f }, { 2.0f,  0.0f, 3.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
        cl::Line({  3.0f,  0.0f, -3.0f }, { 3.0f,  0.0f, 3.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),

        cl::Line({  0.0f, -5.0f, 0.0f }, { 0.0f,  5.0f, 0.0f }, {0.5f, 0.5f, 0.5f, 1.0f}),
    };

    /* Axis indicator*/
    cl::Line xAxis({ -0.5f,  0.0f,  0.0f }, { 0.5f,  0.0f, 0.0f }, {1.0f, 0.2f, 0.2f, 1.0f});
    cl::Line yAxis({  0.0f, -0.5f,  0.0f }, { 0.0f,  0.5f, 0.0f }, {0.2f, 1.0f, 0.2f, 1.0f});
    cl::Line zAxis({  0.0f,  0.0f, -0.5f }, { 0.0f,  0.0f, 0.5f }, {0.2f, 0.2f, 1.0f, 1.0f});

    /* Objects */
    cl::Mesh obj(verteces, indeces, sizeof(verteces), 36);

    float triOffset = 1.0f, triIncrement = 0.005f;
    int currAngle = 0, angleIncrement = 1;
    float triScale = 0.5f, scaleIncrement = 0.005f;

    /* Loop until the user closes the window */
    while (!window.ShouldClose())
    {
        /* Render here */
        glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
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
            glViewport(0, 0, window.GetWidth(), window.GetHeight());

            glm::mat4 projection = glm::perspective(45.0f, (GLfloat)window.GetWidth()/window.GetHeight(), 0.1f, 100.0f);
            glm::mat4 view = camera.GetView();
            glm::mat4 model(1.0f);

            // model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
            // model = glm::rotate(model, -currAngle * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
            // model = glm::scale(model, glm::vec3(triScale, triScale, triScale));

            shader.SetFloat4("setColor", grid[0].GetColor());
            shader.SetMat4("projection", projection);
            shader.SetMat4("view", view);
            shader.SetMat4("model", model);

            for(int i=0; i<15; i++)
                grid[i].Draw();

            shader.SetFloat4("setColor", { 0.7f, 0.7f, 0.7f, 1.0f});
            obj.Draw();
            
            /* Axis indicator*/
            glViewport(25, 25, 100, 100);
            model = glm::mat4(1.0f);
            projection = glm::perspective(45.0f, (GLfloat)1.0f, 0.1f, 100.0f);

            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.1f));
            // model = glm::rotate(model, -currAngle * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));

            shader.SetMat4("projection", projection);
            shader.SetMat4("view", view);
            shader.SetMat4("model", model);

            shader.SetFloat4("setColor", xAxis.GetColor());
            xAxis.Draw();
            shader.SetFloat4("setColor", yAxis.GetColor());
            yAxis.Draw();
            shader.SetFloat4("setColor", zAxis.GetColor());
            zAxis.Draw();

        window.OnUpdate();
    }

    return 0;
}