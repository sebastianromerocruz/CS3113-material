#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>
#include <vector>

#define LOG(argument) std::cout << argument << '\n'

const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

const float BG_RED     = 0.1922f,
            BG_BLUE    = 0.549f,
            BG_GREEN   = 0.9059f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;

const char SPRITESHEET_FILEPATH[] = "assets/george_0.png";

const float MINIMUM_COLLISION_DISTANCE = 1.0f;

const int NUMBER_OF_TEXTURES = 1; // to be generated, that is
const GLint LEVEL_OF_DETAIL  = 0,  // base image level; Level n is the nth mipmap reduction image
            TEXTURE_BORDER   = 0;   // this value MUST be zero

/** NEW STUFF: */
const int SPRITESHEET_DIMENSIONS = 4;
const int SECONDS_PER_FRAME = 4;
const int LEFT  = 0,
          RIGHT = 1,
          UP    = 2,
          DOWN  = 3;

int *ANIMATION_RIGHT = new int[4] { 3, 7, 11, 15 }; // for George to move to the right
int *ANIMATION_LEFT  = new int[4] { 1, 5, 9,  13 }; // for George to move to the left
int *ANIMATION_UP    = new int[4] { 2, 6, 10, 14 }; // for George to move upwards
int *ANIMATION_DOWN  = new int[4] { 0, 4, 8,  12 }; // for George to move downwards

int **GEORGE_WALKING = new int*[4]
{
    ANIMATION_LEFT, ANIMATION_RIGHT, ANIMATION_UP, ANIMATION_DOWN
};

const int FONTBANK_SIZE = 16;

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_program;
glm::mat4 g_view_matrix,
          g_model_matrix,
          g_projection_matrix;

float previous_ticks = 0.0f;

GLuint g_player_texture_id;
glm::vec3 g_player_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_player_movement = glm::vec3(0.0f, 0.0f, 0.0f);

float g_player_speed = 1.0f;  // move 1 unit per second

int *g_animation_indices = GEORGE_WALKING[DOWN];  // start George looking down
int g_animation_frames = SPRITESHEET_DIMENSIONS;
int g_animation_index = 0;

float g_animation_time = 0.0f;

GLuint load_texture(const char* filepath)
{
    // STEP 1: Loading the image file
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
    
    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        assert(false);
    }
    
    // STEP 2: Generating and binding a texture ID to our image
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    // STEP 3: Setting our texture filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    // STEP 4: Setting our texture wrapping modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // the last argument can change depending on what you are looking for
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // STEP 5: Releasing our file from memory and returning our texture id
    stbi_image_free(image);
    
    return textureID;
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Hello, Spritesheets!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_program.Load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_model_matrix = glm::mat4(1.0f);
    
    g_view_matrix = glm::mat4(1.0f);  // Defines the position (location and orientation) of the camera
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);  // Defines the characteristics of your camera, such as clip planes, field of view, projection method etc.
    
    g_program.SetProjectionMatrix(g_projection_matrix);
    g_program.SetViewMatrix(g_view_matrix);
    
    glUseProgram(g_program.programID);
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
    g_player_texture_id = load_texture(SPRITESHEET_FILEPATH);
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_player_movement = glm::vec3(0.0f);
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        g_player_movement.x = 1.0f;
                        break;
                        
                    case SDLK_q:
                        // Quit the game with a keystroke
                        g_game_is_running = false;
                        break;
                        
                    default:
                        break;
                }
                
            default:
                break;
        }
    }
    
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_LEFT])
    {
        g_player_movement.x = -1.0f;
        g_animation_indices = GEORGE_WALKING[LEFT];
    }
    else if (key_state[SDL_SCANCODE_RIGHT])
    {
        g_player_movement.x = 1.0f;
        g_animation_indices = GEORGE_WALKING[RIGHT];
    }
    
    if (key_state[SDL_SCANCODE_UP])
    {
        g_player_movement.y = 1.0f;
        g_animation_indices = GEORGE_WALKING[UP];
    }
    else if (key_state[SDL_SCANCODE_DOWN])
    {
        g_player_movement.y = -1.0f;
        g_animation_indices = GEORGE_WALKING[DOWN];
    }
    
    // This makes sure that the player can't move faster diagonally
    if (glm::length(g_player_movement) > 1.0f)
    {
        g_player_movement = glm::normalize(g_player_movement);
    }
}

/**
 Uses distance formula.
 */
bool check_collision(glm::vec3 &position_a, glm::vec3 &position_b)
{
    return sqrt(pow(position_b[0] - position_a[0], 2) + pow(position_b[1] - position_b[1], 2)) < MINIMUM_COLLISION_DISTANCE;
}

void update()
{
    float ticks = (float) SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks - previous_ticks; // the delta time is the difference from the last frame
    previous_ticks = ticks;

    // Add direction * units per second * elapsed time
    g_player_position += g_player_movement * g_player_speed * delta_time;
    
    g_model_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::translate(g_model_matrix, g_player_position);
    
    if (glm::length(g_player_movement) != 0)
    {
        g_animation_time += delta_time;
        float frames_per_second = (float) 1 / SECONDS_PER_FRAME;
        
        if (g_animation_time >= frames_per_second)
        {
            g_animation_time = 0.0f;
            g_animation_index++;
            
            if (g_animation_index >= g_animation_frames)
            {
                g_animation_index = 0;
            }
        }
    }
}

void draw_object(glm::mat4 &object_model_matrix, GLuint &object_texture_id)
{
    g_program.SetModelMatrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6); // we are now drawing 2 triangles, so we use 6 instead of 3
}

void draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint texture_id, int index, int rows, int cols)
{
    // Step 1: Calculate the UV location of the indexed frame
    float u_coord = (float) (index % cols) / (float) cols;
    float v_coord = (float) (index / cols) / (float) rows;
    
    // Step 2: Calculate its UV size
    float width = 1.0f / (float) cols;
    float height = 1.0f / (float) rows;
    
    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };
    
    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };
    
    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void DrawText(ShaderProgram *program, GLuint font_texture_id, std::string text, float screen_size, float spacing, glm::vec3 position)
{
    // Scale the size of the fontbank in the UV-plane
    // We will use this for spacing and positioning
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;

    // Instead of having a single pair of arrays, we'll have a series of pairs—one for each character
    // Don't forget to include <vector>!
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;

    // For every character...
    for (int i = 0; i < text.size(); i++) {
        // 1. Get their index in the spritesheet, as well as their offset (i.e. their position
        //    relative to the whole sentence)
        int spritesheet_index = (int) text[i];  // ascii value of character
        float offset = (screen_size + spacing) * i;
        
        // 2. Using the spritesheet index, we can calculate our U- and V-coordinates
        float u_coordinate = (float) (spritesheet_index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float v_coordinate = (float) (spritesheet_index / FONTBANK_SIZE) / FONTBANK_SIZE;

        // 3. Inset the current pair in both vectors
        vertices.insert(vertices.end(), {
            offset + (-0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (0.5f * screen_size), -0.5f * screen_size,
            offset + (0.5f * screen_size), 0.5f * screen_size,
            offset + (-0.5f * screen_size), -0.5f * screen_size,
        });

        texture_coordinates.insert(texture_coordinates.end(), {
            u_coordinate, v_coordinate,
            u_coordinate, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate + width, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate, v_coordinate + height,
        });
    }

    // 4. And render all of them using the pairs
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);
    
    program->SetModelMatrix(model_matrix);
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, font_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int) (text.size() * 6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
  
    g_program.SetModelMatrix(g_model_matrix);
    draw_sprite_from_texture_atlas(&g_program, g_player_texture_id,
                                   g_animation_indices[g_animation_index],
                                   SPRITESHEET_DIMENSIONS, SPRITESHEET_DIMENSIONS);
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    delete [] ANIMATION_LEFT;
    delete [] ANIMATION_RIGHT;
    delete [] ANIMATION_UP;
    delete [] ANIMATION_DOWN;
    delete [] GEORGE_WALKING;
    
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

