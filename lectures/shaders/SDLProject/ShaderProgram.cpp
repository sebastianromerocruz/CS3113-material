#define GL_SILENCE_DEPRECATION

#include "ShaderProgram.h"

void ShaderProgram::load(const char *vertex_shader_file, const char *fragment_shader_file)
{
    
    // create the vertex shader
    m_vertex_shader = load_shader_from_file(vertex_shader_file, GL_VERTEX_SHADER);
    // create the fragment shader
    m_fragment_shader = load_shader_from_file(fragment_shader_file, GL_FRAGMENT_SHADER);
    
    // Create the final shader program from our vertex and fragment shaders
    m_program_id = glCreateProgram();
    glAttachShader(m_program_id, m_vertex_shader);
    glAttachShader(m_program_id, m_fragment_shader);
    glLinkProgram(m_program_id);
    
    GLint link_success;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &link_success);
    
    if(link_success == GL_FALSE)
    {
        printf("Error linking shader program!\n");
    }
    
    m_model_matrix_uniform      = glGetUniformLocation(m_program_id, "modelMatrix");
    m_projection_matrix_uniform = glGetUniformLocation(m_program_id, "projectionMatrix");
    m_view_matrix_uniform       = glGetUniformLocation(m_program_id, "viewMatrix");
	m_color_uniform             = glGetUniformLocation(m_program_id, "color");
    m_light_position_uniform    = glGetUniformLocation(m_program_id, "lightPosition");
    
    m_position_attribute = glGetAttribLocation(m_program_id, "position");
    m_tex_coord_attribute = glGetAttribLocation(m_program_id, "texCoord");
	
	set_color(1.0f, 1.0f, 1.0f, 1.0f);
    
}

void ShaderProgram::set_light_position(glm::vec3 position)
{
    glUseProgram(m_program_id);
    glUniform2f(m_light_position_uniform, position.x, position.y);
}

void ShaderProgram::cleanup()
{
    glDeleteProgram(m_program_id);
    glDeleteShader(m_vertex_shader);
    glDeleteShader(m_fragment_shader);
}

GLuint ShaderProgram::load_shader_from_file(const std::string &shader_file, GLenum type)
{
    //Open a file stream with the file name
    std::ifstream infile(shader_file);
    
    if(infile.fail())
    {
        std::cout << "Error opening shader file:" << shader_file << std::endl;
    }
    
    //Create a string buffer and stream the file to it
    std::stringstream buffer;
    buffer << infile.rdbuf();
    
    // Load the shader from the contents of the file
    return load_shader_from_string(buffer.str(), type);
}

GLuint ShaderProgram::load_shader_from_string(const std::string &shader_contents, GLenum type)
{
    // Create a shader of specified type
    GLuint shader_id = glCreateShader(type);
    
    // Get the pointer to the C string from the STL string
    const char *shader_string  = shader_contents.c_str();
    GLint shader_string_length = (GLint) shader_contents.size();
    
    // Set the shader source to the string and compile shader
    glShaderSource(shader_id, 1, &shader_string, &shader_string_length);
    glCompileShader(shader_id);
    
    // Check if the shader compiled properly
    GLint compile_success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_success);
    
    // If the shader did not compile, print the error to stdout
    if (compile_success == GL_FALSE)
    {
        GLchar messages[512];
        glGetShaderInfoLog(shader_id, sizeof(messages), 0, &messages[0]);
        std::cout << messages << std::endl;
    }
    
    // return the shader id
    return shader_id;
}

void ShaderProgram::set_color(float r, float g, float b, float a)
{
	glUseProgram(m_program_id);
	glUniform4f(m_color_uniform, r, g, b, a);
}

void ShaderProgram::set_view_matrix(const glm::mat4 &matrix)
{
    glUseProgram(m_program_id);
    glUniformMatrix4fv(m_view_matrix_uniform, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::set_model_matrix(const glm::mat4 &matrix)
{
    glUseProgram(m_program_id);
    glUniformMatrix4fv(m_model_matrix_uniform, 1, GL_FALSE, &matrix[0][0]);
}

void ShaderProgram::set_projection_matrix(const glm::mat4 &matrix)
{
    glUseProgram(m_program_id);
    glUniformMatrix4fv(m_projection_matrix_uniform, 1, GL_FALSE, &matrix[0][0]);    
}
