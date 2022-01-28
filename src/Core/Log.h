#pragma once

namespace cl{
    enum class LogPriority { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };
    extern LogPriority g_LogLevel;
    extern std::mutex g_LogMutex;

    void Log(std::stringstream& mesasge, LogPriority priority);
    const std::string Timestamp();

// OpenGL error handling
// Probably should update to latest glDebugMessageCallback() 
#ifdef CL_DEBUG
    #define GLCall(function) cl::GLClearError();\
            function;\
            CL_ASSERT(cl::GLLogCall(#function, __FILE__, __LINE__))
    #define GLCallMessage(function, message) cl::GLClearError();\
            function;\
            CL_ASSERT(cl::GLLogCall(#function, __FILE__, __LINE__, message))
#else
    #define GLCall(function) function;
    #define GLCallMessage(function, message) function;
#endif

    void GLClearError();
    bool GLLogCall(const char* function, const char* file, int line);

#ifdef CL_DEBUG
    #define TRACE(...) cl::LogCall(cl::LogPriority::TRACE, "TRACE", __FILE__, __LINE__, __VA_ARGS__);
    #define DEBUG(...) cl::LogCall(cl::LogPriority::DEBUG, "DEBUG", __FILE__, __LINE__, __VA_ARGS__);
    #define INFO(...)  cl::LogCall(cl::LogPriority::INFO,  "INFO" , __FILE__, __LINE__, __VA_ARGS__);
#else
    #define TRACE(...)
    #define DEBUG(...)
    #define INFO(...)
#endif

#define WARN(...)  cl::LogCall(cl::LogPriority::WARN,  "WARN" , __FILE__, __LINE__, __VA_ARGS__);
#define ERROR(...) cl::LogCall(cl::LogPriority::ERROR, "ERROR", __FILE__, __LINE__, __VA_ARGS__);
#define FATAL(...) cl::LogCall(cl::LogPriority::FATAL, "FATAL", __FILE__, __LINE__, __VA_ARGS__);

    template <typename T>
    void recursive(std::stringstream& message, T argument) {
        message << argument << "\n";
    }

    template <typename T, typename... Args>
    void recursive(std::stringstream& message, T argument, Args... args) {
        message << argument << " ";
        recursive(message, args...);
    }

    template <typename... Args>
    void LogCall(LogPriority priority, const char* priority_message, const char* file, unsigned int line, Args... args) {
        if (priority >= g_LogLevel){
            std::scoped_lock lock(g_LogMutex);
            std::stringstream message;

            message << "[" << priority_message << "] ";
            if (priority > LogPriority::INFO)
                message << file << ":" << line << " | ";
        
            recursive(message, args...);
            Log(message, priority);
        }
    }
}