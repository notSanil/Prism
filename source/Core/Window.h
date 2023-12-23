#pragma once
#include "Events/Event.h"
#include <functional>

using WindowEventCallback = std::function<void(const Event&)>;

struct WindowProperties 
{
	bool isVsync = true;
	bool isFullscreen = true;
	int width = 0, height = 0;
	std::string title = " ";
	WindowEventCallback callback;
};

class Window
{
public:
	Window(const WindowProperties& windowProps);
	~Window();
	void Update();
	void PollEvents();
	void* GetWindowHandle() const;
	void SetVsync(bool isVsync);
	bool GetVsync();
	int GetWidth();
	int GetHeight();

private:
	struct WindowData
	{
		bool isVsync;
		void* windowHandle;
		WindowEventCallback eventCallback;
	};
	static uint32_t s_WindowCount;
	WindowData m_Data;
};

