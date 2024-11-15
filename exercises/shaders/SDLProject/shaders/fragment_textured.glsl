uniform sampler2D diffuse;
varying vec2 tex_coord_var;

void main()
{
    gl_FragColor = texture2D(diffuse, tex_coord_var);
}
