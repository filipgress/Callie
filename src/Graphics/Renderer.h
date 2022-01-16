#pragma once

#include <Core/Mesh.h>
#include <Graphics/Shader.h>

namespace cl{
    class Renderer{
        public:
            void Draw(Mesh& obj, Shader& program);
    };
}