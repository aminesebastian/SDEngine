#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D inputTexture;

uniform vec2 SCALE;

void main()	{	
	vec4 color = vec4(0.0);

	color += texture(inputTexture, texCoord0 + (vec2(-3.0) * SCALE.xy)) * (1.0/64.0);
	color += texture(inputTexture, texCoord0 + (vec2(-2.0) * SCALE.xy)) * (6.0/64.0);
	color += texture(inputTexture, texCoord0 + (vec2(-1.0) * SCALE.xy)) * (15.0/64.0);
	color += texture(inputTexture, texCoord0 + (vec2(0.0) * SCALE.xy))  * (20.0/64.0);
	color += texture(inputTexture, texCoord0 + (vec2(1.0) * SCALE.xy))  * (15.0/64.0);
	color += texture(inputTexture, texCoord0 + (vec2(2.0) * SCALE.xy))  * (6.0/64.0);
	color += texture(inputTexture, texCoord0 + (vec2(3.0) * SCALE.xy))  * (1.0/64.0);

	gl_FragColor = color;
}