<h2 align=center>Week 10</h2>

<h1 align=center>Exercise: <em>Special Effects</em></h1>

<h3 align=center>UH IDK WHAT TO PUT HERE PLEASE EDIT THIS</h3>

### Instructions

Last class, we learnt how to create the fading in and out special effects for our program. In this exercise, let's try to implement an earthquake effect.

![shake](/assets/shake.gif)

<sub>**Figure 1**: George so heavy the ground shakes when he falls down</sub>

Modify [**`main.cpp`**](SDLProject/main.cpp), [**`Effects.cpp`**](SDLProject/Effects.cpp), and [**`Effects.h`**](SDLProject/Effects.h) to make the screen shake after George landed after falling down (but not necessarily from jumping).

There is no "right" way to do this, you can implement it however you want to. Nonetheless, we will give you an outline to help you get started.

#### Step 1: Set up the necessary components
1. In [**`Effects.h`**](SDLProject/Effects.h), assign a new EffectType - `SHAKE`. If you need some variables for your shaking effect, also add them here.

**Note** that if you added any variables, don't forget to initialize them in [**`Effects::Effects`**](SDLProject/Effects.cpp) if necessary!

2. In [**`Effects::start`**](SDLProject/Effects.cpp), create a case for `SHAKE` and start your effect here:
```cpp
// ————————————————————— START SHAKING EFFECT ————————————————————— //

// ———————————————————————————————————————————————————————————————— //
```

#### Step 2: Implement your shakng effect
In [**`Effects::update`**](SDLProject/Effects.cpp), create a case for `SHAKE` and implement your effect here:
```cpp
// ——————————— YOUR IMPLEMENTATION OF THE SHAKING EFFECT ——————————— //
        
// ————————————————————————————————————————————————————————————————— //
```
**Hint**: Think of what "shaking" means in terms of view matrix!

**Another hint**: You don't have access to `g_view_matrix` here in Effects.cpp, but main.cpp has access to Effects' ***public*** attributes. And any changes to `g_view_matrix` should be done in main!

#### Step 3: Setting up
4. In [**`main.cpp`**](SDLProject/main.cpp) update(), add your condition to start the shaking effect in the provided space:
```cpp
// ——————————————— CONDITION FOR SHAKING EFFECT ——————————————— //
        
// ———————————————————————————————————————————————————————————— //
```
The code to start the effect is 
```cpp
g_effects->start(SHAKE, <effect_speed>)
```
5. Lastly, if you're updating your `g_view_matrix` (which you should), do it here:
```cpp
// ———————————————————— UPDATE VIEW MATRIX ———————————————————— //

// ———————————————————————————————————————————————————————————— //
```
