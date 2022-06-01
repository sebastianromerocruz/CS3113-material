## Lecture 03

# [**Transformation Practice**](SDLProject/main.cpp)

### 14 Prairial, Year CCXXX

***Song of the day***: _[****]() by Ginger Root (2021)._

### Sections

1. [**A little help**](#part-0-a-little-help)
2. [**Funny heartbeat or, how to scale a model matrix**](#part-1-funny-heartbeat-or-how-to-scale-a-model-matrix)

### Part 0: _A little help_

Our `main.cpp` file is pretty much destined to get bloated with code _real_ fast. The lines necessary to draw a simple triangle are already what might be considered a lot, so it might bear remembering that you can define functions that are not immediately relevant to your game in separate helper files. For instance, I wrote a simple function that prints the contents of a matrix nicely:

```c++
const char MAT_SEP = '\t';

void print_matrix(glm::mat4 &matrix, int size)
{
    for (auto row = 0 ; row < size ; row++)
    {
        for (auto col = 0 ; col < size ; col++)
        {
            std::cout << matrix[row][col] << MAT_SEP;
        }
        
        std::cout << "\n";
    }
}
```

That's a pretty hand, but bulky function. I've created a separate file, [**helper.cpp**](SDLProject/helper.cpp) and threw it in there. In order for `main.cpp` to use it, we simply need to add a declaration somewhere _before_ using it:

```c++
void print_matrix(glm::mat4 &matrix, int size);
```

<sub>**Code Block 1**: Note that this line exists in [**main.cpp**](SDLProject/main.cpp).</sub>

### Part 1: _Funny heartbeat or, how to scale a model matrix_

Let's review the "heartbeat" effect exercises that we started to warm up a little. If we were to print the contents of our model matrix (`model_matrix`), we would see something like this:

```
1	0	0	0	
0	1	0	0	
0	0	1	0	
0	0	0	1
```

<sub>**Figure 1**: The contents of `model_matrix` upon initialisation.</sub>

Once we made our changes to make our triangle scale infinitely by a factor 1% per frame, these values will, of course, change. If we were to print the contents of `model_matrix` in this case, we would see the following:

```
Current Frame: 0
1	0	0	0	
0	1	0	0	
0	0	1	0	
0	0	0	1	

Current Frame: 1
1.01	0	0	0	
0	1.01	0	0	
0	0	1	0	
0	0	0	1	

Current Frame: 2
1.0201	0	0	0	
0	1.0201	0	0	
0	0	1	0	
0	0	0	1	

Current Frame: 3
1.0303	0	0	0	
0	1.0303	0	0	
0	0	1	0	
0	0	0	1	

Current Frame: 4
1.0406	0	0	0	
0	1.0406	0	0	
0	0	1	0	
0	0	0	1	

Current Frame: 5
1.05101	0	0	0	
0	1.05101	0	0	
0	0	1	0	
0	0	0	1	
```

<sub>**Figure 2**: The contents of `model_matrix` for the first 5 frames.</sub>

Although the precision may not be ideal (such is the nature of computers), it does indeed look like our code is adding 1% of 1 to the first two values of the identity matrix. This makes sense; we are working with a model with only two directions, for the z-coordinate should not change. Recall, too, that the fourth 1 in the matrix represents our use of homogeneous coordinates (see lecture 3 if you need a refresher on that).

---

So how do we accomplish a "heartbeat" effect? In other words, how do we get our model to "expand" to a certain point, and then "contract" to, say, its original size?

There are various ways to do this. Here's how to do this in a way that does not necessitate us having to know the contents of our matrix.

```c++

/* Some code... */

const float GROWTH_FACTOR = 1.01f;  // growth rate of 1.0% per frame
const float SHRINK_FACTOR = 0.99f;  // growth rate of -1.0% per frame
const int MAX_FRAME = 40;           // this value is, of course, up to you

int frame_counter = 0;
bool is_growing = true;

void update()
{
    // STEP 1
    glm::vec3 scale_vector;
    frame_counter += 1;
    
    // STEP 2
    if (frame_counter >= MAX_FRAME)
    {
        is_growing = !is_growing;
        frame_counter = 0;
    }
    
    // STEP 4
    scale_vector = glm::vec3(is_growing ? GROWTH_FACTOR : SHRINK_FACTOR,
                             is_growing ? GROWTH_FACTOR : SHRINK_FACTOR,
                             1.0f);
    
    // STEP 4
    model_matrix = glm::scale(model_matrix, scale_vector);
}

/* More code... */
```

<sub>**Code Block 2**: How to achieve a simple "beating" effect.</sub>

Let's go through the steps one-by-one:

1. Regardless of whichever direction we are going to scale, we will need our scale vector, `scale_vector`. Thankfully, declaring a variable without instantiating is perfectly normal C++. In this method, we are keeping track of what is happening by using a "frame count". Since a call to `update()` means that we are in a new frame, we increase its value as well.
2. Before we get to scaling, we need to see if we are still within the "bounds" of the current direction we are scaling in. This is where the `frame_count` comes in handy. By defining a constant `MAX_FRAME` in the global scope and setting it `40`, I intend on letting my model to scale in either direction for about 40 frames. Once it reaches that threshold, I will reset the frame counter and tell the rest of the code that we are no longer going in the same direction (`is_growing = !is_growing;`).
3. So now that we've taken care of determining which direction we are scaling in, we can create the appropriate scale vector to achieve this. We have two choices here: use `GROWTH_FACTOR` or use `SHRINK_FACTOR`. We can accomplish this in a simple one-liner using the [**ternary operator**](https://www.programiz.com/cpp-programming/ternary-operator), as shown above, or we can use an `if`- statement. That is completely up to you.
4. What is left is to simply use `glm::scale()` to update our model matrix.

The result is a very nice and fluid "heartbeat" effect for our triangle:

![heartbeat](assets/heartbeat.gif)

<sub>**Figure 3**: Our triangle beating like a heart.</sub>

### Part 3: Rotating a model matrix

The only real difference between scaling and rotation is the function that we'll be using. As you may have guessed, we use `glm::rotate()` instead.

Let's say that your game started with out pulsating triangle not looking up, but at a 45-degree angle to the normal. We can easily accomplish this by throwing the following lines onto our `main.cpp` file:

```c++
/* Some code... */

const float INIT_TRIANGLE_ANGLE = glm::radians(45.0);

/* Some more code... */

void initialise()
{
    /* Some more code... */
    model_matrix = glm::mat4(1.0f);
    model_matrix = glm::rotate(model_matrix, TRIANGLE_INIT_ANGLE, glm::vec3(0.0f, 0.0f, 1.0f));
}
```