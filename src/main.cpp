#include <SDL2/SDL.h>
#include <iostream>

#include "Execution/GpuExecutor.h"
#include "View/Canvas.h"

#define FRAME_RATE_FREQUENCY 30

#define LOOK_SENSITIVITY 0.005f


struct MouseState {
	int m_x;
	int m_y;
	bool m_mouseDown;
};

void handle_mouse_motion(SDL_Event& event, MouseState& mouseState, GpuExecutor& executor, bool& moved) {
	int x;
	int y;

	SDL_GetMouseState(&x, &y);

	int dx = x - mouseState.m_x;
	int dy = mouseState.m_y - y;

	mouseState.m_x = x;
	mouseState.m_y = y;

	if (!mouseState.m_mouseDown) {
		moved = false;
		return;
	}

	executor.MoveHorizontally(dx * (-M_PI * LOOK_SENSITIVITY));
	executor.MoveVertically(dy * (-M_PI * LOOK_SENSITIVITY));
	moved = true;
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

	MouseState mouseState{ 0, 0, false};
	SDL_GetMouseState(&mouseState.m_x, &mouseState.m_y);

    bool running = true;
	bool moved = false;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

				case SDL_MOUSEMOTION:
					handle_mouse_motion(event, mouseState, executor, moved);
					break;
				
				case SDL_MOUSEBUTTONDOWN:
					std::cout << "MD\n";
					mouseState.m_mouseDown = true;
					break;
				
				case SDL_MOUSEBUTTONUP:
					std::cout << "MU\n";
					mouseState.m_mouseDown = false;

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
			if (fps < 200.0f) std::cout << "fps: " << fps << "\n";
            frame_tick = FRAME_RATE_FREQUENCY;
            lastFpsTime = currentTime;
		}

		if (moved) {
			RenderScene(canvas, buffer, executor);
			moved = false;
		}

		lastTime = currentTime;
    }
}

int main() {
    Canvas canvas;
	GpuExecutor executor;

    Mainloop(canvas, executor);

    return 0;
}
