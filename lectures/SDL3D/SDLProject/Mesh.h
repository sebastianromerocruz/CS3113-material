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

class Mesh
{
private:
    std::vector<float> m_vertices;
    std::vector<float> m_uv_coordinates;
    std::vector<float> m_normals;
    
public:
    void render(ShaderProgram *program);
    void load_OBJ(const char *file_name);

    std::vector<float> const get_vertices()       const { return m_vertices;       };
    std::vector<float> const get_uv_coordinates() const { return m_uv_coordinates; };
    std::vector<float> const get_normals()        const { return m_normals;        };
    
    void set_vertices(std::vector<float> vertices)             { m_vertices = vertices;             };
    void set_uv_coordinates(std::vector<float> uv_coordinates) { m_uv_coordinates = uv_coordinates; };
    void set_normals(std::vector<float> normals)               { m_normals = normals;               };
};
