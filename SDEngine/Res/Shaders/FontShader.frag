#version 400
                                                                        
in vec2 texCoord0;                                                                       

uniform sampler2D Atlas;

uniform vec2 RENDER_TARGET_RESOLUTION;

uniform vec4 COLOR;
uniform float WIDTH;
uniform float EDGE;

uniform vec2 MIN_CLIP;
uniform vec2 MAX_CLIP;

uniform vec2 SCALE;

out vec4 FragColor;	
													
void main()	{		
	float distance       = 1.0f - texture(Atlas, texCoord0).a;
	float totalSmoothing = 0.25f / (100.0f * SCALE.x);
	float alpha          = 1.0f - smoothstep(0.5 - totalSmoothing, 0.5 + totalSmoothing, distance);

	if(alpha <= 0.0f){
		discard;
	}

	FragColor      = COLOR * vec4(1.0f, 1.0f, 1.0f, alpha);
}
