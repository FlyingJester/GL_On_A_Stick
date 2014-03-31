#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>

/////
// This is ugly on the inside. It's just a proof of concept.
//

void IncRenderFrame(void);
void IncEngineFrame(void);

int GetRenderFrame(void);
int GetEngineFrame(void);

void ResetRenderFrame(void);
void ResetEngineFrame(void);

void InitDrawQueue(void);
void CloseDrawQueue(void);

extern SDL_mutex *queueMutex;

void PushFunction(int(*func)(void *), void *);
