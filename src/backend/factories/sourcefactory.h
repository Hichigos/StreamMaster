#pragma once

#include "../utils/shared.h"
#include "../adapters/sources/source.h"

class SourceFactory {
public:
	SourceFactory() = delete;

	static SourcePtr createSource(const SourceType type);
};