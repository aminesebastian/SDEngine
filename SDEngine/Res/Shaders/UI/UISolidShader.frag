#version 400
in vec2 screenPos0;

uniform vec2 RENDER_TARGET_RESOLUTION; // The resolution we are drawing to.
uniform vec2 LOCATION;				   // Bottom left location of the quad.
uniform vec2 CENTER_LOCATION;		   // The center (in pixels) of the quad.
uniform vec2 SIZE;					   // The size (in pixels) of the quad.
uniform vec4 COLOR;					   // The color oft the quad.
uniform vec4 BORDER_RADIUS;			   // The radius of the corners (in pixels) clockwise starting in the top right.
uniform float EDGE_SOFTNESS;		   // How soft the edge of the quad should be (in pixels).

out vec4 FragColor;

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, vec4 Radius) {
    Radius.xy = (CenterPosition.x > 0.0) ? Radius.xy : Radius.zw;
    Radius.x  = (CenterPosition.y > 0.0) ? Radius.x  : Radius.y;
    
    vec2 outerEdge = abs(CenterPosition)-Size+Radius.x;
    return min(max(outerEdge.x,outerEdge.y),0.0) + length(max(outerEdge,0.0)) - Radius.x;
}
void main() {
	// Calculate the center of the quad.
	vec2 centerCoord = LOCATION + SIZE/2.0f;

    // Calculate distance to edge.   
    float distance = roundedBoxSDF(gl_FragCoord.xy - LOCATION - SIZE/2.0f, SIZE / 2.0f, BORDER_RADIUS);
        
    // Smooth the result (free antialiasing).
    float smoothedAlpha =  1.0f-smoothstep(0.0f, EDGE_SOFTNESS * 2.0f, distance);
    FragColor = vec4(COLOR.r, COLOR.g, COLOR.b, COLOR.a * smoothedAlpha);
}