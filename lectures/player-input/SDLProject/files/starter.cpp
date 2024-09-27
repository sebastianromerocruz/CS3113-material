///**
// * @file main.cpp
// * @author Sebastián Romero Cruz (sebastian.romerocruz@nyu.edu)
// * @brief A simple g_shader_program to demonstrate player input in OpenGL.
// * @date 2024-06-10
// *
// * @copyright NYU Tandon (c) 2024
// */
//#define GL_SILENCE_DEPRECATION
//#define STB_IMAGE_IMPLEMENTATION
//#define GL_GLEXT_PROTOTYPES 1
//#define LOG(argument) std::cout << argument << '\n'
//
//#ifdef _WINDOWS
//    #include <GL/glew.h>
//#endif
//
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include "glm/mat4x4.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "ShaderProgram.h"
//#include "stb_image.h"
//
//enum AppStatus { RUNNING, TERMINATED };
//
//constexpr int WINDOW_WIDTH  = 640,
//              WINDOW_HEIGHT = 480;
//
//constexpr float BG_RED     = 0.9765625f,
//                BG_GREEN   = 0.97265625f,
//                BG_BLUE    = 0.9609375f,
//                BG_OPACITY = 1.0f;
//
//constexpr int VIEWPORT_X      = 0,
//              VIEWPORT_Y      = 0,
//              VIEWPORT_WIDTH  = WINDOW_WIDTH,
//              VIEWPORT_HEIGHT = WINDOW_HEIGHT;
//
//constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
//               F_SHADER_PATH[] = "shaders/fragment_textured.glsl";
//
//constexpr float MILLISECONDS_IN_SECOND = 1000.0f;
//
//constexpr GLint NUMBER_OF_TEXTURES = 1,
//                LEVEL_OF_DETAIL    = 0,
//                TEXTURE_BORDER     = 0;
//
//constexpr char SHIELD_SPRITE_FILEPATH[] = "shield.png";
//constexpr glm::vec3 INIT_SCALE = glm::vec3(2.0f, 2.3985f, 0.0f);
//
//SDL_Window* g_display_window = nullptr;
//AppStatus g_app_status = RUNNING;
//
//ShaderProgram g_shader_program = ShaderProgram();
//
//GLuint g_shield_texture_id;
//
//glm::mat4 g_view_matrix,
//          g_shield_matrix,
//          g_projection_matrix;
//
//float g_previous_ticks = 0.0f;
//                                                               
//glm::vec3 g_shield_position = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 g_shield_movement = glm::vec3(0.0f, 0.0f, 0.0f);
//                                                               
//float g_shield_speed = 5.0f;  // move 1 unit per second
//
//void initialise();
//void process_input();
//void update();
//void render();
//void shutdown();
//
//GLuint load_texture(const char* filepath);
//void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id);
//
//
//GLuint load_texture(const char* filepath)
//{
//    // STEP 1: Loading the image file
//    int width, height, number_of_components;
//    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components,
//                                     STBI_rgb_alpha);
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
//    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER,
//                 GL_RGBA, GL_UNSIGNED_BYTE, image);
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
//
//void initialise()
//{
//    SDL_Init(SDL_INIT_VIDEO);
//    g_display_window = SDL_CreateWindow("Hello, Playa Input!",
//                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                                      WINDOW_WIDTH, WINDOW_HEIGHT,
//                                      SDL_WINDOW_OPENGL);
//    
//    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
//    SDL_GL_MakeCurrent(g_display_window, context);
//    
//    if (g_display_window == nullptr)
//    {
//        shutdown();
//    }
//    
//#ifdef _WINDOWS
//    glewInit();
//#endif
//    
//    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
//    
//    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
//    
//    g_shield_texture_id = load_texture(SHIELD_SPRITE_FILEPATH);
//    g_shield_matrix     = glm::mat4(1.0f);
//    g_view_matrix       = glm::mat4(1.0f);
//    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
//    
//    g_shader_program.set_projection_matrix(g_projection_matrix);
//    g_shader_program.set_view_matrix(g_view_matrix);
//    
//    glUseProgram(g_shader_program.get_program_id());
//    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
//    
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//
//void process_input()
//{
//    SDL_Event event;
//    while (SDL_PollEvent(&event))
//    {
//        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
//        {
//            g_app_status = TERMINATED;
//        }
//    }
//}
//
//
//void update()
//{
//    /* DELTA TIME */
//    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND;
//    float delta_time = ticks - g_previous_ticks;
//    g_previous_ticks = ticks;
//
//    /* GAME LOGIC */
//    g_shield_position += g_shield_movement * g_shield_speed * delta_time;
//    
//    /* TRANSFORMATIONS */
//    g_shield_matrix = glm::mat4(1.0f);
//    
//    g_shield_matrix = glm::translate(g_shield_matrix, g_shield_position);
//    g_shield_matrix = glm::scale(g_shield_matrix, INIT_SCALE);
//}
//
//
//void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id)
//{
//    g_shader_program.set_model_matrix(object_model_matrix);
//    glBindTexture(GL_TEXTURE_2D, object_texture_id);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//
//void render()
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//    
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
//    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false,
//                          0, vertices);
//    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
//    
//    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT,
//                          false, 0, texture_coordinates);
//    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
//    
//    // Bind texture
//    draw_object(g_shield_matrix, g_shield_texture_id);
//    
//    // We disable two attribute arrays now
//    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
//    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
//    
//    SDL_GL_SwapWindow(g_display_window);
//}
//
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
