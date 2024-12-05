attribute vec4 position;
attribute vec2 tex_coord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

varying vec2 tex_coord_var;
varying vec2 pix_position;

void main()
{
    vec4 p = model_matrix * position;
    pix_position = vec2(p.x, p.y);
    tex_coord_var = tex_coord;
    gl_Position = projection_matrix * view_matrix * p;
}
