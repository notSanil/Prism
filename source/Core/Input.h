#pragma once
#include "Events/Event.h"
#include "Events/KeyboardEvents.h"
#include "Events/MouseEvents.h"


class Input
{
public:
	static bool IsKeyPressed(KeyCode KeyCode);
	static bool IsMouseButtonPressed(MouseButtonCode buttonCode);
	static void OnEvent(const Event& e);
	static double GetMouseX() { return mouseX; }
	static double GetMouseY() { return mouseY; }

private:
	static std::unordered_map<KeyCode, bool> s_PressedKeys;
	static std::unordered_map<MouseButtonCode, bool> s_PressedMouseButtons;

	static double mouseX, mouseY;
};