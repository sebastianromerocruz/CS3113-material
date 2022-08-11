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

enum EntityType { PLAYER, PLATFORM, ENEMY, CUBE, SHIP, FLOOR };
enum RenderMode { REGULAR, BILLBOARD };

class Entity
{
    const int ROTATION_SPEED = 45;
    
public:
    EntityType entity_type;
    RenderMode render_mode;
    
    Mesh* mesh;
    GLuint texture_id;
    glm::mat4 model_matrix;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    float speed;
    
    float width;
    float height;
    float depth;
    
    Entity();
    
    bool check_collision(Entity *other);
    void update(float delta_time, Entity *player, Entity *objects, int object_count);
    void render(ShaderProgram *program);
    void draw_billboard(ShaderProgram *program);
};
