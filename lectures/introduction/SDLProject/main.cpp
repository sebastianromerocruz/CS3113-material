// Getting all our libraries in
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

const float BG_RED     = 0.20f,
            BG_BLUE    = 0.90f,
            BG_GREEN   = 0.59f,
            BG_OPACITY = 1.0f;

SDL_Window* g_display_window;
bool g_game_is_running = true;

int main()
{
    // Initialising the graphics engine
    SDL_Init(SDL_INIT_VIDEO);
    
    g_display_window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    
    /**
     Create an opengl context for your window, and make it the CURRENT context
     
     A context basically stores all of the states of a game.
     */
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
    
    SDL_Event event;
    while (g_game_is_running)
    {
        /**
         Our games will eventually go here.
         */
        
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            {
                g_game_is_running = false;
            }
        }
        
        // These two, for now, should be the last in your game loop
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(g_display_window);
        
        
    }
    
    SDL_Quit();
    return 0;
}
