#include "Mesh.h"

void Mesh::load_OBJ(const char *file_name, float texture_repeat_rate) {
    
    std::ifstream in_file(file_name);
    std::string line;
    std::vector<float> file_vertices;
    std::vector<float> file_uvs;
    std::vector<float> file_normals;
    
    while (getline(in_file, line)) {
        
        std::istringstream string_stream(line);
        std::string token;
        getline(string_stream, token, ' ');
        
        if(token == "v") {
            while(getline(string_stream, token, ' ')) {
                if(token.size() > 0) {
                    file_vertices.push_back(atof(token.c_str()));
                }
            }
        } else if(token == "vn") {
            while(getline(string_stream, token, ' ')) {
                if(token.size() > 0) {
                    file_normals.push_back(atof(token.c_str()));
                }
            }
        } else if(token == "vt") {
            int num_UVs = 0;
            while(getline(string_stream, token, ' ') && num_UVs < 2) {
                if(token.size() > 0) {
                    printf("token: [%s]\n", token.c_str());
                    file_uvs.push_back(atof(token.c_str()) * texture_repeat_rate);
                    num_UVs++;
                }
            }
        } else if(token == "f") {
            while(getline(string_stream, token, ' ')) {
                std::istringstream faceStream(token);
                std::string faceToken;
                int type = 0;
                while(getline(faceStream, faceToken, '/')) {
                    if(faceToken.size() > 0 && faceToken != "\r") {
                        int index = atoi(faceToken.c_str())-1;
                        switch(type) {
                            case 0:
                                vertices.push_back(file_vertices[index*3]);
                                vertices.push_back(file_vertices[(index*3)+1]);
                                vertices.push_back(file_vertices[(index*3)+2]);
                                break;
                            case 1:
                                uvs.push_back(file_uvs[(index*2)]);
                                uvs.push_back(1.0f - file_uvs[(index*2)+1]);
                                break;
                            case 2:
                                normals.push_back(file_normals[(index*3)]);
                                normals.push_back(file_normals[(index*3)+1]);
                                normals.push_back(file_normals[(index*3)+2]);
                                break;
                        }
                        type++;
                    }
                }
            }
        }
    }
}

void Mesh::render(ShaderProgram *program) {
    
    glVertexAttribPointer(program->positionAttribute, 3, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, uvs.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, (int)(vertices.size() / 3));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
