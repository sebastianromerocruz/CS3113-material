enum Animation { IDLE, ATTACK };

class Entity
{
private:
   // ————— TEXTURES ————— //
   std::vector<GLuint> m_texture_ids;  // Vector of texture IDs for different animations

   // ————— ANIMATIONS ————— //
   std::vector<std::vector<int>> m_animations;  // Indices for each animation type

   glm::vec3 m_movement;
   glm::vec3 m_position;
   glm::vec3 m_scale;

   glm::mat4 m_model_matrix;
   float m_speed;
   
   int m_animation_cols;
   int m_animation_frames, m_animation_index, m_animation_rows;
   
   Animation m_current_animation;  // Current animation state
   int* m_animation_indices = nullptr;
   float m_animation_time = 0.0f;
   
public:
   static constexpr int SECONDS_PER_FRAME = 6;

   // ————— CONSTRUCTORS ————— //
   Entity();
   Entity(std::vector<GLuint> texture_ids, float speed, std::vector<std::vector<int>> animations,
          float animation_time, int animation_frames, int animation_index,
          int animation_cols, int animation_rows, Animation animation);
   ~Entity();

   // ————— METHODS ————— //
   void draw_sprite_from_texture_atlas(ShaderProgram* program);
   void update(float delta_time);
   void render(ShaderProgram* program);

   // Animation control
   void set_animation_state(Animation new_animation);
   void normalise_movement() { m_movement = glm::normalize(m_movement); };
   
   // Getters and Setters
   glm::vec3 const get_position() const { return m_position; }
   glm::vec3 const get_movement() const { return m_movement; }
   glm::vec3 const get_scale() const { return m_scale; }
   float const get_speed() const { return m_speed; }

   void const set_position(glm::vec3 new_position) { m_position = new_position; }
   void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; }
   void const set_scale(glm::vec3 new_scale) { m_scale = new_scale; }
   void const set_speed(float new_speed) { m_speed = new_speed; }
};
