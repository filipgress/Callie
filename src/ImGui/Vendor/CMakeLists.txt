set(imgui_src
    imconfig.h
    imgui_impl_glfw.h
    imgui_impl_opengl3.h
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    imgui_internal.h
    imgui_impl_opengl3_loader.h

    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
)

add_library(imgui ${imgui_src})