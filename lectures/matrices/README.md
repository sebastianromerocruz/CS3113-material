## Lecture 03

# You're getting old, Matrix

### 12 Prairial, Year CCXXX

***Song of the day***: _[****]() by ._

### Sections


### Part 1: _The XYZs of Animation_

How do we simulate movement through space? Physics teaches to do so via methods called vectors, whereby each vector contains a X-, Y-, and Z-coordinate in [**cartesian space**](https://en.wikipedia.org/wiki/Cartesian_coordinate_system):

![cartesian](assets/1920px-Cartesian-coordinate-system.svg.png)

<sub>**Figure 1**: Illustration of a Cartesian coordinate plane. Four points are marked and labeled with their coordinates: (2, 3) in green, (−3, 1) in red, (−1.5, −2.5) in blue, and the origin (0, 0) in purple.</sub>

This makes sense—we exist as we know it in three-dimensional space, and as such it takes three values to determine a location in each of those three dimensions.

It will not surprise you to learn that modelling in video games works in exactly the same way. In just about every situation, we will be dealing with an initial set of coordinates, represented by an X-value, a Y-value, and a Z-value, and then some sort of **transformation** being done upon this set of coordinates. The resulting set of coordinates will then be displayed on our screen and appear to us as _movement_.

---

Naturally, there are ways with which OpenGL performs these operations, and they are based on the principles of **linear algebra** or, more specifically, on [**matrix**](https://en.wikipedia.org/wiki/Matrix_(mathematics)) operations:

![matrix](assets/Matris.png)

<sub>**Figure 2**: An _m_ × _n_ matrix. The _m_ rows are horizontal and the _n_ columns are vertical. Each element of a matrix is often denoted by a variable with two subscripts. For example, a<sub>2,1</sub> represents the element at the second row and first column of the matrix.</sub>

### Part 2: _Matrix Arithmetic_

Before we move, it may be necessary to brush up on some linear algebra—specifically, on matrix operations.

#### **Scalar Multiplication**

This one is easy. A **scalar** value is any value that has a magnitude but not a physical direction. Examples of these are temperature, ages, and mass. Vectors, by definition, _are_ have a physical direction, so we don't have to worry about changing it. We simply multiply all of the values inside the matrix by that scalar value to get our result. For example:

![scal-mult](assets/scal-mult.png)

<sub>**Figure 3**: The matrix (1, 2, 3) being multiplied by the scalar value 4.</sub>

#### **Matrix Multiplication**

Matrix multiplication has some interesting rules

### Part 3: _Modeling Operations_

Why matrices? It turns out that both their representative and operative abilities—that is, what they can represent and how they can interact with each other—is a pretty accurate and elegant way of representing transformations on objects.

#### **Scaling**

For example, consider the top of a pyramid that is, say, 100 metres tall. Using cartesian coordinates, we might represent its apex as point (0, 0, 100.0)—that is, it shoots straight upwards 100 metres in the air, but (ideally) does not move away from the origin in neither the x- nor the y-directions. The matrix equivalent of this cartesian notation is a **vector**, and looks something like this:

![0-0-100](assets/0-0-100.svg)

<sub>**Figure 3**: A 1 × 3 matrix, or a _vector_, representing the cartesian point (0.0, 0.0, 100.0).</sub>

Why is this helpful? Let's say we wanted to make our pyramid twice as big. This would mean that its highest point—its apex—would no longer rest at point (0.0, 0.0, 100.0), but rather at point (0.0, 0.0, 200.0). This is known as **scaling**, and is the simplest of the three main transformations that we will learn how to do in this course. This is because, if you have a matrix indicating a point in space, and you want to scale it, all you have to do is multiply each of its coordinate values by that scalar value (e.g. 2). Let's look at the other two.

#### **Rotation**

Here's where matrices really come in handy. Scaling things is easy because you are multiplying a vector (something with both a magnitude and a direction) by a **scalar value** (something that only has a magnitude). But what happens when you want to operate on a vector in a certain direction?

The act of rotation—moving a certain object around a given axis—is, by definition, a directed operation, so multiplying by a scalar won't do. For this, we need a special matrix:

![rot-matrix](assets/rot-matrix.svg)

<sub>**Figure 3**: A 1 × 3 matrix, or a _vector_, representing the cartesian point (0.0, 0.0, 100.0).</sub>