#pragma once

#include <glm/glm.hpp>

namespace cl{
    class Renderer{
        public:
            static int Init();

            static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
            static void OnWindowResize(const unsigned int width, const unsigned int height);
            static void SetClearColor(const glm::vec4& color);

            static void Draw(unsigned int count, bool isWireframe);
            static void Clear();
    };
}