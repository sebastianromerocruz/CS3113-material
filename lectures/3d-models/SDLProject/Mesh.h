#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1


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
    
        void Render(ShaderProgram *program);
        void LoadOBJ(const char *fileName);
    
        std::vector<float> vertices;
        std::vector<float> uvs;
        std::vector<float> normals;
};
