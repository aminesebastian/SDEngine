#version 400
in vec2 texCoord0;
in vec2 screenPos0;

uniform vec2 RENDER_TARGET_RESOLUTION;
uniform float SCREEN_ASPECT_RATIO;
uniform float SHAPE_ASPECT_RATIO;
uniform float X_BORDER_RADIUS;
uniform float Y_BORDER_RADIUS;
uniform float BORDER_RADIUS;
uniform vec2 SHAPE_SIZE;
uniform vec4 TINT;

uniform sampler2D IMAGE;

out vec4 FragColor;

void main() {;
	vec2 absoluteShapeCoordinate = abs((texCoord0 *2.0) - 1.0f) * SHAPE_SIZE;
	float doubledRadius = BORDER_RADIUS * 2.0f;
	vec2 cornersVector  = SHAPE_SIZE - vec2(doubledRadius);
	float thickness		= 4.0f;
	vec2 chokeVector	= SHAPE_SIZE - thickness;
	float radialAlpha	= 1.0f;
	float squareAlpha	= 1.0f;

	if(absoluteShapeCoordinate.x > cornersVector.x && absoluteShapeCoordinate.y > cornersVector.y) {
		radialAlpha = (doubledRadius-length(cornersVector - absoluteShapeCoordinate))/doubledRadius;
	}else{
		float horizontal = smoothstep(SHAPE_SIZE.x, SHAPE_SIZE.x - thickness, absoluteShapeCoordinate.x); 
		float vertical = smoothstep(SHAPE_SIZE.y, SHAPE_SIZE.y - thickness, absoluteShapeCoordinate.y); 
	}

	FragColor = texture(IMAGE, texCoord0) * TINT * smoothstep(0.0, 0.00, radialAlpha) * squareAlpha;
}


