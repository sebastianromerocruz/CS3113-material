enum EntityType { PLATFORM, PLAYER };

class Entity
{
private:
    bool m_is_active = true;
    
    // ––––– ANIMATION ––––– //
    int *m_animation_right = NULL; // move to the right
    int *m_animation_left  = NULL; // move to the left
    int *m_animation_up    = NULL; // move upwards
    int *m_animation_down  = NULL; // move downwards
    
    // ––––– PHYSICS (GRAVITY) ––––– //
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;
    
    float m_width  = 1;
    float m_height = 1;
    
public:
    // ––––– STATIC ATTRIBUTES ––––– //
    static const int SECONDS_PER_FRAME = 4;
    static const int LEFT  = 0,
                     RIGHT = 1,
                     UP    = 2,
                     DOWN  = 3;
    
    // ––––– SETUP AND RENDERING ––––– //
    GLuint m_texture_id;
    glm::mat4 m_model_matrix;
    EntityType m_type;
    
    // ––––– TRANSLATIONS ––––– //
    float m_speed;
    glm::vec3 m_movement;
    
    // ––––– ANIMATIONS ––––– //
    int **m_walking          = new int*[4] { m_animation_left, m_animation_right, m_animation_up, m_animation_down };
    int *m_animation_indices = NULL;
    int m_animation_frames   = 0;
    int m_animation_index    = 0;
    float m_animation_time   = 0.0f;
    int m_animation_cols     = 0;
    int m_animation_rows     = 0;
    
    // ––––– PHYSICS (JUMPING) ––––– //
    bool m_is_jumping     = false;
    float m_jumping_power = 0;
    
    // ––––– PHYSICS (COLLISIONS) ––––– //
    bool m_collided_top    = false;
    bool m_collided_bottom = false;
    bool m_collided_left   = false;
    bool m_collided_right  = false;

    // ––––– METHODS ––––– //
    Entity();
    ~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint texture_id, int index);
    void update(float delta_time, Entity *collidable_entities, int collidable_entity_count);
    void render(ShaderProgram *program);
    
    void const check_collision_y(Entity *collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity *collidable_entities, int collidable_entity_count);
    bool const check_collision(Entity *other) const;
    
    void activate()   { m_is_active = true;  };
    void deactivate() { m_is_active = false; };
    
    // ––––– GETTERS ––––– //
    glm::vec3  const get_position()     const { return m_position;     };
    glm::vec3  const get_movement()     const { return m_movement;     };
    glm::vec3  const get_velocity()     const { return m_velocity;     };
    glm::vec3  const get_acceleration() const { return m_acceleration; };
    int        const get_width()        const { return m_width;        };
    int        const get_height()       const { return m_height;       };
    EntityType const get_entity_type()  const { return m_type;         };
    
    // ––––– SETTERS ––––– //
    void const set_position(glm::vec3 new_position)         { m_position = new_position;         };
    void const set_movement(glm::vec3 new_movement)         { m_movement = new_movement;         };
    void const set_velocity(glm::vec3 new_velocity)         { m_velocity = new_velocity;         };
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration = new_acceleration; };
    void const set_width(float new_width)                   { m_width  = new_width;              };
    void const set_height(float new_height)                 { m_height = new_height;             };
    void const set_entity_type(EntityType new_type)         { m_type = new_type;                 };
};
