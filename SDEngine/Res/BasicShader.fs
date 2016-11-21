#version 120

varying vec2 texCoord0;
varying vec3 normal0;
varying out vec4 outColor;

uniform sampler2D albedo;

uniform vec3 lightDirection;
uniform float lightIntensity;
uniform vec3 lightColor;

uniform float ambientIntensity;
uniform vec3 ambientColor;

void main() {
	vec4 ambientContribution = vec4(ambientColor*ambientIntensity, 1.0);
	float directionalLight = max(dot(-lightDirection, normal0), 0.0)*lightIntensity*2;	
	
	outColor = (directionalLight +  ambientContribution)* texture2D(albedo, texCoord0);
		
}