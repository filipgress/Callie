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

            ERROR("Shader unable to link or validate:", &infoLog[0]);
            CL_DEBUGBREAK();
            
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

            ERROR("Shader unable to validate:", &infoLog[0]);
            CL_DEBUGBREAK();
            
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
        if (m_ShaderID)
            GLCall(glDeleteProgram(m_ShaderID));
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

            ERROR("Shader", type, "unable to compile:", &infoLog[0]);
            CL_DEBUGBREAK();

            return 0;
        }

        return id;
    }

    const std::string Shader::ParseShader(const std::string& filepath){
        std::ifstream stream(filepath, std::ios::in);
        std::string shaderSource = "";

        if(!stream.is_open()){
            ERROR("Failed to read:", filepath)
            CL_DEBUGBREAK();

            stream.close();
            return "";
        }

        std::string line;
        while(getline(stream, line))
            shaderSource += line + "\n";
        
        std::cout << shaderSource;
        stream.close();
        return shaderSource;
    }

    void Shader::Bind() const{
        GLCall(glUseProgram(m_ShaderID));
    }

    void Shader::Unbind() const{
        GLCall(glUseProgram(0));
    }
}