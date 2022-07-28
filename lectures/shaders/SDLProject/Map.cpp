#include "Map.h"

Map::Map(int width, int height, unsigned int *level_data, GLuint texture_id, float tile_size, int tile_count_x, int tile_count_y)
{
    this->width = width;
    this->height = height;
    
    this->level_data = level_data;
    this->texture_id = texture_id;
    
    this->tile_size = tile_size;
    this->tile_count_x = tile_count_x;
    this->tile_count_y = tile_count_y;
    
    this->build();
}

void Map::build()
{
    for(int y = 0; y < this->height; y++)
    {
        for(int x = 0; x < this->width; x++) {
            int tile = this->level_data[y * this->width + x];
            
            if (tile == 0) continue;
            
            float u = (float) (tile % this->tile_count_x) / (float) this->tile_count_x;
            float v = (float) (tile / this->tile_count_x) / (float) this->tile_count_y;
            
            float tile_width = 1.0f/ (float) this->tile_count_x;
            float tile_height = 1.0f/ (float) this->tile_count_y;
            
            float x_offset = -(this->tile_size / 2); // From center of tile
            float y_offset = (this->tile_size / 2); // From center of tile
            
            this->vertices.insert(vertices.end(), {
                x_offset + (this->tile_size * x), y_offset + -this->tile_size * y,
                x_offset + (this->tile_size * x), y_offset + (-this->tile_size * y) - this->tile_size,
                x_offset + (this->tile_size * x) + this->tile_size, y_offset + (-this->tile_size * y) - this->tile_size,
                x_offset + (this->tile_size * x), y_offset + -this->tile_size * y,
                x_offset + (this->tile_size * x) + this->tile_size, y_offset + (-this->tile_size * y) - tile_size,
                x_offset + (this->tile_size * x) + this->tile_size, y_offset + -this->tile_size * y
            });
            
            this->texture_coordinates.insert(texture_coordinates.end(), {
                u, v,
                u, v + (tile_height),
                u + tile_width, v + (tile_height),
                u, v,
                u + tile_width, v + (tile_height),
                u + tile_width, v
            });
        }
    }
    
    this->left_bound   = 0 - (this->tile_size / 2);
    this->right_bound  = (this->tile_size * this->width) - (this->tile_size / 2);
    this->top_bound    = 0 + (this->tile_size / 2);
    this->bottom_bound = -(this->tile_size * this->height) + (this->tile_size / 2);
}

void Map::render(ShaderProgram *program)
{
    glm::mat4 model_matrix = glm::mat4(1.0f);
    program->SetModelMatrix(model_matrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, this->vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, this->texture_coordinates.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, this->texture_id);
    
    glDrawArrays(GL_TRIANGLES, 0, (int) this->vertices.size() / 2);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

bool Map::is_solid(glm::vec3 position, float *penetration_x, float *penetration_y)
{
    *penetration_x = 0;
    *penetration_y = 0;
    
    if (position.x < this->left_bound || position.x > this->right_bound) return false;
    if (position.y > this->top_bound || position.y < this->bottom_bound) return false;
    
    int tile_x = floor((position.x + (this->tile_size / 2)) / this->tile_size);
    int tile_y = -(ceil(position.y - (this->tile_size / 2))) / this->tile_size; // Our array counts up as Y goes down.
    
    if (tile_x < 0 || tile_x >= this->width) return false;
    if (tile_y < 0 || tile_y >= this->height) return false;
    
    int tile = level_data[tile_y * this->width + tile_x];
    if (tile == 0) return false;
    
    float tile_center_x = (tile_x * this->tile_size);
    float tile_center_y = -(tile_y * this->tile_size);
    
    *penetration_x = (this->tile_size / 2) - fabs(position.x - tile_center_x);
    *penetration_y = (this->tile_size / 2) - fabs(position.y - tile_center_y);
    
    return true;
}
