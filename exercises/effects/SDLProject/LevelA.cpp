#include "LevelA.h"
#include "Utility.h"
#include <vector>

#define LEVEL_HEIGHT 8

unsigned int LEVELA_DATA[] =
{
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 16, 16, 16, 16, 16, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 17, 17, 17, 17, 17, 12, 12, 12, 12, 12
};

LevelA::~LevelA()
{
    delete m_game_state.player;
    delete m_game_state.map;
}

void LevelA::initialise()
{
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/platformPack_tilesheet@2.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id,
                               1.0f, 14, 7);
    
    std::vector<std::vector<int>> animations =
    {
        { 0, 1, 2, 3, 4, 5 },  // Rest
        { 0, 1, 2, 3, 4 },     // Charging
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);
    
    std::vector<GLuint> texture_ids =
    {
        Utility::load_texture("assets/B_witch_idle.png"),
        Utility::load_texture("assets/B_witch_charge.png")
    };
    
    m_game_state.player = new Entity(
        texture_ids,               // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        animations,                // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        1,                         // animation column amount
        6,                         // animation row amount
        0.7f,                      // width
        0.7f,                      // height
        PLAYER,                    // entity type
        REST                       // player state
    );
        
    m_game_state.player->set_position(glm::vec3(6.0f, 0.0f, 0.0f));
}

void LevelA::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, nullptr, 0,
                                m_game_state.map);
    
    if (m_game_state.player->get_position().y < -10.0f) m_game_state.next_scene_id = 1;
}

void LevelA::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
}
