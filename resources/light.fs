// Simple Lambertian lighting
// Mario Gonzalez
//
// Based on 'WebGL - A Beginners Guide'
#ifdef GL_SL
precision highp float;
#endif

uniform vec3 uLightDirection;	// Incoming light source direction
uniform vec4 uLightDiffuse;		// Light diffuse component
uniform vec4 uMaterialDiffuse;	// Material diffuse color

varying vec4 vFinalColor;

void main(void) { 
	vec4 invertedColor = vec4(1.0 - vFinalColor.r, 1.0 - vFinalColor.g, 1.0 - vFinalColor.b, 1.0);
	gl_FragColor = invertedColor;
}
