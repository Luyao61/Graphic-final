
#version 120
varying float xpos;
varying float ypos;
varying float zpos;
varying vec3 normal;
varying vec3 pcolor;

uniform sampler2D myTexture;

//varying vec4 pcolor;
void main (void) {
    
    vec4 color = texture2D(myTexture,gl_TexCoord[0].st);
    if (color[0] < 0.5 && color[1] <0.5 && color[2] <0.5) {
        discard;
    }
    else{
        //pcolor = (normal * 0.5) + (vec3(0.5,0.5,0.5));
        gl_FragColor = vec4 (normal[0]*0.5+0.5, normal[1]*0.5+0.5, normal[2]*0.5+0.5, 1.0);
    }
}

