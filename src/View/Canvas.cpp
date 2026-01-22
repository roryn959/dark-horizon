#include "View/Canvas.h"

Canvas::Canvas() :
    m_pWindow{},
    m_pRenderer{},
	m_pTexture{}
{
    m_pWindow = SDL_CreateWindow("Black Hole Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
	m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_W, WINDOW_H);
	SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
}

void Canvas::ApplyPixels(uint32_t* pixelBuffer) {
	SDL_UpdateTexture(m_pTexture, nullptr, pixelBuffer, WINDOW_W * sizeof(uint32_t));
    SDL_RenderCopy(m_pRenderer, m_pTexture, nullptr, nullptr);
    SDL_RenderPresent(m_pRenderer);
}

Canvas::~Canvas() {
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();
}