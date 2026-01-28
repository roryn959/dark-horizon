#include <SDL2/SDL.h>
#include <iostream>

#include "Execution/GpuExecutor.h"
#include "View/Canvas.h"

#define FRAME_RATE_FREQUENCY 30

#define LOOK_SENSITIVITY 0.005f


struct MousePosition {
	int m_x;
	int m_y;
};

void handle_mouse_motion(SDL_Event& event, MousePosition& mp, GpuExecutor& executor) {
	int x;
	int y;

	SDL_GetMouseState(&x, &y);

	int dx = x - mp.m_x;
	int dy = mp.m_y - y;

	executor.MoveHorizontally(dx * (-M_PI * LOOK_SENSITIVITY));
	executor.MoveVertically(dy * (-M_PI * LOOK_SENSITIVITY));

	mp.m_x = x;
	mp.m_y = y;
}

void RenderScene(Canvas& canvas, uint32_t* buffer, GpuExecutor& executor) {
	executor.TracePhotons(buffer);
	canvas.ApplyPixels(buffer);
}

void Mainloop(Canvas& canvas, GpuExecutor& executor) {
	uint32_t buffer[NUM_PIXELS];

	RenderScene(canvas, buffer, executor);

	float lastTime = SDL_GetTicks() / 1000.0f;
    float lastFpsTime = lastTime;
    int frame_tick = FRAME_RATE_FREQUENCY;

	MousePosition mousePosition;
	SDL_GetMouseState(&mousePosition.m_x, &mousePosition.m_y);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

				case SDL_MOUSEMOTION:
					handle_mouse_motion(event, mousePosition, executor);
					break;

                default:
                    break;
            }
        }

		// Time-based processing
        float currentTime = SDL_GetTicks() / 1000.0f;
        float dt = currentTime - lastTime;

		// Calculate fps
        --frame_tick;
        if (0 == frame_tick) {
            float fps = FRAME_RATE_FREQUENCY / (currentTime - lastFpsTime);
            std::cout << "fps: " << fps << "\n";
            frame_tick = FRAME_RATE_FREQUENCY;
            lastFpsTime = currentTime;
		}

		RenderScene(canvas, buffer, executor);

		lastTime = currentTime;
    }
}

int main() {
    Canvas canvas;
	GpuExecutor executor;

    Mainloop(canvas, executor);

    return 0;
}
