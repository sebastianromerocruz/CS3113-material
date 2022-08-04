#pragma once
#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Mesh.h"

enum EntityType { PLAYER, PLATFORM, ENEMY, CUBE, SHIP };

class Entity
{
    const int ROTATION_SPEED = 45;
    
public:
    EntityType entity_type;
    
    Mesh* mesh;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    
    float speed;
    
    GLuint texture_id;
//    float *vertices;
//    float *texture_coordinates;
//    int number_of_vertices;
    
    glm::mat4 model_matrix;
    
    Entity();
    
    void update(float delta_time);
    void render(ShaderProgram *program);
};
