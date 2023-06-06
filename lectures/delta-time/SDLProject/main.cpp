#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define LOG(argument) std::cout << argument << '\n'

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

const float BG_RED     = 0.1922f,
            BG_BLUE    = 0.549f,
            BG_GREEN   = 0.9059f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X      = 0,
          VIEWPORT_Y      = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex.glsl",
           F_SHADER_PATH[] = "shaders/fragment.glsl";

const int TRIANGLE_RED     = 1.0,
          TRIANGLE_BLUE    = 0.4,
          TRIANGLE_GREEN   = 0.4,
          TRIANGLE_OPACITY = 1.0;

const float MILLISECONDS_IN_SECOND = 1000.0;
const float DEGREES_PER_SECOND     = 90.0f;

SDL_Window* g_display_window;

bool g_game_is_running = true;
bool g_is_growing      = true;

ShaderProgram g_program;
glm::mat4 g_view_matrix,
          g_model_matrix,
          g_projection_matrix,
          g_trans_matrix;

float g_triangle_x      = 0.0f;
float g_triangle_rotate = 0.0f;
float g_previous_ticks  = 0.0f;

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Hello, Delta Time!",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_program.Load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);  // Defines the position (location and orientation) of the camera
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    g_trans_matrix = g_model_matrix;
    
    g_program.SetProjectionMatrix(g_projection_matrix);
    g_program.SetViewMatrix(g_view_matrix);
    // Notice we haven't set our model matrix yet!
    
    g_program.SetColor(TRIANGLE_RED, TRIANGLE_BLUE, TRIANGLE_GREEN, TRIANGLE_OPACITY);
    
    glUseProgram(g_program.programID);
    
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
    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks - g_previous_ticks; // the delta time is the difference from the last frame
    g_previous_ticks = ticks;

    g_triangle_x += 1.0f * delta_time;
    g_triangle_rotate += DEGREES_PER_SECOND * delta_time; // 90-degrees per second
    g_model_matrix = glm::mat4(1.0f);

    /* Translate -> Rotate */
    g_model_matrix = glm::translate(g_model_matrix, glm::vec3(g_triangle_x, 0.0f, 0.0f));
    g_model_matrix = glm::rotate(g_model_matrix, glm::radians(g_triangle_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    g_program.SetModelMatrix(g_model_matrix);
    
    float vertices[] =
    {
         0.5f, -0.5f,
         0.0f,  0.5f,
        -0.5f, -0.5f
    };
    
    glVertexAttribPointer(g_program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(g_program.positionAttribute);
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }

/**
 Start here—we can see the general structure of a game loop without worrying too much about the details yet.
 */
int main(int argc, char* argv[])
{
    initialise();
    
    while (g_game_is_running)
    {
        process_input();
        update();
        render();
    }
    
    shutdown();
    return 0;
}

