#pragma once

#include <cstdint>
#include <cstddef>

#include "View/Canvas.h"


class CpuExecutor {
public:
	CpuExecutor();
	~CpuExecutor();

	void TracePhotons(uint32_t* pixelBuffer);

private:
	void TracePhoton(uint32_t* pixelBuffer, size_t i);
};