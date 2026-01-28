#pragma once

#include <math.h>
#include <iostream>

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

inline Vector Cross(Vector v0, Vector v1) {
	return Vector {
		v0.m_1 * v1.m_2 - v0.m_2 * v1.m_1,
		v0.m_2 * v1.m_0 - v0.m_0 * v1.m_2,
		v0.m_0 * v1.m_1 - v0.m_1 * v1.m_0
	 };
}

inline float Distance(Vector v0, Vector v1) {
	float dx = v0.m_0 - v1.m_0;
	float dy = v0.m_1 - v1.m_1;
	float dz = v0.m_2 - v1.m_2;

	return sqrt(dx * dx + dy * dy + dz * dz);
}

inline uint32_t ColourToUint32(Vector v) {
	return 	( 	static_cast<uint32_t>(0xFF) 		<< 24 	) 	|
			( 	static_cast<uint32_t>(255 * v.m_0) 	<< 16 	) 	|
			( 	static_cast<uint32_t>(255 * v.m_1) 	<< 8 	)	|
				static_cast<uint32_t>(255 * v.m_2);
}

inline std::ostream& operator<<(std::ostream& os, const Vector& v) {
	os << "(" << v.m_0 << ", " << v.m_1 << ", " << v.m_2 << ")";
	return os;
}