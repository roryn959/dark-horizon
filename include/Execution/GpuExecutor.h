#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>

#include "View/Canvas.h"


class GpuExecutor {
public:
    GpuExecutor();
    ~GpuExecutor();

    void TracePhotons(uint32_t* buffer);

private:
	void InitialiseDevice();
	void InitialiseCommandQueue();
	void CheckCommandBuffer();
	void InitialisePipeline();

    struct Impl;     // opaque
    Impl* impl;      // pimpl to hide Objective-C++
};
