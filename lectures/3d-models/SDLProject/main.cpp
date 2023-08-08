#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP      0.0166666f
#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720
#define NEAR_PLANE_DIST     0.1f
#define FAR_PLANE_DIST      100.0F
#define FOV_ANGLE           45.0f
#define CRATE_COUNT         1
#define ENEMY_COUNT         1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"

float CUBE_VERTICES[] =
{
    // Face 1
    -0.5,  0.5, -0.5, -0.5,  0.5,  0.5,  0.5,  0.5,  0.5,
    -0.5,  0.5, -0.5,  0.5,  0.5,  0.5,  0.5,  0.5, -0.5,

    // Face 2
     0.5, -0.5, -0.5,  0.5, -0.5,  0.5, -0.5, -0.5,  0.5,
     0.5, -0.5, -0.5, -0.5, -0.5,  0.5, -0.5, -0.5, -0.5,

    // Face 3
    -0.5,  0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5,  0.5,
    -0.5,  0.5, -0.5, -0.5, -0.5,  0.5, -0.5,  0.5,  0.5,

    // Face 4
     0.5,  0.5,  0.5,  0.5, -0.5,  0.5,  0.5, -0.5, -0.5,
     0.5,  0.5,  0.5,  0.5, -0.5, -0.5,  0.5,  0.5, -0.5,

    // Face 5
    -0.5,  0.5,  0.5, -0.5, -0.5,  0.5,  0.5, -0.5,  0.5,
    -0.5,  0.5,  0.5,  0.5, -0.5,  0.5,  0.5,  0.5,  0.5,

    // Face 6
     0.5,  0.5, -0.5,  0.5, -0.5, -0.5, -0.5, -0.5, -0.5,
     0.5,  0.5, -0.5, -0.5, -0.5, -0.5, -0.5,  0.5, -0.5
};

float CUBE_TEXTURE_COORDINATES[] =
{
    // Face 1
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    // Face 2
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    
    // Face 3
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    
    // Face 4
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    
    // Face 5
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    
    // Face 6
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f
};

const char* CRATE_FILEPATH = "assets/crate0_diffuse.png";
const char* MARIO_FILEPATH = "assets/mario_body.png";

struct GameState
{
    Entity *crates;
    Entity *enemies;
};

SDL_Window* g_display_window;
bool        g_game_is_running = true;

ShaderProgram g_shader_program;
glm::mat4     g_view_matrix,
              g_model_matrix,
              g_projection_matrix;

GameState g_state;

float g_last_ticks  = 0;
float g_accumulator = 0.0f;

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Hello, 3D!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    g_shader_program.load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    g_view_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::mat4(1.0f);
    
    // Using our new projection model
    g_projection_matrix = glm::perspective(glm::radians(FOV_ANGLE),               // Field of view
                                           (float) WINDOW_WIDTH / WINDOW_HEIGHT,  // Aspect ratio
                                           NEAR_PLANE_DIST,                       // Distance from the camera to the near plane
                                           FAR_PLANE_DIST);                       // Distance from the camera to the far plane
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    g_shader_program.set_colour(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(g_shader_program.get_program_id());
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    glEnable(GL_DEPTH_TEST);  // Enable comparisons to the buffer.
    glDepthMask(GL_TRUE);     // Enable writing to the depth buffer.
    
    /**
     The depth function is how an incoming pixel is compared against one that is already there.
     
     `GL_LEQUAL` means “use the incoming pixel if its distance from the camera is less than
     or equal to what is there already.”
     */
    glDepthFunc(GL_LEQUAL);   // Set the depth function.

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    GLuint cube_texture_id = Util::load_texture(CRATE_FILEPATH);
    
    g_state.crates = new Entity[CRATE_COUNT];
    g_state.crates[0].set_texture_id(cube_texture_id);
    g_state.crates[0].set_position(glm::vec3(0.0f, 0.0f, -5.0f));
    g_state.crates[0].set_vertices(CUBE_VERTICES);
    g_state.crates[0].set_texture_coordinates(CUBE_TEXTURE_COORDINATES);
    g_state.crates[0].set_number_of_vertices(36);
//    g_state.crates[0].set_rotation(glm::vec3(45.0f, 0.0f, 0.0f));
    g_state.crates[0].set_entity_type(CUBE);
    
    g_state.enemies = new Entity[ENEMY_COUNT];
        
    GLuint mario_texture_id = Util::load_texture(MARIO_FILEPATH);
    Mesh* mario_mesh = new Mesh();
    mario_mesh->LoadOBJ("assets/mario.obj");
    
    g_state.enemies[0].set_texture_id(mario_texture_id);
    g_state.enemies[0].set_position(glm::vec3(-10.0f, -20.0f, -50.0f));
    g_state.enemies[0].set_entity_type(ENEMY);
    g_state.enemies[0].set_mesh(mario_mesh);
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
                g_game_is_running = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                        
                }
                break;
        }
    }
}

void update()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - g_last_ticks;
    g_last_ticks = ticks;
    
    deltaTime += g_accumulator;
    if (deltaTime < FIXED_TIMESTEP)
    {
        g_accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP)
    {
        for (int i = 0; i < CRATE_COUNT; i++) g_state.crates[i].update(FIXED_TIMESTEP);
        for (int i = 0; i < ENEMY_COUNT; i++) g_state.enemies[i].update(FIXED_TIMESTEP);
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    g_accumulator = deltaTime;
}


void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (int i = 0; i < CRATE_COUNT; i++) g_state.crates[i].render(&g_shader_program);
    for (int i = 0; i < ENEMY_COUNT; i++) g_state.enemies[i].render(&g_shader_program);
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();
}

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
