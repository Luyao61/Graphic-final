#version 120

varying vec3 normal;

varying float xpos;
varying float ypos;
varying float zpos;
void main(void) {
    
    normal = normalize(gl_NormalMatrix * gl_Normal);

    xpos = clamp(gl_Vertex.x,0.0,1.0);
    ypos = clamp(gl_Vertex.y,0.0,1.0);
    zpos = clamp(gl_Vertex.z,0.0,1.0);
    
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_Position = ftransform();
}
