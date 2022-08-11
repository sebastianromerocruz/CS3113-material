#pragma once
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

class Mesh {
    public:
        void render(ShaderProgram *program);
        void load_OBJ(const char *file_name, float texture_repeat_rate);
    
        std::vector<float> vertices;
        std::vector<float> uvs;
        std::vector<float> normals;
};
