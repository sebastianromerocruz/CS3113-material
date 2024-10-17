//#define GL_SILENCE_DEPRECATION
//#define STB_IMAGE_IMPLEMENTATION
//#define LOG(argument) std::cout << argument << '\n'
//#define GL_GLEXT_PROTOTYPES 1
//#define FIXED_TIMESTEP 0.0166666f
//#define PLATFORM_COUNT 5
//
//#ifdef _WINDOWS
//#include <GL/glew.h>
//#endif
//
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include <SDL_mixer.h>
//#include "glm/mat4x4.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "ShaderProgram.h"
//#include "stb_image.h"
//#include "cmath"
//#include <ctime>
//#include <vector>
//#include <cstdlib>
//#include "Entity.h"
//
//// ––––– STRUCTS AND ENUMS ––––– //
//struct GameState
//{
//    Entity* player;
//    Entity* platforms;
//};
//
//// ––––– CONSTANTS ––––– //
//const int WINDOW_WIDTH  = 640,
//          WINDOW_HEIGHT = 480;
//
//const float BG_RED     = 0.1922f,
//            BG_BLUE    = 0.549f,
//            BG_GREEN   = 0.9059f,
//            BG_OPACITY = 1.0f;
//
//const int VIEWPORT_X = 0,
//          VIEWPORT_Y = 0,
//          VIEWPORT_WIDTH  = WINDOW_WIDTH,
//          VIEWPORT_HEIGHT = WINDOW_HEIGHT;
//
//const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
//           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";
//
//const float MILLISECONDS_IN_SECOND = 1000.0;
//const char SPRITESHEET_FILEPATH[] = "assets/george_0.png";
//const char PLATFORM_FILEPATH[]    = "assets/platformPack_tile027.png";
//
//const int NUMBER_OF_TEXTURES = 1;
//const GLint LEVEL_OF_DETAIL  = 0;
//const GLint TEXTURE_BORDER   = 0;
//
//const int CD_QUAL_FREQ    = 44100,
//          AUDIO_CHAN_AMT  = 2,     // stereo
//          AUDIO_BUFF_SIZE = 4096;
//
//const char BGM_FILEPATH[] = "assets/crypto.mp3",
//           SFX_FILEPATH[] = "assets/bounce.wav";
//
//const int PLAY_ONCE = 0,    // play once, loop never
//          NEXT_CHNL = -1,   // next available channel
//          ALL_SFX_CHNL = -1;
//
//
//Mix_Music *g_music;
//Mix_Chunk *g_jump_sfx;
//
//// ––––– GLOBAL VARIABLES ––––– //
//GameState g_state;
//
//SDL_Window* g_display_window;
//bool g_game_is_running = true;
//
//ShaderProgram g_program;
//glm::mat4 g_view_matrix, g_projection_matrix;
//
//float g_previous_ticks = 0.0f;
//float g_accumulator = 0.0f;
//
//// ––––– GENERAL FUNCTIONS ––––– //
//GLuint load_texture(const char* filepath)
//{
//    int width, height, number_of_components;
//    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
//    
//    if (image == NULL)
//    {
//        LOG("Unable to load image. Make sure the path is correct.");
//        assert(false);
//    }
//    
//    GLuint textureID;
//    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
//    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    
//    stbi_image_free(image);
//    
//    return textureID;
//}
//
//void initialise()
//{
//    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
//    g_display_window = SDL_CreateWindow("Hello, Physics (again)!",
//                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                                      WINDOW_WIDTH, WINDOW_HEIGHT,
//                                      SDL_WINDOW_OPENGL);
//    
//    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
//    SDL_GL_MakeCurrent(g_display_window, context);
//    
//#ifdef _WINDOWS
//    glewInit();
//#endif
//    
//    // ––––– VIDEO ––––– //
//    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
//    
//    g_program.load(V_SHADER_PATH, F_SHADER_PATH);
//    
//    g_view_matrix = glm::mat4(1.0f);
//    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
//    
//    g_program.set_projection_matrix(g_projection_matrix);
//    g_program.set_view_matrix(g_view_matrix);
//    
//    glUseProgram(g_program.get_program_id());
//    
//    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
//    
//    // ––––– BGM ––––– //
//    Mix_OpenAudio(CD_QUAL_FREQ, MIX_DEFAULT_FORMAT, AUDIO_CHAN_AMT, AUDIO_BUFF_SIZE);
//    
//    // STEP 1: Have openGL generate a pointer to your music file
//    g_music = Mix_LoadMUS(BGM_FILEPATH); // works only with mp3 files
//    
//    // STEP 2: Play music
//    Mix_PlayMusic(
//                  g_music,  // music file
//                  -1        // -1 means loop forever; 0 means play once, look never
//                  );
//    
//    // STEP 3: Set initial volume
//    Mix_VolumeMusic(MIX_MAX_VOLUME / 2.0);
//    
//    // ––––– SFX ––––– //
//    g_jump_sfx = Mix_LoadWAV(SFX_FILEPATH);
//    
//    // ––––– PLATFORMS ––––– //
//    GLuint platform_texture_id = load_texture(PLATFORM_FILEPATH);
//    
//    g_state.platforms = new Entity[PLATFORM_COUNT];
//    
//    // Set the type of every platform entity to PLATFORM
//    for (int i = 0; i < PLATFORM_COUNT - 2; i++)
//    {
//        g_state.platforms[i].set_texture_id(platform_texture_id);
//        g_state.platforms[i].set_position(glm::vec3(i - 1.0f, -3.0f, 0.0f));
//        g_state.platforms[i].set_width(1.0f);
//        g_state.platforms[i].set_height(1.0f);
//        g_state.platforms[i].set_entity_type(PLATFORM);
//        g_state.platforms[i].update(0.0f, NULL, NULL, 0);
//    }
//    
//    g_state.platforms[PLATFORM_COUNT - 1].set_texture_id(platform_texture_id);
//    g_state.platforms[PLATFORM_COUNT - 1].set_position(glm::vec3(-1.5f, -2.35f, 0.0f));
//    g_state.platforms[PLATFORM_COUNT - 1].set_width(1.0f);
//    g_state.platforms[PLATFORM_COUNT - 1].set_height(1.0f);
//    g_state.platforms[PLATFORM_COUNT - 1].set_entity_type(PLATFORM);
//    g_state.platforms[PLATFORM_COUNT - 1].update(0.0f, NULL, NULL, 0);
//    
//    g_state.platforms[PLATFORM_COUNT - 2].set_texture_id(platform_texture_id);
//    g_state.platforms[PLATFORM_COUNT - 2].set_position(glm::vec3(2.5f, -2.5f, 0.0f));
//    g_state.platforms[PLATFORM_COUNT - 2].set_width(1.0f);
//    g_state.platforms[PLATFORM_COUNT - 1].set_height(1.0f);
//    g_state.platforms[PLATFORM_COUNT - 2].set_entity_type(PLATFORM);
//    g_state.platforms[PLATFORM_COUNT - 2].update(0.0f, NULL, NULL, 0);
//    
//    
//    // ––––– PLAYER (GEORGE) ––––– //
//    // Existing
//    GLuint player_texture_id = load_texture(SPRITESHEET_FILEPATH);
//
//    int player_walking_animation[4][4] =
//    {
//        { 1, 5, 9, 13 },  // for George to move to the left,
//        { 3, 7, 11, 15 }, // for George to move to the right,
//        { 2, 6, 10, 14 }, // for George to move upwards,
//        { 0, 4, 8, 12 }   // for George to move downwards
//    };
//
//    glm::vec3 acceleration = glm::vec3(0.0f,-4.905f, 0.0f);
//
//    g_state.player = new Entity(
//        player_texture_id,         // texture id
//        5.0f,                      // speed
//        acceleration,              // acceleration
//        3.0f,                      // jumping power
//        player_walking_animation,  // animation index sets
//        0.0f,                      // animation time
//        4,                         // animation frame amount
//        0,                         // current animation index
//        4,                         // animation column amount
//        4,                         // animation row amount
//        1.0f,                      // width
//        1.0f,                       // height
//        PLAYER
//    );
//
//
//    // Jumping
//    g_state.player->set_jumping_power(3.0f);
//    
//    // ––––– GENERAL ––––– //
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//void process_input()
//{
//    g_state.player->set_movement(glm::vec3(0.0f));
//    
//    SDL_Event event;
//    while (SDL_PollEvent(&event))
//    {
//        switch (event.type) {
//            // End game
//            case SDL_QUIT:
//            case SDL_WINDOWEVENT_CLOSE:
//                g_game_is_running = false;
//                break;
//                
//            case SDL_KEYDOWN:
//                switch (event.key.keysym.sym) {
//                    case SDLK_q:
//                        // Quit the game with a keystroke
//                        g_game_is_running = false;
//                        break;
//                        
//                    case SDLK_SPACE:
//                        // Jump
//                        if (g_state.player->get_collided_bottom())
//                        {
//                            g_state.player->jump();
//                            Mix_PlayChannel(NEXT_CHNL, g_jump_sfx, 0);
//                            
//                            /**
//                             Mix_FadeInChannel(channel_id, sound_chunk, loops, fade_in_time);
//                             
//                             
//                             */
//                        }
//                        break;
//                        
//                    case SDLK_h:
//                        // Stop music
//                        Mix_HaltMusic();
//                        break;
//                        
//                    case SDLK_p:
//                        Mix_PlayMusic(g_music, -1);
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
//    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
//
//    if (key_state[SDL_SCANCODE_LEFT])
//    {
//        g_state.player->move_left();
//    }
//    else if (key_state[SDL_SCANCODE_RIGHT])
//    {
//        g_state.player->move_right();
//    }
//    
//    if (glm::length(g_state.player->get_movement()) > 1.0f)
//    {
//        g_state.player->normalise_movement();
//    }
//}
//
//void update()
//{
//    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
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
//    while (delta_time >= FIXED_TIMESTEP)
//    {
//        g_state.player->update(FIXED_TIMESTEP, NULL, g_state.platforms, PLATFORM_COUNT);
//        delta_time -= FIXED_TIMESTEP;
//    }
//    
//    g_accumulator = delta_time;
//}
//
//void render()
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//    
//    g_state.player->render(&g_program);
//    
//    for (int i = 0; i < PLATFORM_COUNT; i++) g_state.platforms[i].render(&g_program);
//    
//    SDL_GL_SwapWindow(g_display_window);
//}
//
//void shutdown()
//{
//    SDL_Quit();
//    
//    delete [] g_state.platforms;
//    delete g_state.player;
//}
//
//// ––––– GAME LOOP ––––– //
//int main(int argc, char* argv[])
//{
//    initialise();
//    
//    while (g_game_is_running)
//    {
//        process_input();
//        update();
//        render();
//    }
//    
//    shutdown();
//    return 0;
//}
