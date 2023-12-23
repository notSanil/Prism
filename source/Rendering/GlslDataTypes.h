#pragma once
#include <array>
#define DATA_TYPE(name) name,
enum class DataType
{
#include "GlslDataTypes-def.h"
};

#undef DATA_TYPE

static constexpr int NumberOfDataTypes()
{
	return 0
#define DATA_TYPE(name) +1
#include "GlslDataTypes-def.h"
#undef DATA_TYPE
		;
}

extern std::array<std::string, NumberOfDataTypes()> DataTypeToString;