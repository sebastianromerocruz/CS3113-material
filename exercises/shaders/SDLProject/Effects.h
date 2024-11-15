#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum EffectType { NONE, FADEIN, FADEOUT, GROW, SHRINK, SHAKE };

class Effects {
private:
    ShaderProgram m_shader_program;
    EffectType    m_current_effect;

    float m_alpha;
    float m_effect_speed;
    float m_size;
    glm::vec3 m_view_offset;

public:
    static constexpr float MAX_OFFSET = 0.025f;

    Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix);

    void draw_overlay();

    void start(EffectType effect_type);
    void start(EffectType effect_type, float effect_speed);

    void update(float delta_time);
    void render();

    glm::vec3 get_view_offset() const { return m_view_offset; };
};
