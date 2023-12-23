#pragma once
#define BIT(x) 1 << x


enum class EventClass
{
	None			= BIT(0), 
	WindowEvent		= BIT(1), 
	MouseEvent		= BIT(2), 
	KeyboardEvent	= BIT(3),
};

enum class EventType
{
	None,
	WindowCloseEvent, WindowResizeEvent, WindowMinimisedEvent, WindowMaximisedEvent,
	MouseMovedEvent, MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseScrollEvent,
	KeyPressedEvent, KeyReleasedEvent
};


class Event
{
public:
#define EVENT_TYPE(x) 	virtual const char* ToString() const \
	{ \
		return #x; \
	}; \
	virtual EventType GetType() const \
	{\
		return EventType::x;\
	}


#define EVENT_CLASS(x) virtual EventClass GetEventClass()const \
	{ \
		return EventClass::x;\
	}


	EVENT_TYPE(None)
	EVENT_CLASS(None)
};