<h4 align=center>NYU Tandon School of Engineering<h4>
<h3 align=center>CS-UY 3113 Summer 2023</h3>
<h1 align=center>Project 6: <em>Platformer</em></h1>
<h3 align=center><em>Due: 3:00pm, Friday, December 15th, 2023</em></h3>
 
### Submission instructions
1. You _must_ use delta time, the `Entity`, `Map`, and `Scene` classes, and fixed time steps in this assignment.
2. You should submit the link to your repo as a comment on [**Brightspace**](https://brightspace.nyu.edu/d2l/lms/dropbox/admin/folders_manage.d2l?ou=311022&dst=1).
4. Do not use any OpenGL functionality that we have not learned in class.
5. All of your `main.cpp` file should contain a header comment block as follows:

```c++
/**
* Author: [Your name here]
* Assignment: [Your game's name here]
* Date due: 2023-12-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
```

***No late submissions will be accepted***.

### Ground Rules

- It must be something other than a Mario-styled platformer. If you're going to have gravity-based platforming mechanics (i.e. jumping), make sure to make your gameplay mechanics original!
- No [**flappy bird**](https://youtu.be/fQoJZuBwrkU) games or [**doodle jump**](https://youtu.be/wjofzwaC_Oo)-style.
- Top down ([**Legend of Zelda**](https://youtu.be/UQlP9sHf5Ho?t=1473)-style) is totally ok.
- [**Shoot-em-up**](https://en.wikipedia.org/wiki/Shoot_'em_up) style (vertical or horizontal) is ok, but it must have bullets/projectiles.

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
4. [**Player Can Lose (10%)**](#requirement-4-ai-20)
5. [**Players Can Win (10%)**](#requirement-5-players-can-win-10)
6. [**Audio (10%)**](#requirement-6-audio-10)

#### _Tips_

1. [**Common Issues**](#common-issues)
2. [**Extra Credit**](#extra-credit)

### Requirement 1: _Menu Screen (10%)_

- Show the name of your game and the text `"Press enter to start"` (or any variation thereof). The keycode for enter is: `SDLK_RETURN`
- This can be a solid color background with text on it.
- The menu must be a different `Scene` object. Do not just show/hide text.

### Requirement 2: _2 Minutes of Gameplay (40%)_

- If you have just 1 level, it must be **huge** and with multiple obstacles.
- You can also have 3 shorter levels.
- You must have some sort of screen scrolling,

### Requirement 3: _Moving AI (20%)_

- There should be at least three of these. They can all look and behave the same way.
- They must move around in some way.

### Requirement 4: _Players Can Lose (10%)_

- Some way for the player to lose (touching an AI, falling off a ledge, running out of time, etc.)


### Requirement 5: _Players Can Win (10%)_

- Some way for the player to win (defeating all AI, collecting all the coins, etc.)

### Requirement 6: _Audio (10%)_

- You must have at least 1 looping background music.
- There must be at least 1 sound effect (jumping, player being defeated, etc.)

---

### _Common Issues_

More planning = less coding. Work out your levels and ideas on paper as well as what classes, objects, textures, Audio you’ll need before you start coding.

### _Extra Credit_

- Apply at least one special effect.
- Use at least one logic-based shader effect. This can be a little tricky, but you can make it as simple as you like.