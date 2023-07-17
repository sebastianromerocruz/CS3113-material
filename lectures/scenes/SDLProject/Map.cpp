#include "Map.h"

Map::Map(int width, int height, unsigned int *level_data, GLuint texture_id, float tile_size, int tile_count_x, int tile_count_y)
{
    m_width = width;
    m_height = height;
    
    m_level_data = level_data;
    m_texture_id = texture_id;
    
    m_tile_size = tile_size;
    m_tile_count_x = tile_count_x;
    m_tile_count_y = tile_count_y;
    
    build();
}

void Map::build()
{
    for(int y = 0; y < m_height; y++)
    {
        for(int x = 0; x < m_width; x++) {
            int tile = m_level_data[y * m_width + x];
            
            if (tile == 0) continue;
            
            float u = (float) (tile % m_tile_count_x) / (float) m_tile_count_x;
            float v = (float) (tile / m_tile_count_x) / (float) m_tile_count_y;
            
            float tile_width = 1.0f/ (float) m_tile_count_x;
            float tile_height = 1.0f/ (float) m_tile_count_y;
            
            float x_offset = -(m_tile_size / 2); // From center of tile
            float y_offset = (m_tile_size / 2); // From center of tile
            
            this->m_vertices.insert(m_vertices.end(), {
                x_offset + (m_tile_size * x), y_offset + -m_tile_size * y,
                x_offset + (m_tile_size * x), y_offset + (-m_tile_size * y) - m_tile_size,
                x_offset + (m_tile_size * x) + m_tile_size, y_offset + (-m_tile_size * y) - m_tile_size,
                x_offset + (m_tile_size * x), y_offset + -m_tile_size * y,
                x_offset + (m_tile_size * x) + m_tile_size, y_offset + (-m_tile_size * y) - m_tile_size,
                x_offset + (m_tile_size * x) + m_tile_size, y_offset + -m_tile_size * y
            });
            
            this->m_texture_coordinates.insert(m_texture_coordinates.end(), {
                u, v,
                u, v + (tile_height),
                u + tile_width, v + (tile_height),
                u, v,
                u + tile_width, v + (tile_height),
                u + tile_width, v
            });
        }
    }
    
    m_left_bound   = 0 - (m_tile_size / 2);
    m_right_bound  = (m_tile_size * m_width) - (m_tile_size / 2);
    m_top_bound    = 0 + (m_tile_size / 2);
    m_bottom_bound = -(m_tile_size * m_height) + (m_tile_size / 2);
}

void Map::render(ShaderProgram *program)
{
    glm::mat4 model_matrix = glm::mat4(1.0f);
    program->SetModelMatrix(model_matrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, m_vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, m_texture_coordinates.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    
    glDrawArrays(GL_TRIANGLES, 0, (int) m_vertices.size() / 2);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

bool Map::is_solid(glm::vec3 position, float *penetration_x, float *penetration_y)
{
    *penetration_x = 0;
    *penetration_y = 0;
    
    if (position.x < m_left_bound || position.x > m_right_bound) return false;
    if (position.y > m_top_bound || position.y < m_bottom_bound) return false;
    
    int tile_x = floor((position.x + (m_tile_size / 2)) / m_tile_size);
    int tile_y = -(ceil(position.y - (m_tile_size / 2))) / m_tile_size; // Our array counts up as Y goes down.
    
    if (tile_x < 0 || tile_x >= m_width) return false;
    if (tile_y < 0 || tile_y >= m_height) return false;
    
    int tile = m_level_data[tile_y * m_width + tile_x];
    if (tile == 0) return false;
    
    float tile_center_x = (tile_x * m_tile_size);
    float tile_center_y = -(tile_y * m_tile_size);
    
    *penetration_x = (m_tile_size / 2) - fabs(position.x - tile_center_x);
    *penetration_y = (m_tile_size / 2) - fabs(position.y - tile_center_y);
    
    return true;
}
