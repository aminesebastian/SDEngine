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
uniform vec4 COLOR;

out vec4 FragColor;

float udRoundBox( vec3 p, vec3 b, float r )
{
  return length(max(abs(p)-b,0.0))-r;
}

// substracts shape d1 from shape d2
float opS( float d1, float d2 )
{
    return max(-d1,d2);
}

// to get the border of a udRoundBox, simply substract a smaller udRoundBox !
float udRoundBoxBorder( vec3 p, vec3 b, float r, float borderFactor )
{
  return opS(udRoundBox(p, b*borderFactor, r), udRoundBox(p, b, r));
}

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
		//radialAlpha = smoothstep(0.0, 0.05, radialAlpha);
	}else{
		float horizontal = smoothstep(SHAPE_SIZE.x, SHAPE_SIZE.x - thickness, absoluteShapeCoordinate.x); 
		float vertical = smoothstep(SHAPE_SIZE.y, SHAPE_SIZE.y - thickness, absoluteShapeCoordinate.y); 
		//squareAlpha = horizontal * vertical;
	}

	FragColor = COLOR * smoothstep(0.0, 0.00, radialAlpha) * squareAlpha;


//	vec2 uv = texCoord0;
//	
//    // box setup
//    vec3 boxPosition = vec3(0.5, 0.5, 0.0);
//    vec3 boxSize = vec3(SHAPE_SIZE/RENDER_TARGET_RESOLUTION, 1.0f);
//    float boxRounding = 0.1;
//    
//    // render the box
//    vec3 curPosition = vec3(uv, 0.0);    
//    float dist = udRoundBoxBorder(curPosition - boxPosition, boxSize, boxRounding, 0.0);    
//    float THRESHOLD = 0.0001;
//    if (dist <= THRESHOLD) {
//        FragColor = vec4(1.0);
//	}
	//FragColor = vec4(vec3(udRoundBox(vec3(adjustedCoords, 0.0f), vec3(2.0, 2.0, 0.0), .01f)), 0.2f); 
}


