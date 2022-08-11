#include "Entity.h"

Entity::Entity()
{
    this->position     = glm::vec3(0.0f);
    this->acceleration = glm::vec3(0.0f);
    this->velocity     = glm::vec3(0.0f);
    this->rotation     = glm::vec3(0.0f);
    this->scale        = glm::vec3(1.0f);
    
    this->model_matrix = glm::mat4(1.0f);
    
    this->width  = 1.0f;
    this->height = 1.0f;
    this->depth  = 1.0f;
    
    this->speed = 0.0f;
}

bool Entity::check_collision(Entity *other)
{
    float x_distance = fabs(this->position.x - other->position.x) - ((this->width  + other->width)  / 2.0f);
    float y_distance = fabs(this->position.y - other->position.y) - ((this->height + other->height) / 2.0f);
    float z_distance = fabs(this->position.z - other->position.z) - ((this->depth  + other->depth)  / 2.0f);
    
    if (x_distance < 0 && y_distance < 0 && z_distance < 0) return true;
    
    return false;
}

void Entity::update(float delta_time, Entity *player, Entity *objects, int object_count)
{
    glm::vec3 previous_position = this->position;
    
    this->velocity += this->acceleration * delta_time;
    this->position += this->velocity * delta_time;
    
    if (this->render_mode == BILLBOARD)
    {
        float direction_x = this->position.x - player->position.x;
        float direction_y = this->position.z - player->position.z;
        this->rotation.y = glm::degrees(atan2f(direction_x, direction_y));
    }
    
    if (this->entity_type == PLAYER)
    {
        for (int i = 0; i < object_count; i++)
        {
            // Ignore collisions with the floor
            if (objects[i].entity_type == FLOOR) continue;
            
            if (this->check_collision(&objects[i]))
            {
                this->position = previous_position;
                break;
            }
        }
    }
    
    this->model_matrix = glm::mat4(1.0f);
    this->model_matrix = glm::translate(this->model_matrix, this->position);
    this->model_matrix = glm::scale(this->model_matrix, this->scale);
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Entity::render(ShaderProgram *program)
{
    program->SetModelMatrix(this->model_matrix);
    
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    
    switch(this->render_mode)
    {
        case BILLBOARD:
            this->draw_billboard(program);
            break;
            
        case REGULAR:
            this->mesh->render(program);
            break;
    }
}

void Entity::draw_billboard(ShaderProgram *program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texture_coordinates[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
