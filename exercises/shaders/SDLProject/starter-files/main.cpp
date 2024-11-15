//#define GL_SILENCE_DEPRECATION
//#define GL_GLEXT_PROTOTYPES 1
//#define FIXED_TIMESTEP 0.0166666f
//#define LEVEL1_WIDTH 14
//#define LEVEL1_HEIGHT 8
//#define LEVEL1_LEFT_EDGE 5.0f
//#define MAX_RGB 255
//
//#ifdef _WINDOWS
//#include <GL/glew.h>
//#endif
//
//#include <SDL_mixer.h>
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include "glm/mat4x4.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "ShaderProgram.h"
//#include "cmath"
//#include <ctime>
//#include <vector>
//#include "Entity.h"
//#include "Map.h"
//#include "Utility.h"
//#include "Scene.h"
//#include "LevelA.h"
//#include "Effects.h"
//
//// ––––– CONSTANTS ––––– //
//constexpr int WINDOW_WIDTH  = 1280,
//          WINDOW_HEIGHT = 960;
//
//constexpr float BG_RED = 27.0f / MAX_RGB,
//            BG_BLUE    = 43.0f / MAX_RGB,
//            BG_GREEN   = 52.0f / MAX_RGB,
//            BG_OPACITY = 1.0f;
//
//constexpr int VIEWPORT_X = 0,
//          VIEWPORT_Y = 0,
//          VIEWPORT_WIDTH  = WINDOW_WIDTH,
//          VIEWPORT_HEIGHT = WINDOW_HEIGHT;
//
//constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
//           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";
//
//constexpr float MILLISECONDS_IN_SECOND = 1000.0;
//
//enum AppStatus { RUNNING, TERMINATED };
//
//// ––––– GLOBAL VARIABLES ––––– //
//Scene  *g_curr_scene = nullptr;
//LevelA *g_levelA = nullptr;
//
//Effects *g_effects = nullptr;
//Scene   *g_levels[1];
//
//SDL_Window* g_display_window = nullptr;
//
//ShaderProgram g_shader_program;
//glm::mat4 g_view_matrix, g_projection_matrix;
//
//float g_previous_ticks = 0.0f;
//float g_accumulator = 0.0f;
//
//bool g_is_colliding_bottom = false;
//
//AppStatus g_app_status = RUNNING;
//
//void swtich_to_scene(Scene *scene);
//void initialise();
//void process_input();
//void update();
//void render();
//void shutdown();
//
//void switch_to_scene(Scene *scene)
//{
//    g_curr_scene = scene;
//    g_curr_scene->initialise();
//}
//
//void initialise()
//{
//    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
//    g_display_window = SDL_CreateWindow("Hello, Shaders!",
//                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                                        WINDOW_WIDTH, WINDOW_HEIGHT,
//                                        SDL_WINDOW_OPENGL);
//
//    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
//    SDL_GL_MakeCurrent(g_display_window, context);
//
//#ifdef _WINDOWS
//    glewInit();
//#endif
//
//    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
//
//    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
//
//    g_view_matrix = glm::mat4(1.0f);
//    g_projection_matrix = glm::ortho(-3.75f, 3.75f, -2.8125f, 2.8125f, -1.0f, 1.0f);
//
//    g_shader_program.set_projection_matrix(g_projection_matrix);
//    g_shader_program.set_view_matrix(g_view_matrix);
//
//    glUseProgram(g_shader_program.get_program_id());
//
//    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    g_levelA = new LevelA();
//    g_levels[0] = g_levelA;
//
//    switch_to_scene(g_levels[0]);
//
//    g_effects = new Effects(g_projection_matrix, g_view_matrix);
//}
//
//void process_input()
//{
//    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
//    g_curr_scene->get_state().player->set_movement(glm::vec3(0.0f));
//
//    SDL_Event event;
//    while (SDL_PollEvent(&event))
//    {
//        switch (event.type) {
//            case SDL_QUIT:
//            case SDL_WINDOWEVENT_CLOSE:
//                g_app_status = TERMINATED;
//                break;
//
//            case SDL_KEYDOWN:
//                switch (event.key.keysym.sym) {
//                    case SDLK_q: g_app_status = TERMINATED;     break;
//
//                    default:
//                        break;
//                }
//
//            default:
//                break;
//        }
//    }
//
//    const Uint8 *key_state = SDL_GetKeyboardState(nullptr);
//
//    if (key_state[SDL_SCANCODE_C] &&
//        g_curr_scene->get_state().player->get_collided_bottom())
//    {
//        g_curr_scene->get_state().player->set_player_state(CHARGING);
//        g_effects->start(SHAKE);
//    }
//    else
//    {
//        g_curr_scene->get_state().player->set_player_state(REST);
//        g_effects->start(NONE);
//    }
//
//    if (glm::length(g_curr_scene->get_state().player->get_movement()) > 1.0f)
//        g_curr_scene->get_state().player->normalise_movement();
//
//}
//
//void update()
//{
//    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND;
//    float delta_time = ticks - g_previous_ticks;
//    g_previous_ticks = ticks;
//
//    delta_time += g_accumulator;
//
//    if (delta_time < FIXED_TIMESTEP)
//    {
//        g_accumulator = delta_time;
//        return;
//    }
//
//    while (delta_time >= FIXED_TIMESTEP) {
//        g_curr_scene->update(FIXED_TIMESTEP);
//        g_effects->update(FIXED_TIMESTEP);
//
//        g_is_colliding_bottom = g_curr_scene->get_state().player->get_collided_bottom();
//
//        delta_time -= FIXED_TIMESTEP;
//    }
//
//    g_accumulator = delta_time;
//
//    // Prevent the camera from showing anything outside of the "edge" of the level
//    g_view_matrix = glm::mat4(1.0f);
//
//    if (g_curr_scene->get_state().player->get_position().x > LEVEL1_LEFT_EDGE)
//    {
//        g_view_matrix = glm::translate(
//                                g_view_matrix,
//                                glm::vec3(
//                                    -g_curr_scene->get_state().player->get_position().x,
//                                    3.75f,
//                                    0.0f
//                                )
//                        );
//    }
//    else
//    {
//        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5.0f, 3.75f, 0.0f));
//    }
//
//    g_view_matrix = glm::translate(g_view_matrix, g_effects->get_view_offset());
//}
//
//void render()
//{
//    g_shader_program.set_view_matrix(g_view_matrix);
//
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    g_curr_scene->render(&g_shader_program);
//
//    g_effects->render();
//
//    SDL_GL_SwapWindow(g_display_window);
//}
//
//void shutdown()
//{
//    SDL_Quit();
//
//    delete g_levelA;
//    delete g_effects;
//}
//
//// ––––– DRIVER GAME LOOP ––––– //
//int main(int argc, char* argv[])
//{
//    initialise();
//
//    while (g_app_status == RUNNING)
//    {
//        process_input();
//        update();
//        render();
//    }
//
//    shutdown();
//    return 0;
//}
