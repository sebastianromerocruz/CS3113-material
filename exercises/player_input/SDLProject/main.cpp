/**
* Author: Allan V
* CS 3113: User input exercise
* 26 Prairial, Year CCXXXI
* Tandon School of Engineering
**/
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define LOG(argument) std::cout << argument << '\n'
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include <cmath>

const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

const float BG_RED     = 0.9608f,
            BG_BLUE    = 0.9608f,
            BG_GREEN   = 0.9608f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X      = 0,
          VIEWPORT_Y      = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const char FLOWER_SPRITE[] = "sprites/flower.png";

const float ROT_SPEED = 100.0f;

const glm::vec3 FLOWER_INIT_POS = glm::vec3(0.0f, 0.0f, 0.0f),
                FLOWER_INIT_SCA = glm::vec3(1.5f, 1.5f, 0.0f);

const int NUMBER_OF_TEXTURES = 1;
const GLint LEVEL_OF_DETAIL  = 0,
            TEXTURE_BORDER   = 0;

const float MILLISECONDS_IN_SECOND = 1000.0;
const float DEGREES_PER_SECOND     = 90.0f;

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_flower_program;
GLuint        g_flower_texture_id;

glm::mat4 g_view_matrix,
          g_flower_model_matrix,
          g_projection_matrix;

float g_previous_ticks  = 0.0f;
float g_rot_angle = 0.0f;

// ———————————————— PART 1 ———————————————— //
glm::vec3 g_position;
glm::vec3 g_movement;
float speed = 2.0;

float x = 0.0;

// ———————————————— PART 1 ———————————————— //


GLuint load_texture(const char* filepath)
{
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
    
    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    
    return textureID;
}


void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("User input exercise",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT,
                                        SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    g_flower_program.Load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_flower_model_matrix = glm::mat4(1.0f);
    g_flower_model_matrix = glm::translate(g_flower_model_matrix, FLOWER_INIT_POS);
    g_flower_model_matrix = glm::scale(g_flower_model_matrix, FLOWER_INIT_SCA);
    
    g_flower_program.SetProjectionMatrix(g_projection_matrix);
    g_flower_program.SetViewMatrix(g_view_matrix);
    
    g_position = glm::vec3(0.0f, 0.0f, 0.0f);
    g_movement = glm::vec3(0.0f, 0.0f, 0.0f);
    
    glUseProgram(g_flower_program.programID);
    g_flower_texture_id = load_texture(FLOWER_SPRITE);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
}


void process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = !g_game_is_running;
                break;
                
        }
    }
    // ———————————————— PART 2 ———————————————— //
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
    if (key_state[SDL_SCANCODE_LEFT]) {
        g_movement.x = -1.0;
    }
    if (key_state[SDL_SCANCODE_RIGHT]) {
        g_movement.x = 1.0;
    }
    if (key_state[SDL_SCANCODE_UP]) {
        g_movement.y = 1.0;
    }
    if (key_state[SDL_SCANCODE_DOWN]) {
        g_movement.y = -1.0;
    }
    // ———————————————— PART 2 ———————————————— //
}


void update()
{
    /** ———— DELTA TIME CALCULATIONS ———— **/
    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    /** ———— RESETTING MODEL MATRIX ———— **/
    g_flower_model_matrix = glm::mat4(1.0f);
    g_flower_model_matrix = glm::scale(g_flower_model_matrix, FLOWER_INIT_SCA);
    g_flower_model_matrix = glm::translate(g_flower_model_matrix, FLOWER_INIT_POS);
    
    // ———————————————— PART 3 ———————————————— //
    g_position += g_movement * speed * delta_time;
    g_flower_model_matrix = glm::translate(g_flower_model_matrix, g_position);
    // ———————————————— PART 3 ———————————————— //
    
    /** ———— ROTATING SPRITE ———— **/
    g_rot_angle += ROT_SPEED * delta_time;
    g_flower_model_matrix = glm::rotate(g_flower_model_matrix, glm::radians(g_rot_angle), glm::vec3(0.0f, 1.0f, 0.0f));
}


void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float flower_vertices[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f
    };

    float flower_texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };
    
    glVertexAttribPointer(g_flower_program.positionAttribute, 2, GL_FLOAT, false, 0, flower_vertices);
    glEnableVertexAttribArray(g_flower_program.positionAttribute);
    
    glVertexAttribPointer(g_flower_program.texCoordAttribute, 2, GL_FLOAT, false, 0, flower_texture_coordinates);
    glEnableVertexAttribArray(g_flower_program.texCoordAttribute);
    
    g_flower_program.SetModelMatrix(g_flower_model_matrix);
    glBindTexture(GL_TEXTURE_2D, g_flower_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(g_flower_program.positionAttribute);
    glDisableVertexAttribArray(g_flower_program.texCoordAttribute);
    
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
