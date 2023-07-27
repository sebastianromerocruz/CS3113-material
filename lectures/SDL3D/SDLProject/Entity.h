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

enum EntityType { PLAYER, PLATFORM, ENEMY, CUBE };

class Entity
{
    const int ROTATION_SPEED = 45;
private:
    float     *m_vertices;
    float     *m_texture_coordinates;
    
    EntityType m_entity_type;
    
    glm::vec3  m_position;
    glm::vec3  m_velocity;
    glm::vec3  m_acceleration;
    glm::vec3  m_rotation;
    
    float      m_movement_speed;
    
    GLuint     m_texture_id;
    int        m_number_of_vertices;
    
    glm::mat4  m_model_matrix;
    
public:
    Entity();
    
    void update(float delta_time);
    void render(ShaderProgram *program);
    
    EntityType const get_entity_type()        const { return m_entity_type;        };
    glm::mat4  const get_model_matrix()       const { return m_model_matrix;       };
    glm::vec3  const get_position()           const { return m_position;           };
    glm::vec3  const get_velocity()           const { return m_velocity;           };
    glm::vec3  const get_rotation()           const { return m_rotation;           };
    glm::vec3  const get_acceleration()       const { return m_acceleration;       };
    float      const get_movement_speed()     const { return m_movement_speed;     };
    GLuint     const get_texture_id()         const { return m_texture_id;         };
    int        const get_number_of_vertices() const { return m_number_of_vertices; };
    
    void set_vertices(float *vertices)                       { m_vertices            = vertices;            };
    void set_texture_coordinates(float *texture_coordinates) { m_texture_coordinates = texture_coordinates; }
    void set_number_of_vertices(int number_of_vertices)      { m_number_of_vertices  = number_of_vertices;  };
    void set_movement_speed(float new_speed)                 { m_movement_speed      = new_speed;           };
    void set_texture_id(GLuint texture_id)                   { m_texture_id          = texture_id;          };
    void set_entity_type(EntityType entity_type)             { m_entity_type         = entity_type;         };
    void set_model_matrix(glm::mat4 matrix)                  { m_model_matrix        = matrix;              };
    void set_velocity(glm::vec3 new_velocity)                { m_velocity            = new_velocity;        };
    void set_rotation(glm::vec3 new_rotation)                { m_rotation            = new_rotation;        };
    void set_acceleration(glm::vec3 new_acceleration)        { m_acceleration        = new_acceleration;    };
    void set_position(glm::vec3 new_position)                { m_position            = new_position;        };
};
