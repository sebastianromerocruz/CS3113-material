<h4 align=center>NYU Tandon School of Engineering<h4>
<h3 align=center>CS-UY 3113 Summer 2024</h3>
<h1 align=center>Project 5: <em>Platformer</em></h1>
<h3 align=center><em>Due: 11:59pm, Saturday, November 23rd, 2024</em></h3>
 
### Submission instructions
1. You _must_ use delta time, the `Entity`, `Map`, and `Scene` classes, and fixed time steps in this assignment.
2. You should submit your homework on [**Brightspace**](https://brightspace.nyu.edu/d2l/lms/dropbox/admin/folders_manage.d2l?ou=289892).
3. You should also push that same version to your GitHub account. Note that any commits done after the deadline will be ignored.
4. For this assignment you should turn only the C++ files that are relevant to your project (`main.cpp`, `Entity.cpp`, `Entity.h`/`Entity.hpp`, `Map.cpp`, `Map.h`/`Map.hpp`, `Utility.cpp`, `Utility.h`/`Utility.cpp`, etc.). You _must_ also submit any image and audio files that you used in your project. Do **not** submit any other files.
4. Do not use any OpenGL functionality that we have not learned in class.
5. All of your `main.cpp` file should contain a header comment block as follows:

```c++
/**
* Author: [Your name here]
* Assignment: Platformer
* Date due: 2023-08-01, 11:59pm
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

1. [**Menu Screen (10%)**](#requirement-1-menu-screen-10)
2. [**3 Levels (40%)**](#requirement-2-3-levels-40)
3. [**3 Lives (20%)**](#requirement-3-3-lives-20)
4. [**AI (20%)**](#requirement-4-ai-20)
5. [**Audio (10%)**](#requirement-5-audio-10)

#### _Tips_

1. [**Common Issues**](#common-issues)
2. [**Extra Credit**](#extra-credit)

### Requirement 1: _Menu Screen (10%)_

- Show the name of your game and the text `"Press enter to start"` (or any variation thereof). The keycode for enter is: `SDLK_RETURN`
- This can be a solid color background with text on it.
- The menu must be a different `Scene` object. Do not just show/hide text.

### Requirement 2: _3 Levels (40%)_

- Your game needs to have 3 levels. They do not need to be long or complicated.
- They must scroll! (no single screen games) If you do not have 3 scrolling levels, your grade for the entire project will be 0.
- This is a platformer game, it needs to have platforms.

### Requirement 3: _3 Lives (20%)_

- The player gets 3 lives for the entire game (not per level).
- If the player runs out of lives, show a “You Lose” text.
- If the player gets to the end of your game, show text a "You Win” text.

### Requirement 4: _AI (20%)_

- At least 1 type of moving AI (place a couple of these AI in your game).
- If the player touches the AI, the player dies.
- Each of your levels must have at least 1 AI.

### Requirement 5: _Audio (10%)_

- You must have at least 1 looping background music.
- There should be at least 1 sound effect (such as for jumping, when the player or AI dies, etc.)

---

### _Common Issues_

You might find it easier to work on the movement first (without any obstacles in the way). Once the movement is working, then add your environment.

### _Extra Credit_

- Have a pseudo-start screen where, if the player presses a button, the entire screen freezes. Do _not_ make this a separate `Scene` object.
- Add at least one [**special effect**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/fx) to your game.
