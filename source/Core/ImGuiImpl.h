#pragma once


class ImGuiImpl
{
public:
	static void Init();
	static void StartFrame();
	static void EndFrame();
	static void Destroy();
};
