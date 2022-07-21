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

Then, we can "draw" the map by assigning a specific index from the tilesheet to a location in the map using an array of indices:

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

<sub>**Code Block 2**: A numerical "drawing" of our stage.</sub>

The result, superimposed with the indices above, looks like this:

![tilemap-numbered](assets/numbered-tilemap.png)

<sub>**Figure 3**: Representing a scene using specific tiles from a tilesheet.</sub>

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
    
    // Here, the level_data is the numerical "drawing" of the map
    unsigned int *level_data;
    GLuint texture_id;
    
    float tile_size;
    int tile_count_x;
    int tile_count_y;
    
    // Just like with rendering text, we're rendering several sprites at once
    // So we need vectors to store their respective vertices and texture coordinates
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;
    
    // The boundaries of the map
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
    // Since this is a 2D map, we need a nested for-loop
    for(int y = 0; y < this->height; y++)
    {
        for(int x = 0; x < this->width; x++) {
            // Get the current tile
            int tile = this->level_data[y * this->width + x];
            
            // If the tile number is 0 i.e. not solid, skip to the next one
            if (tile == 0) continue;
            
            // Otherwise, calculate its UV-coordinated
            float u = (float) (tile % this->tile_count_x) / (float) this->tile_count_x;
            float v = (float) (tile / this->tile_count_x) / (float) this->tile_count_y;
            
            // And work out their dimensions and posititions
            float tile_width  = 1.0f / (float) this->tile_count_x;
            float tile_height = 1.0f / (float) this->tile_count_y;
            
            float x_offset = -(this->tile_size / 2); // From center of tile
            float y_offset = (this->tile_size / 2); // From center of tile
            
            // So we can store them inside our std::vectors
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
    
    // The bounds are dependent on the size of the tiles
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
    // Loop at this beautiful initialisation. That's literally it
    GLuint map_texture_id = load_texture("assets/tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, LEVEL_1_DATA, map_texture_id, 1.0f, 4, 1);
}

void shutdown()
{
    delete state.map;
}
```

<sub>**Code Block 3, 4, and 5**: The `Map` class implemented. Keep in mind that now we have no need for the `state.platforms` array, so you can get rid of all the code that involves it. _This includes the `state.player`'s `update()` method_. We'll be looking at that momentarily.</sub>

Now, we need to update a few more things in order to make this map collidable with our player—or rather, the other way around. To do this, we need to convert our entities' positions to the grid coordinates of the tilemap and check if a tile is actually there. This requires us to overload our collision-checking methods to work for both `Entity` objects and `Map` objects.

These new `check_collision_y()` and `check_collision_x()` methods will be probing for three points and checking whether the tile that we are colliding with is supposed to be solid or not. For instance, in the case of a pit:

![vert](assets/vert.png)
![horiz](assets/horiz.png)

<sub>**Figures 4 and 5**: Collision detection in both cardinal coordinates.</sub>

Our map, therefore needs to have a way to check whether its tiles are solid or not:

```c++
bool Map::is_solid(glm::vec3 position, float *penetration_x, float *penetration_y)
{
    // The penetration between the map and the object
    // The reason why these are pointers is because we want to reassign values
    // to them in case that we are colliding. That way the object that originally
    // passed them as values will keep track of these distances
    // inb4: we're passing by reference
    *penetration_x = 0;
    *penetration_y = 0;
    
    // If we are out of bounds, it is not solid
    if (position.x < this->left_bound || position.x > this->right_bound) return false;
    if (position.y > this->top_bound || position.y < this->bottom_bound) return false;
    
    int tile_x = floor((position.x + (this->tile_size / 2)) / this->tile_size);
    int tile_y = -(ceil(position.y - (this->tile_size / 2))) / this->tile_size; // Our array counts up as Y goes down.
    
    // If the tile index is negative or greater than the dimensions, it is not solid
    if (tile_x < 0 || tile_x >= this->width) return false;
    if (tile_y < 0 || tile_y >= this->height) return false;
    
    // If the tile index is 0 i.e. an open space, it is not solid
    int tile = level_data[tile_y * this->width + tile_x];
    if (tile == 0) return false;
    
    // If none of these are true, then it is solid
    float tile_center_x = (tile_x * this->tile_size);
    float tile_center_y = -(tile_y * this->tile_size);
    
    // And we likely have some overlap
    *penetration_x = (this->tile_size / 2) - fabs(position.x - tile_center_x);
    *penetration_y = (this->tile_size / 2) - fabs(position.y - tile_center_y);
    
    return true;
}
```

<sub>**Code Block 6**: This method serves a double purpose; it both a) tells us whether or not a specific tile is solid, and b) tells the calling scope by how much we've penetrated this given tile (via `penetration_x` and `penetration_y` pointers).</sub>

To put this method into context, consider our `Entity` class's current implementations of `check_collision_y()` and `check_collision_x()`

```c++
#include "Map.h"

class Entity
{
private:
    // Now, update should check for both objects in the game AND the map
    void update(float delta_time, Entity *player, Entity *objects, int object_count, Map *map);

    void const check_collision_y(Entity *collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity *collidable_entities, int collidable_entity_count);

    // Overloading our methods to check for only the map
    void const check_collision_y(Map *map);
    void const check_collision_x(Map *map);
}
```
```c++
void Entity::update(float delta_time, Entity *player, Entity *objects, int object_count, Map *map)
{
    // We make two calls to our check_collision methods, one for the collidable objects and one for
    // the map.
    position.y += velocity.y * delta_time;
    check_collision_y(objects, object_count);
    check_collision_y(map);
    
    position.x += velocity.x * delta_time;
    check_collision_x(objects, object_count);
    check_collision_x(map);
}

void const Entity::check_collision_y(Map *map)
{
    // Probes for tiles above
    glm::vec3 top       = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left  = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    
    // Probes for tiles below
    glm::vec3 bottom       = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left  = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    
    // f
    float penetration_x = 0;
    float penetration_y = 0;
    
    // If the map is solid, check the top three points
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
    
    // And the bottom three points
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
    // Probes for tiles; the x-checking is much simpler
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

<sub>**Code Blocks 7 and 8**: Collisions with the map and collisions with other entities are inherently different—for the better.</sub>

