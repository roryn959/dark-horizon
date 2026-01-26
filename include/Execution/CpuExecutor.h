#pragma once

#include <cstdint>
#include <cstddef>
#include <math.h>
#include <iostream>

#include "Model/Photon.h"
#include "Model/Vector.h"
#include "View/Canvas.h"

#define GRAPH_SIZE 50.0

constexpr double GRAPH_HALFRANGE = GRAPH_SIZE / 2;

constexpr double WIN_TO_GRAPH_W_FACTOR = WINDOW_W / GRAPH_SIZE; 
constexpr double WIN_TO_GRAPH_H_FACTOR = WINDOW_H / GRAPH_SIZE;

constexpr double ASPECT_RATIO = WINDOW_W / WINDOW_H;
constexpr double VERTICAL_FOV = M_PI_2;
const double HORIZONTAL_FOV = 2 * atan((VERTICAL_FOV / 2) * ASPECT_RATIO); 
const double HALF_VERTICAL_EXTENT = tan(VERTICAL_FOV / 2);
const double HALF_HORIZONTAL_EXTENT = HALF_VERTICAL_EXTENT * ASPECT_RATIO;

#define LAMBDA 0.02
#define STEP_K 2.5

#define ACCRETION_DISC_INNER_RADIUS 6.0
#define ACCRETION_DISC_OUTER_RADIUS 15.0

constexpr inline int graphToWindowX(double x) {
    return round( (x + (GRAPH_SIZE / 2) ) * (WIN_TO_GRAPH_W_FACTOR) );
}

constexpr inline int graphToWindowY(double y) {
    return WINDOW_H - round( (y + (GRAPH_SIZE / 2) ) * (WIN_TO_GRAPH_H_FACTOR) );
}

constexpr inline double windowToGraphX(int x) {
	return  (x / (double) WIN_TO_GRAPH_W_FACTOR) - GRAPH_HALFRANGE;
}

constexpr inline double windowToGraphY(int y) {
	return  -1 * ((y / (double) WIN_TO_GRAPH_H_FACTOR) - GRAPH_HALFRANGE);
}

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

	uint32_t GetAccretionDiscColour(double r);

	void TracePhoton(uint32_t* pixelBuffer, size_t i);

	std::vector<Photon> m_photons;
};