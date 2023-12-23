#pragma once
#include <Rendering/Texture.h>
#include <Rendering/Shader.h>
#include <Core/Camera.h>
#include <Rendering/Material.h>
#include <Rendering/Light.h>
#include <Rendering/Model.h>
#include <Core/Renderspace.h>
#include <Events/Event.h>

class SceneRenderer
{
public:
	SceneRenderer();
	~SceneRenderer() = default;

	void Render();
	void GuiRender();
	void OnEvent(const Event& e);

private:
	void InitRenderingResources();
	void RenderRenderspace();
	void RenderLightControls();
	void RenderMaterialControls();
	void RenderMeshStats();
	void RenderDebugInfo();
	void RenderShaderInfo();

private:
	bool m_Running = false;

	std::unique_ptr<Texture> m_DiffuseLightMap;
	std::unique_ptr<Texture> m_SpecularLightMap;
	std::unordered_map<std::string, std::unique_ptr<Shader>> m_Shaders;

	Material m_Material = Material();
	Light m_Light = Light();
	Model m_Model;

	bool m_WireframeMode = false;

	Model m_QuadModel;

	Renderspace m_FlatRenderSpace;
	Renderspace m_GoochRenderSpace;
	Renderspace m_LineDetectionRenderSpace;

	static const std::unordered_set<std::string> m_ReservedUniforms;

	const int32_t m_WindowWidth = 1280, m_WindowHeight = 720;
};

