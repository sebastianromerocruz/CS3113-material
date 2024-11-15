attribute vec4 position;
attribute vec2 tex_coord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

varying vec2 tex_coord_var;

void main()
{
    vec4 p = view_matrix * model_matrix  * position;
    tex_coord_var = tex_coord;
    gl_Position = projection_matrix * p;
}
