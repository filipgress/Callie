#pragma once

#include <GL/glew.h>
#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace cl{
    class Shader{
        private:
            GLuint m_ShaderID;
            std::unordered_map<std::string, GLint> m_UniformLocationCache;

        public: 
            Shader(const std::string&, const std::string&);
            ~Shader();

            void Bind() const;
            void Unbind() const;

            // set uniforms of various types
            void SetInt(const std::string& name, int value);
            void SetFloat(const std::string& name, float value);
            void SetFloat2(const std::string& name, const glm::vec2& value);
            void SetFloat3(const std::string& name, const glm::vec3& value);
            void SetFloat4(const std::string& name, const glm::vec4& value);
            void SetMat4(const std::string& name, const glm::mat4& value);

        private:
            GLuint CompileShader(GLenum type, const std::string&);
            const std::string ParseShader(const std::string&);

            GLint GetUniformLocation(const std::string&);
    };
}