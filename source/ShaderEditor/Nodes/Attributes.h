#pragma once
#include "Rendering/GlslDataTypes.h"

struct InputAttribute
{
	DataType Type;
	const void* Value;
	std::string Name;
};

struct OutputAttribute
{
	DataType Type;
	void* Value;
	std::string Name;
};