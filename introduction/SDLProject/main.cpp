#include <iostream>

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>

// Some constants necessary to get started
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// And some variables as well
SDL_Window* display_window;
bool game_is_running = true;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);  // Initialising
    
    display_window = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    
    /**
     Create an OpenGL context for an OpenGL window, and make it current.
     
     A context stores all of the state associated with this instance of OpenGL. Think of a context as an object that holds all of OpenGL; when
     a context is destroyed, OpenGL is destroyed.
     */
    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    
    /**
     Set up an OpenGL context for rendering into an OpenGL window.
     */
    SDL_GL_MakeCurrent(display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    /**
     glClearColor specifies the red, green, blue, and alpha values used by glClear to clear the color buffers. In other words, we are making our
     screen the colour RBG (0, 0, 0) with an opacity of 1.0 (a solid colour).
     
     Values specified by glClearColor are clamped to the range 0 to 1.
     */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    
    SDL_Event event; // The SDL_Event type is a union that contains structures for the different event types, like quitting or closing
    while (game_is_running) {
        /**
         Our games will go here, eventually!
         */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                game_is_running = false;
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT); // Quite simply: clear the space in memory holding our colours
        SDL_GL_SwapWindow(display_window); // Update a window with whatever OpenGL is rendering
    }
    
    SDL_Quit();
    return 0;
}
