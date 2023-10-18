<h2 align=center>Lecture 11</h2>

<h1 align=center>Physics: <em>Gravity</em></h1>

<h3 align=center>8 Messidor, Year CCXXX</h3>

***Song of the day***: _[**Far far away**](https://youtu.be/2GM3X-GBt5E) by Yohane [CV: Aika Kobayashi] (2023)._

### Sections

1. [**Fixed Time-step**](#part-1-fixed-time-step)
2. [**Gravity**](#part-2-gravity)

### Part 1: _Fixed Time-Step_

Just like in real life, video game physics must follow very rigid, consistent laws. One of those axiomatic measures that must be foolproof is the amount of space a body moves over time. We took our first step towards standarising this by adopting delta time. Currently, however, our timestep is as fast as our computer can go, which makes it variable. For instance, if our computer slows down / speeds up for whatever reason, we might end up with something like this:

![skip-collision](assets/skip-collision.png)

<sub>**Figure 1**: By slowing down right before a collision, the block has re-rendered beyond the way, avoiding a collision altogether.</sub>

```c++
#define FIXED_TIMESTEP 0.0166666f
float g_time_accumulator = 0.0f;

void update()
{
    // ————— DELTA TIME ————— //
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks - g_previous_ticks; // the delta time is the difference from the last frame
    g_previous_ticks = ticks;

    // ————— FIXED TIMESTEP ————— //
    // STEP 1: Keep track of how much time has passed since last step
    delta_time += g_time_accumulator;

    // STEP 2: Accumulate the ammount of time passed while we're under our fixed timestep
    if (delta_time < FIXED_TIMESTEP)
    {
        g_time_accumulator = delta_time;
        return;
    }

    // STEP 3: Once we exceed our fixed timestep, apply that elapsed time into the objects' update function invocation
    while (delta_time >= FIXED_TIMESTEP)
    {
        // Notice that we're using FIXED_TIMESTEP as our delta time
        g_game_state.player->update(FIXED_TIMESTEP, g_game_state.platforms, PLATFORM_COUNT);
        delta_time -= FIXED_TIMESTEP;
    }

    g_time_accumulator = delta_time;
}
```

<sub>**Code Block 1**: Accounting for the possibility of our computer messing with "space".</sub>

This may not make a huge difference in the games we've seen and made so far, but you can certainly imagine let's-players clipping through the walls of your game by exploiting the fact that you didn't take this into account. The fun in that is only when you don't expect it.

### Part 2: _Gravity_

Gravity, as defined by physics, is an acceleration—the rate at which velocity increases with respect to time. For Planet Earth, this rate of change of another rate of change is usually approximated to be 9.81 m/s<sup>2</sup>. Now, since we have position, and we have acceleration, we must also have the middle derivative, velocity:

```c++
// Entity.h
class Entity
{
private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

public:
    // ————— GETTERS ————— //
    glm::vec3 const get_position()     const { return m_position; };
    glm::vec3 const get_velocity()     const { return m_velocity; };
    glm::vec3 const get_acceleration() const { return m_acceleration; };
    glm::vec3 const get_movement()     const { return m_movement; };

    // ————— SETTERS ————— //
    void const set_position(glm::vec3 new_position) { m_position = new_position; };
    void const set_velocity(glm::vec3 new_velocity) { m_velocity = new_velocity; };
    void const set_acceleration(glm::vec3 new_position) { m_acceleration = new_position; };
    void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; };
}
```
```c++
// Entity.cpp
Entity::Entity()
{
    m_position     = glm::vec3(0.0f);
    m_velocity     = glm::vec3(0.0f);
    m_acceleration = glm::vec3(0.0f);

    // More stuff...
}

void Entity::update(float delta_time)
{
    // Some stuff...

    // Our character moves from left to right, so they need an initial velocity
    m_velocity.x = m_movement.x * m_speed;
    
    // And we add the gravity next
    m_velocity += m_acceleration * delta_time;
    m_position += m_velocity * delta_time;

    // Apply transformation
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
}
```

```c++
// main.cpp
#define ACC_OF_GRAVITY -9.81f

void initialise()
{
    // Some code...

    // ————— PLAYER ————— //
    g_game_state.player = new Entity();
    g_game_state.player->set_position(glm::vec3(0.0f));
    g_game_state.player->set_movement(glm::vec3(0.0f));
    g_game_state.player->set_acceleration(glm::vec3(0.0f, ACC_OF_GRAVITY, 0.0f));
}
```

<sub>**Code Blocks 2, 3, and 4**: Keeping track of our three physical vectors. For this model, we are commenting out all user movement in the vertical direction.</sub>

Of course, our player will drop endlessly, so let's add a platform:

```c++
// main.cpp
#define PLATFORM_COUNT 3

struct GameState
{
    Entity* player;
    Entity* platforms;
};

void initialise()
{
    // ————— PLATFORM ————— //
    g_game_state.platforms = new Entity[PLATFORM_COUNT];

    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        g_game_state.platforms[i].m_texture_id = load_texture(PLATFORM_FILEPATH);
        g_game_state.platforms[i].set_position(glm::vec3(i - 1.0f, -3.0f, 0.0f));
        g_game_state.platforms[i].update(0.0f, NULL, 0);
    }
}

void render()
{
    for (int i = 0; i < PLATFORM_COUNT; i++) g_game_state.platforms[i].render(&g_shader_program);
}
```

<sub>**Code Block 5**: Adding a few platform entities.</sub>

And some simple collision detection between `Entity` objects:

```c++
// Entity.h
class Entity
{
private:
    // Some stuff...

    // This is an assumption based on our render() method's vertices
    // Think of using getters and setters to change these if necessary
    int m_width  = 1,
        m_height = 1;

public:
    bool const check_collision(Entity *other) const;

    // This is where keeping Entity objects nicely organised works nicely
    void update(float delta_time, Entity* collidable_entities, int entity_count);
}
```
```c++
// Entity.cpp
bool const Entity::check_collision(Entity* other) const
{
    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}

void Entity::update(float delta_time, Entity* collidable_entities, int entity_count)
{
    // Check for collisions
    for (int i = 0; i < entity_count; i++)
    {
        if (check_collision(&collidable_entities[i])) return;
    }
}
```
```c++
// main.cpp
void initialise()
{
    // ————— PLATFORM ————— //
    g_game_state.platforms = new Entity[PLATFORM_COUNT];

    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        g_game_state.platforms[i].m_texture_id = load_texture(PLATFORM_FILEPATH);
        g_game_state.platforms[i].set_position(glm::vec3(i - 1.0f, -3.0f, 0.0f));
        g_game_state.platforms[i].update(0.0f, NULL, 0);
    }
}

void update()
{
    while (delta_time >= FIXED_TIMESTEP)
    {
        g_game_state.player->update(FIXED_TIMESTEP, g_game_state.platforms, PLATFORM_COUNT);
        delta_time -= FIXED_TIMESTEP;
    }
}
```

<sub>**Code Blocks 5 and 6**: Collision detection in `Entity`'s `update()` method.</sub>

Of course, we are stuck, since we are blocking all movement at collision. We'll fix that soon enough.
