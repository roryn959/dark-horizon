#include <metal_stdlib>
using namespace metal;

// ---------- Physics/Maths Constants ----------

#define PI 3.14159265f
#define LAMBDA 0.05f
#define STEP_K 2.5f

#define ACCRETION_DISC_INNER_RADIUS 4.0f
#define ACCRETION_DISC_OUTER_RADIUS 8.00f

// ---------- GUI Constants ----------

#define WINDOW_W 800
#define WINDOW_H 800
#define GRAPH_SIZE 30.0f

constant float GRAPH_HALFRANGE = GRAPH_SIZE / 2.0f;
constant float ASPECT_RATIO = WINDOW_W / float(WINDOW_H);

inline float _VERTICAL_FOV() { return PI / 2.0f; }
#define VERTICAL_FOV _VERTICAL_FOV()

inline float _HORIZONTAL_FOV() { return 2 * atan((VERTICAL_FOV / 2) * ASPECT_RATIO); }
#define HORIZONTAL_FOV _HORIZONTAL_FOV()

inline float _HALF_VERTICAL_EXTENT() { return tan(VERTICAL_FOV / 2); }
#define HALF_VERTICAL_EXTENT _HALF_VERTICAL_EXTENT()

inline float _HALF_HORIZONTAL_EXTENT() { return HALF_VERTICAL_EXTENT * ASPECT_RATIO; }
#define HALF_HORIZONTAL_EXTENT _HALF_HORIZONTAL_EXTENT()

// ---------- VECTOR ----------

struct Vector {
	float m_0;
	float m_1;
	float m_2;
};

inline Vector operator+(Vector v0, Vector v1) {
	return Vector{ v0.m_0 + v1.m_0, v0.m_1 + v1.m_1, v0.m_2 + v1.m_2 };
}

inline Vector operator-(Vector v0, Vector v1) {
	return Vector{ v0.m_0 - v1.m_0, v0.m_1 - v1.m_1, v0.m_2 - v1.m_2 };
}

inline Vector operator*(Vector v, float s) {
	return Vector{ v.m_0 * s, v.m_1 * s, v.m_2 * s };
}

inline Vector operator*(float s, Vector v) {
	return v * s;
}

inline Vector operator*(Vector v0, Vector v1) {
	return Vector{ v0.m_0 * v1.m_0, v0.m_1 * v1.m_1, v0.m_2 * v1.m_2 };
}

inline Vector operator/(Vector v, float s) {
	return Vector{ v.m_0 / s, v.m_1 / s, v.m_2 / s };
}

inline float Magnitude(Vector v) {
	return sqrt(v.m_0 * v.m_0 + v.m_1 * v.m_1 + v.m_2 * v.m_2);
}

inline Vector Normalise(Vector v) {
	float mag = Magnitude(v);
	return Vector{ v.m_0 / mag, v.m_1 / mag, v.m_2 / mag };
}

inline float Dot(Vector v0, Vector v1) {
	return v0.m_0 * v1.m_0 + v0.m_1 * v1.m_1 + v0.m_2 * v1.m_2;
}

inline uint32_t ColourToUint32(Vector v) {
	return 	( 	static_cast<uint32_t>(0xFF) 		<< 24 	) 	|
			( 	static_cast<uint32_t>(255 * v.m_0) 	<< 16 	) 	|
			( 	static_cast<uint32_t>(255 * v.m_1) 	<< 8 	)	|
				static_cast<uint32_t>(255 * v.m_2);
}

constant Vector COLOUR_INNER_ACCRETION_DISC = { 1.0f, 0.8f, 0.35f };
constant Vector COLOUR_OUTER_ACCRETION_DISC = { 1.0f, 0.94f, 0.82f };
constant Vector ACCRETION_DISC_COLOUR_DIFFERENCE = COLOUR_OUTER_ACCRETION_DISC - COLOUR_INNER_ACCRETION_DISC;

// ---------- Photon ----------

struct Photon {
	Vector m_position;
	Vector m_direction;
};

// ---------- Helper Functions ----------

Vector GenerateInitialDirection(uint i) {
	float wx = 2 * ( ( (i % WINDOW_W) + 0.5f ) / WINDOW_W ) - 1;
	float wy = 1 - 2 * ( (i / WINDOW_W + 0.5f ) / WINDOW_H );

	Vector dir = { wx * HALF_HORIZONTAL_EXTENT, wy * HALF_VERTICAL_EXTENT, 1.0 };
	return Normalise(dir);
}

uint32_t GetAccretionDiscColour(float r) {
	float outness = (r - ACCRETION_DISC_INNER_RADIUS) / (ACCRETION_DISC_OUTER_RADIUS - ACCRETION_DISC_INNER_RADIUS);
	return ColourToUint32(COLOUR_INNER_ACCRETION_DISC + outness * ACCRETION_DISC_COLOUR_DIFFERENCE);
}

Vector inline f(Vector position, Vector direction, float r, float curvature) {
	Vector r_hat = position / r;

	Vector e_r = r_hat - Dot(r_hat, direction) * direction;
	return -curvature * e_r;
}

Photon MarchPhoton(Photon photon) {
	float r = Magnitude(photon.m_position);
	float curvature = 3.0 / (r * r * r);
	float h = LAMBDA / (1.0 + STEP_K * curvature);

	Vector v1 = photon.m_direction;
	Vector x1 = photon.m_position;
	float r1 = Magnitude(x1);
	float s_r1 = 1.0f - (2.0f / r1);
	Vector k1_x = s_r1 * v1;
	Vector k1_v = f(x1, Normalise(v1), r, curvature);

	Vector x2 = x1 + 0.5f * h * k1_x;
	Vector v2 = Normalise(v1 + 0.5 * h * k1_v);
	float r2 = Magnitude(x2);
	float s_r2 = 1.0f - (2.0f / r2);
	Vector k2_x = s_r2 * v2;
	Vector k2_v = f(x2, Normalise(v2), r, curvature);

	Vector x3 = x1 + 0.5f * h * k2_x;
	Vector v3 = Normalise(v1 + 0.5 * h * k2_v);
	float r3 = Magnitude(x3);
	float s_r3 = 1.0f - (2.0f / r3);
	Vector k3_x = s_r3 * v3;
	Vector k3_v = f(x3, Normalise(v3), r, curvature);

	Vector x4 = x1 + h * k3_x;
	Vector v4 = Normalise(v1 + h * k3_v);
	float r4 = Magnitude(x4);
	float s_r4 = 1.0f - (2.0f / r4);
	Vector k4_x = s_r4 * v4;
	Vector k4_v = f(x4, Normalise(v4), r, curvature);
	
	photon.m_position = photon.m_position + (h / 6.0f) * (k1_x + 2*k2_x + 2*k3_x + k4_x);
	photon.m_direction = photon.m_direction + (h / 6.0f) * (k1_v + 2*k2_v + 2*k3_v + k4_v);
	photon.m_direction = Normalise(photon.m_direction);

	return photon;
}

// ---------- KERNEL ----------

kernel void TracePhoton(
    device 		uint* 		pixelBuffer 		[[ buffer(0) ]],
    			uint 		i 					[[ thread_position_in_grid ]]
) {
	Vector initialPosition{ 0.0f, 2.0f, -GRAPH_HALFRANGE };
	Vector initialDirection = GenerateInitialDirection(i);

	Photon photon = Photon{ initialPosition, initialDirection };

	uint MAX_STEPS = 5000;
	uint steps = 0;
	while (steps < MAX_STEPS) {
		Vector p = photon.m_position;
		float r = Magnitude(p);
		if (r <= 3.0f) {
			pixelBuffer[i] = 0xFF000000;
			return;
		}

		if (p.m_1 > -LAMBDA && p.m_1 < LAMBDA && r > ACCRETION_DISC_INNER_RADIUS && r < ACCRETION_DISC_OUTER_RADIUS) {
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

		photon = MarchPhoton(photon);
		//photon.m_position = p + photon.m_direction * LAMBDA;
		++steps;
	}

	pixelBuffer[i] = 0xFF000000;
}
