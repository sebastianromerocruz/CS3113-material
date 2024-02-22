<h2 align=center>Lecture 04: <em>In-Class Exercise</em></h2>

<h1 align=center>Exercise: <em>User Input</em></h1>

<h3 align=center>23 Pegasus Moon, Imperial Year MMXXIV<h3>

### Instructions

If you run the [**following `main.cpp`**](SDLProject/main.cpp) file, you will see the following animated sprite (which you can download [**here**](SDLProject/sprites/flower.png)):


![flower_static](assets/flower_static.gif)

<sub>**Figure 1**: Starting behaviour</sub>

Our goal is for the user to be able to move the sprite around the screen using the **arrow keys**, as the following gif illustrates:

![flower_example](assets/flower_example.gif)

<sub>**Figure 2**: Desired behaviour</sub>

We will achieve this through the following three steps:

#### STEP 1: _Set up the necessary data structures_

As the [**notes**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/player-input#part-3-keeping-track-of-the-players-motion) from last class explain, in order efficiently translate objects in OpenGL, we need:

1. A vector to keep track of **how much we have moved in all three Cartesian directions**.
2. A vector to keep track of **whether the player has moved in a given frame or not**, and if so, **in what direction**.
3. A value that tells `update` by **how much we want to move in any given direction per frame**.

In the space provided for part 1:

```c++
// ———————————————— PART 1 ———————————————— //

// ———————————————— PART 1 ———————————————— //
```

Create the above data structures to set us up for the next step.

#### STEP 2: _Keep track of whether the user wants to move or not_

The second data structure from step 1 is used to keep track of whether the user wants to move in a given direction. That is, if the user is **holding down** the:

- **Up-arrow key**: Our data structure would mark a `1.0f` in the y-direction.
- **Down-arrow key**: Our data structure would mark a `-1.0f` in the y-direction.
- **Left-arrow key**: Our data structure would mark a `-1.0f` in the x-direction.
- **Right-arrow key**: Our data structure would mark a `1.0f` in the x-direction.

You can check out the relevant notes [**here**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/player-input#keyboard-state) (code block 5). Also remember that if the user releases the key, the flower should stop moving as well (to do this you would need to reset the movement vector).

Keep in mind that, if the user is moving in both the x- and y-direction, you will need to **normalise** this behaviour so that the sprite doesn't move faster than necessary.

In the space provided for part 2:

```cpp
    // ———————————————— PART 2 ———————————————— //
    
    // ———————————————— PART 2 ———————————————— //
```

Write the necessary C++ code to record whether the user has moved (and if so, in which direction).

#### STEP 3: _Apply the necessary transformations_

Finally, just like we did [**earlier**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/player-input#part-3-keeping-track-of-the-players-motion) (code block 12), write necessary C++ code to properly apply the translations onto our flower's model matrix in order for it at the speed we chose in step 1. Don't forget to adjust for different frame rates!

```c++
    // ———————————————— PART 3 ———————————————— //
    
    // ———————————————— PART 3 ———————————————— //
```

#### BONUS POINT: _Apply rotation mechanism_

To earn a bonus point for this exercise, you will add a rotation mechanism depending on the user input:

- **r key**: Our flower would start rotating clockwise (without stopping)
- **c key**: Our flower would start rotating counter-clockwise (without stopping)
- **s key**: Our flower would stop any rotation

For this part of the exercise, you should use **keystrokes** instead of keyboard state. The relevant notes are [**here**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/player-input#keystrokes).

In the space provided for this part:

```c++
    // ———————————————— BONUS ———————————————— //
    
    // ———————————————— BONUS ———————————————— //
```

Write the necessary code to rotate the flower according to user input. Remember we're using keystrokes this time!

<!-- You can find the solutions [**here**](SDLProject/solution/solution.cpp), though you should of course give it a shot before you look at it! -->