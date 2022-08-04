#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP  0.0166666f
#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720
#define NEAR_PLANE_DIST 0.1f
#define FAR_PLANE_DIST  100.0F
#define FOV_ANGLE       45.0f
#define CRATE_COUNT     1
#define ENEMY_COUNT     2
#define SHIP_COUNT      1

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

const char* CRATE_FILEPATH   = "assets/crate0_diffuse.png";
const char* MARIO_FILEPATH   = "assets/mario_body.png";
const char* PIKACHU_FILEPATH = "assets/pikachu.png";
const char* SHIP_FILEPATH    = "assets/ship.png";

SDL_Window* display_window;
bool game_is_running = true;

ShaderProgram program;
glm::mat4 view_matrix, model_matrix, projection_matrix;

struct GameState
{
    Entity *player;
    Entity *crates;
    Entity *enemies;
    Entity *ships;
};

GameState state;

float last_ticks = 0;
float accumulator = 0.0f;

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    display_window = SDL_CreateWindow("Hello, 3D!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(display_window);
    SDL_GL_MakeCurrent(display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    view_matrix = glm::mat4(1.0f);
    model_matrix = glm::mat4(1.0f);
    
    // Using our new projection model
    projection_matrix = glm::perspective(glm::radians(FOV_ANGLE),               // Field of view
                                         (float) WINDOW_WIDTH / WINDOW_HEIGHT,  // Aspect ratio
                                         NEAR_PLANE_DIST,                       // Distance from the camera to the near plane
                                         FAR_PLANE_DIST);                       // Distance from the camera to the far plane
    
    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    glEnable(GL_DEPTH_TEST);  // Enable comparisons to the buffer.
    glDepthMask(GL_TRUE);     // Enable writing to the depth buffer.
    glDepthFunc(GL_LEQUAL);   // Set the depth function.

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    state.player = new Entity();
    state.player->entity_type = PLAYER;
    state.player->position = glm::vec3(0, 1.5f, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    
    GLuint cube_texture_id = Util::load_texture(CRATE_FILEPATH);
    
    Mesh* cube_mesh = new Mesh();
    cube_mesh->load_OBJ("assets/cube.obj");
    
    state.crates = new Entity[CRATE_COUNT];
    state.crates[0].texture_id = cube_texture_id;
    state.crates[0].position = glm::vec3(0.0f, 0.0f, -5.0f);
    state.crates[0].entity_type = CUBE;
    state.crates[0].mesh = cube_mesh;
    
    state.enemies = new Entity[ENEMY_COUNT];
    
    GLuint mario_texture_id = Util::load_texture(MARIO_FILEPATH);
    Mesh* mario_mesh = new Mesh();
    mario_mesh->load_OBJ("assets/mario.obj");
    
    state.enemies[0].texture_id = mario_texture_id;
    state.enemies[0].position = glm::vec3(-10.0f, -20.0f, -50.0f);
    state.enemies[0].entity_type = ENEMY;
    state.enemies[0].mesh = mario_mesh;
    
    GLuint pikachu_texture_id = Util::load_texture(PIKACHU_FILEPATH);
    Mesh* pikachu_mesh = new Mesh();
    pikachu_mesh->load_OBJ("assets/pikachu.obj");
    
    state.enemies[1].texture_id = pikachu_texture_id;
    state.enemies[1].position = glm::vec3(2.0f, 0.0f, -5.0f);
    state.enemies[1].entity_type = ENEMY;
    state.enemies[1].mesh = pikachu_mesh;
    
    state.ships = new Entity[SHIP_COUNT];
    GLuint ship_texture_id = Util::load_texture(SHIP_FILEPATH);
    Mesh* ship_mesh = new Mesh();
    ship_mesh->load_OBJ("assets/ship.obj");
    
    state.ships[0].texture_id = ship_texture_id;
    state.ships[0].position = glm::vec3(10.0f, 5.0f, 10.0f);
    state.ships[0].entity_type = SHIP;
    state.ships[0].mesh = ship_mesh;
    state.ships[0].acceleration = glm::vec3(0.0f, 0.0f, -10.0f);
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
                game_is_running = false;
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
    float deltaTime = ticks - last_ticks;
    last_ticks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP)
    {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP)
    {
        state.player->update(FIXED_TIMESTEP);
        
        for (int i = 0; i < CRATE_COUNT; i++) state.crates[i].update(FIXED_TIMESTEP);
        for (int i = 0; i < ENEMY_COUNT; i++) state.enemies[i].update(FIXED_TIMESTEP);
        for (int i = 0; i < SHIP_COUNT; i++)  state.ships[i].update(FIXED_TIMESTEP);
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
}


void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (int i = 0; i < CRATE_COUNT; i++) state.crates[i].render(&program);
    for (int i = 0; i < ENEMY_COUNT; i++) state.enemies[i].render(&program);
    for (int i = 0; i < SHIP_COUNT; i++)  state.ships[i].render(&program);
    
    SDL_GL_SwapWindow(display_window);
}

void shutdown()
{
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    initialise();
    
    while (game_is_running)
    {
        process_input();
        update();
        render();
    }
    
    shutdown();
    return 0;
}
