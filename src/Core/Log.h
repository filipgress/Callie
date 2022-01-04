#pragma once

#include <Core/Base.h>

namespace Callie{
    enum class LogPriority { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };
    extern LogPriority g_LogLevel;
    extern std::mutex g_LogMutex;

    void Log(std::stringstream& mesasge);
    const std::string Timestamp();

    // OpenGL error handling
#define GLCall(x) Callie::GLClearError();\
        x;\
        CL_DEBUGBREAK(Callie::GLLogCall(#x, __FILE__, __LINE__))

#define GLCallMessage(x, y) Callie::GLClearError();\
        x;\
        CL_DEBUGBREAK(Callie::GLLogCall(#x, __FILE__, __LINE__, y))

    void GLClearError();
    bool GLLogCall(const char* function, const char* file, int line);

#define TRACE(...) Callie::LogCall(Callie::LogPriority::TRACE, "TRACE", __FILE__, __LINE__, __VA_ARGS__);
#define DEBUG(...) Callie::LogCall(Callie::LogPriority::DEBUG, "DEBUG", __FILE__, __LINE__, __VA_ARGS__);
#define INFO(...)  Callie::LogCall(Callie::LogPriority::INFO,  "INFO" , __FILE__, __LINE__, __VA_ARGS__);
#define WARN(...)  Callie::LogCall(Callie::LogPriority::WARN,  "WARN" , __FILE__, __LINE__, __VA_ARGS__);
#define ERROR(...) Callie::LogCall(Callie::LogPriority::ERROR, "ERROR", __FILE__, __LINE__, __VA_ARGS__);
#define FATAL(...) Callie::LogCall(Callie::LogPriority::FATAL, "FATAL", __FILE__, __LINE__, __VA_ARGS__);

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

            message << "[" << priority_message << "] " <<
                        file << ":" << line << " | ";
            recursive(message, args...);
            Log(message);
        }
    }
}