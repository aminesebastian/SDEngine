#version 400                                                                        
                        
uniform mat4 MODEL_MATRIX;
uniform vec2 RENDER_TARGET_RESOLUTION;
uniform float BORDER_RADIUS;

layout (location = 0) in vec3 position;                                             
layout (location = 1) in vec2 texCoord;                                                                                          
                             
out vec2 texCoord0;                                                                                                                         

void main() {       
    gl_Position    = MODEL_MATRIX * vec4(position, 1.0);
    texCoord0      = texCoord;         
}