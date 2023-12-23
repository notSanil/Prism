#include "pch.h"
#include "Input.h"

std::unordered_map<KeyCode, bool> Input::s_PressedKeys;
std::unordered_map<KeyCode, bool> Input::s_PressedMouseButtons;
double Input::mouseX = 0.0, Input::mouseY = 0.0;

bool Input::IsKeyPressed(KeyCode keyCode)
{
	return s_PressedKeys.count(keyCode) == 0 ? false : s_PressedKeys[keyCode];
}

bool Input::IsMouseButtonPressed(MouseButtonCode buttonCode)
{
	return s_PressedMouseButtons.count(buttonCode) == 0 ? false : s_PressedMouseButtons[buttonCode];
}

void Input::OnEvent(const Event& e)
{
	if (e.GetType() == EventType::KeyPressedEvent)
	{
		const KeyPressed& keyPressed = static_cast<const KeyPressed&>(e);
		s_PressedKeys[keyPressed.GetKey()] = true;
	}
	else if (e.GetType() == EventType::KeyReleasedEvent)
	{
		const KeyReleased& keyReleased = static_cast<const KeyReleased&>(e);
		s_PressedKeys[keyReleased.GetKey()] = false;
	}
	else if (e.GetType() == EventType::MouseButtonPressedEvent)
	{
		const MouseButtonPressed& buttonPressed = static_cast<const MouseButtonPressed&>(e);
		s_PressedMouseButtons[buttonPressed.GetButton()] = true;
	}
	else if (e.GetType() == EventType::MouseButtonReleasedEvent)
	{
		const MouseButtonReleased& buttonReleased = static_cast<const MouseButtonReleased&>(e);
		s_PressedMouseButtons[buttonReleased.GetButton()] = false;
	}
	else if (e.GetType() == EventType::MouseMovedEvent)
	{
		const MouseMoved& mouseMoved = static_cast<const MouseMoved&>(e);
		mouseX = mouseMoved.GetMouseX();
		mouseY = mouseMoved.GetMouseY();
	}
}