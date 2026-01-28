#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>

#include "Model/Vector.h"
#include "View/Canvas.h"

#define GRAPH_SIZE 30.0f
constexpr float GRAPH_HALFRANGE = GRAPH_SIZE / 2.0f;


class GpuExecutor {
public:
    GpuExecutor();
    ~GpuExecutor();

	inline void MoveHorizontally(float theta) { m_theta += theta; }
	inline void MoveVertically(float phi) { m_phi += phi; if (m_phi > M_PI_2) m_phi = M_PI_2; if (m_phi < -M_PI_2) m_phi = -M_PI_2; }

    void TracePhotons(uint32_t* buffer);

private:
	float m_rho;
	float m_theta;
	float m_phi;

	Vector ComputeCameraPosition();

	void InitialiseDevice();
	void InitialiseCommandQueue();
	void CheckCommandBuffer();
	void InitialisePipeline();

    struct Impl;
    Impl* impl;
};
