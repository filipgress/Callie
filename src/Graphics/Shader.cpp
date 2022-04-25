#include <clpch.h>

#include <Graphics/Shader.h>

namespace cl{
    Shader::Shader(const std::string& vertexLocation, const std::string& fragmentLocation){
        GLCall(m_ShaderID = glCreateProgram());

        GLuint vs = CompileShader(GL_VERTEX_SHADER, ParseShader(vertexLocation));
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, ParseShader(fragmentLocation));

        GLCall(glAttachShader(m_ShaderID, vs));
        GLCall(glAttachShader(m_ShaderID, fs));

        GLint isLinked = 0;
        GLCall(glLinkProgram(m_ShaderID));
        GLCall(glGetProgramiv(m_ShaderID, GL_LINK_STATUS, (int *) &isLinked));
        if (isLinked == GL_FALSE)
        {
            GLint length = 0;
            GLCall(glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &length));

            std::vector<GLchar> infoLog(length);
            GLCall(glGetProgramInfoLog(m_ShaderID, length, &length, &infoLog[0]));

            CL_CORE_ASSERT("Unable to link shader program:", &infoLog[0]);
            
            GLCall(glDeleteShader(vs));
            GLCall(glDeleteShader(fs));

            GLCall(glDeleteProgram(m_ShaderID));
            m_ShaderID = 0;

            return;
        }

        GLint isValidated = 0;
        GLCall(glValidateProgram(m_ShaderID));
        GLCall(glGetProgramiv(m_ShaderID, GL_VALIDATE_STATUS, (int *) &isValidated));
        if (isValidated == GL_FALSE)
        {
            GLint length = 0;
            GLCall(glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &length));

            std::vector<GLchar> infoLog(length);
            GLCall(glGetProgramInfoLog(m_ShaderID, length, &length, &infoLog[0]));

            CL_CORE_ASSERT("Unable to validate shader program:", &infoLog[0]);
            
            GLCall(glDeleteShader(vs));
            GLCall(glDeleteShader(fs));

            GLCall(glDeleteProgram(m_ShaderID));
            m_ShaderID = 0;

            return;
        }

        GLCall(glDetachShader(m_ShaderID, vs));
        GLCall(glDetachShader(m_ShaderID, fs));
    }

    Shader::~Shader(){
        if (m_ShaderID){
            GLCall(glDeleteProgram(m_ShaderID));
            m_ShaderID = 0;
        }
    }

    GLuint Shader::CompileShader(GLenum type, const std::string& shaderSource){
        GLCall(GLuint id = glCreateShader(type));
        const GLchar* source = (const GLchar*)shaderSource.c_str();

        GLCall(glShaderSource(id, 1, &source, nullptr));
        GLCall(glCompileShader(id));

        GLint isCompiled;
        GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled));
        if (isCompiled == GL_FALSE){
            GLint length;
            GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

            std::vector<GLchar> infoLog(length);
            GLCall(glGetShaderInfoLog(id, length, &length, &infoLog[0]));

            if (type == GL_VERTEX_SHADER){
                CL_CORE_ASSERT("Unable to compile vertex shader:", &infoLog[0]);
            } else if (type == GL_FRAGMENT_SHADER){
                CL_CORE_ASSERT("Unable to compile fragment shader:", &infoLog[0]);
            }

            return 0;
        }

        return id;
    }

    const std::string Shader::ParseShader(const std::string& filepath){
        INFO(filepath);
        std::ifstream stream(filepath, std::ios::in);
        std::string shaderSource = "";

        if(!stream.is_open()){
            CL_CORE_ASSERT("Failed to read:", filepath)

            stream.close();
            return "";
        }

        std::string line;
        while(getline(stream, line))
            shaderSource += line + "\n";
        
        stream.close();
        return shaderSource;
    }

    void Shader::Bind() const{
        GLCall(glUseProgram(m_ShaderID));
    }

    void Shader::Unbind() const{
        GLCall(glUseProgram(0));
    }

    GLint Shader::GetUniformLocation(const std::string& name){
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        GLCall(GLint location = glGetUniformLocation(m_ShaderID, name.c_str()));
        m_UniformLocationCache[name] = location;
        return location;
    }

    void Shader::SetInt(const std::string& name, int value){
        GLint location = GetUniformLocation(name);

        if (location != -1)
            GLCall(glUniform1i(location, value));
    }
    void Shader::SetFloat(const std::string& name, float value){
        GLint location = GetUniformLocation(name);

        if (location != -1)
            GLCall(glUniform1f(location, value));
    }
    void Shader::SetFloat2(const std::string& name, const glm::vec2& value){
        GLint location = GetUniformLocation(name);

        if (location != -1)
            GLCall(glUniform2f(location, value.x, value.y));
    }
    void Shader::SetFloat3(const std::string& name, const glm::vec3& value){
        GLint location = GetUniformLocation(name);

        if (location != -1)
            GLCall(glUniform3f(location, value.x, value.y, value.z));
    }
    void Shader::SetFloat4(const std::string& name, const glm::vec4& value){
        GLint location = GetUniformLocation(name);

        if (location != -1)
            GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
    }
    void Shader::SetMat4(const std::string& name, const glm::mat4& value){
        GLint location = GetUniformLocation(name);

        if (location != -1)
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}