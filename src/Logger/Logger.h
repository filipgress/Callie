#pragma once

#include <sstream>
#include <mutex>
#include <signal.h>

#define TRACE(...) Callie::log(Callie::LogPriority::TRACE, "TRACE", __FILE__, __LINE__, __VA_ARGS__);
#define DEBUG(...) Callie::log(Callie::LogPriority::DEBUG, "DEBUG", __FILE__, __LINE__, __VA_ARGS__);
#define INFO(...)  Callie::log(Callie::LogPriority::INFO,  "INFO",  __FILE__, __LINE__, __VA_ARGS__);
#define WARN(...)  Callie::log(Callie::LogPriority::WARN,  "WARN",  __FILE__, __LINE__, __VA_ARGS__);
#define ERROR(...) Callie::log(Callie::LogPriority::ERROR, "ERROR", __FILE__, __LINE__, __VA_ARGS__);
#define FATAL(...) Callie::log(Callie::LogPriority::FATAL, "FATAL", __FILE__, __LINE__, __VA_ARGS__);

#define ASSERT(x) if (!(x)) raise(SIGINT); // __debugbreak()
#define GLCall(x) Callie::GLClearError();\
    x;\
    ASSERT(Callie::GLLogCall(#x, __FILE__, __LINE__))

namespace Callie{
    enum class LogPriority { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };
    extern LogPriority log_level;
    extern std::mutex log_mutex;

    void toFile(std::stringstream& log);
    const std::string getTimestamp();
    void GLClearError();
    bool GLLogCall(const char* function, const char* file, int line);
        
    // Templates
    template <typename T>
    void recursive(std::stringstream& log, T argument) {
        log << argument << "\n";
    }

    template <typename T, typename... Args>
    void recursive(std::stringstream& log, T argument, Args... args) {
        log << argument << " ";
        recursive(log, args...);
    }

    template <typename... Args>
    void log(LogPriority messagePriority, const char*priorityString, const char* file, unsigned int line, Args... args) {
        if (messagePriority >= log_level) {
            std::scoped_lock lock(log_mutex);
            std::stringstream log;

            log << "[" << priorityString << "] " <<
                file << ":" << line << " | ";
            recursive(log, args...);
            
            toFile(log);
        }
    }
}