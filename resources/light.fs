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
varying vec3 ec_pos;

void main(void) { 
	vec4 invertedColor = vec4(1.0 - vFinalColor.r, 1.0 - vFinalColor.g, 1.0 - vFinalColor.b, 1.0);
	vec3 ec_normal = normalize(cross(dFdx(ec_pos), dFdy(ec_pos)));
	gl_FragColor = vec4(0.5, 0.5, 0.5, 1.0) + vec4(ec_normal*0.5, 1.0);
}
