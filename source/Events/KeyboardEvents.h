#pragma once
#include "Event.h"

typedef int KeyCode;


class Key : public Event
{
public:
	Key(KeyCode keyCode)
		: m_KeyCode(keyCode)
	{}

	EVENT_CLASS(KeyboardEvent)

protected:
	KeyCode m_KeyCode;
};

class KeyPressed : public Key
{
public:
	KeyPressed(KeyCode keyCode)
		:Key(keyCode)
	{}

	EVENT_TYPE(KeyPressedEvent)

	KeyCode GetKey() const
	{
		return m_KeyCode;
	}
};

class KeyReleased : public Key
{
public:
	KeyReleased(KeyCode keyCode)
		:Key(keyCode)
	{}

	EVENT_TYPE(KeyReleasedEvent)

	KeyCode GetKey() const
	{
		return m_KeyCode;
	}
};
