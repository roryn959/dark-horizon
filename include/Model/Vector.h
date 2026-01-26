#pragma once

#include <math.h>
#include <iostream>

struct Vector {
	double m_0;
	double m_1;
	double m_2;
};

inline constexpr Vector operator+(const Vector& v0, const Vector& v1) {
	return Vector{ v0.m_0 + v1.m_0, v0.m_1 + v1.m_1, v0.m_2 + v1.m_2 };
}

inline constexpr Vector operator-(const Vector& v0, const Vector& v1) {
	return Vector{ v0.m_0 - v1.m_0, v0.m_1 - v1.m_1, v0.m_2 - v1.m_2 };
}

inline constexpr Vector operator*(const Vector& v, double s) {
	return Vector{ v.m_0 * s, v.m_1 * s, v.m_2 * s };
}

inline constexpr Vector operator*(double s, const Vector& v) {
	return v * s;
}

inline constexpr Vector operator*(const Vector& v0, const Vector& v1) {
	return Vector{ v0.m_0 * v1.m_0, v0.m_1 * v1.m_1, v0.m_2 * v1.m_2 };
}

inline constexpr Vector operator/(const Vector& v, double s) {
	return Vector{ v.m_0 / s, v.m_1 / s, v.m_2 / s };
}

inline constexpr double Magnitude(const Vector& v) {
	return sqrt(v.m_0 * v.m_0 + v.m_1 * v.m_1 + v.m_2 * v.m_2);
}

inline constexpr Vector Normalise(const Vector& v) {
	double mag = Magnitude(v);
	return Vector{ v.m_0 / mag, v.m_1 / mag, v.m_2 / mag };
}

inline constexpr double Dot(const Vector& v0, const Vector& v1) {
	return v0.m_0 * v1.m_0 + v0.m_1 * v1.m_1 + v0.m_2 * v1.m_2;
}

inline constexpr uint32_t ColourToUint32(const Vector& v) {
	return 	( 	static_cast<uint32_t>(0xFF) 		<< 24 	) 	|
			( 	static_cast<uint32_t>(255 * v.m_0) 	<< 16 	) 	|
			( 	static_cast<uint32_t>(255 * v.m_1) 	<< 8 	)	|
				static_cast<uint32_t>(255 * v.m_2);
}

inline std::ostream& operator<<(std::ostream& os, const Vector& v) {
	os << "(" << v.m_0 << ", " << v.m_1 << ", " << v.m_2 << ")";
	return os;
}