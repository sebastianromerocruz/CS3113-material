<h2 align=center>Lecture 13</h2>

<h1 align=center>Exercise: <em>Entity Types</em></h1>

<h3 align=center>1 Messidor, Year CCXXXI</h3>

### Instructions

Using the enum [**`EntityType`**](SDLProject/Entity.h) and the `Entity` class's `m_is_active` and `m_type` attributes, write code that will make a random platform disappear when our player steps on it:

![example](assets/example.gif)

<sub>**Figure 1**: Desired behaviour.</sub>

Note that, in order to generate a random integer from 0 to `limit` in C++, one needs to do the following:

```cpp
#include <cstdlib>

int main()
{
    int limit = 100;
    int random_int = rand() % limit;

    return 0;
}
```