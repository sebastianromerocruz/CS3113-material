#include "Mesh.h"

void Mesh::LoadOBJ(const char *fileName) {
    
    std::ifstream infile(fileName);
    std::string line;
    std::vector<float> fileVertices;
    std::vector<float> fileUVs;
    std::vector<float> fileNormals;
    
    while (getline(infile, line)) {
        
        std::istringstream sStream(line);
        std::string token;
        getline(sStream, token, ' ');
        
        if(token == "v") {
            while(getline(sStream, token, ' ')) {
                if(token.size() > 0) {
                    fileVertices.push_back(atof(token.c_str()));
                }
            }
        } else if(token == "vn") {
            while(getline(sStream, token, ' ')) {
                if(token.size() > 0) {
                    fileNormals.push_back(atof(token.c_str()));
                }
            }
        } else if(token == "vt") {
            int numUVs = 0;
            while(getline(sStream, token, ' ') && numUVs < 2) {
                if(token.size() > 0) {
                    printf("token: [%s]\n", token.c_str());
                    fileUVs.push_back(atof(token.c_str()));
                    numUVs++;
                }
            }
        } else if(token == "f") {
            while(getline(sStream, token, ' ')) {
                std::istringstream faceStream(token);
                std::string faceToken;
                int type = 0;
                while(getline(faceStream, faceToken, '/')) {
                    if(faceToken.size() > 0 && faceToken != "\r") {
                        int index = atoi(faceToken.c_str())-1;
                        switch(type) {
                            case 0:
                                vertices.push_back(fileVertices[index*3]);
                                vertices.push_back(fileVertices[(index*3)+1]);
                                vertices.push_back(fileVertices[(index*3)+2]);
                                break;
                            case 1:
                                uvs.push_back(fileUVs[(index*2)]);
                                uvs.push_back(1.0f - fileUVs[(index*2)+1]);
                                break;
                            case 2:
                                normals.push_back(fileNormals[(index*3)]);
                                normals.push_back(fileNormals[(index*3)+1]);
                                normals.push_back(fileNormals[(index*3)+2]);
                                break;
                        }
                        type++;
                    }
                }
            }
        }
    }
}

void Mesh::Render(ShaderProgram *program) {
    
    glVertexAttribPointer(program->get_position_attribute(), 3, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->get_position_attribute());
    
    glVertexAttribPointer(program->get_tex_coord_attribute(), 2, GL_FLOAT, false, 0, uvs.data());
    glEnableVertexAttribArray(program->get_tex_coord_attribute());
    
    glDrawArrays(GL_TRIANGLES, 0, (int)(vertices.size() / 3));
    
    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coord_attribute());
}
