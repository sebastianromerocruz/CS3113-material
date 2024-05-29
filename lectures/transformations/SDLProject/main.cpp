/**
 * @file main.cpp
 * @author Sebastián Romero Cruz (sebastian.romerocruz@nyu.edu)
 * @brief A simple g_shader_program that creates a window with a blue background
 * Renders in a colored triangle that will be able to rotate along x or y-axis 
 * and be able to scale
 * @date 2024-05-26
 * 
 * @copyright Copyright NYU Tandon (c) 2024
 */


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

enum AppStatus { RUNNING, TERMINATED };

constexpr char V_SHADER_PATH[] = "shaders/vertex.glsl",
           F_SHADER_PATH[] = "shaders/fragment.glsl";

constexpr int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

constexpr float BG_RED     = 0.1922f,
            BG_BLUE    = 0.549f,
            BG_GREEN   = 0.9059f,
            BG_OPACITY = 1.0f;

constexpr int VIEWPORT_X      = 0,
          VIEWPORT_Y      = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr int TRIANGLE_RED     = 1.0,
          TRIANGLE_BLUE    = 0.4,
          TRIANGLE_GREEN   = 0.4,
          TRIANGLE_OPACITY = 1.0;

constexpr float G_GROWTH_FACTOR = 1.01f;
constexpr float G_SHRINK_FACTOR = 0.99f;
constexpr int G_MAX_FRAME       = 40;

constexpr float G_ROT_ANGLE  = glm::radians(1.5f);
constexpr float G_TRAN_VALUE = 0.025f;

AppStatus g_app_status = RUNNING;

SDL_Window* g_display_window;

bool g_game_is_running = true;
bool g_is_growing      = true;
int g_frame_counter    = 0;

ShaderProgram g_shader_program = ShaderProgram();;
glm::mat4 g_view_matrix,
          g_model_matrix,
          g_projection_matrix,
          g_tran_matrix;

void print_matrix(glm::mat4 &matrix, int size);

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Triangle!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);  // Defines the position (location and orientation) of the camera
    g_model_matrix = glm::mat4(1.0f);  // Defines every translation, rotations, or scaling applied to an object
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    g_tran_matrix = g_model_matrix;
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    // Notice we haven't set our model matrix yet!
    
    g_shader_program.set_colour(TRIANGLE_RED, TRIANGLE_BLUE, TRIANGLE_GREEN, TRIANGLE_OPACITY);
    
    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
}

void process_input()
{
    SDL_Event event;
    while (g_app_status == RUNNING)
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            g_app_status = TERMINATED;
        }
    }
}

void update()
{
//    LOG(++frame_counter);
    g_frame_counter++;
    
    // Step 1
    glm::vec3 scale_vector;
    
    // Step 2
    if (g_frame_counter >= G_MAX_FRAME)
    {
        g_is_growing = !g_is_growing;
        g_frame_counter = 0;
    }
    
    // Step 3
    scale_vector = glm::vec3(g_is_growing ? G_GROWTH_FACTOR : G_SHRINK_FACTOR,
                             g_is_growing ? G_GROWTH_FACTOR : G_SHRINK_FACTOR,
                             1.0f);
    
    // Step 4
    g_model_matrix = glm::translate(g_model_matrix, glm::vec3(G_TRAN_VALUE, G_TRAN_VALUE, 0.0f));
    g_model_matrix = glm::scale(g_model_matrix, scale_vector);
    g_model_matrix = glm::rotate(g_model_matrix, G_ROT_ANGLE, glm::vec3(0.0f, 0.0f, 1.0f));
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    g_shader_program.set_model_matrix(g_model_matrix);
    
    float vertices[] =
    {
        0.5f, -0.5f,
        0.0f, 0.5f,
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
