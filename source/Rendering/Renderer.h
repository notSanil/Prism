#pragma once
#include <glm.hpp>
#include "Events/Event.h"
#include "Rendering/Shader.h"
#include "Rendering/Light.h"
#include "Core/Camera.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"


class Renderer
{
public:
	static void Init(int windowWidth, int windowHeight);

	static void SetClearColor(const glm::vec4& color);
	static void ClearScreen();
	static void Render(Model& model, const Material& material);
	static void OnEvent(const Event& e);
	static void SetWireframeMode(bool isWireframeMode);
	static void SetViewport(uint32_t width, uint32_t height);

	static void ChangeShader(Shader* shader);

	static void BeginScene(const Light& sceneLight, const Camera& sceneCamera);
	static void EndScene();

	static void BeginSecondRender();
	static void EndSecondRender();

private:
	static Shader* s_CurrentShader;
};

