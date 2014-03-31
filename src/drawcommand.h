#pragma once


typedef struct {
  unsigned int Buffer;
} LineArg;

int QueueLine(void *arg);
int Line(void *arg);

int QueueFlipScreen(void *window);
int FlipScreen(void *window);
