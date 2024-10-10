<h2 align=center>Week 05</h2>

<h1 align=center>Exercise: <em>User Input and Collision Detection</em></h1>

---

## Sections

[**Goal**](#goal)
1. [**Movement and Translation**](#trans)
2. [**Accumulate Scaling**](#scale)
3. [**Collision Detection**](#coll)
4. [**Checking If The Drops Still Exist**](#exist)

---

<a id="goal"></a>

## Goal

In this exercise, you'll implement a simple game mechanic where drops move horizontally based on user input and drop vertically when triggered by a specific key. You will also implement basic collision detection to check if the drops land inside a beaker:

<p align=center>
    <img src="assets/win.gif"></img>
</p>

<sub>**Figure 1**: The game mechanic's "win" condition.</sub>

You can find the starter code [**here**](SDLProject/main.cpp). This starter code renders the following scene, where no transformations are being applied and no collision detection is present.

<p align=center>
    <img src="assets/starter.png"></img>
</p>

<sub>**Figure 2**: Your starting situation.</sub>

Note, too, that the following have been defined for you:

```c++
constexpr float DROPS_SPEED = 3.0f; // speed of drop fall

// For beaker translation
glm::vec3 g_beaker_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_beaker_movement = glm::vec3(0.0f, 0.0f, 0.0f);

// For drop translation
glm::vec3 g_drops_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_drops_movement = glm::vec3(0.0f, 0.0f, 0.0f);

// For drop scaling
glm::vec3 g_drops_scale = glm::vec3(0.0f, 0.0f, 0.0f);  // scale trigger vector
glm::vec3 g_drops_size  = glm::vec3(1.0f, 1.0f, 0.0f);  // scale accumulator vector
```

You will _not_ need any additional constants / global variables, but you can if you want to.

<br>

<a id="trans"></a>

### Part 1: _Movement and Translation_

_**Note**: The entirety of part 1 is done within the `process_input()` function._

The requirements here are simple. Every frame, your completed `process_input()` should be:

1. Checking if the player has pressed the `l` (lowercase L) key _once_. If they do, your code should tell the drop model to be translated downwards during `update()`.
2. Checking if the player is _holding_ either the `A` key or the `D` key. If they are, your code should tell the drop model to be translated either to the left (`A`) or right (`D`) during `update()`. **The player should not have the ability to move the drop model vertically in your final solution**, but you can temporarily implement it as well for collision testing purposes. 

Note that, in this exercise, we're not going to normalise movement because we actually want to simulate proportionate downward movement in two directions (basically, don't touch the normalisation code).

**Hint**: Remember that, for movement _that depends on constant user input_ (i.e. holding), each frame should start under the assumption that no key is being pressed or held.

<br>

<a id="scale"></a>

### Part 2: _Accumulate Scaling_

_**Note**: The entirety of the rest of the exercise is done within the `update()` function._

Just like with translations, our drops will also be potentially accumulating scaling factors (i.e. if they touch the beaker, it would be scaling down i.e. shrinking). In the `ACCUMULATOR LOGIC` section of `update()` (already in the starter file), write code that will take into account this potential change in scale accumulation:

```cpp
void update()
{
    ...

    // --- ACCUMULATOR LOGIC --- //
    // Translation accumulation
    g_drops_position += g_drops_movement * DROPS_SPEED * delta_time;

    // Scaling accumulation
    // TODO

    ...
}
```

Note that a "lose" condition is already implemented in this exercise (if the drop falls below -7 in the y-direction), so you don't have to implement it yourself:

```cpp
void update()
{
    ...
    // --- TERMINATE GAME IF BEAKER MISSED --- //
    if (g_drops_position.y < -7.0f) g_app_status = TERMINATED;
}
```

<p align=center>
    <img src="assets/lose.gif"></img>
</p>

<sub>**Figure 3**: The game mechanic's "lose" condition.</sub>

<br>

<a id="coll"></a>

### Part 3: _Collision Detection_

1. In the `COLLISION LOGIC` section of `update()`, write code that will register a collision using [**box-to-box logic**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/collision-detection#box-to-box-collisions). Note here, though, that we only want to register a collision if:
    - The box-to-box collision math and logic is `true`, but _also_ if...
    - The droplet hasn't disappeared already (i.e. scale of 0).

2. If a collision is detected, simply write code that will trigger shrinking in both the x- and y-directions in the following frame.

<br>

<a id="exist"></a>

### Part 4: _Checking If The Drops Still Exist_

Since we only want to scale down if the drops' accumulated scale is positive, we need a safeguard that will prevent the next frame's `update()` from continuing to scale the drops once they disappear. If we don't do this, we'll get some funky negative scaling:

<p align=center>
    <img src="assets/negative-scaling.gif"></img>
</p>

<sub>**Figure 4**: Infinite negative scaling.</sub>

Moreover, the drop is _still_ falling, even though we've already collided. So, write a check below your code from part 3 that:
- Zeroes out the drops' accumulated scale.
- Turns off the drops' scaling triggers.
- Turns off the drops' falling trigger.