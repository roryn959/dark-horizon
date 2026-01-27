#include "Execution/CpuExecutor.h"


CpuExecutor::CpuExecutor() :
	m_photons{}
{
}

CpuExecutor::~CpuExecutor() {
}

void CpuExecutor::TracePhotons(uint32_t* pixelBuffer) {
	for (int i = 0; i < NUM_PIXELS; ++i) {
		TracePhoton(pixelBuffer, i);
	}
}

void CpuExecutor::MarchPhoton(Photon& photon) {
	float r = Magnitude(photon.m_position);

	float curvature = 3.0 / (r * r * r);
	float h = LAMBDA / (1.0 + STEP_K * curvature);

	auto f = [&r, &curvature](const Vector& position, const Vector& direction) -> Vector {
		float r = Magnitude(position);
		Vector r_hat = position / r;

		Vector e_r = r_hat - Dot(r_hat, direction) * direction;
		return (-3.0 / (r * r * r)) * e_r;
	};

	Vector v1 = photon.m_direction;
	Vector x1 = photon.m_position;
	float r1 = Magnitude(x1);
	float s_r1 = 1.0 - (2.0 / r1);
	Vector k1_x = s_r1 * v1;
	Vector k1_v = f(x1, Normalise(v1));

	Vector x2 = x1 + 0.5 * h * k1_x;
	Vector v2 = Normalise(v1 + 0.5 * h * k1_v);
	float r2 = Magnitude(x2);
	float s_r2 = 1.0 - (2.0 / r2);
	Vector k2_x = s_r2 * v2;
	Vector k2_v = f(x2, Normalise(v2));

	Vector x3 = x1 + 0.5 * h * k2_x;
	Vector v3 = Normalise(v1 + 0.5 * h * k2_v);
	float r3 = Magnitude(x3);
	float s_r3 = 1.0 - (2.0 / r3);
	Vector k3_x = s_r3 * v3;
	Vector k3_v = f(x3, Normalise(v3));

	Vector x4 = x1 + h * k3_x;
	Vector v4 = Normalise(v1 + h * k3_v);
	float r4 = Magnitude(x4);
	float s_r4 = 1.0 - (2.0 / r4);
	Vector k4_x = s_r4 * v4;
	Vector k4_v = f(x4, Normalise(v4));
	
	photon.m_position = photon.m_position + (h / 6.0) * (k1_x + 2*k2_x + 2*k3_x + k4_x);
	photon.m_direction = photon.m_direction + (h / 6.0) * (k1_v + 2*k2_v + 2*k3_v + k4_v);
	photon.m_direction = Normalise(photon.m_direction);
}

Vector CpuExecutor::GenerateInitialDirection(size_t i) {
	float wx = 2 * ( ( (i % WINDOW_W) + 0.5 ) / WINDOW_W ) - 1;
	float wy = 1 - 2 * ( (i / WINDOW_W + 0.5 ) / WINDOW_H );

	Vector dir = { wx * HALF_HORIZONTAL_EXTENT, wy * HALF_VERTICAL_EXTENT, 1.0 };

	return Normalise(dir);
}

uint32_t CpuExecutor::GetAccretionDiscColour(float r) {
	float outness = (r - ACCRETION_DISC_INNER_RADIUS) / (ACCRETION_DISC_OUTER_RADIUS - ACCRETION_DISC_INNER_RADIUS);
	return ColourToUint32(COLOUR_INNER_ACCRETION_DISC + outness * ACCRETION_DISC_COLOUR_DIFFERENCE);
}

void CpuExecutor::TracePhoton(uint32_t* pixelBuffer, size_t i) {
	Vector initialPosition{ 0.0, 5.0, -GRAPH_HALFRANGE };
	Vector initialDirection = GenerateInitialDirection(i);

	Photon photon{ initialPosition, initialDirection };

	size_t MAX_STEPS = 2500;
	size_t steps = 0;
	while (steps < MAX_STEPS) {
		Vector p = photon.m_position;
		float r = Magnitude(p);
		if (r <= 3.0) {
			pixelBuffer[i] = 0xFF000000;
			return;
		}

		if (p.m_1 > -0.2 && p.m_1 < 0.2 && r > ACCRETION_DISC_INNER_RADIUS && r < ACCRETION_DISC_OUTER_RADIUS) {
			pixelBuffer[i] = GetAccretionDiscColour(r);
			return;
		}

		if (
			(p.m_0 < -GRAPH_HALFRANGE)	||
			(p.m_0 > GRAPH_HALFRANGE) 	||
			(p.m_1 < -GRAPH_HALFRANGE) 	||
			(p.m_1 > GRAPH_HALFRANGE) 	||
			(p.m_2 < -GRAPH_HALFRANGE) 	||
			(p.m_2 > GRAPH_HALFRANGE)
		) {
			pixelBuffer[i] = 0xFF000000;
			return;
		}

		//MarchPhoton(photon);
		photon.m_position = p + photon.m_direction * LAMBDA;
		++steps;
	}

	pixelBuffer[i] = 0xFF000000;
}