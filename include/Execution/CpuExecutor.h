#pragma once

#include <cstdint>
#include <cstddef>
#include <math.h>
#include <iostream>

#include "Model/Photon.h"
#include "Model/Vector.h"
#include "View/Canvas.h"

#define GRAPH_SIZE 50.0

constexpr float GRAPH_HALFRANGE = GRAPH_SIZE / 2;

constexpr float ASPECT_RATIO = WINDOW_W / WINDOW_H;
constexpr float VERTICAL_FOV = M_PI_2;
const float HORIZONTAL_FOV = 2 * atan((VERTICAL_FOV / 2) * ASPECT_RATIO); 
const float HALF_VERTICAL_EXTENT = tan(VERTICAL_FOV / 2);
const float HALF_HORIZONTAL_EXTENT = HALF_VERTICAL_EXTENT * ASPECT_RATIO;

#define LAMBDA 0.02
#define STEP_K 2.5

#define ACCRETION_DISC_INNER_RADIUS 6.0
#define ACCRETION_DISC_OUTER_RADIUS 15.0

constexpr Vector COLOUR_INNER_ACCRETION_DISC{ 1.0, 0.8, 0.35 };
constexpr Vector COLOUR_OUTER_ACCRETION_DISC{ 1.0, 0.94, 0.82 };
constexpr Vector ACCRETION_DISC_COLOUR_DIFFERENCE = COLOUR_OUTER_ACCRETION_DISC - COLOUR_INNER_ACCRETION_DISC;

class CpuExecutor {
public:
	CpuExecutor();
	~CpuExecutor();

	void TracePhotons(uint32_t* pixelBuffer);

	void MarchPhoton(Photon&);

private:
	Vector GenerateInitialDirection(size_t i);

	uint32_t GetAccretionDiscColour(float r);

	void TracePhoton(uint32_t* pixelBuffer, size_t i);

	std::vector<Photon> m_photons;
};