#include <pch.h>

static constexpr int NumberOfDataTypes()
{
	return 0
#define DATA_TYPE(name) +1
#include "GlslDataTypes-def.h"
#undef DATA_TYPE
		;
}

#define DATA_TYPE(name) #name,
std::array<std::string, NumberOfDataTypes()> DataTypeToString{
#include "Rendering/GlslDataTypes-def.h"
};
#undef DATA_TYPE