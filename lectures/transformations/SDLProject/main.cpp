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
constexpr int   G_MAX_FRAME     = 40;

constexpr float G_ROT_ANGLE  = glm::radians(1.5f);
constexpr float G_TRAN_VALUE = 0.025f;

constexpr float BASE_SCALE = 1.0f,      // The unscaled size of your object
                MAX_AMPLITUDE = 0.01f,  // The most our triangle will be scaled up/down
                PULSE_SPEED = 10.0f;    // How fast you want your triangle to "beat"

AppStatus g_app_status = RUNNING;

SDL_Window* g_display_window;

bool g_is_growing   = true;
int g_frame_counter = 0;

ShaderProgram g_shader_program = ShaderProgram();;
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
    /** NEW STUFF HERE! **/
    g_frame_counter++;
    
    // STEP 1: Declare our transformation vectors
    glm::vec3 translation_vector;
    glm::vec3 scale_vector;
    glm::vec3 rotation_triggers;
    
    // STEP 2: Once we reach our max frame limit, we switch scale "direction"
    if (g_frame_counter >= G_MAX_FRAME)
    {
        g_is_growing = !g_is_growing;
        g_frame_counter = 0;
    }
    
    // STEP 3: Decide if the matrix will be scaled up or scaled down
    translation_vector = glm::vec3(G_TRAN_VALUE, G_TRAN_VALUE, 0.0f);
    rotation_triggers  = glm::vec3(0.0f, 0.0f, 1.0f);
    scale_vector       = glm::vec3(
                                   g_is_growing ? G_GROWTH_FACTOR : G_SHRINK_FACTOR,
                                   g_is_growing ? G_GROWTH_FACTOR : G_SHRINK_FACTOR,
                                   1.0f
                                   );
    
//    float scale_factor = BASE_SCALE + MAX_AMPLITUDE * glm::cos(g_frame_counter / PULSE_SPEED);
//    glm::vec3 scale_factors = glm::vec3(scale_factor, scale_factor, 0.0f);
    
    // STEP 4: Our transformations
    g_model_matrix = glm::translate(g_model_matrix, translation_vector);
    g_model_matrix = glm::rotate(g_model_matrix, G_ROT_ANGLE, rotation_triggers);
    g_model_matrix = glm::scale(g_model_matrix, scale_vector);
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


int main(int argc, char* argv[])
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
