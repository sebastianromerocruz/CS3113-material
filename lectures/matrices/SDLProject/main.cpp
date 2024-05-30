/**
 * @file main.cpp
 * @author Sebastián Romero Cruz (sebastian.romerocruz@nyu.edu)
 * @brief A simple g_shader_program that creates a window with a blue background
 * Renders in a colored triangle that will be able to rotate along x or y-axis
 * and be able to scale
 * @date 2024-05-29
 *
 * @copyright NYU Tandon (c) 2024
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
enum ScaleDirection { GROWING, SHRINKING };

constexpr char V_SHADER_PATH[] = "shaders/vertex.glsl",
               F_SHADER_PATH[] = "shaders/fragment.glsl";

constexpr int WINDOW_WIDTH  = 640 * 2,
              WINDOW_HEIGHT = 480 * 2;

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

constexpr float GROWTH_FACTOR = 1.01f;
constexpr float SHRINK_FACTOR = 0.99f;
constexpr int   MAX_FRAME     = 40;

constexpr float BASE_SCALE = 1.0f,      // The unscaled size of your object
                MAX_AMPLITUDE = 0.01f,  // The most our triangle will be scaled up/down
                PULSE_SPEED = 10.0f;    // How fast you want your triangle to "beat"

constexpr float ROT_ANGLE  = glm::radians(1.5f);
constexpr float TRAN_VALUE = 0.025f;

AppStatus g_app_status = RUNNING;

SDL_Window* g_display_window;

ScaleDirection g_scale_direction = GROWING;
int g_frame_counter = 0;

ShaderProgram g_shader_program = ShaderProgram();
glm::mat4 g_view_matrix,
          g_model_matrix,
          g_projection_matrix;


void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Transformations!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

    if (g_display_window == nullptr)
    {
        std::cerr << "Error: SDL window could not be created.\n";
        SDL_Quit();
        exit(1);

    }

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    g_shader_program.set_colour(TRIANGLE_RED, TRIANGLE_BLUE, TRIANGLE_GREEN, TRIANGLE_OPACITY);

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
            g_app_status = TERMINATED;
        }
    }
}


void update()
{
    g_frame_counter++;
    
    /** TRANSLATE */
    glm::vec3 g_translation_factors = glm::vec3(TRAN_VALUE, 0.0f, 0.0f);
    g_model_matrix = glm::translate(g_model_matrix, g_translation_factors);
    
    /** ROTATE */
    glm::vec3 g_rotation_triggers = glm::vec3(0.0f, 0.0f, 1.0f);
    g_model_matrix = glm::rotate(g_model_matrix, ROT_ANGLE, g_rotation_triggers);
    
    /** SCALING **/
    float scale_factor = BASE_SCALE + MAX_AMPLITUDE * glm::cos(g_frame_counter / PULSE_SPEED);
    glm::vec3 scale_factors = glm::vec3(scale_factor, scale_factor, 0.0f);
    g_model_matrix = glm::scale(g_model_matrix, scale_factors);
    
//    if (g_frame_counter >= MAX_FRAME)
//    {
//        g_frame_counter = 0;
//        g_scale_direction = g_scale_direction == GROWING ?
//                            g_scale_direction = SHRINKING :
//                            g_scale_direction = GROWING;
//    }
//
//    glm::vec3 scale_factors = glm::vec3(
//                                        // x-direction
//                                        g_scale_direction == GROWING ?
//                                        GROWTH_FACTOR : SHRINK_FACTOR,
//
//                                        // y-direction
//                                        g_scale_direction == GROWING ?
//                                        GROWTH_FACTOR : SHRINK_FACTOR,
//
//                                        // z-direction
//                                        0.0f
//                                        );
//
//
//    g_model_matrix = glm::scale(g_model_matrix, scale_factors);
}


void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    g_shader_program.set_model_matrix(g_model_matrix);

    float vertices[] =
    {
         0.5f, -0.5f,
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


int main()
{
    initialise();

    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
