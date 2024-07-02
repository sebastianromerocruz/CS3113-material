#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/mat4x4.hpp"

class ShaderProgram {
    public:
	
		void Load(const char *vertexShaderFile, const char *fragmentShaderFile);
		void Cleanup();

		void SetModelMatrix(const glm::mat4 &matrix);
        void SetProjectionMatrix(const glm::mat4 &matrix);
        void SetViewMatrix(const glm::mat4 &matrix);
	
		void SetColor(float r, float g, float b, float a);
	
        GLuint LoadShaderFromString(const std::string &shaderContents, GLenum type);
        GLuint LoadShaderFromFile(const std::string &shaderFile, GLenum type);
    
        GLuint programID;
    
        GLuint projectionMatrixUniform;
        GLuint modelMatrixUniform;
        GLuint viewMatrixUniform;
		GLuint colorUniform;
	
        GLuint positionAttribute;
        GLuint texCoordAttribute;
    
        GLuint vertexShader;
        GLuint fragmentShader;
};
