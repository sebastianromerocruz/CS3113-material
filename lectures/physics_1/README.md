## Lecture 10

# [**Physics**](SDLProject/main.cpp)

### 17 Messidor, Year CCXXX

***Song of the day***: _[**Bootlegger Boogie**](https://youtu.be/6Cs54V-F0c8) by Kristofer Maddigan (2022)._

### Sections

1. [**Fixed Time-step**](#part-1-fixed-time-step)
2. [**Gravity**](#part-2-gravity)

### Part 1: _Fixed Time-Step_

Just like in real life, video game physics must follow very rigid, consistent laws. One of those axiomatic measures that must be foolproof is the amount of space a body moves over time. We took our first step towards standarising this by adopting delta time. Currently, however, our timestep is as fast as our computer can go, which makes it variable. For instance, if our computer slows down / speeds up for whatever reason, we might end up with something like this:

![skip-collision](assets/skip-collision.png)

<sub>**Figure 1**: By slowing down right before a collision, the block has re-rendered beyond the way, avoiding a collision altogether.</sub>

```c++
#define FIXED_TIMESTEP 0.0166666f
float accumulator = 0.0f;

void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - previous_ticks;
    previous_ticks = ticks;

    // STEP 1: Keep track of how much time has passed since the last step    
    delta_time += accumulator;
    
    // STEP 2: Accumulate the amount of time passed while we're under our fixed timestep
    if (delta_time < FIXED_TIMESTEP)
    {
        accumulator = delta_time;
        return;
    }
    
    // STEP 3: Once we exceed that time apply that elapsed time into the objects' update
    while (delta_time >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not delta_time
        state.player->update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        delta_time -= FIXED_TIMESTEP;
    }
    
    accumulator = delta_time;
}
```

<sub>**Code Block 1**: Accounting for the possibility of our computer messing with "space".</sub>

This may not make a huge difference in the games we've seen and made so far, but you can certainly imagine let's-players clipping through the walls of your game by exploiting the fact that you didn't take this into account. The fun in that is only when you don't expect it.

### Part 2: _Gravity_

Gravity, as defined by physics, is an acceleration—the rate at which velocity increases with respect to time. For Planet Earth, this rate of change of another rate of change is usually approximated to be 9.81 m/s<sup>2</sup>. Now, since we have position, and we have acceleration, we must also have the middle derivative, velocity:

```c++
class Entity
{
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

// More stuff...

public:

}

Entity::Entity()
{
    position     = glm::vec3(0.0f);
    velocity     = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);

    // More stuff...
}

void Entity::update(float delta_time)
{
    // Some stuff...

    // Our character moves from left to right, so they need an initial velocity
    velocity.x = movement.x * speed;
    
    // Now we add the rest of the gravity physics
    velocity += acceleration * delta_time;
    position += velocity * delta_time;
}
```

```c++
void initialise()
{
    // Some code...

    state.player->acceleration = glm::vec3(0.0f, -0.981f, 0.0f);
}
```

<sub>**Code Blocks 2 and 3**: Keeping track of our three physical vectors. For this model, we are commenting out all user movement in the vertical direction.</sub>

Of course, our player will drop endlessly, so let's add a platform:

```c++
struct GameState
{
    Entity* player;
    Entity* platforms;
};

void initialise()
{
    GLuint platform_texture_id = load_texture(PLATFORM_FILEPATH);
    
    state.platforms = new Entity[PLATFORM_COUNT];
    
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        state.platforms[i].texture_id = platform_texture_id;
        state.platforms[i].set_position(glm::vec3(i - 1.0f, -3.0f, 0.0f));
        state.platforms[i].update(0.0f, NULL, 0);
    }
}

void update()
{
    for (int i = 0; i < PLATFORM_COUNT; i++) state.platforms[i].render(&program);
}
```

<sub>**Code Block 4**: Adding a few platform entities.</sub>

And some simple detection between `Entity` objects:

```c++
class Entity
{
private:
    // Some stuff...

    // This is a lucky assumption—it will vary with each sprite
    // Think of using getters and setters to change these
    int width = 1;
    int height = 1;

public:
    bool const check_collision(Entity *other) const;

    // This is where keeping Entity objects nicely organised works nicely
    void update(float delta_time, Entity *collidable_entities, int collidable_intity_count);
}

bool const Entity::check_collision(Entity *other) const
{
    float x_distance = fabs(position.x - other->position.x) - ((width  + other->width)  / 2.0f);
    float y_distance = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    
    return x_distance < 0.0f && y_distance < 0.0f;
}

void Entity::update(float delta_time, Entity *collidable_entities, int collidable_entity_count)
{
    // Check for collisions
    for (int i = 0; i < collidable_entity_count; i++)
    {
        if (check_collision(&collidable_entities[i])) return;
    }
}
```

```c++
void initialise()
{
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        state.platforms[i].texture_id = platform_texture_id;
        state.platforms[i].set_position(glm::vec3(i - 1.0f, -3.0f, 0.0f));
        state.platforms[i].update(0.0f, NULL, 0);
    }
}

void update()
{
    while (delta_time >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        delta_time -= FIXED_TIMESTEP;
    }
}
```

<sub>**Code Blocks 5 and 6**: Collision detection in `Entity`'s `update()` method.</sub>

Of course, we are stuck, since we are blocking all movement at collision. See if you can fix that.