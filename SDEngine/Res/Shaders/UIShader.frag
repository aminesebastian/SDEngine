#version 400
in vec2 texCoord0;

uniform vec2 RENDER_TARGET_RESOLUTION;
uniform float SCREEN_ASPECT_RATIO;
uniform float SHAPE_ASPECT_RATIO;
uniform float X_BORDER_RADIUS;
uniform float Y_BORDER_RADIUS;
uniform float BORDER_RADIUS;
uniform vec2 SHAPE_SIZE;
uniform vec4 COLOR;

out vec4 FragColor;
	
float roundedRectangle (vec2 uv, vec2 size, float radius, float thickness) {
  float d = length(max(abs(uv), size) - size) - radius;
  return 1.0 - smoothstep(thickness, thickness+0.01, d);
}

void main()	{		
	vec2 npos = texCoord0;		   // 0.0 .. 1.0
	vec2 uv = (2.0 * npos - 1.0);          // -1.0 .. 1.0
	
	float adjustedBorder = BORDER_RADIUS / 100.0f;


	float thickness = 0.01f;
	float sizeOffset = 1.0f - thickness - 0.01f;
	vec2 size = vec2(sizeOffset - adjustedBorder, sizeOffset - adjustedBorder);
	float intensity = roundedRectangle(uv, size, adjustedBorder, thickness);
	
	FragColor = mix(vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(COLOR.rgb, 1.0f), intensity) * COLOR.a;
}


