#pragma once

#include <GL/glew.h>

namespace cl{
    class Shader{
        private:
            GLuint m_ShaderID;

        public: 
            Shader(const std::string&, const std::string&);
            ~Shader();

            void Bind() const;
            void Unbind() const;

            inline GLuint GetShader() const { return m_ShaderID; }

        private:
            GLuint CompileShader(GLenum type, const std::string&);
            const std::string ParseShader(const std::string&);
    };
}