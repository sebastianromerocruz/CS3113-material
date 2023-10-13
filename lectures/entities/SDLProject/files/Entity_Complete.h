class Entity
{
private:
    int *m_animation_right = NULL, // move to the right
        *m_animation_left  = NULL, // move to the left
        *m_animation_up    = NULL, // move upwards
        *m_animation_down  = NULL; // move downwards
    
    // ————— TRANSFORMATIONS ————— //
    glm::vec3 m_movement;
    glm::vec3 m_position;
    glm::mat4 m_model_matrix;
    float     m_speed;
    
    // ————— TEXTURES ————— //
    GLuint    m_texture_id;
    
public:
    // ————— STATIC VARIABLES ————— //
    static const int SECONDS_PER_FRAME = 4;
    static const int LEFT  = 0,
                     RIGHT = 1,
                     UP    = 2,
                     DOWN  = 3;
    
    // ————— ANIMATION ————— //
    int **m_walking = new int*[4]
    {
        m_animation_left,
        m_animation_right,
        m_animation_up,
        m_animation_down
    };
    
    int m_animation_frames = 0,
        m_animation_index  = 0,
        m_animation_cols   = 0,
        m_animation_rows   = 0;
    
    int  *m_animation_indices = NULL;
    float m_animation_time    = 0.0f;

    // ————— METHODS ————— //
    Entity();
    ~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint texture_id, int index);
    void update(float delta_time);
    void render(ShaderProgram *program);
    
    void move_left()  { m_movement.x = -1.0f; };
    void move_right() { m_movement.x = 1.0f;  };
    void move_up()    { m_movement.y = 1.0f;  };
    void move_down()  { m_movement.y = -1.0f; };
    
    // ————— GETTERS ————— //
    glm::vec3 const get_position()   const { return m_position;   };
    glm::vec3 const get_movement()   const { return m_movement;   };
    GLuint    const get_texture_id() const { return m_texture_id; };
    float     const get_speed()      const { return m_speed;      };
    
    // ————— SETTERS ————— //
    void const set_position(glm::vec3 new_position)  { m_position   = new_position;     };
    void const set_movement(glm::vec3 new_movement)  { m_movement   = new_movement;     };
    void const set_texture_id(GLuint new_texture_id) { m_texture_id = new_texture_id;   };
    void const set_speed(float new_speed)            { m_speed      = new_speed;        };
};

