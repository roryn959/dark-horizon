#include "Execution/CpuExecutor.h"


CpuExecutor::CpuExecutor()
{
}

CpuExecutor::~CpuExecutor() {
}

void CpuExecutor::TracePhotons(uint32_t* pixelBuffer) {
	for (int i = 0; i < NUM_PIXELS; ++i) {
		TracePhoton(pixelBuffer, i);
	}
}

void CpuExecutor::TracePhoton(uint32_t* pixelBuffer, size_t i) {
	pixelBuffer[i] = 0xFFFF0000;
}