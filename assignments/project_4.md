<h4 align=center>NYU Tandon School of Engineering<h4>
<h3 align=center>CS-UY 3113 Spring 2024</h3>
<h1 align=center>Project 4: <em>Rise of the AI</em></h1>
<h3 align=center><em>Due: 11:59pm, Saturday, March 23rd, 2024</em></h3>
 
### Submission instructions
1. You _must_ use delta time, the `Entity` and `Map` classes, and fixed time steps in this assignment.
2. You should submit the link to your repo as a comment on [**Brightspace**](https://brightspace.nyu.edu/d2l/lms/dropbox/admin/mark/folder_submissions_users.d2l?db=793154&ou=354438).
3. You should also push that same version to your GitHub account. Note that any commits done after the deadline will be ignored.
4. Do not use any OpenGL functionality that we have not learned in class.
5. All of your `.cpp` files should contain a header comment block as follows:

```c++
/**
* Author: [Your name here]
* Assignment: Rise of the AI
* Date due: 2024-03-23, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
```

***No late submissions will be accepted***.

### You must use new assets (art, music, and sound) for this project!

You can use the fontsheet, but other than that, you can not use anything from the lectures, even if you have not used them on previous projects. I.e. no `sophie.png`, `george_0.png`, `tileset.png`, and none of the music or sound effects, etc. You _must_ find new assets.

There are some great sources for free assets:
- More by the same [**artist**](https://kenney.nl/assets).
- [**itch.io**](https://itch.io/game-assets/free) has tons of awesome free graphics.
- Lots of different kinds of [**free assets**](https://opengameart.org/).
- Royalty free [**music**](https://incompetech.com/music/royalty-free/music.html).

### Sections

#### _Requirements_

1. [**3 AI With Different Behaviors (40%)**](#requirement-1-3-ai-with-different-behaviors-40)
2. [**Defeat Enemies (20%)**](#requirement-2-defeat-enemies-20)
3. [**'You Lose' Message (20%)**](#requirement-3-you-lose-message-20)
3. [**'You Win' Message (20%)**](#requirement-4-you-win-message-20)

#### _Tips_

1. [**Common Issues**](#common-issues)
2. [**Extra Credit**](#extra-credit)

### Requirement 1: _3 AI With Different Behaviors (40%)_

- There needs to be **3 or more enemies** with basic AI.
- Each AI should have a **different behavior**. For instance, one could me walking, another patrolling, another jumping or shooting. All AI must be doing _something_ other than standing around.
- It is OK to use the same image/textures for all 3 of your AI.
- Note on the level's layout:
    - Your AI must not all be on one (flat) floor. You will lose points if all 3 of your AI are on the same floor.
    - At least 1 of your AI must be on a platform.

### Requirement 2: _Defeat Enemies (20%)_

- The player must be able to get rid of each individual enemy one at a time (i.e. jumping).
- Enemies that are gone should not be seen anymore or able to be "collidable".

### Requirement 3: _'You Lose' Message (20%)_

- If the player touches an enemy, gets shot, etc., show text the text “You Lose”. Each letter must be rendered independently using the text function we wrote in class.

### Requirement 4: _'You Win' Message (20%)_

- If the player gets rid of all enemies, show text the text “You Lose”. Each letter must be rendered independently using the text function we wrote in class.

---

### _Common Issues_

Check collisions from the players perspective. In other words, check if the player collided with the enemy, not if the enemy collided with the player. You may need to add code to `Entity.h` and `Entity.cpp` to make this work.

### _Extra Credit_

Implement another way for an enemy to kill the player, or for the player to kill an enemy. This does not necessarily have to be shooting–you can definitely get as creative as you like.
