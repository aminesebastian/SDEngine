#version 400
                                                                        
in vec2 texCoord0;                                                                       

uniform sampler2D Sprite;
uniform vec3 TINT = vec3(1.0f, 1.0f, 1.0f);

layout (location = 0) out vec4 Output;   
							
void main()	{			
	vec4 textureSample = texture(Sprite, texCoord0);
	if(textureSample.a < 1.0f) {
		discard;
	}
	Output = texture(Sprite, texCoord0) * vec4(TINT, 1.0f);	
}
