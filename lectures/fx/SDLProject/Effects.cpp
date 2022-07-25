#include "Effects.h"

Effects::Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
    // Non textured Shader
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);
    
    this->current_effect = NONE;
    this->alpha = 1.0f;
    this->effect_speed = 1.0;
    this->size = 10.0f;
    
    this->view_offset = glm::vec3(0.0f);
}

void Effects::draw_overlay()
{
    glUseProgram(this->program.programID);

    float vertices[] =
    {
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        
        -0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5
    };

    glVertexAttribPointer(this->program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(this->program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(this->program.positionAttribute);
}

void Effects::start(EffectType effect_type, float effect_speed)
{
    this->current_effect = effect_type;
    this->effect_speed = effect_speed;

    switch (this->current_effect)
    {
        case NONE:                             break;
        case FADEIN:  this->alpha     = 1.0f;  break;
        case FADEOUT: this->alpha     = 0.0f;  break;
        case GROW:    this->size      = 0.0f;  break;
        case SHRINK:  this->size      = 10.0f; break;
        case SHAKE:   this->time_left = 1.0f;  break;
    }
}


void Effects::update(float delta_time)
{
   switch (this->current_effect)
   {
       case NONE: break;
           
       // Fades
       case FADEIN:
           this->alpha -= delta_time * this->effect_speed;
           if (this->alpha <= 0) this->current_effect = NONE;
           
           break;
       case FADEOUT:
           if (this->alpha < 1.0f) this->alpha += delta_time * this->effect_speed;
           
           break;
           
       case GROW:
           if (this->size < 10.0f) this->size += delta_time * this->effect_speed; break;
           
       case SHRINK:
           if (this->size >= 0.0f) this->size -= delta_time * this->effect_speed;
           if (this->size < 0) this->current_effect = NONE;
           
           break;
           
       case SHAKE:
           this->time_left -= delta_time * this->effect_speed;
           if (this->time_left <= 0.0f)
           {
               this->view_offset = glm::vec3(0.0f, 0.0f, 0.0f);
               this->current_effect = NONE;
           } else
           {
               float min = -0.1f;
               float max =  0.0f;
               float offset_value = ((float) rand() / RAND_MAX) * (max - min) + min;
               this->view_offset = glm::vec3(offset_value, offset_value, 0.0f);
           }
   }
}

void Effects::render()
{
    glm::mat4 model_matrix = glm::mat4(1.0f);

    switch (this->current_effect)
    {
        case NONE:
            break;
            
        case GROW:
        case SHRINK:
        case FADEOUT:
        case FADEIN:
            // Expand the current square a bit
            model_matrix = glm::scale(model_matrix,
                                      glm::vec3(this->size,
                                                this->current_effect != GROW && this->current_effect != SHRINK ?
                                                    this->size : this->size * 0.75f,
                                                0.0f));
            
            this->program.SetModelMatrix(model_matrix);
            this->program.SetColor(0.0f, 0.0f, 0.0f, this->alpha);
            this->draw_overlay();

            break;
            
        case SHAKE:
            // Shake doesn't change the model matrix, so we're good here
            break;
    }
}
