//
//  helper.cpp
//  SDLProject
//
//  Created by Sebastián Romero Cruz on 5/31/22.
//  Copyright © 2022 ctg. All rights reserved.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

// We might not need all these libraries yet, but I'll leave them there just in case
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <stdio.h>

const char MAT_SEP = '\t';

/**
 * Prints a formatted representation of a size x size matrix onto
 * the user's console. The \t character is used for horizontal
 * separation.
 *
 * For example, printing a 4 x 4 identity matrix
 *
 *      print_matrix(glm::mat4(1.0f), 4);
 *
 * Prints:
 *
 *     1    0    0    0
 *     0    1    0    0
 *     0    0    1    0
 *     0    0    0    1
 *
 * @param matrix Reference to the matrix.
 * @param size The size of the matrix
 */
void print_matrix(glm::mat4 &matrix, int size)
{
    for (auto row = 0 ; row < size ; row++)
    {
        for (auto col = 0 ; col < size ; col++)
        {
            // Print row
            std::cout << matrix[row][col] << MAT_SEP;
        }
        
        // Visually "move on" to the next row
        std::cout << "\n";
    }
}
