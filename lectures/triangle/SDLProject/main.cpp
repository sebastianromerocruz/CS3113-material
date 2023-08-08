#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"                // 4x4 Matrix
#include "glm/gtc/matrix_transform.hpp"  // Matrix transformation methods
#include "ShaderProgram.h"               // We'll talk about these later in the course

// Our window dimensions
const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

// Heartbeat stuff
const float GROWTH_FACTOR = 1.01f;  // grow by 1.0% / frame
const float SHRINK_FACTOR = 0.99f;  // grow by -1.0% / frame
const int MAX_FRAMES = 40;

// Rotation stuff
const float ROT_ANGLE = glm::radians(1.0f);

// Translation stuff
const float TRAN_VALUE = 0.025f;

int g_frame_counter = 0;
bool g_is_growing = true;

// Background color components
const float BG_RED     = 0.1922f,
            BG_BLUE    = 0.549f,
            BG_GREEN   = 0.9059f,
            BG_OPACITY = 1.0f;

// Our viewport—or our "camera"'s—position and dimensions
const int VIEWPORT_X      = 0,
          VIEWPORT_Y      = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

// Our shader filepaths; these are necessary for a number of things
// Not least, to actually draw our shapes 
// We'll have a whole lecture on these later
const char V_SHADER_PATH[] = "shaders/vertex.glsl",
           F_SHADER_PATH[] = "shaders/fragment.glsl";

// Our object's fill colour
const float TRIANGLE_RED     = 1.0,
            TRIANGLE_BLUE    = 0.4,
            TRIANGLE_GREEN   = 0.4,
            TRIANGLE_OPACITY = 1.0;

bool g_game_is_running = true;
SDL_Window* g_display_window;

ShaderProgram g_shader_program;

glm::mat4 g_view_matrix,        // Defines the position (location and orientation) of the camera
          g_model_matrix,       // Defines every translation, rotation, and/or scaling applied to an object; we'll look at these next week
          g_projection_matrix;  // Defines the characteristics of your camera, such as clip panes, field of view, projection method, etc.

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Hello, Triangle!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    // Initialise our camera
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    // Load up our shaders
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    // Initialise our view, model, and projection matrices
    g_view_matrix       = glm::mat4(1.0f);  // Defines the position (location and orientation) of the camera
    g_model_matrix      = glm::mat4(1.0f);  // Defines every translation, rotations, or scaling applied to an object
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
//    g_model_matrix = glm::translate(g_model_matrix, glm::vec3(5.0f, 0.0f, 0.0f));
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    // Notice we haven't set our model matrix yet!
    
    g_shader_program.set_colour(TRIANGLE_RED, TRIANGLE_BLUE, TRIANGLE_GREEN, TRIANGLE_OPACITY);
    
    // Each object has its own unique ID
    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
}

void process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_game_is_running = false;
        }
    }
}

void update()
{
    glm::vec3 scale_vector;
    g_frame_counter += 1;
    
    if (g_frame_counter >= MAX_FRAMES)
    {
        g_is_growing = !g_is_growing;
        g_frame_counter = 0;
    }
    
    scale_vector = glm::vec3(g_is_growing ? GROWTH_FACTOR : SHRINK_FACTOR,
                             g_is_growing ? GROWTH_FACTOR : SHRINK_FACTOR,
                             1.0f);
    
    g_model_matrix = glm::translate(g_model_matrix, glm::vec3(TRAN_VALUE, 0.0f, 0.0f));
    g_model_matrix = glm::rotate(g_model_matrix, ROT_ANGLE, glm::vec3(0.0f, 1.0f, 0.0f));
    g_model_matrix = glm::scale(g_model_matrix, scale_vector);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    g_shader_program.set_model_matrix(g_model_matrix);
    
    float vertices[] =
    {
         1.0f, -0.5f,
         0.0f,  0.5f,
        -0.5f, -0.5f
    };
    
    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }

/**
 Start here—we can see the general structure of a game loop without worrying too much about the details yet.
 */
int main(int argc, char* argv[])
{
    // Initialise our program—whatever that means
    initialise();
    
    while (g_game_is_running)
    {
        process_input();  // If the player did anything—press a button, move the joystick—process it
        update();         // Using the game's previous state, and whatever new input we have, update the game's state
        render();         // Once updated, render those changes onto the screen
    }
    
    shutdown();  // The game is over, so let's perform any shutdown protocols
    return 0;
}
