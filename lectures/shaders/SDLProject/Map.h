#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Map {
private:
    int width;
    int height;
    
    unsigned int *level_data;
    GLuint texture_id;
    
    float tile_size;
    int tile_count_x;
    int tile_count_y;
    
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;
    
    float left_bound, right_bound, top_bound, bottom_bound;
    
public:
    Map(int width, int height, unsigned int *level_data, GLuint texture_id, float tile_size, int
    tile_count_x, int tile_count_y);
    
    void build();
    void render(ShaderProgram *program);
    bool is_solid(glm::vec3 position, float *penetration_x, float *penetration_y);
    
    // Getters
    int const get_width()  const  { return this->width;  }
    int const get_height() const  { return this->height; }
    
    unsigned int* const get_level_data() const { return this->level_data; }
    GLuint        const get_texture_id() const { return this->texture_id; }
    
    float const get_tile_size() const { return this->tile_size; }
    int const get_tile_count_x() const { return this->tile_count_x; }
    int const get_tile_count_y() const { return this->tile_count_y; }
    
    std::vector<float> const get_vertices()            const { return this->vertices;             }
    std::vector<float> const get_texture_coordinates() const { return this-> texture_coordinates; }
    
    float const get_left_bound()   const { return this->left_bound;   }
    float const get_right_bound()  const { return this->right_bound;  }
    float const get_top_bound()    const { return this->top_bound;    }
    float const get_bottom_bound() const { return this->bottom_bound; }
};
