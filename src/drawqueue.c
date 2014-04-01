#include "drawqueue.h"
#include "glextra/glExtra.h"
#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_opengl.h>

/////
// Before and after the mutex queueMutex is created, the main thread is the owner of all ListItems.
//
// Otherwise, you must hold the queueMutex to touch them!
//

static SDL_atomic_t render_frame;
static SDL_atomic_t engine_frame;

SDL_GLContext threadglcontext;

typedef struct {

  int(*func)(void *);
  void *data;

  void *next; //Moves toward LastItem
  void *prev; //Moves toward FirstItem
} ListItem;

/////
// First is enqueue, Last is dequeue.
// NULL signifies an empty list.
//

ListItem *FirstItem;
ListItem *LastItem;

/////
// All modifications to First or Last must ensure that if one is set to NULL, the other is as well.
//

SDL_mutex *queueMutex = NULL;
SDL_Thread* renderThread = NULL;
SDL_atomic_t ThreadDie;

int RenderThread(void *data){

  int width, height;
  SDL_GLContext *context = ((void **)data)[0];
  SDL_Window    *window  = ((void **)data)[1];


  /////
  // This demo is designed to work even on Intel GMA cards.
  //
  // In fact, it was written on a machine with one.
  // The horror...the horror...
  //

  if(SDL_GL_MakeCurrent(window, *((SDL_GLContext*)context))!=0)
    SDL_ShowSimpleMessageBox(0, "Error", SDL_GetError(), NULL);

  LoadGLFunctions();

  SDL_GetWindowSize(window, &width, &height);
  /////
  // Allow direct use of raster coordinates.
  //
  /*
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (double)width, (double)height, 0.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
*/
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  SDL_GL_SwapWindow(window);

  while(!SDL_AtomicGet(&ThreadDie)){
    SDL_LockMutex(queueMutex);

    assert(((LastItem==NULL)&&(FirstItem==NULL))||((LastItem!=NULL)&&(FirstItem!=NULL)));

    if(LastItem==NULL){
      SDL_UnlockMutex(queueMutex);
      continue;
    }

    LastItem->func(LastItem->data);

    if((LastItem->next==NULL)&&(LastItem->prev==NULL)){
      assert(LastItem==FirstItem);
      free(LastItem);
      LastItem = NULL;
      FirstItem = NULL;


    }
    else{
      ListItem *OldLast = LastItem;
      LastItem = OldLast->prev;

      assert(LastItem->next==OldLast); //Ensure the list is linked properly.

      LastItem->next=NULL; //This is the final item.

      free(OldLast);
    }

    SDL_UnlockMutex(queueMutex);

  }
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  SDL_AtomicSet(&ThreadDie, 0);
  SDL_GL_DeleteContext(*((SDL_GLContext*)context));
  free(data);
  return 0;
}

void InitDrawQueue(void){

  ResetRenderFrame();
  ResetEngineFrame();

  SDL_AtomicSet(&ThreadDie, 0);
  FirstItem = NULL;
  LastItem = NULL;
  queueMutex = SDL_CreateMutex();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  {
    SDL_GLContext oldcontext = SDL_GL_GetCurrentContext();
    if(!oldcontext)
      SDL_ShowSimpleMessageBox(0, "Error", SDL_GetError(), NULL);

    threadglcontext = SDL_GL_CreateContext(SDL_GL_GetCurrentWindow());

    {
      int ContextIsShared = 0;
      int AttribErr = SDL_GL_GetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT,&ContextIsShared);
      assert(AttribErr == 0);
      assert(ContextIsShared);
    }


    SDL_GL_MakeCurrent(SDL_GL_GetCurrentWindow(), oldcontext);
  }
  //SDL_ShowSimpleMessageBox(0, "Error", SDL_GetError(), NULL);
  {
    void ** data = malloc(sizeof(void *)<<1);
    data[0] = &threadglcontext;
    data[1] = SDL_GL_GetCurrentWindow();
    renderThread = SDL_CreateThread(RenderThread, "GLRenderer", data);

  }
}

void CloseDrawQueue(void){
  SDL_AtomicSet(&ThreadDie, 1);
  while(SDL_AtomicGet(&ThreadDie)){
    SDL_Delay(0x10); //Real idle.
  }

  SDL_DestroyMutex(queueMutex);
}

void PushFunction(int(*func)(void *), void *data){

  ListItem *newFirst = malloc(sizeof(ListItem));

  newFirst->prev = NULL;

  newFirst->func = func;
  newFirst->data = data;

  SDL_LockMutex(queueMutex);

  assert(  ((FirstItem == NULL)&&(LastItem == NULL)) || ((FirstItem != NULL)&&(LastItem != NULL)) );

  // If we have no items in the queue.
  if((LastItem==NULL)||(FirstItem==NULL)){
    //Verify a valid state.
    assert((FirstItem==NULL)&&(LastItem==NULL));

    //Set up the queue as a single item.

    FirstItem = newFirst;
    LastItem = newFirst;

    newFirst->next = NULL;
    newFirst->prev = NULL;
  }
  // If we have one item in the queue.
  else if(LastItem==FirstItem){
    //Verify a valid state.
    assert((LastItem->prev==NULL)&&(LastItem->next==NULL));

    //Set up the queue as two items.

    //prev is enqueue.
    LastItem->prev = newFirst;
    newFirst->next = LastItem;
    newFirst->prev = NULL;
    FirstItem = newFirst;
  }
  // Set things up for full queue operation.
  else{
    FirstItem->prev = newFirst;
    newFirst->next = FirstItem;
    newFirst->prev = NULL;
    FirstItem = newFirst;
  }

  SDL_UnlockMutex(queueMutex);
}

/////
// Frame counters.
//

void IncRenderFrame(void){
  SDL_AtomicIncRef(&render_frame);
}

void IncEngineFrame(void){
  SDL_AtomicIncRef(&engine_frame);
}

int GetRenderFrame(void){
  return SDL_AtomicGet(&render_frame);
}

int GetEngineFrame(void){
  return SDL_AtomicGet(&engine_frame);
}

void ResetRenderFrame(void){
  SDL_AtomicSet(&render_frame, 0);
}

void ResetEngineFrame(void){
  SDL_AtomicSet(&engine_frame, 0);
}
