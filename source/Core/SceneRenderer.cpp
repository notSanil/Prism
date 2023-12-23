#include "pch.h"
#include "SceneRenderer.h"
#include <Rendering/Renderer.h>
#include <imgui.h>
#include <gtc/type_ptr.hpp>
#include <Rendering/DataTypeGuiRenderer.h>

const std::unordered_set<std::string> SceneRenderer::m_ReservedUniforms = { "viewProj", "light.ambient", "light.diffuse", "light.specular", "light.position", "viewDirection" };


SceneRenderer::SceneRenderer()
{
	InitRenderingResources();
}

void SceneRenderer::InitRenderingResources()
{
	m_Model = Model::FromCollada("Resources/Models/FullChairdae.dae");

	m_DiffuseLightMap = std::make_unique<Texture>("Resources/Textures/container2.png");
	m_SpecularLightMap = std::make_unique<Texture>("Resources/Textures/container2_specular.png");

	m_Shaders["Gooch Shader"] = std::make_unique<Shader>("Resources/Shaders/vertexShader.glsl", "Resources/Shaders/goochShader.glsl");
	m_Shaders["Line Draw Shader"] = std::make_unique<Shader>("Resources/Shaders/vertexShader.glsl", "Resources/Shaders/lineDrawShader.glsl");
	m_Shaders["Flat Color Shader"] = std::make_unique<Shader>("Resources/Shaders/vertexShader.glsl", "Resources/Shaders/flatColorShader.glsl");

	m_Light.ambient = glm::vec3(0.2f);
	m_Light.diffuse = glm::vec3(0.5f);
	m_Light.specular = glm::vec3(1.0f);
	m_Light.position = glm::vec3(0.0f, 2.0f, 6.0f);

	m_Material.shininess = 32.0f;

	std::vector<Vertex> vertices = {
		Vertex({-1.0, -1.0, 0.0}, {}, {0.0, 0.0}),
		Vertex({1.0, -1.0, 0.0}, {}, {1.0, 0.0}),
		Vertex({1.0,  1.0, 0.0}, {}, {1.0,  1.0}),
		Vertex({-1.0,  1.0, 0.0}, {}, {0.0, 1.0})
	};

	std::vector<uint32_t> indexData = { 0, 1, 2, 0, 2, 3 };

	m_QuadModel = Model();
	m_QuadModel.AddMesh(Mesh(vertices, indexData, glm::mat4(1.0)));
	m_QuadModel.Compile();

	m_FlatRenderSpace = Renderspace("Flat Render", std::make_unique<AntiAliasedFramebuffer>(m_WindowWidth, m_WindowHeight), m_Shaders.at("Flat Color Shader").get());
	m_GoochRenderSpace = Renderspace("Gooch Render", std::make_unique<AntiAliasedFramebuffer>(m_WindowWidth, m_WindowHeight), m_Shaders.at("Gooch Shader").get());
	m_LineDetectionRenderSpace = Renderspace("Final Gooch Render", std::make_unique<AntiAliasedFramebuffer>(m_WindowWidth, m_WindowHeight), m_Shaders.at("Line Draw Shader").get());
}

void SceneRenderer::RenderRenderspace()
{
	//TODO: Right now the render space size is determined by the gui size
	//We need to fix this so that it is somehow determined by some other factor
	m_FlatRenderSpace.RenderGui();
	m_LineDetectionRenderSpace.RenderGui();
	//m_GoochRenderSpace.RenderGui();
}

void SceneRenderer::RenderLightControls()
{
	ImGui::Begin("Shader Controls");

	ImGui::SliderFloat3("Ambient Light", glm::value_ptr(m_Light.ambient), 0.0f, 1.0f);
	ImGui::SliderFloat3("Diffuse Light", glm::value_ptr(m_Light.diffuse), 0.0f, 1.0f);
	ImGui::SliderFloat3("Specular Light", glm::value_ptr(m_Light.specular), 0.0f, 1.0f);
	ImGui::SliderFloat3("Light Position", glm::value_ptr(m_Light.position), -10.0f, 10.0f);

	ImGui::End();
}

static uint32_t mostSignificantBit(uint32_t n)
{
	return std::numeric_limits<decltype(n)>::digits - std::countl_zero(n) - 1;
}

void SceneRenderer::RenderMaterialControls()
{
	ImGui::Begin("Material Controls");
	int shinePower = mostSignificantBit((uint32_t)m_Material.shininess);
	if (ImGui::SliderInt("Shine", &shinePower, 1, 6))
	{
		m_Material.shininess = static_cast<float>(1 << shinePower);
	}
	ImGui::End();
}

void SceneRenderer::RenderMeshStats()
{
	ImGui::Begin("Mesh Stats");
	ImGui::Text("Vertices: %d", m_Model.GetVertexCount());
	ImGui::Text("Indices: %d", m_Model.GetIndexCount());
	ImGui::Text("Triangles: %d", m_Model.GetTriangleCount());
	ImGui::End();
}

void SceneRenderer::RenderDebugInfo()
{
	ImGui::Begin("Debug Info");
	if (ImGui::Checkbox("Wireframe Mode", &m_WireframeMode))
	{
		Renderer::SetWireframeMode(m_WireframeMode);
	}
	ImGui::End();
}

void SceneRenderer::RenderShaderInfo()
{
	ImGui::Begin("Shader Info");
	for (auto& [name, shader] : m_Shaders)
	{
		if (ImGui::TreeNode(name.c_str()))
		{
			auto& uniformsInfo = shader->GetUniformInfo();
			for (auto& [variableName, variableInfo] : uniformsInfo)
			{
				/*if (m_ReservedUniforms.count(variableName) != 0)
					continue;*/

				glm::vec3 random(1.0);

				RenderUniformGui(variableInfo.Type, &random, variableName);
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();

}

void SceneRenderer::Render()
{
	Renderer::SetClearColor({ 0.7, 0.2, 0.8, 1.0 });
	m_FlatRenderSpace.Render([&](Shader* shader)
		{
			Renderer::Render(m_Model, m_Material);
		}, false, m_Light);

	m_GoochRenderSpace.Render([&](Shader* shader)
		{
			Renderer::Render(m_Model, m_Material);
		}, false, m_Light);

	m_LineDetectionRenderSpace.Render([&](Shader* shader)
		{
			m_GoochRenderSpace.GetFramebuffer().GetColorTexture().Bind(shader->GetSamplerBindPoint("colorTexture"));
			m_GoochRenderSpace.GetFramebuffer().GetDepthStencilTexture().Bind(shader->GetSamplerBindPoint("depthTexture"));

			Renderer::Render(m_QuadModel, m_Material);
		}, true, m_Light);
}

void SceneRenderer::GuiRender()
{
	//ImGui::ShowDemoWindow();
	RenderRenderspace();
	RenderLightControls();
	//RenderMaterialControls();
	//RenderMeshStats();
	RenderShaderInfo();
	RenderDebugInfo();
}

void SceneRenderer::OnEvent(const Event & e)
{
	m_FlatRenderSpace.OnEvent(e);
	m_GoochRenderSpace.OnEvent(e);
	m_LineDetectionRenderSpace.OnEvent(e);
}
