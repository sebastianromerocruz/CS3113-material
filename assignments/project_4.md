<h4 align=center>NYU Tandon School of Engineering<h4>
<h3 align=center>CS-UY 3113 Fall 2024</h3>
<h1 align=center>Project 4: <em>Rise of the AI</em></h1>
<h3 align=center><em>Due: 11:59pm, Saturday, November 3rd, 2024</em></h3>
 
### Submission instructions
1. You _must_ use delta time, a version of our the `Entity` and `Map` classes, and fixed time steps in this assignment.
2. You should submit the link to your Github repo on [**Brightspace**](https://brightspace.nyu.edu/d2l/home/399853).
3. Note that any commits done after the deadline will be ignored.
4. Do not use any OpenGL functionality that we have not learned in class.
5. All of your `.cpp` files should contain a header comment block as follows:

```c++
/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2024-11-03, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
```

***No late submissions will be accepted***.

### You must use new assets (art, music, and sound) for this project!

You can use the fontsheet, but other than that, you can not use anything from the lectures, even if you have not used them on previous projects. I.e. no `sophie.png`, `george_0.png`, `tileset.png`, witches, and none of the music or sound effects, etc. You _must_ find new assets.

There are some great sources for free assets:
- More by the same [**artist**](https://kenney.nl/assets).
- [**itch.io**](https://itch.io/game-assets/free) has tons of awesome free graphics.
- Lots of different kinds of [**free assets**](https://opengameart.org/).
- Royalty free [**music**](https://incompetech.com/music/royalty-free/music.html).

---

### Sections

#### _Requirements_

1. [**3 AI With Different Behaviors (60%)**](#1)
2. [**Defeat Enemies (20%)**](#2)
3. [**'You Lose' Message (10%)**](#3)
3. [**'You Win' Message (10%)**](#4)

#### _Tips_

1. [**Common Issues**](#issues)
2. [**Extra Credit**](#extra)

---

<a id="1"></a>

### Requirement 1: _3 AI With Different Behaviors (60%)_

- There needs to be **3 or more enemies** with basic AI. (20% each)
- Each AI should have a **different behavior**. For instance, one could be walking, another patrolling, another jumping or shooting. All AI must be doing _something_ other than standing around.
- It is OK to use the same image/textures for all 3 of your AI.
- Note on the level's layout:
    - Your AI must not all be on one (flat) floor. You will lose points if all 3 of your AI are on the same floor.
    - At least 1 of your AI must be on a platform.

<br>
<a id="2"></a>

### Requirement 2: _Defeat Enemies (20%)_

- The player must be able to get rid of each individual enemy one at a time (i.e. jumping).
- Enemies that are gone should not be seen anymore or able to be "collidable".

<br>
<a id="3"></a>

### Requirement 3: _'You Lose' Message (10%)_

- If the player touches an enemy, gets shot, etc., show text the text “You Lose”. Each letter must be rendered independently using the text function we wrote in class.

<br>
<a id="4"></a>

### Requirement 4: _'You Win' Message (10%)_

- If the player gets rid of all enemies, show text the text “You Win. Each letter must be rendered independently using the text function we wrote in class.

---

<a id="issues"></a>

### _Common Issues_

Check collisions __from the players perspective. In other words, check if the player collided with the enemy, not if the enemy collided with the player. You will need to add code to `Entity.h` and `Entity.cpp` to make this work.


<br>
<a id="extra"></a>

### _Extra Credit_

Implement another way for an enemy to kill the player, or for the player to kill an enemy. This does not necessarily have to be shooting–you can definitely get as creative as you like.
