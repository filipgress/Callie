#include <clpch.h>

#include <GL/glew.h>
#include <Graphics/Renderer.h>

namespace cl{
    int Renderer::Init(){
        GLenum err = glewInit();
        if (GLEW_OK != err){
            CL_CORE_ASSERT("Unable to initialize GLEW!", glewGetErrorString(err));
            return -1;
        }

        GLCall(glEnable(GL_DEPTH_TEST));
        return 0;
    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
        GLCall(glViewport(x, y, width, height));
    }
    
    void Renderer::OnWindowResize(const unsigned int width, const unsigned int height){
        GLCall(glViewport(0, 0, width, height));
    }

    void Renderer::SetClearColor(const glm::vec4& color){
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
    }

    void Renderer::Clear(){
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void Renderer::Draw(unsigned int count, bool isWireframe=false){
        if (!isWireframe){
            GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0));
        } else{
            GLCall(glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, 0));
        }
    }
}