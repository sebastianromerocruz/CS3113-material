//
//  sprite.cpp
//  SDLProject
//
//  Created by Sebastián Romero Cruz on 6/16/22.
//  Copyright © 2022 ctg. All rights reserved.
//

#include "sprite.hpp"

//Sprite::Sprite(std::string sprite_filepath)
//{
//    this->m_sprite_filepath = sprite_filepath;
//    load_texture();
//}
//
//void Sprite::load_texture()
//{
//    int filepath_length = this->m_sprite_filepath.length();
//    char filepath[filepath_length + 1];
//    strcpy(filepath, this->m_sprite_filepath.c_str());
//    
//    // STEP 1: Loading the image file
//    int width, height, number_of_components;
//    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);
//    
//    if (image == NULL)
//    {
//        LOG("Unable to load image. Make sure the path is correct.");
//        assert(false);
//    }
//    
//    // STEP 2: Generating and binding a texture ID to our image
//    GLuint textureID;
//    glGenTextures(NUMBER_OF_TEXTURES, &this->m_texture_id);
//    glBindTexture(GL_TEXTURE_2D, &this->m_texture_id);
//    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);
//    
//    // STEP 3: Setting our texture filter parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    
//    // STEP 4: Releasing our file from memory and returning our texture id
//    stbi_image_free(image);
//}
