#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Util
{
public:
    static GLuint load_texture(const char* file_path);
    static void draw_text(ShaderProgram *program, int font_texture_id, std::string text, float size, float spacing, glm::vec3 position);
};
