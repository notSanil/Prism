#include "pch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Events/WindowEvents.h"
#include "Events/MouseEvents.h"
#include "Events/KeyboardEvents.h"

uint32_t Window::s_WindowCount = 0;

Window::Window(const WindowProperties& props)
{
	m_Data.eventCallback = props.callback;
	SetVsync(props.isVsync);

	if (s_WindowCount == 0)
	{
		if (!glfwInit())
		{
			LOG_ERROR("GLFW init failed!");
		}
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_MAXIMIZED, props.isFullscreen ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	m_Data.windowHandle = static_cast<void*>(glfwCreateWindow(props.width, props.height, props.title.c_str(), 
		nullptr, nullptr));

	if (!m_Data.windowHandle)
	{
		LOG_ERROR("Window creation failed!");
	}

	glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_Data.windowHandle), &m_Data);

	glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Data.windowHandle));
	//This makes it glad dependent, find alternative
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));


	float xscale, yscale;
	glfwGetWindowContentScale(static_cast<GLFWwindow*>(m_Data.windowHandle), &xscale, &yscale);

	glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(m_Data.windowHandle), [](GLFWwindow* window)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowClose e;
			data->eventCallback(e);
		}
	);

	glfwSetWindowSizeCallback(static_cast<GLFWwindow*>(m_Data.windowHandle), [](GLFWwindow* window, int width, int height)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowResize e(width, height);
			data->eventCallback(e);
		}
	);

	glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(m_Data.windowHandle), [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS)
			{
				MouseButtonPressed e((MouseButtonCode)button);
				data->eventCallback(e);
			}
			else
			{
				MouseButtonReleased e((MouseButtonCode)button);
				data->eventCallback(e);
			}
		}
	);

	glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_Data.windowHandle), [](GLFWwindow* window, double xpos, double ypos) 
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseMoved e(xpos, ypos);
			data->eventCallback(e);
		}
	);

	glfwSetScrollCallback(static_cast<GLFWwindow*>(m_Data.windowHandle), [](GLFWwindow* window, double xoff, double yoff)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseScroll e(xoff, yoff);
			data->eventCallback(e);
		}
	);

	glfwSetKeyCallback(static_cast<GLFWwindow*>(m_Data.windowHandle), [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				KeyPressed e(key);
				data->eventCallback(e);
			}
			else if (action == GLFW_RELEASE)
			{
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				KeyReleased e(key);
				data->eventCallback(e);
			}
		}
	);

	++s_WindowCount;
}

Window::~Window()
{
	glfwDestroyWindow(static_cast<GLFWwindow*>(m_Data.windowHandle));
	
	if (s_WindowCount == 1)
		glfwTerminate();

	--s_WindowCount;
}

void Window::Update()
{
	glfwSwapBuffers(static_cast<GLFWwindow*>(m_Data.windowHandle));
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void* Window::GetWindowHandle() const
{
	return m_Data.windowHandle;
}

void Window::SetVsync(bool isVsync)
{
	m_Data.isVsync = isVsync;
	glfwSwapInterval(m_Data.isVsync ? 1 : 0);
}

bool Window::GetVsync()
{
	return m_Data.isVsync;
}

int Window::GetHeight()
{
	int height;
	glfwGetWindowSize(static_cast<GLFWwindow*>(m_Data.windowHandle), nullptr, &height);
	return height;
}

int Window::GetWidth()
{
	int width;
	glfwGetWindowSize(static_cast<GLFWwindow*>(m_Data.windowHandle), &width, nullptr);
	return width;
}