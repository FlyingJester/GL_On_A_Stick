#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define WINDOW_TITLE "Threaded OpenGL Demo"
#define SDL2_VIDEO_FLAGS SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_INPUT_FOCUS|SDL_WINDOW_MOUSE_FOCUS

void **MakeWindow(int w, int h);
