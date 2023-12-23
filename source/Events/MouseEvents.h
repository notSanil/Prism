#pragma once
#include "Event.h"

typedef int MouseButtonCode;


class MouseButton : public Event
{
public:
	MouseButton(MouseButtonCode buttonCode)
		: m_ButtonCode(buttonCode)
	{}

	EVENT_CLASS(MouseEvent)

protected:
	MouseButtonCode m_ButtonCode;
};

class MouseButtonPressed : public MouseButton
{
public:
	MouseButtonPressed(MouseButtonCode buttonCode)
		:MouseButton(buttonCode)
	{}

	EVENT_TYPE(MouseButtonPressedEvent)

	MouseButtonCode GetButton() const
	{
		return m_ButtonCode;
	}
};

class MouseButtonReleased : public MouseButton
{
public:
	MouseButtonReleased(MouseButtonCode buttonCode)
		:MouseButton(buttonCode)
	{}

	EVENT_TYPE(MouseButtonReleasedEvent)

	MouseButtonCode GetButton() const
	{
		return m_ButtonCode;
	}
};

class MouseScroll : public Event
{
public:
	MouseScroll(double x, double y)
		:m_Scrollx(x), m_Scrolly(y)
	{}

	EVENT_TYPE(MouseScrollEvent)
	EVENT_CLASS(MouseEvent)

	double GetScrollx() const
	{
		return m_Scrollx;
	}
	
	double GetScrolly() const
	{
		return m_Scrolly;
	}
private:
	double m_Scrollx, m_Scrolly;
};

class MouseMoved : public Event
{
public:
	MouseMoved(double x, double y) :
		m_MouseX(x), m_MouseY(y)
	{}

	EVENT_TYPE(MouseMovedEvent)
	EVENT_CLASS(MouseEvent)

	double GetMouseX() const
	{
		return m_MouseX;
	}
	
	double GetMouseY() const
	{
		return m_MouseY;
	}

private:
	double m_MouseX, m_MouseY;
};
