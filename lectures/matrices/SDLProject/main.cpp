/**
 * @file main.cpp
 * @author Sebastián Romero Cruz (src402@nyu.edu) and Wesley Zhuang (wz2445@nyu.edu)
 * @brief A simple g_shader_program that creates a window with a blue background
 * Renders in a colored triangle that will be able to infinitely scale. This is used to show 
 * the use of matrices 
 * @date 2024-05-27
 *
 * @copyright NYU Tandon (c) 2024
 */

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
    #include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"                
#include "glm/gtc/matrix_transform.hpp"  
#include "ShaderProgram.h" 

constexpr int WINDOW_WIDTH = 640,
              WINDOW_HEIGHT = 480;

constexpr float BG_RED = 0.1922f,
            BG_BLUE = 0.549f,
            BG_GREEN = 0.9059f,
            BG_OPACITY = 1.0f;

constexpr int VIEWPORT_X = 0,
              VIEWPORT_Y = 0,
              VIEWPORT_WIDTH = WINDOW_WIDTH,
              VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_SHADER_PATH[] = "shaders/vertex.glsl",
               F_SHADER_PATH[] = "shaders/fragment.glsl";

constexpr int TRIANGLE_RED = 1.0,
              TRIANGLE_BLUE = 0.4,
              TRIANGLE_GREEN = 0.4,
              TRIANGLE_OPACITY = 1.0;

constexpr float GROWTH_FACTOR = 1.01f;
constexpr float SHRINK_FACTOR = 0.99f;
constexpr int MAX_FRAME = 40;

int  g_frame_counter = 0;
bool g_is_growing = true;

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_shader_program;
glm::mat4 view_matrix, model_matrix, projection_matrix;

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Triangle!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    if (g_display_window == nullptr)
    {
        std::cerr << "Error: SDL window could not be created.\n";
        SDL_Quit();
        exit(1);
        
    }
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    view_matrix = glm::mat4(1.0f);  // Defines the position (location and orientation) of the camera
    model_matrix = glm::mat4(1.0f);  // Defines every translation, rotations, or scaling applied to an object
    projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    g_shader_program.set_projection_matrix(projection_matrix);
    g_shader_program.set_view_matrix(view_matrix);
    // Notice we haven't set our model matrix yet!
    
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
            g_game_is_running = false;
        }
    }
}

void update()
{
    // This scale vector will make the x- and y-coordinates of the triangle
    // grow by a factor of 1% of it of its original size every frame.
    float scale_factor = 1.01;
    glm::vec3 scale_vector = glm::vec3(scale_factor, scale_factor, 1.0f);
    
    // We replace the previous value of the model matrix with the scaled
    // value of model matrix. This would mean that  glm::scale() returns
    // a matrix, which it does!
    model_matrix = glm::scale(model_matrix, scale_vector);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    g_shader_program.set_model_matrix(model_matrix);
    
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
