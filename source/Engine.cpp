#include "pch.h"
#include "Engine.h"
#include "Core/Window.h"
#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "Core/Input.h"
#include "Core/ImGuiImpl.h"
#include <imgui.h>
#include "Rendering/Renderer.h"

Engine* Engine::s_Engine = nullptr;

Engine::Engine(const char* title)
{
	if (s_Engine != nullptr)
		return;
	s_Engine = this;

	InitWindow(title);
	Renderer::Init(m_WindowWidth, m_WindowHeight);
	ImGuiImpl::Init();

	m_SceneRenderer = std::make_unique<SceneRenderer>();
	m_ShaderEditor = std::make_unique<ShaderEditor>();
}

Engine::~Engine()
{
	ImGuiImpl::Destroy();
}

void Engine::InitWindow(const char* title)
{
	WindowProperties winProperties;
	winProperties.width = m_WindowWidth;
	winProperties.height = m_WindowHeight;
	winProperties.isVsync = true;
	winProperties.callback = std::bind(&Engine::EventCallback, this, std::placeholders::_1);
	winProperties.title = title;
	winProperties.isFullscreen = true;

	m_Window = std::make_unique<Window>(winProperties);
	//Depending on the fullscreen mode of the window, the height and width may change, hence we set it back
	m_WindowWidth = m_Window->GetWidth();
	m_WindowHeight = m_Window->GetHeight();
}

void Engine::EventCallback(const Event& e)
{
	if (e.GetType() == EventType::WindowCloseEvent)
	{
		m_Running = false;
	}

	Input::OnEvent(e);
	Renderer::OnEvent(e);
	m_SceneRenderer->OnEvent(e);
	m_ShaderEditor->OnEvent(e);
}

void Engine::Run()
{
	m_Running = true;

	m_Timer.Reset();

	while (m_Running)
	{
		m_Window->PollEvents();

		float deltaTime = m_Timer.Elapsed();
		m_Timer.Reset();
		deltaTime = std::min(deltaTime, 0.05f);
		
		if (m_IsSceneRendererOpen)
			m_SceneRenderer->Render();

		if (m_IsShaderEditorOpen)
			m_ShaderEditor->Render();
		GUIRender();

		m_Window->Update();
	}
}

void Engine::GUIRender()
{
	Renderer::SetViewport(m_WindowWidth, m_WindowHeight);
	Renderer::ClearScreen();
	ImGuiImpl::StartFrame();
	RenderDockspace();

	ImGuiImpl::EndFrame();
}

void Engine::RenderDockspace()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("Primary DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("PrimaryDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Close", nullptr, &m_Running);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Scene Renderer", nullptr, &m_IsSceneRendererOpen);
			ImGui::MenuItem("Shader Editor", nullptr, &m_IsShaderEditorOpen);

			ImGui::EndMenu();
		}

		ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_Reorderable;

		if (ImGui::BeginTabBar("Tab bar", tabBarFlags))
		{

			if (ImGui::BeginTabItem("Scene Renderer", &m_IsSceneRendererOpen))
			{
				m_SceneRenderer->GuiRender();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Shader Editor", &m_IsShaderEditorOpen))
			{
				m_ShaderEditor->GuiRender();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}
