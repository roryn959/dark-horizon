#include <SDL2/SDL.h>
#include <iostream>

#include "View/Canvas.h"

#define GPU_BUILD 0

#if GPU_BUILD
#include "Execution/GpuExecutor.h"
typedef GpuExecutor Executor;
#else
#include "Execution/CpuExecutor.h"
typedef CpuExecutor Executor;
#endif

#define FRAME_RATE_FREQUENCY 30


void RenderScene(Canvas& canvas, uint32_t* buffer, Executor& executor) {
	executor.TracePhotons(buffer);
	canvas.ApplyPixels(buffer);
}

void Mainloop(Canvas& canvas, Executor& executor) {
	uint32_t buffer[NUM_PIXELS];

	RenderScene(canvas, buffer, executor);

	float lastTime = SDL_GetTicks() / 1000.0f;
    float lastFpsTime = lastTime;
    int frame_tick = FRAME_RATE_FREQUENCY;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

				case SDL_KEYDOWN:
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
            //std::cout << "fps: " << fps << "\n";
            frame_tick = FRAME_RATE_FREQUENCY;
            lastFpsTime = currentTime;
		}

		//RenderScene(canvas, buffer, executor);

		lastTime = currentTime;
    }
}

int main() {
    Canvas canvas;
	Executor executor;

    Mainloop(canvas, executor);

    return 0;
}
