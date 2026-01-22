#pragma once

#include <SDL2/SDL.h>

#define WINDOW_W 820
#define WINDOW_H 820

constexpr int NUM_PIXELS = WINDOW_W * WINDOW_H;

class Canvas {
public:
    Canvas();
    ~Canvas();

    inline SDL_Window* GetWindow() { return m_pWindow; };
    inline SDL_Renderer* GetRenderer() { return m_pRenderer; };

	void ApplyPixels(uint32_t* pixelBuffer);

private:
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pTexture;
};