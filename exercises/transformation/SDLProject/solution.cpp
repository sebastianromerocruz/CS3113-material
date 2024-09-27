//#define GL_SILENCE_DEPRECATION
//#define GL_GLEXT_PROTOTYPES 1
//#define LOG(argument) std::cout << argument << '\n'
//
//#ifdef _WINDOWS
//#include <GL/glew.h>
//#endif
//
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include "glm/mat4x4.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "ShaderProgram.h"
//
//
//enum AppStatus { RUNNING, TERMINATED };
//
//constexpr char V_SHADER_PATH[] = "shaders/vertex.glsl",
//           F_SHADER_PATH[] = "shaders/fragment.glsl";
//
//constexpr int WINDOW_WIDTH = 640 * 2,
//          WINDOW_HEIGHT = 480 * 2;
//
//constexpr float BG_RED = 0.1922f,
//            BG_BLUE = 0.549f,
//            BG_GREEN = 0.9059f,
//            BG_OPACITY = 1.0f;
//
//constexpr int VIEWPORT_X = 0,
//          VIEWPORT_Y = 0,
//          VIEWPORT_WIDTH = WINDOW_WIDTH,
//          VIEWPORT_HEIGHT = WINDOW_HEIGHT;
//
//constexpr int TRIANGLE_RED     = 1.0,
//          TRIANGLE_BLUE    = 0.4,
//          TRIANGLE_GREEN   = 0.4,
//          TRIANGLE_OPACITY = 1.0;
//
//SDL_Window* g_display_window;
//
//AppStatus g_app_status = RUNNING;
//
//
//bool g_is_growing = true;
//int  g_frame_counter = 0;
//
//ShaderProgram g_shader_program;
//glm::mat4 g_view_matrix,
//          g_model_matrix,
//          g_projection_matrix;
//
//// ——————————— GLOBAL VARS AND CONSTS FOR TRANSFORMATIONS ——————————— //
//constexpr float BASE_SCALE = 1.0f,      // The unscaled size of your object
//                MAX_AMPLITUDE = 0.1f,  // The most our triangle will be scaled up/down
//                PULSE_SPEED = 10.0f;    // How fast you want your triangle to "beat"
//
//constexpr float RADIUS = 2.0f;      // radius of your circle
//constexpr float ORBIT_SPEED = 1.0f;  // rotational speed
//float       g_angle = 0.0f;     // current angle
//float       g_x_offset = 0.0f, // current x and y coordinates
//            g_y_offset = 0.0f;
//float g_previous_ticks = 0;
//float g_pulse_time = 0.0f;
//
//// —————————————————————————————————————————————————————————————————— //
//
//void initialise()
//{
//    SDL_Init(SDL_INIT_VIDEO);
//    g_display_window = SDL_CreateWindow("Transformation Exercise",
//        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//        WINDOW_WIDTH, WINDOW_HEIGHT,
//        SDL_WINDOW_OPENGL);
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
//    g_view_matrix       = glm::mat4(1.0f);
//    g_model_matrix      = glm::mat4(1.0f);
//    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
//
//    g_shader_program.set_projection_matrix(g_projection_matrix);
//    g_shader_program.set_view_matrix(g_view_matrix);
//    g_shader_program.set_colour(TRIANGLE_RED, TRIANGLE_BLUE, TRIANGLE_GREEN, TRIANGLE_OPACITY);
//
//    glUseProgram(g_shader_program.get_program_id());
//
//    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
//}
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
//void update()
//{
//    float ticks = (float) SDL_GetTicks() / 1000.0f; // current # of ticks
//    float delta_time = ticks - g_previous_ticks;    // time difference from the last frame
//    g_previous_ticks = ticks;
//    
//    // ——————————— PULSE TRANSFORMATIONS ——————————— //
//    // Instead of using g_frame_counter, use delta_time to accumulate time for pulsing.
//    g_pulse_time += delta_time * PULSE_SPEED; // Scale the pulse speed by delta_time
//    float scale_factor = BASE_SCALE + MAX_AMPLITUDE * glm::sin(g_pulse_time); // Use sine for a smoother pulse
//    
//    // ——————————— ORBIT TRANSFORMATIONS ——————————— //
//    // Use delta_time to make the orbiting independent of framerate
//    g_angle += ORBIT_SPEED * delta_time;  // increment g_angle by ORBIT_SPEED scaled by delta_time
//
//    // Calculate new x, y using trigonometry for orbit
//    g_x_offset = RADIUS * glm::cos(g_angle);
//    g_y_offset = RADIUS * glm::sin(g_angle);
//
//    // Reset the model matrix and apply transformations
//    g_model_matrix = glm::mat4(1.0f);
//    g_model_matrix = glm::translate(g_model_matrix, glm::vec3(g_x_offset, g_y_offset, 0.0f));
//    g_model_matrix = glm::scale(g_model_matrix, glm::vec3(scale_factor, scale_factor, 1.0f));
//}
//
//
//void render() {
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    g_shader_program.set_model_matrix(g_model_matrix);
//
//    float vertices[] =
//    {
//         0.5f, -0.5f,
//         0.0f,  0.5f,
//        -0.5f, -0.5f
//    };
//
//    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
//    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
//
//    SDL_GL_SwapWindow(g_display_window);
//}
//
//void shutdown() { SDL_Quit(); }
//
///**
// Start here—we can see the general structure of a game loop without worrying too much about the details yet.
// */
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
