//#define GL_SILENCE_DEPRECATION
//#define STB_IMAGE_IMPLEMENTATION
//#define LOG(argument) std::cout << argument << '\n'
//
//#ifdef _WINDOWS
//#include <GL/glew.h>
//#endif
//
//#define GL_GLEXT_PROTOTYPES 1
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include "glm/mat4x4.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "ShaderProgram.h"
//#include "stb_image.h"
//#include "cmath"
//#include <ctime>
//
//enum AppStatus { RUNNING, TERMINATED };
//
//constexpr float WINDOW_SIZE_MULT = 2.0f;
//
//constexpr int WINDOW_WIDTH  = 640 * WINDOW_SIZE_MULT,
//              WINDOW_HEIGHT = 480 * WINDOW_SIZE_MULT;
//
//constexpr float BG_RED     = 0.9765625f,
//                BG_GREEN   = 0.97265625f,
//                BG_BLUE    = 0.9609375f,
//                BG_OPACITY = 1.0f;
//
//constexpr int VIEWPORT_X = 0,
//          VIEWPORT_Y = 0,
//          VIEWPORT_WIDTH  = WINDOW_WIDTH,
//          VIEWPORT_HEIGHT = WINDOW_HEIGHT;
//
//constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
//           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";
//
//constexpr GLint NUMBER_OF_TEXTURES = 1;
//constexpr GLint LEVEL_OF_DETAIL    = 0;
//constexpr GLint TEXTURE_BORDER     = 0;
//
//constexpr float MILLISECONDS_IN_SECOND = 1000.0;
//
//constexpr char BEAKER_SPRITE_FILEPATH[] = "beaker.png",
//               DROPS_SPRITE_FILEPATH[]  = "drops.png";
//
//constexpr float MINIMUM_COLLISION_DISTANCE = 1.0f;
//constexpr glm::vec3 INIT_SCALE_DROPS  = glm::vec3(1.0f, 1.0f, 0.0f),
//                    INIT_POS_DROPS    = glm::vec3(-3.0f, 2.0f, 0.0f),
//                    INIT_SCALE_BEAKER = glm::vec3(3.0f, 3.0f, 0.0f),
//                    INIT_POS_BEAKER   = glm::vec3(3.0f, -2.0f, 0.0f);
//
//SDL_Window* g_display_window;
//
//AppStatus g_app_status = RUNNING;
//ShaderProgram g_shader_program = ShaderProgram();
//glm::mat4 g_view_matrix, g_beaker_matrix, g_projection_matrix, g_drops_matrix;
//
//float g_previous_ticks = 0.0f;
//
//GLuint g_beaker_texture_id;
//GLuint g_drops_texture_id;
//
//constexpr float DROPS_SPEED = 3.0f;  // speed of drop fall
//
//glm::vec3 g_beaker_position = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 g_beaker_movement = glm::vec3(0.0f, 0.0f, 0.0f);
//
//glm::vec3 g_drops_position = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 g_drops_movement = glm::vec3(0.0f, 0.0f, 0.0f);
//
//glm::vec3 g_drops_scale = glm::vec3(0.0f, 0.0f, 0.0f);  // scale trigger vector
//glm::vec3 g_drops_size  = glm::vec3(1.0f, 1.0f, 0.0f);  // scale accumulator vector
//
//void initialise();
//void process_input();
//void update();
//void render();
//void shutdown();
//
//GLuint load_texture(const char* filepath)
//{
//    // STEP 1: Loading the image file
//    int width, height, number_of_components;
//    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
//
//    if (image == NULL)
//    {
//        LOG("Unable to load image. Make sure the path is correct.");
//        assert(false);
//    }
//
//    // STEP 2: Generating and binding a texture ID to our image
//    GLuint textureID;
//    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
//
//    // STEP 3: Setting our texture filter parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//    // STEP 4: Releasing our file from memory and returning our texture id
//    stbi_image_free(image);
//
//    return textureID;
//}
//
//void initialise()
//{
//    SDL_Init(SDL_INIT_VIDEO);
//    g_display_window = SDL_CreateWindow("User-Input and Collisions Exercise",
//                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                                      WINDOW_WIDTH, WINDOW_HEIGHT,
//                                      SDL_WINDOW_OPENGL);
//
//    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
//    SDL_GL_MakeCurrent(g_display_window, context);
//
//
//    if (g_display_window == nullptr) shutdown();
//
//#ifdef _WINDOWS
//    glewInit();
//#endif
//
//    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
//
//    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
//
//    g_beaker_matrix = glm::mat4(1.0f);
//    g_drops_matrix = glm::mat4(1.0f);
//    g_drops_matrix = glm::translate(g_drops_matrix, glm::vec3(1.0f, 1.0f, 0.0f));
//    g_drops_position += g_drops_movement;
//
//    g_view_matrix = glm::mat4(1.0f);
//    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
//
//    g_shader_program.set_projection_matrix(g_projection_matrix);
//    g_shader_program.set_view_matrix(g_view_matrix);
//
//    glUseProgram(g_shader_program.get_program_id());
//
//    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
//
//    g_beaker_texture_id = load_texture(BEAKER_SPRITE_FILEPATH);
//    g_drops_texture_id = load_texture(DROPS_SPRITE_FILEPATH);
//
//    // enable blending
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//void process_input()
//{
//    // Every frame, turn off the x-component of the movement trigger vector only
//    // since that is the only component that relies on user-input
//    g_drops_movement.x = 0.0f;
//
//    SDL_Event event;
//    while (SDL_PollEvent(&event))
//    {
//        switch (event.type)
//        {
//            // End game
//            case SDL_QUIT:
//            case SDL_WINDOWEVENT_CLOSE:
//                g_app_status = TERMINATED;
//                break;
//
//            case SDL_KEYDOWN:
//                switch (event.key.keysym.sym)
//                {
//                    case SDLK_q: g_app_status = TERMINATED; break;
//
//                    // If the player presses the lowercase L key, then we're going to
//                    // activate the drops' movement trigger vector's y-component in the
//                    // downwards (negative) direction
//                    case SDLK_l: g_drops_movement.y = -1.0;
//
//                    default: break;
//                }
//
//            default:
//                break;
//        }
//    }
//
//
//    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
//
//    // Left and right movement for the drops is user-input dependent
//    if      (key_state[SDL_SCANCODE_A]) g_drops_movement.x = -1.0f;
//    else if (key_state[SDL_SCANCODE_D]) g_drops_movement.x = 1.0f;
//
//    if (glm::length(g_drops_movement) > 1.0f)
//    {
//        // In this particular case, I'm not going to normalise because I am simulating
//        // gravity
//        // g_drops_movement = glm::normalize(g_drops_movement);
//    }
//}
//
//void update()
//{
//    // --- DELTA TIME CALCULATIONS --- //
//    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND;
//    float delta_time = ticks - g_previous_ticks;
//    g_previous_ticks = ticks;
//
//    // --- ACCUMULATOR LOGIC --- //
//    g_drops_position += g_drops_movement * DROPS_SPEED * delta_time;
//    
//    // in addition to translation, we no scale with respect to delta time as well
//    g_drops_size += g_drops_scale * DROPS_SPEED * delta_time;
//
//    // --- TRANSLATION --- //
//    g_beaker_matrix = glm::mat4(1.0f);
//    g_beaker_matrix = glm::translate(g_beaker_matrix, INIT_POS_BEAKER);
//
//    g_drops_matrix = glm::mat4(1.0f);
//    g_drops_matrix = glm::translate(g_drops_matrix, INIT_POS_DROPS);
//    g_drops_matrix = glm::translate(g_drops_matrix, g_drops_position);
//
//    // --- SCALING --- //
//    g_beaker_matrix = glm::scale(g_beaker_matrix, INIT_SCALE_BEAKER);
//
//    g_drops_matrix  = glm::scale(g_drops_matrix, INIT_SCALE_DROPS);
//    g_drops_matrix  = glm::scale(g_drops_matrix, g_drops_size);
//
//    // --- COLLISION LOGIC --- //
//    // 1. Calculate box-to-box collision values
//    float x_distance = fabs(g_drops_position.x + INIT_POS_DROPS.x - INIT_POS_BEAKER.x) -
//        ((INIT_SCALE_BEAKER.x + INIT_SCALE_DROPS.x) / 2.0f);
//
//    float y_distance = fabs(g_drops_position.y + INIT_POS_DROPS.y - INIT_POS_BEAKER.y) -
//        ((INIT_SCALE_BEAKER.y + INIT_SCALE_DROPS.y) / 2.0f);
//
//    // 2. Now, we want to turn on the shrinking triggers only if:
//    //      - there's a collision, and if...
//    //      - the size of the drops has not gone into the negatives (if this happens, the
//    //        drops will actually start scaling up again—this is called negative scaling)
//    if (x_distance < 0.0f && y_distance < 0.0f && g_drops_size.y > 0.0f && g_drops_size.x > 0.0f)
//        // 3. We trigger scaling via the drops scale vector (like we trigger movement with
//        //    the movement vector instead of the position vector)
//        g_drops_scale = glm::vec3(-1.0, -1.0, 0.0);
//
//    // 4. Finally, we're going to turn off the following things once the size of the drops
//    //    reaches zero:
//    //      - The scale trigger vector
//    //      - The size accumulator vector (to prevent any negative scaling)
//    //      - The negative movement of the drops, to prevent the game from ending
//    if (g_drops_size.x <= 0.0f || g_drops_size.y <= 0.0f)
//    {
//        g_drops_size = glm::vec3(0.0f, 0.0f, 0.0f);   // Prevents negative scaling
//        g_drops_scale = glm::vec3(0.0f, 0.0f, 0.0f);  // Stops further scaling
//        g_drops_movement.y = 0.0f;
//    }
//
//    // --- TERMINATE GAME IF BEAKER MISSED --- //
//    if (g_drops_position.y < -7.0f) g_app_status = TERMINATED;
//}
//
//void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id)
//{
//    g_shader_program.set_model_matrix(object_model_matrix);
//    glBindTexture(GL_TEXTURE_2D, object_texture_id);
//    glDrawArrays(GL_TRIANGLES, 0, 6); // we are now drawing 2 triangles, so we use 6 instead of 3
//}
//
//void render() {
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // Vertices
//    float vertices[] = {
//        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
//        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
//    };
//
//    // Textures
//    float texture_coordinates[] = {
//        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
//        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
//    };
//
//    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
//    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
//
//    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
//    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
//
//    // Bind texture
//    draw_object(g_drops_matrix, g_drops_texture_id);
//    draw_object(g_beaker_matrix, g_beaker_texture_id);
//
//    // We disable two attribute arrays now
//    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
//    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
//
//    SDL_GL_SwapWindow(g_display_window);
//}
//
//void shutdown() { SDL_Quit(); }
//
//
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
//
