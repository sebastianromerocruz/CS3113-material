enum EntityType { PLATFORM, PLAYER, ENEMY  };

class Entity
{
private:
    bool m_is_active = true;
    
    EntityType m_entity_type;
    
    int *m_animation_right = NULL; // move to the right
    int *m_animation_left  = NULL; // move to the left
    int *m_animation_up    = NULL; // move upwards
    int *m_animation_down  = NULL; // move downwards
    
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;
    
    float m_width  = 1;
    float m_height = 1;
    
public:
    // Static attributes
    static const int SECONDS_PER_FRAME = 4;
    static const int LEFT  = 0,
    RIGHT = 1,
    UP    = 2,
    DOWN  = 3;
    
    // Existing
    GLuint    m_texture_id;
    glm::mat4 m_model_matrix;
    
    // Translating
    float     m_speed;
    glm::vec3 m_movement;
    
    // Animating
    int **m_walking            = new int*[4] { m_animation_left, m_animation_right, m_animation_up, m_animation_down };
    int  *m_animation_indices  = NULL;
    int   m_animation_frames   = 0;
    int   m_animation_index    = 0;
    float m_animation_time     = 0.0f;
    int   m_animation_cols     = 0;
    int   m_animation_rows     = 0;
    
    // Jumping
    bool  m_is_jumping    = false;
    float m_jumping_power = 0;
    
    // Colliding
    bool m_collided_top    = false;
    bool m_collided_bottom = false;
    bool m_collided_left   = false;
    bool m_collided_right  = false;
    
    // Methods
    Entity();
    ~Entity();
    
    void draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint texture_id, int index);
    void update(float delta_time, Entity *player, Entity *collidable_entities, int collidable_entity_count);
    void render(ShaderProgram *program);
    
    void const check_collision_y(Entity *collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity *collidable_entities, int collidable_entity_count);
    bool const check_collision(Entity *other) const;
    
    void move_left()  { m_movement.x = -1.0f; };
    void move_right() { m_movement.x = 1.0f;  };
    void move_up()    { m_movement.y = 1.0f;  };
    void move_down()  { m_movement.y = -1.0f; };
    
    void activate()   { m_is_active = true;  };
    void deactivate() { m_is_active = false; };
    
    EntityType const get_entity_type()    const { return m_entity_type;   };
    glm::vec3  const get_position()       const { return m_position;      };
    glm::vec3  const get_movement()       const { return m_movement;      };
    glm::vec3  const get_velocity()       const { return m_velocity;      };
    glm::vec3  const get_acceleration()   const { return m_acceleration;  };
    float      const get_jumping_power () const { return m_jumping_power; };
    float      const get_speed()          const { return m_speed;         };
    int        const get_width()          const { return m_width;         };
    int        const get_height()         const { return m_height;        };
    
    void const set_entity_type(EntityType new_entity_type)  { m_entity_type   = new_entity_type;      };
    void const set_position(glm::vec3 new_position)         { m_position      = new_position;         };
    void const set_movement(glm::vec3 new_movement)         { m_movement      = new_movement;         };
    void const set_velocity(glm::vec3 new_velocity)         { m_velocity      = new_velocity;         };
    void const set_speed(float new_speed)                   { m_speed         = new_speed;            };
    void const set_jumping_power(float new_jumping_power)   { m_jumping_power = new_jumping_power;   };
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration  = new_acceleration;     };
    void const set_width(float new_width)                   { m_width         = new_width;            };
    void const set_height(float new_height)                 { m_height        = new_height;           };
};

