#pragma once
#include "Events/Event.h"
#include "Core/Window.h"
#include <memory>
#include "Core/Camera.h"
#include "Core/Timer.h"
#include "Core/SceneRenderer.h"
#include "ShaderEditor/ShaderEditor.h"


class Engine
{
public:
	Engine(const char* title);
	~Engine();
	static const Engine* Get() { return s_Engine; }

	void Run();
	const Window* GetWindow() const { return m_Window.get(); };

private:
	void EventCallback(const Event& e);
	void InitWindow(const char* title);
	void GUIRender();

	void RenderDockspace();

private:
	static Engine* s_Engine;

	bool m_Running = false;
	std::unique_ptr<Window> m_Window;
	int32_t m_WindowWidth = 1280, m_WindowHeight = 720;
	Timer m_Timer;

	bool m_IsShaderEditorOpen = true;
	bool m_IsSceneRendererOpen = false;
	std::unique_ptr<SceneRenderer> m_SceneRenderer = nullptr;
	std::unique_ptr<ShaderEditor> m_ShaderEditor = nullptr;
};

