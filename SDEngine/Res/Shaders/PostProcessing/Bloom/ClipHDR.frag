#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform sampler2D Input;
uniform float THRESHOLD;

layout (location = 0) out vec4 Result;   

void main()	{	
	vec3 lighting	 = texture(Input, texCoord0).rgb;
	float brightness = dot(lighting.rgb, vec3(0.2126, 0.7152, 0.0722));
	Result			 = vec4(lighting * max(0.0, brightness-THRESHOLD), brightness);
}