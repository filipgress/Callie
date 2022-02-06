#pragma once

#include <GLFW/glfw3.h>
#include <Window/Event.h>

namespace cl{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Callie Editor",
			        uint32_t width = 1100,
			        uint32_t height = 650)
			: Title(title), Width(width), Height(height) {}
	};

	class Window{
		using EventCallbackFn = std::function<void(Event&)>;

		private:
			struct WindowData{
				std::string Title;
				unsigned int Width, Height;
				bool VSync;

				EventCallbackFn EventCallback;
			};

			GLFWwindow* m_Window;
			WindowData m_Data;
			static unsigned int s_GLFWWindowCount;

		public:
			Window(const WindowProps& props = WindowProps());
			~Window();

			int Init();

			void OnUpdate();

			unsigned int GetWidth() const { return m_Data.Width; }
			unsigned int GetHeight() const { return m_Data.Height; }
			void* GetNativeWindow() const { return m_Window; }

			// Window Attribs
			bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
			void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

			void SetVSync(bool enabled);
			bool IsVSync() const;
		
		private:
			void Shutdown();
			static void GLFWErrorCallback(int error, const char* description);
	};
}