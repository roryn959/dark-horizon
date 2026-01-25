#include "Execution/CpuExecutor.h"


CpuExecutor::CpuExecutor() :
	m_photons{}
{
	// Vector topLeft{ -X_HALFRANGE, Y_HALFRANGE };
	// int n = 100;

	// double angle_start = 0.0;
	// double angle_end = -M_PI / 2.0;

	// for (int i = 0; i < n; ++i) {
	// 	double t = (n == 1) ? 0.0 : double(i) / (n - 1);
	// 	double angle = angle_start + t * (angle_end - angle_start);

	// 	Vector vel = {
	// 		cos(angle),
	// 		sin(angle)
	// 	};

	// 	m_photons.push_back(Photon{ topLeft, vel });
	// }

	for (double i = 2.95; i < 3.05; i += 0.001) {
		m_photons.push_back(Photon{ { 0.0, i }, {1.0, 0.0} });
	}

}

CpuExecutor::~CpuExecutor() {
}

void CpuExecutor::TracePhotons(uint32_t* pixelBuffer) {
	for (int i = 0; i < NUM_PIXELS; ++i) {
		TracePhoton(pixelBuffer, i);
	}

	for (Photon& photon : m_photons) {
		ProcessTimeStep(photon);

		Vector cartesianPosition = photon.m_position;
		int wx = graphToWindowX( cartesianPosition.m_0 );
		int wy = graphToWindowY( cartesianPosition.m_1 ); 

		if (wx >= WINDOW_W || wy > WINDOW_H) {
			continue;
		}

		int i = wy * WINDOW_W + wx;

		if (i >= NUM_PIXELS) continue;
		pixelBuffer[i] = 0xFFFFFFFF;
	}
}

void CpuExecutor::ProcessTimeStep(Photon& photon) {

	Vector positionPolar = CartesianToPolar(photon.m_position);
	double r = positionPolar.m_0;
	double theta = positionPolar.m_1;

	if (r <= 2.0) return;

	auto f = [](const Vector& position, const Vector& direction) -> Vector {
		double r = Magnitude(position);
		Vector r_hat = position / r;

		Vector e_r = r_hat - Dot(r_hat, direction) * direction;
		return (-3.0 / (r * r * r)) * e_r;
	};

	Vector v1 = photon.m_direction;
	Vector x1 = photon.m_position;
	double r1 = Magnitude(x1);
	double s_r1 = 1.0 - (2.0 / r1);
	Vector k1_x = s_r1 * v1;
	Vector k1_v = f(x1, Normalise(v1));

	Vector x2 = x1 + 0.5 * LAMBDA * k1_x;
	Vector v2 = Normalise(v1 + 0.5 * LAMBDA * k1_v);
	double r2 = Magnitude(x2);
	double s_r2 = 1.0 - (2.0 / r2);
	Vector k2_x = s_r2 * v2;
	Vector k2_v = f(x2, Normalise(v2));

	Vector x3 = x1 + 0.5 * LAMBDA * k2_x;
	Vector v3 = Normalise(v1 + 0.5 * LAMBDA * k2_v);
	double r3 = Magnitude(x3);
	double s_r3 = 1.0 - (2.0 / r3);
	Vector k3_x = s_r3 * v3;
	Vector k3_v = f(x3, Normalise(v3));

	Vector x4 = x1 + LAMBDA * k3_x;
	Vector v4 = Normalise(v1 + LAMBDA * k3_v);
	double r4 = Magnitude(x4);
	double s_r4 = 1.0 - (2.0 / r4);
	Vector k4_x = s_r4 * v4;
	Vector k4_v = f(x4, Normalise(v4));
	
	photon.m_position = photon.m_position + (LAMBDA / 6.0) * (k1_x + 2*k2_x + 2*k3_x + k4_x);
	photon.m_direction = photon.m_direction + (LAMBDA / 6.0) * (k1_v + 2*k2_v + 2*k3_v + k4_v);
	photon.m_direction = Normalise(photon.m_direction);
}

void CpuExecutor::TracePhoton(uint32_t* pixelBuffer, size_t i) {
	int wx = i % WINDOW_W;
	int wy = i / WINDOW_W;

	double gx = windowToGraphX(wx);
	double gy = windowToGraphY(wy);

	double r = sqrt( gx * gx + gy * gy );

	if (r <= 2.0f) {
		pixelBuffer[i] = 0xFF000000;
		return;
	}

	double theta = atan2(gy, gx);

	double A_r = A(r);

	double s_r = sqrt(A_r);
	double s_theta = r;

	int red = round(255 * (Squash(s_r)));
	int blue = 0;//round(255 * (Squash(s_theta)));

	pixelBuffer[i] = (255 << 24) | (red << 16) | (0 << 8) | (blue);
}