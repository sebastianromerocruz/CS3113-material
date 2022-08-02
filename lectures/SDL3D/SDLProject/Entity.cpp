#include "Entity.h"

Entity::Entity()
{
    this->position     = glm::vec3(0.0f);
    this->acceleration = glm::vec3(0.0);
    this->velocity     = glm::vec3(0.0f);
    this->rotation     = glm::vec3(0.0f);
    
    this->model_matrix = glm::mat4(1.0f);
    
    this->speed = 0.0f;
}

void Entity::update(float delta_time)
{
    this->velocity += this->acceleration * delta_time;
    this->position += this->velocity * delta_time;
    
    if (this->entity_type == CUBE) this->rotation += glm::vec3(this->ROTATION_SPEED * delta_time, this->ROTATION_SPEED * delta_time, 0.0f);
    
    this->model_matrix = glm::mat4(1.0f);
    this->model_matrix = glm::translate(this->model_matrix, this->position);
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.x), glm::vec3(1.0, 1.0f, 0.0f));
}

void Entity::render(ShaderProgram *program)
{
    program->SetModelMatrix(this->model_matrix);
    
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    
    glVertexAttribPointer(program->positionAttribute, 3, GL_FLOAT, false, 0, this->vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, this->texture_coordinates);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, this->number_of_vertices);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

