#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP  0.0166666f
#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720
#define NEAR_PLANE_DIST 0.1f
#define FAR_PLANE_DIST  100.0F
#define FOV_ANGLE       45.0f
#define CRATE_COUNT     3

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

const char* CRATE_FILEPATH = "assets/crate0_diffuse.png";
const char* FLOOR_FILEPATH = "assets/100_1453_seamless.JPG";

SDL_Window* display_window;
bool game_is_running = true;

ShaderProgram program;
glm::mat4 view_matrix, model_matrix, projection_matrix;

struct GameState
{
    Entity *floor;
    Entity *player;
    Entity *crates;
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
    state.player->position = glm::vec3(0, 0.5f, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    
    GLuint floor_texture_id = Util::load_texture(FLOOR_FILEPATH);
    Mesh* floor_mesh = new Mesh();
    floor_mesh->load_OBJ("assets/cube.obj", 50);
    
    state.floor = new Entity();
    state.floor->entity_type = FLOOR;
    state.floor->texture_id = floor_texture_id;
    state.floor->position = glm::vec3(0.0f, -0.5f, 0.0f);
    state.floor->mesh = floor_mesh;
    state.floor->scale = glm::vec3(100.0, 0.5f, 100.0f);
    state.floor->update(0.0f, state.player, NULL, 0);
    
    GLuint crate_texture_id = Util::load_texture(CRATE_FILEPATH);
    Mesh* crate_mesh = new Mesh();
    crate_mesh->load_OBJ("assets/cube.obj", 1);
    
    state.crates = new Entity[CRATE_COUNT];
    
    state.crates[0].entity_type = CUBE;
    state.crates[0].texture_id = crate_texture_id;
    state.crates[0].position = glm::vec3(1.0f, 0.25f, -3.0f);
    state.crates[0].mesh = crate_mesh;
    
    state.crates[1].entity_type = CUBE;
    state.crates[1].texture_id = crate_texture_id;
    state.crates[1].position = glm::vec3(0.0f, 0.25f, -3.0f);
    state.crates[1].mesh = crate_mesh;
    
    state.crates[2].entity_type = CUBE;
    state.crates[2].texture_id = crate_texture_id;
    state.crates[2].position = glm::vec3(-1.0f, 0.25f, -3.0f);
    state.crates[2].mesh = crate_mesh;
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
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_A])      { state.player->rotation.y += 1.0f; }
    else if (keys[SDL_SCANCODE_D]) { state.player->rotation.y -= 1.0f; }
    
    state.player->velocity.x = 0;
    state.player->velocity.z = 0;
    
    if (keys[SDL_SCANCODE_W])
    {
        state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * -2.0f;
        state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * -2.0f;
    }
    else if (keys[SDL_SCANCODE_S])
    {
        state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * 2.0f;
        state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * 2.0f;
    }
}

void update()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float delta_time = ticks - last_ticks;
    last_ticks = ticks;
    
    delta_time += accumulator;
    if (delta_time < FIXED_TIMESTEP)
    {
        accumulator = delta_time;
        return;
    }
    
    while (delta_time >= FIXED_TIMESTEP)
    {
        state.player->update(FIXED_TIMESTEP, state.player, state.crates, CRATE_COUNT);
        
        for (int i = 0; i < CRATE_COUNT; i++) state.crates[i].update(FIXED_TIMESTEP, state.player, state.crates, CRATE_COUNT);

        delta_time -= FIXED_TIMESTEP;
    }
    
    accumulator = delta_time;
    
    view_matrix = glm::mat4(1.0f);
    view_matrix = glm::rotate(view_matrix, glm::radians(state.player->rotation.y), glm::vec3(0.0f, -1.0f, 0.0f));
    view_matrix = glm::translate(view_matrix, -state.player->position);
}


void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    program.SetViewMatrix(view_matrix);
    state.floor->render(&program);
    
    for (int i = 0; i < CRATE_COUNT; i++) state.crates[i].render(&program);
    
    SDL_GL_SwapWindow(display_window);
}

void shutdown()
{
    SDL_Quit();
    
    delete state.floor;
    delete state.player;
    delete [] state.crates;
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
