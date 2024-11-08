//#pragma once
//#define GL_SILENCE_DEPRECATION
//
//#ifdef _WINDOWS
//#include <GL/glew.h>
//#endif
//
//#define GL_GLEXT_PROTOTYPES 1
//#include <vector>
//#include <math.h>
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include "glm/mat4x4.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "ShaderProgram.h"
//
//enum EffectType { NONE, FADEIN, FADEOUT, GROW, SHRINK };
//
//class Effects {
//private:
//    ShaderProgram m_shader_program;
//    EffectType    m_current_effect;
//    
//    float m_alpha;
//    float m_effect_speed;
//    float m_size;
//    
//public:
//    static constexpr float MAX_OFFSET = 0.025f;
//    static constexpr int OVERLAY_SIZE = 10;
//    
//    Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix);
//
//    void draw_overlay();
//    
//    void start(EffectType effect_type);
//    void start(EffectType effect_type, float effect_speed);
//    
//    void update(float delta_time);
//    void render();
//};
