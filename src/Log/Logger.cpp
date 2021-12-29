#include "Logger.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <GL/glew.h>
#include <sstream>
#include <mutex>

namespace Callie{
    LogPriority log_level = LogPriority::DEBUG;
    std::mutex log_mutex;

    void toFile(std::stringstream& log){
        std::cout << log.str();
        std::ofstream file("log.txt", std::ios::app);
        if (file.is_open()){
            file << getTimestamp() << " " << log.str();
            file.close();
        }
        else
            std::cout << "Unable to open log file!\n";
    }

    const std::string getTimestamp() {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm gmt; localtime_r(&currentTime, &gmt); //convert time

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
            std::stringstream log;
            log << "[OpenGl error (" << error << ")]: " << function <<
                " " << file << ":" << line << std::endl;

            toFile(log);

            status = false;
        }
        return status;
    }
}