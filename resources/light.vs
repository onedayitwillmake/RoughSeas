// Simple Lambertian lighting
// Mario Gonzalez
//
// Based on 'WebGL - A Beginners Guide'

attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform mat4 uNMatrix;

uniform vec3 uLightDirection;	// Incoming light source direction
uniform vec4 uLightDiffuse;		// Light diffuse component
uniform vec4 uMaterialDiffuse;	// Material diffuse color

varying vec4 vFinalColor;
 
void main( void ) {
	vec3 N = normalize( vec3( uNMatrix * vec4( gl_Normal, 1.0) ) );
	vec3 L = normalize( uLightDirection );
	float lambertTerm = dot( N, -L );

	vFinalColor = uMaterialDiffuse * uLightDiffuse * gl_Color * lambertTerm;
	vFinalColor.a = 1.0;

	gl_Position = uPMatrix * uMVMatrix * vec4( gl_Vertex.xyz, 1.0 );
}