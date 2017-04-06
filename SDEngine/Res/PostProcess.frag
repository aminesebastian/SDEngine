#version 400
                                                                        
in vec2 texCoord0;                                                                  

uniform vec3 CAMERA_POS;

uniform sampler2D worldPosition;
uniform sampler2D albedo;
uniform sampler2D RMAO;
uniform sampler2D normal;
uniform sampler2D texCoord;
uniform sampler2D matID;
uniform sampler2D HDR;
uniform sampler2D finalComp;

out vec4 fragColor;


uniform float offset[10] = float[]( 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5 );
uniform float weight[10] = float[]( 0.2270270270, 0.1945945946, 0.1216216216,
                                   0.0540540541, 0.0162162162, 0.2270270270, 0.1945945946, 0.1216216216,
                                   0.0540540541, 0.0162162162  );
void main()	{	
    vec4 bloom = texture( HDR, texCoord0) * weight[0];
	float bloomSizeThreshold = 1024 * min(1.0, max(0.25, length(texture(HDR, texCoord0))));

    for (int i=1; i<10; i++) {
        bloom +=
            texture( HDR, ( texCoord0+vec2(0.0, offset[i])/bloomSizeThreshold ) )
                * weight[i];
        bloom +=
            texture( HDR, ( texCoord0+vec2(0.0, -offset[i])/bloomSizeThreshold ) )
                * weight[i];

		bloom +=
            texture( HDR, ( texCoord0+vec2(offset[i], 0.0)/bloomSizeThreshold ) )
                * weight[i];
        bloom +=
            texture( HDR, ( texCoord0+vec2(-offset[i], 0.0)/bloomSizeThreshold ) )
                * weight[i];
		//
        bloom +=
            texture( HDR, ( texCoord0+vec2(offset[i], offset[i])/bloomSizeThreshold ) )
                * weight[i];
        bloom +=
            texture( HDR, ( texCoord0+vec2(offset[i], -offset[i])/bloomSizeThreshold ) )
                * weight[i];

		bloom +=
            texture( HDR, ( texCoord0+vec2(-offset[i], offset[i])/bloomSizeThreshold ) )
                * weight[i];
        bloom +=
            texture( HDR, ( texCoord0+vec2(-offset[i], -offset[i])/bloomSizeThreshold ) )
                * weight[i];
    }

	float gamma = 2.2;
	vec4 composite = (bloom) + texture(finalComp, texCoord0);
	//vec4 composite = texture(finalComp, texCoord0);
	vec4 gammaCorrected = pow(composite, vec4(1.0 / gamma));
    fragColor = gammaCorrected;
}