uniform sampler2D diffuse;
varying vec2 texCoordVar;

void main()
{
    vec4 colour = texture2D(diffuse, texCoordVar);
    vec3 luminance = vec3(dot(vec3(0.2126, 0.7152, 0.0722), colour.rgb));
    vec3 interpolation = mix(luminance, colour.rgb, 2.0);
    
    gl_FragColor = vec4(interpolation, colour.a);
}
