<h4 align=center>NYU Tandon School of Engineering<h4>
<h3 align=center>CS-UY 3113 Summer 2024</h3>
<h1 align=center>Project 6: <em>Student's Choice</em></h1>
<h3 align=center><em>Due: 2:00pm, Friday, December 6th _or_ Wednesday, December 11th, 2024</em></h3>
 
### Submission instructions

1. You _must_ use delta time, the `Entity`, `Map`, and `Scene` classes, and fixed time steps in this assignment.
2. You must have pushed your homework on **Github** _and_ have it ready to present by the start of class time.
3. Do not use any OpenGL functionality that we have not learned in class (unless I have approved its use).
4. Your `main.cpp` file should contain a header comment block as follows:

```c++
/**
* Author: [Your name here]
* Assignment: [Your game's name here]
* Date due: [Your presentation date], 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
```

***No late submissions will be accepted***.

### Ground Rules

- It must be something ***other than a Mario-styled platformer***. If you're going to have gravity-based mechanics (i.e. things falling, etc.), make sure to make your ideas original! If you make a platformer for your final project, you will receive an automatic 25% penalty.
- No [**flappy bird**](https://youtu.be/fQoJZuBwrkU) games or [**doodle jump**](https://youtu.be/wjofzwaC_Oo)-style.
- Top down ([**Legend of Zelda**](https://youtu.be/UQlP9sHf5Ho?t=1473)-style) is totally ok.
- [**Shoot-em-up**](https://en.wikipedia.org/wiki/Shoot_'em_up) style (vertical or horizontal) is ok, but it must have bullets/projectiles.
- Any of the requirements listed below may be replaced by another feature of your choosing _as long as it is approved by Prof. Romero Cruz_.
- If you present on the first available presentation date (December 6th), you will receive 10 extra points for this project. In other words, you can get an 110% if you complete all of the requirements.

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
2. [**2 Minutes of Gameplay (40%)**](#requirement-2-2-minutes-of-gameplay-40)
3. [**Moving AI (20%)**](#requirement-3-moving-ai-20)
4. [**Players Can Win and Lose (10%)**](#requirement-4-ai-20)
5. [**Shader Logic (10%)**](#requirement-5-players-can-win-10)
6. [**Audio (10%)**](#requirement-6-audio-10)

#### _Tips_

1. [**Common Issues**](#common-issues)
2. [**Extra Credit**](#extra-credit)

### Requirement 1: _Menu Screen (10%)_

- Show the name of your game and the text `"Press enter to start"` (or any variation thereof). The keycode for enter is: `SDLK_RETURN`
- This can be a solid color background with text on it.
- The menu must be a different `Scene` object. Do not just show/hide text.

### Requirement 2: _2 Minutes of Gameplay (40%)_

- Must have 2 minutes worth of **significant** gameplay. This does not mean _exactly_ 2 minutes, but rather that the game can go on for 2+ minutes.
- Must have 3 levels ([**`Scene`**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/scenes)).

If your idea doesn't revolve around multi-level-based design, whatever scene structure that you implement _must_ contain around the same quantity of content as a 3-level game.

### Requirement 3: _Moving AI (20%)_

- There should be at least three distinct types of these.
- They must transform in some way.

These don't necessarily need to be enemies! Homing projectiles, moving platforms/obstacles also count as long as they are distinct from each other in behaviour.

### Requirement 4: _Players Can Win and Lose (10%)_

- Some way for the player to lose (touching an AI, falling off a ledge, running out of time, etc.)
- Some way for the player to win (defeating all AI, collecting all the coins, etc.)

Note that past students have done games where there are no win/lose conditions (e.g. a farming sim, etc.). This is also fine, but you _must_ let us know in advance.

### Requirement 5: _Shader Logic (10%)_

- Use at least one logic-based (i.e. `if`-statements) [**shader**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/shaders) effect.
- It can be any of the ones we talked about in class (or of your own design), but it _must_ contribute the overall theme and experience of your game (i.e. don't just make the screen red for the sake of it).

### Requirement 6: _Audio (10%)_

- You must have at least 1 looping background music.
- There must be at least _3_ sound effects (walking, player being defeated, shooting, etc.)

---

### _Common Issues_

More planning = less coding. Work out your levels and ideas on paper as well as what classes, objects, textures, and audio you’ll need before you start coding.

For the love of all that is holy and numinous, ***do not procrastinate***.

### _Extra Credit_

- Apply at least one [**special effect**](https://github.com/sebastianromerocruz/CS3113-material/tree/main/lectures/fx). This can be one of the ones we wrote in class or a new one of your making.

### _Penalties_

- **-10pts**: Did not submit [**final project proposal**](https://forms.gle/nnThNUHVudJXbK6c8) by 11/24/24.
- **-10pts**: Did not attend play-testing session on 11/15/24.