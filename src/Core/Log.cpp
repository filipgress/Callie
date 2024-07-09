#include <clpch.h>

#include <Core/Log.h>
#include <GL/glew.h>

namespace cl{
    LogPriority g_LogLevel = LogPriority::DEBUG;
    std::mutex g_LogMutex;

    void Log(std::stringstream& message, LogPriority priority) {
        std::cout << message.str();

        if (priority > LogPriority::INFO){
            std::ofstream file("log.txt", std::ios::app);
            if (file.is_open()){
                file << Timestamp() << " " << message.str();
                file.close();
            } else
                std::cout << "Unable to open log file!\n";
        }
    }

    const std::string Timestamp(){
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm gmt; 
        localtime_r(&currentTime, &gmt);

        std::stringstream ss;
        ss << std::put_time(&gmt, "%d-%m-%Y %T");
        return ss.str();
    }

    void GLClearError(){
        while (glGetError() != GL_NO_ERROR);
    }

    bool GLLogCall(const char* function, const char* file, int line){
        bool status = true;
        while (GLenum error = glGetError()){
            std::stringstream message;
            message << "[OpenGl error (" << error << ")]: " << function <<
                " " << file << ":" << line << std::endl;

            Log(message, LogPriority::ERROR);

            status = false;
        }
        return status;
    }

    bool GLLogCallMessage(const char* function, const char* file, int line, const char* message){
        bool status = true;
        while (GLenum error = glGetError()){
            std::stringstream log;
            log << "[OpenGl error (" << error << ")]: " << function <<
                " " << file << ":" << line << " | " << message << std::endl;

            Log(log, LogPriority::ERROR);

            status = false;
        }
        return status;
    }
}
