## Lecture 14

# [**Tilesets and Tile Maps**](SDLProject/main.cpp)

### 1 Thermidor, Year CCXXX

***Song of the day***: _[**Flower Tower**](https://youtu.be/Rwzy6Qt8gq8) performed by Sayuri (2022)._

---

### Sections

1. [**Optimising Our Platforms**](#part-1-optimising-our-platforms)

### Part 1: **Optimising Our Platforms**

We've been, throughout the semester, constantly optiminsing on the structures that we introduce. The `Entity` class, spritesheets, etc. are all data structures that help abstract away a lot of the work that repeats in game design patterns. So what is next in our optimisations? So far, our map building scheme looks a bit inefficient. For example, to build a simple platformer map, we did the following a couple of lectures ago:

```c++
state.platforms = new Entity[PLATFORM_COUNT];
    
state.platforms[PLATFORM_COUNT - 1].texture_id = platform_texture_id;
state.platforms[PLATFORM_COUNT - 1].set_position(glm::vec3(-1.5f, -2.35f, 0.0f));
state.platforms[PLATFORM_COUNT - 1].set_width(0.4f);
state.platforms[PLATFORM_COUNT - 1].update(0.0f, NULL, 0);

for (int i = 0; i < PLATFORM_COUNT - 2; i++)
{
    state.platforms[i].texture_id = platform_texture_id;
    state.platforms[i].set_position(glm::vec3(i - 1.0f, -3.0f, 0.0f));
    state.platforms[i].set_width(0.4f);
    state.platforms[i].update(0.0f, NULL, 0);
}

state.platforms[PLATFORM_COUNT - 2].texture_id = platform_texture_id;
state.platforms[PLATFORM_COUNT - 2].set_position(glm::vec3(2.5f, -2.5f, 0.0f));
state.platforms[PLATFORM_COUNT - 2].set_width(0.4f);
state.platforms[PLATFORM_COUNT - 2].update(0.0f, NULL, 0);
```

<sub>**Code Block 1**: Not exactly modular.</sub>

So how do we fix this? The answer is the analogue of the spritesheet: the tilesheet.

![tileset](assets/tileset_link.gif)
![tilemap](assets/tilemap.png)

<sub>**Figures 1 and 2**: A tilemap built from a tileset.</sub>

Here, figure 2 is built from repeating patters of tiles from figure 1. The way we do this is by assigning each tile a number, and then telling the program which number we would like rendered onto the screen at a specific location. So, for instance, if our tileset looks like this:

![tileset-example](assets/tileset-example.png)

We could potentially build a world like this one:

![world-example](assets/world-example.png)

By doing the following:

![tilemap-example](assets/tilemap-example.png)
![tilemap-numbered](assets/numbered-tilemap.png)

```c++
 unsigned int level_data[] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 1, 1, 1, 1,
   1, 1, 1, 1, 0, 0, 1, 1, 1, 3, 2, 2, 2, 2,
   2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2
};
```

This actually works in a very similar way to how we build and render text—it's simply evenly spaced sprite rendering with images of platforms instead of images of characters. For this, we will create a new class, `Map`, that will help us do this:

```c++
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
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Map {
private:
    int width;
    int height;
    
    unsigned int *level_data;
    GLuint texture_id;
    
    float tile_size;
    int tile_count_x;
    int tile_count_y;
    
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;
    
    float left_bound, right_bound, top_bound, bottom_bound;
    
public:
    // Constructor
    Map(int width, int height, unsigned int *level_data, GLuint texture_id, float tile_size, int
    tile_count_x, int tile_count_y);
    
    // Methods
    void build();
    void render(ShaderProgram *program);
    
    // Getters
    int const get_width()  const  { return this->width;  }
    int const get_height() const  { return this->height; }
    
    unsigned int* const get_level_data() const { return this->level_data; }
    GLuint        const get_texture_id() const { return this->texture_id; }
    
    float const get_tile_size() const { return this->tile_size; }
    int const get_tile_count_x() const { return this->tile_count_x; }
    int const get_tile_count_y() const { return this->tile_count_y; }
    
    std::vector<float> const get_vertices()            const { return this->vertices;             }
    std::vector<float> const get_texture_coordinates() const { return this-> texture_coordinates; }
    
    float const get_left_bound()   const { return this->left_bound;   }
    float const get_right_bound()  const { return this->right_bound;  }
    float const get_top_bound()    const { return this->top_bound;    }
    float const get_bottom_bound() const { return this->bottom_bound; }
};
```
```c++
#include "Map.h"

Map::Map(int width, int height, unsigned int *level_data, GLuint texture_id, float tile_size, int tile_count_x, int tile_count_y)
{
    this->width = width;
    this->height = height;
    
    this->level_data = level_data;
    this->texture_id = texture_id;
    
    this->tile_size = tile_size;
    this->tile_count_x = tile_count_x;
    this->tile_count_y = tile_count_y;
    
    this->build();
}

void Map::build()
{
    for(int y = 0; y < this->height; y++)
    {
        for(int x = 0; x < this->width; x++) {
            int tile = this->level_data[y * this->width + x];
            
            if (tile == 0) continue;
            
            float u = (float) (tile % this->tile_count_x) / (float) this->tile_count_x;
            float v = (float) (tile / this->tile_count_x) / (float) this->tile_count_y;
            
            float tile_width = 1.0f/ (float) this->tile_count_x;
            float tile_height = 1.0f/ (float) this->tile_count_y;
            
            float x_offset = -(this->tile_size / 2); // From center of tile
            float y_offset = (this->tile_size / 2); // From center of tile
            
            this->vertices.insert(vertices.end(), {
                x_offset + (this->tile_size * x), y_offset + -this->tile_size * y,
                x_offset + (this->tile_size * x), y_offset + (-this->tile_size * y) - this->tile_size,
                x_offset + (this->tile_size * x) + this->tile_size, y_offset + (-this->tile_size * y) - this->tile_size,
                x_offset + (this->tile_size * x), y_offset + -this->tile_size * y,
                x_offset + (this->tile_size * x) + this->tile_size, y_offset + (-this->tile_size * y) - tile_size,
                x_offset + (this->tile_size * x) + this->tile_size, y_offset + -this->tile_size * y
            });
            
            this->texture_coordinates.insert(texture_coordinates.end(), {
                u, v,
                u, v + (tile_height),
                u + tile_width, v + (tile_height),
                u, v,
                u + tile_width, v + (tile_height),
                u + tile_width, v
            });
        }
    }
    
    this->left_bound   = 0 - (this->tile_size / 2);
    this->right_bound  = (this->tile_size * this->width) - (this->tile_size / 2);
    this->top_bound    = 0 + (this->tile_size / 2);
    this->bottom_bound = -(this->tile_size * this->height) + (this->tile_size / 2);
}

void Map::render(ShaderProgram *program)
{
    glm::mat4 model_matrix = glm::mat4(1.0f);
    program->SetModelMatrix(model_matrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, this->vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, this->texture_coordinates.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    
    glDrawArrays(GL_TRIANGLES, 0, (int) this->vertices.size() / 2);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
```
```c++
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 5

#include "Map.h"

struct GameState
{
    Entity *player;
    Entity *enemies;
    
    // Notice that Entity* platforms is gone
    Map *map;
    
    Mix_Music *bgm;
    Mix_Chunk *jump_sfx;
};

unsigned int LEVEL_1_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2
};

void initialise()
{
    GLuint map_texture_id = load_texture("assets/tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, LEVEL_1_DATA, map_texture_id, 1.0f, 4, 1);
}

void shutdown()
{
    delete state.map;
}
```

Now, we need to update a few more things in order to make this map collidable with our player—or rather, the other way around. To do this, we need to convert our entities' positions to the grid coordinates of the tilemap and check if a tile is actually there. This requires us to overload our collision-checking methods to work for both `Entity` objects and `Map` objects:

```c++
#include "Map.h"

class Entity
{
private:
    void update(float delta_time, Entity *player, Entity *objects, int object_count, Map *map);

    void const check_collision_y(Entity *collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity *collidable_entities, int collidable_entity_count);
    void const check_collision_y(Map *map);
    void const check_collision_x(Map *map);
}
```
```c++
void Entity::update(float delta_time, Entity *player, Entity *objects, int object_count, Map *map)
{
    position.y += velocity.y * delta_time;
    check_collision_y(objects, object_count);
    check_collision_y(map);
    
    position.x += velocity.x * delta_time;
    check_collision_x(objects, object_count);
    check_collision_x(map);
}

void const Entity::check_collision_y(Map *map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->is_solid(top, &penetration_x, &penetration_y) && velocity.y > 0)
    {
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && velocity.y > 0)
    {
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && velocity.y > 0)
    {
        position.y -= penetration_y;
        velocity.y = 0;
        collided_top = true;
    }
    
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && velocity.y < 0)
    {
    position.y += penetration_y;
    velocity.y = 0;
    collided_bottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0)
    {
        position.y += penetration_y;
        velocity.y = 0;
        collided_bottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0)
    {
    position.y += penetration_y;
    velocity.y = 0;
        collided_bottom = true;
        
    }
}

void const Entity::check_collision_x(Map *map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    
    float penetration_x = 0;
    float penetration_y = 0;
    
    if (map->is_solid(left, &penetration_x, &penetration_y) && velocity.x < 0)
    {
        position.x += penetration_x;
        velocity.x = 0;
        collided_left = true;
}
    if (map->is_solid(right, &penetration_x, &penetration_y) && velocity.x > 0)
    {
        position.x -= penetration_x;
        velocity.x = 0;
        collided_right = true;
    }
}
```