#include "Entity.h"

Entity::Entity()
{
    m_position     = glm::vec3(0.0f);
    m_acceleration = glm::vec3(0.0);
    m_velocity     = glm::vec3(0.0f);
    m_rotation     = glm::vec3(0.0f);
    
    m_model_matrix = glm::mat4(1.0f);
    
    m_movement_speed = 0.0f;
}

void Entity::update(float delta_time)
{
    m_velocity += m_acceleration * delta_time;
    m_position += m_velocity * delta_time;
    
    if (m_entity_type == CUBE) m_rotation += glm::vec3(ROTATION_SPEED * delta_time, ROTATION_SPEED * delta_time, 0.0f);
    
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_rotation.x), glm::vec3(1.0, 1.0f, 0.0f));
}

void Entity::render(ShaderProgram *program)
{
    program->set_model_matrix(m_model_matrix);
    
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    
    glVertexAttribPointer(program->get_position_attribute(), 3, GL_FLOAT, false, 0, m_vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coord_attribute(), 2, GL_FLOAT, false, 0, m_texture_coordinates);
    glEnableVertexAttribArray(program->get_tex_coord_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, m_number_of_vertices);
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coord_attribute());
}
