#pragma once

#include <math.h>
#include <iostream>

struct Vector {
	double m_0;
	double m_1;
};

inline Vector operator+(const Vector& v0, const Vector& v1) {
	return Vector{ v0.m_0 + v1.m_0, v0.m_1 + v1.m_1 };
}

inline Vector operator-(const Vector& v0, const Vector& v1) {
	return Vector{ v0.m_0 - v1.m_0, v0.m_1 - v1.m_1 };
}

inline Vector operator*(const Vector& v0, double s) {
	return Vector{ v0.m_0 * s, v0.m_1 * s };
}

inline Vector operator*(double s, const Vector& v0) {
	return Vector{ v0.m_0 * s, v0.m_1 * s };
}

inline Vector operator*(const Vector& v0, const Vector& v1) {
	return Vector{ v0.m_0 * v1.m_0, v0.m_1 * v1.m_1 };
}

inline Vector operator/(const Vector& v0, double s) {
	return Vector{ v0.m_0 / s, v0.m_1 / s };
}

inline double Magnitude(const Vector& v) {
	return sqrt(v.m_0 * v.m_0 + v.m_1 * v.m_1);
}

inline Vector Normalise(const Vector& v) {
	double mag = sqrt(v.m_0 * v.m_0 + v.m_1 * v.m_1);
	return Vector{ v.m_0 / mag, v.m_1 / mag };
}

inline double Dot(const Vector& v0, const Vector& v1) {
	return v0.m_0 * v1.m_0 + v0.m_1 * v1.m_1;
}

inline Vector PolarToCartesian(const Vector& v) {
	return Vector{ v.m_0 * cos(v.m_1), v.m_0 * sin(v.m_1) };
}

inline Vector CartesianToPolar(const Vector& v) {
	return Vector{ sqrt( v.m_0 * v.m_0 + v.m_1 * v.m_1 ), atan2(v.m_1, v.m_0) };
}

inline std::ostream& operator<<(std::ostream& os, const Vector& v) {
	os << "(" << v.m_0 << ", " << v.m_1 << ")";
	return os;
}