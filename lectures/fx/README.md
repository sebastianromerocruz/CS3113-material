## Lecture 17

# [**Effects**](SDLProject/main.cpp)

### 8 Thermidor, Year CCXXX

***Song of the day***: _[**Trumpet Concerto in E-flat major**](https://youtu.be/bG8nAxNd3zA) performed by Franz Joseph Haydn, performed by the Vienna Philharmonic (2021)._

---

### Sections

1. [**Multiple Levels**](#part-1-multiple-levels)
2. [**Effects**](#part-2-effects)
    1. [**Fade-In**](#fade-in)
    2. [**Fade-Out**](#fade-out)

### Part 1: _Multiple Levels_

We left off last time with a level system for our games. The next step is to be able to create and switch between these levels. For this, I will prove the concept by making an almost identical version of `LevelA`, called `LevelB`, and switch between the two of them. You can find the code to the `LevelB` class [**here**](SDLProject/LevelB.h) and [**here**](SDLProject/LevelB.cpp), but really they only differ in their level data.

What I'd instead like to follow is how we switch between these levels. The first step is to create an array of `Scenes` from which we will pick from later:

```c++
// Scene.h
struct GameState
{
    Map *map;
    Entity *player;
    Entity *enemies;
    
    Mix_Music *bgm;
    Mix_Chunk *jump_sfx;
    
    int next_scene_id;
};
```
```c++
// LevelA.cpp
void LevelA::update(float delta_time)
{
    this->state.player->update(delta_time, state.player, state.enemies, this->ENEMY_COUNT, this->state.map);

    // When the player falls, they'll teleport into the next level    
    if (this->state.player->get_position().y < -10.0f) state.next_scene_id = 1;
}
```
```c++
// main.cpp
#include "Scene.h"
#include "LevelA.h"
#include "LevelB.h"

Scene *current_scene;
LevelA *levelA;
LevelB *levelB;

Effects *effects;

Scene *levels[2];

void initialise()
{
    levelA = new LevelA();
    levelB = new LevelB();
    
    levels[0] = levelA;
    levels[1] = levelB;

    // Start at level A
    switch_to_scene(levels[0]);
}

int main(int argc, char* argv[])
{
    while (game_is_running)
    {
        if (current_scene->state.next_scene_id >= 0) switch_to_scene(levels[current_scene->state.next_scene_id]);
    }
}
```

<sub>**Code Blocks 1, 2, and 3**: Adding a level-changing mechanism. Essentially, in the `main()`, if the `state`'s `next_scene_id` is anything that over 0, we switch scenes. This only happens when the `if`-statement in `LevelA.cpp` triggers.</sub>

The result is something like the following:

![level-switch](assets/level-switching.gif)

<sub>**Figure 1**: George falling from Level A to Level B.</sub>

### Part 2: _Effects_

We should be proud of ourselves; we have reached a point where we're going to focus on the polish of our games instead of working on the actual mechanics. The first thing we should think about is adding some special effects, such as transitions and shaking.

How does this work? For transitions such as fade-ins and -outs, the trick is to create a black square that covers the entire screen. For fade-ins, we want to make this black square go from completely solid to completely invisible. We do this by manipulating the [**alpha-channel**](https://en.wikipedia.org/wiki/Alpha_compositing). We'll thus keep track of this value and change it accordingly.

#### Setup

The first thing we're going to do is set up an individual class for effects, called `Effects`:

```c++
// Effects.h
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

enum EffectType { NONE, FADEIN };

class Effects {
    ShaderProgram program;
    float alpha;
    EffectType current_effect;

public:
    glm::vec3 view_offset;
    
    Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix);

    void draw_overlay();
    void start(EffectType effect_type);
    void update(float delta_time);
    void render();
};
```
```c++
#include "Effects.h"

Effects::Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
    // Non textured Shader
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);
    
    this->current_effect = NONE;
    this->alpha = 1.0f;
}

void Effects::draw_overlay()
{
    glUseProgram(this->program.programID);

    float vertices[] =
    {
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        
        -0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5
    };

    glVertexAttribPointer(this->program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(this->program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(this->program.positionAttribute);
}

void Effects::start(EffectType effect_type)
{
    this->current_effect = effect_type;

    switch (this->current_effect)
    {
        case NONE:   break;
        case FADEIN: break;
    }
}


void Effects::update(float delta_time)
{
   switch (this->current_effect)
   {
       case NONE:   break;
       case FADEIN: break;
   }
}

void Effects::render()
{
    glm::mat4 model_matrix = glm::mat4(1.0f);

    switch (this->current_effect)
    {
        case NONE:   break;
        case FADEIN: break;
    }
}
```

<sub>**Code Block 4**: A skeleton for our `Effects` class.</sub>

As you can see, right now we don't have much beyond no effect `NONE` and `FADEIN`, though neither is really doing much. Let's change that.

#### Fade-In

With fade-ins, we want to start with an α-value of 1:

```c++
void Effects::start(EffectType effect_type)
{
    this->current_effect = effect_type;

    switch (this->current_effect)
    {
        case NONE:                       break;   
        case FADEIN: this->alpha = 1.0f; break;
    }
}
```

And gradually go down to 0. We'll, of course, use our delta-time for that:

```c++
void Effects::update(float delta_time)
{
   switch (this->current_effect)
   {
       case NONE:   break;
       
       case FADEIN:
            this->alpha -= delta_time;
            if (this->alpha <= 0) this->current_effect = NONE;
                
            break;
   }
}
```

Note that once we reach an α-value of 0, we go back to a `NONE` effect. The one other thing that we have to add is in the `render()` method, where we'll update the overlay with its current α-value:

```c++
void Effects::render()
{
    glm::mat4 model_matrix = glm::mat4(1.0f);

    switch (this->current_effect)
    {
        case NONE:   break;
        case FADEIN:
            this->program.SetModelMatrix(model_matrix);
            this->program.SetColor(0.0f, 0.0f, 0.0f, this->alpha); // current α-value
            this->draw_overlay();

            break;
    }
}
```

Finally, let's set this all in motion in `main.cpp`:

```c++
#include "Effects.h"

Effects *effects;

void initialise()
{
    effects = new Effects(projection_matrix, view_matrix);
    effects->start(FADEIN, 5.0f);
}

void update()
{
    while (delta_time >= FIXED_TIMESTEP) {
        effects->update(FIXED_TIMESTEP);
    }
}

void render()
{
    glUseProgram(program.programID);
    current_scene->render(&program);
    effects->render();
}
```

And we get the following effect:

![fadein](assets/fadein.gif)

<sub>**Figure 2**: Fade-in transition effect.</sub>

#### Fade-Out

Fading out works basically in the exact same way, but with increasing the α-value. Keep in mind that we should not switch the effect back to `NONE` when we reach 1. Otherwise, the black screen will disappear:

```c++
void Effects::start(EffectType effect_type)
{
    this->current_effect = effect_type;

    switch (this->current_effect)
    {
        case NONE:                             break;
        case FADEIN:  this->alpha     = 1.0f;  break;
        case FADEOUT: this->alpha     = 0.0f;  break;
    }
}

void Effects::update(float delta_time)
{
   switch (this->current_effect)
   {
       case NONE: break;
           
       // Fades
       case FADEIN:
           this->alpha -= delta_time;
           if (this->alpha <= 0) this->current_effect = NONE;
           
           break;
       case FADEOUT:
           if (this->alpha < 1.0f) this->alpha += delta_time;
}

void Effects::render()
{
    glm::mat4 model_matrix = glm::mat4(1.0f);

    switch (this->current_effect)
    {
        case FADEOUT:
        case FADEIN:
            this->program.SetModelMatrix(model_matrix);
            this->program.SetColor(0.0f, 0.0f, 0.0f, this->alpha);
            this->draw_overlay();

            break;
    }
}
```
```c++
enum EffectType { NONE, FADEIN, FADEOUT };
```
```c++
void initialise()
{
    effects->start(FADEIN, 1.0f);
}
```

The result:

![fadeout](assets/fadeout.gif)

<sub>**Figure 3**: Fade-out transition effect.</sub>

### Growing and Shrinking

Having our overlay grow and shrink in and out of the screen works pretty much identical to the fading effects, except instead of changing `alpha`, we can change a `size` attribute:

```c++
enum EffectType { NONE, FADEIN, FADEOUT, GROW, SHRINK };

class Effects {
    ShaderProgram program;
    float alpha;
    float size;
    EffectType current_effect;

public:
    Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix);

    void draw_overlay();
    void start(EffectType effect_type);
    void update(float delta_time);
    void render();
};
```
```c++
Effects::Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
    // Non textured Shader
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    program.SetProjectionMatrix(projection_matrix);
    program.SetViewMatrix(view_matrix);
    
    this->current_effect = NONE;
    this->alpha = 1.0f;
    this->size = 10.0f;
}

void Effects::start(EffectType effect_type)
{
    this->current_effect = effect_type;

    switch (this->current_effect)
    {
        case NONE:                             break;
        case FADEIN:  this->alpha     = 1.0f;  break;
        case FADEOUT: this->alpha     = 0.0f;  break;
        case GROW:    this->size      = 0.0f;  break;
        case SHRINK:  this->size      = 10.0f; break;
    }
}

void Effects::update(float delta_time)
{
   switch (this->current_effect)
   {
       case NONE: break;
           
       // Fades
       case FADEIN:
           this->alpha -= delta_time * this->effect_speed;
           if (this->alpha <= 0) this->current_effect = NONE;
           
           break;
       case FADEOUT:
           if (this->alpha < 1.0f) this->alpha += delta_time * this->effect_speed;
           
           break;
           
       case GROW:
           if (this->size < 10.0f) this->size += delta_time * this->effect_speed; break;
           
       case SHRINK:
           if (this->size >= 0.0f) this->size -= delta_time * this->effect_speed;
           if (this->size < 0) this->current_effect = NONE;
           
           break;
    }
}

void Effects::render()
{
    glm::mat4 model_matrix = glm::mat4(1.0f);

    switch (this->current_effect)
    {
        case NONE:
            break;
            
        case GROW:
        case SHRINK:
        case FADEOUT:
        case FADEIN:
            // Expand the current square a bit
            model_matrix = glm::scale(model_matrix,
                                      glm::vec3(this->size,
                                                this->current_effect != GROW && this->current_effect != SHRINK ?
                                                    this->size : this->size * 0.75f,
                                                0.0f));
            
            this->program.SetModelMatrix(model_matrix);
            this->program.SetColor(0.0f, 0.0f, 0.0f, this->alpha);
            this->draw_overlay();

            break;
    }
}
```

Notice here that we had to modify the model matrix to reflect the size changes of our overlay. The multiplication by 0.75 is so that it grows in a rectangular pattern instead of a square pattern.

### Shaking

A really cool effect that you can add to your game is a shaking effect to simulate the ground shaking. For this, we will have the view matrix shift rapidly for a second or two. So we need to keep track of both the time remaining and random directions that it will be shifting:

```c++
enum EffectType { NONE, FADEIN, FADEOUT, GROW, SHRINK, SHAKE };

class Effects {
    float time_left;
    EffectType current_effect;

public:
    // How much the view matrix will be shifted in a random direction
    glm::vec3 view_offset;
};
```
```c++
Effects::Effects(glm::mat4 projection_matrix, glm::mat4 view_matrix)
{
    this->view_offset = glm::vec3(0.0f);
}

void Effects::start(EffectType effect_type)
{
    this->current_effect = effect_type;

    switch (this->current_effect)
    {
        case SHAKE:   this->time_left = 1.0f;  break;
    }
}

void Effects::update(float delta_time)
{
   switch (this->current_effect)
   {
       case SHAKE:
           this->time_left -= delta_time;
           if (this->time_left <= 0.0f)
           {
               this->view_offset = glm::vec3(0.0f, 0.0f, 0.0f);
               this->current_effect = NONE;
           } else
           {
               float min = -0.1f;
               float max =  0.0f;

               // Generate random value for the offset in both direction
               float offset_value = ((float) rand() / RAND_MAX) * (max - min) + min;

               this->view_offset = glm::vec3(offset_value, offset_value, 0.0f);
           }
   }
}
```
```c++
bool is_colliding_bottom = false;

void update()
{
    while (delta_time >= FIXED_TIMESTEP) {
        if (is_colliding_bottom == false && current_scene->state.player->collided_bottom) effects->start(SHAKE, 1.0f);
        
        is_colliding_bottom = current_scene->state.player->collided_bottom;
    }

    view_matrix = glm::translate(view_matrix, effects->view_offset);
}
```

Result:

![shake](assets/shake.gif)