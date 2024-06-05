<h4 align=center>NYU Tandon School of Engineering<h4>
<h3 align=center>CS-UY 3113 Summer 2023</h3>
<h1 align=center>Project 1: <em>Simple 2D Scene</em></h1>
<h3 align=center><em>Due: 11:59pm, Thursday, June 13th, 2023</em></h3>
 
### Submission instructions
1. You must use delta time in your animations.
2. You should submit your homework on [**Brightspace**](https://brightspace.nyu.edu/d2l/lms/dropbox/admin/folders_manage.d2l?ou=289892).
3. You should also push that same version to your GitHub account. Note that any commits done after the deadline will be ignored.
4. For this assignment you should turn in **one** C++ file named `main.cpp` _and_ any image files that you used in your project. Do **not** submit any other files.
4. Do not use any OpenGL functionality that we have not learned in class.
5. The `main.cpp` file you submit should contain a header comment block as follows:

```c++
/**
* Author: [Your name here]
* Assignment: Simple 2D Scene
* Date due: 2023-06-11, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
```

***No late submissions will be accepted***.

### Sections

#### _Requirements_

1. [**At Least Two Objects (50%)**](#requirement-1-at-least-two-objects-50)
2. [**Movement (25%)**](#requirement-2-movement-25)
3. [**Rotation (25%)**](#requirement-3-rotation-25)

#### _Tips_

1. [**Common Issues**](#common-issues)
2. [**Extra Credit**](#extra-credit)

### Requirement 1: _At Least Two Objects (50%)_

- There needs to be at least 2 different "objects" in the scene.
- Each object must use a different texture. These textures must _not_ be solid colours, but images. You may use any images that you like.

### Requirement 2: _Movement (25%)_

- At least one of your objects must be **moving** (i.e. translation).

### Requirement 3: _Rotation (25%)_

- At least one of your objects must be **rotating**.

### Common Issues

If your screen is flickering, remember that you have to, _in order_:

1. Call `glClear`.
2. Draw every single one of your objects (i.e. everything you want to be in your scene)
3. Call `SDL_GL_SwapWindow`.

### Extra Credit

- Have at least one of your objects change scale.
- Have _both_ of your objects translate in a pattern _other than_ vertically and horizontally.
