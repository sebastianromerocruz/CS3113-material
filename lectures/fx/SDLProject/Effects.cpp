#include "Effects.h"

Effects::Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
    // Non textured Shader
    m_program.load("shaders/vertex.glsl", "shaders/fragment.glsl");
    m_program.set_projection_matrix(projection_matrix);
    m_program.set_view_matrix(view_matrix);
    
    m_current_effect = NONE;
    m_alpha = 1.0f;
    m_effect_speed = 1.0;
    m_size = 10.0f;
    
    m_view_offset = glm::vec3(0.0f);
}

void Effects::draw_overlay()
{
    glUseProgram(this->m_program.get_program_id());

    float vertices[] =
    {
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        
        -0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5
    };

    glVertexAttribPointer(m_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(m_program.get_position_attribute());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(m_program.get_position_attribute());
}

void Effects::start(EffectType effect_type, float effect_speed)
{
    m_current_effect = effect_type;
    m_effect_speed = effect_speed;

    switch (m_current_effect)
    {
        case NONE:                         break;
        case FADEIN:  m_alpha     = 1.0f;  break;
        case FADEOUT: m_alpha     = 0.0f;  break;
        case GROW:    m_size      = 0.0f;  break;
        case SHRINK:  m_size      = 10.0f; break;
        case SHAKE:   m_time_left = 1.0f;  break;
    }
}


void Effects::update(float delta_time)
{
   switch (m_current_effect)
   {
       case NONE: break;
           
       // Fades
       case FADEIN:
           m_alpha -= delta_time * m_effect_speed;
           if (m_alpha <= 0) m_current_effect = NONE;
           
           break;
       case FADEOUT:
           if (m_alpha < 1.0f) m_alpha += delta_time * m_effect_speed;
           
           break;
           
       case GROW:
           if (m_size < 10.0f) m_size += delta_time * m_effect_speed; break;
           
       case SHRINK:
           if (m_size >= 0.0f) m_size -= delta_time * m_effect_speed;
           if (m_size < 0)     m_current_effect = NONE;
           
           break;
           
       case SHAKE:
           m_time_left -= delta_time * m_effect_speed;
           if (m_time_left <= 0.0f)
           {
               m_view_offset = glm::vec3(0.0f, 0.0f, 0.0f);
               m_current_effect = NONE;
           } else
           {
               float min = -0.1f;
               float max =  0.0f;
               float offset_value = ((float) rand() / RAND_MAX) * (max - min) + min;
               
               m_view_offset = glm::vec3(offset_value, offset_value, 0.0f);
           }
   }
}

void Effects::render()
{
    glm::mat4 model_matrix = glm::mat4(1.0f);

    switch (m_current_effect)
    {
        case NONE:
            break;
            
        case GROW:
        case SHRINK:
        case FADEOUT:
        case FADEIN:
            // Expand the current square a bit
            model_matrix = glm::scale(model_matrix,
                                      glm::vec3(m_size,
                                                m_current_effect != GROW && m_current_effect != SHRINK ?
                                                    m_size : m_size * 0.75f,
                                                0.0f));
            
            this->m_program.set_model_matrix(model_matrix);
            this->m_program.set_colour(0.0f, 0.0f, 0.0f, m_alpha);
            this->draw_overlay();

            break;
            
        case SHAKE:
            // Shake doesn't change the model matrix, so we're good here
            break;
    }
}
