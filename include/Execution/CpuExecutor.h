#pragma once

#include <cstdint>
#include <cstddef>
#include <math.h>
#include <iostream>

#include "Model/Photon.h"
#include "Model/Vector.h"
#include "View/Canvas.h"

#define GRAPH_W 20.0
#define GRAPH_H 20.0

#define LAMBDA 0.001

constexpr double X_HALFRANGE = GRAPH_W / 2;
constexpr double Y_HALFRANGE = GRAPH_H / 2;
constexpr double WIN_TO_GRAPH_W_FACTOR = WINDOW_W / GRAPH_W; 
constexpr double WIN_TO_GRAPH_H_FACTOR = WINDOW_H / GRAPH_H;

constexpr inline int graphToWindowX(double x) {
    return round( (x + (GRAPH_W / 2) ) * (WIN_TO_GRAPH_W_FACTOR) );
}

constexpr inline int graphToWindowY(double y) {
    return WINDOW_H - round( (y + (GRAPH_H / 2) ) * (WIN_TO_GRAPH_H_FACTOR) );
}

constexpr inline double windowToGraphX(int x) {
	return  (x / (double) WIN_TO_GRAPH_W_FACTOR) - X_HALFRANGE;
}

constexpr inline double windowToGraphY(int y) {
	return  -1 * ((y / (double) WIN_TO_GRAPH_H_FACTOR) - Y_HALFRANGE);
}

class CpuExecutor {
public:
	CpuExecutor();
	~CpuExecutor();

	void TracePhotons(uint32_t* pixelBuffer);

	void ProcessTimeStep(Photon&);

private:
	void TracePhoton(uint32_t* pixelBuffer, size_t i);

	inline double A(double r) { return (1 - (2 / r)); }

	inline double Squash(double x) { return x / (x + 1); }

	std::vector<Photon> m_photons;
};