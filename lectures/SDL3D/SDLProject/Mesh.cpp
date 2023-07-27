#include "Mesh.h"

void Mesh::load_OBJ(const char *file_name)
{
    
    std::ifstream in_file(file_name);
    std::string line;
    
    std::vector<float> file_vertices;
    std::vector<float> file_uvs;
    std::vector<float> file_normals;
    
    while (getline(in_file, line))
    {
        
        std::istringstream string_stream(line);
        std::string token;
        getline(string_stream, token, ' ');
        
        if(token == "v")
        {
            while(getline(string_stream, token, ' '))
            {
                if(token.size() > 0)
                {
                    file_vertices.push_back(atof(token.c_str()));
                }
            }
        } else if(token == "vn")
        {
            while(getline(string_stream, token, ' '))
            {
                if(token.size() > 0)
                {
                    file_normals.push_back(atof(token.c_str()));
                }
            }
        } else if(token == "vt")
        {
            int num_UVs = 0;
            
            while(getline(string_stream, token, ' ') && num_UVs < 2)
            {
                if(token.size() > 0)
                {
                    printf("token: [%s]\n", token.c_str());
                    file_uvs.push_back(atof(token.c_str()));
                    num_UVs++;
                }
            }
        } else if(token == "f")
        {
            while(getline(string_stream, token, ' '))
            {
                std::istringstream face_stream(token);
                std::string face_token;
                int type = 0;
                while(getline(face_stream, face_token, '/')) {
                    if(face_token.size() > 0 && face_token != "\r")
                    {
                        int index = atoi(face_token.c_str())-1;
                        switch(type)
                        {
                            case 0:
                                m_vertices.push_back(file_vertices[index*3]);
                                m_vertices.push_back(file_vertices[(index*3)+1]);
                                m_vertices.push_back(file_vertices[(index*3)+2]);
                                break;
                            case 1:
                                m_uv_coordinates.push_back(file_uvs[(index*2)]);
                                m_uv_coordinates.push_back(1.0f - file_uvs[(index*2)+1]);
                                break;
                            case 2:
                                m_normals.push_back(file_normals[(index*3)]);
                                m_normals.push_back(file_normals[(index*3)+1]);
                                m_normals.push_back(file_normals[(index*3)+2]);
                                break;
                        }
                        type++;
                    }
                }
            }
        }
    }
}

void Mesh::render(ShaderProgram *program)
{
    
    glVertexAttribPointer(program->get_position_attribute(), 3, GL_FLOAT, false, 0, m_vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coord_attribute(), 2, GL_FLOAT, false, 0, m_uv_coordinates.data());
    glEnableVertexAttribArray(program->get_tex_coord_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, (int)(m_vertices.size() / 3));
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coord_attribute());
}
