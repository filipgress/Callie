#include <clpch.h>

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Core/Config.h>
#include <Core/Base.h>
#include <Core/Log.h>

#include <Events/Event.h>
#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>
#include <Events/ApplicationEvent.h>


// Window dimensions
const float toRadians = M_PI / 180.0f;
const GLint WIDTH = 800, HEIGHT = 600;
GLint uniformModel, uniformProjection;
GLuint VAO, IBO, VBO, shader;

// Vertex Shader
static const char* vShader = "\
#version 330\n\
\n\
layout (location = 0) in vec3 pos;\n\
out vec4 vCol;\n\
\n\
uniform mat4 projection;\n\
uniform mat4 model;\n\
\n\
void main(){\n\
    gl_Position = projection * model * vec4(pos, 1.0f);\n\
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);\n\
}";

// Fragment Shader
static const char* fShader = "\
#version 330\n\
\n\
in vec4 vCol;\n\
out vec4 color;\n\
\n\
void main(){\n\
    color = vCol;\n\
}";

void CreateTriangle(){
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

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verteces), verteces, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLint program, const char* shaderCode, GLenum shaderType){
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    GLint result;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result){
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        if (shaderType == GL_VERTEX_SHADER)
            std::cout << "VERTEX BUFFER\n";
        else
            std::cout << "FRAGMENT BUFFER\n";

        ERROR("Error compiling shader: ", shaderType, eLog);
    }

    glAttachShader(program, shader);
}

void CompileShaders(){
    shader = glCreateProgram();
    if (!shader)
        ERROR("Unable to create shader program!")

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        ERROR("Unable to link properly!", eLog);
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result){
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        ERROR("Unable to validate program!", eLog);
    }

    uniformModel        = glGetUniformLocation(shader, "model");
    uniformProjection   = glGetUniformLocation(shader, "projection");
}

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
    window = glfwCreateWindow(WIDTH, HEIGHT, "Callie", NULL, NULL);
    if (!window)
    {
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
        return 1;
    }

    /* Setup Viewport size */
    glViewport(0, 0, bufferWidth, bufferHeight);

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << PROJECT_NAME << " | " PROJECT_VERSION << "v\n";



    CreateTriangle();
    CompileShaders();

    glEnable(GL_DEPTH_TEST);
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
        

        glUseProgram(shader);
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.0f));
            // model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}