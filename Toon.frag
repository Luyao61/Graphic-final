#version 120
varying vec3 normal;

void main()
{
    float intensity;
    vec4 color;
    vec3 n = normalize(normal);
    intensity = dot(vec3(gl_LightSource[0].position),n);

    if (intensity > 0.95)
        color = vec4(gl_Color.x,gl_Color.y,gl_Color.z,1.0);
    else if (intensity > 0.5)
        color = vec4(gl_Color.x * 0.6 ,gl_Color.y * 0.6 ,gl_Color.z * 0.6 ,1.0);
    else if (intensity > 0.25)
        color = vec4(gl_Color.x * 0.4 ,gl_Color.y * 0.4 ,gl_Color.z * 0.4 ,1.0);
    else
        color = vec4(gl_Color.x * 0.2 ,gl_Color.y * 0.2 ,gl_Color.z * 0.2 ,1.0);
    gl_FragColor = color;
    
}
