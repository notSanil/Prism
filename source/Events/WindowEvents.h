#pragma once
#include "Event.h"


class WindowClose : public Event
{
public:
	WindowClose() = default;

	EVENT_TYPE(WindowCloseEvent)
	EVENT_CLASS(WindowEvent)
};

class WindowResize : public Event
{
public:
	WindowResize(int newWidth, int newHeight)
		: m_Width(newWidth), m_Height(newHeight)
	{}
	EVENT_TYPE(WindowResizeEvent)
	EVENT_CLASS(WindowEvent)

	int GetWidth() const
	{
		return m_Width;
	}
	
	int GetHeight() const
	{
		return m_Height;
	}

private:

	int m_Width, m_Height;
};