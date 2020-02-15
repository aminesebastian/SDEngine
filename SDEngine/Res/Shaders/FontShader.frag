#version 400
                                                                        
in vec2 texCoord0;                                                                       

uniform sampler2D Atlas;
uniform vec4 COLOR;
uniform float WIDTH;
uniform float EDGE;

out vec4 FragColor;	
													
void main()	{			
	float distance = 1.0f - texture(Atlas, texCoord0).a;
	float alpha    = 1.0f - smoothstep(WIDTH, EDGE, distance);

	if(alpha <= 0.0f){
		discard;
	}

	FragColor      = COLOR * vec4(1.0f, 1.0f, 1.0f, alpha);
}
