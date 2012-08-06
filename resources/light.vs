// Simple Lambertian lighting
// Mario Gonzalez
//
// Based on 'WebGL - A Beginners Guide'

attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform mat4 uNMatrix;

varying vec3 vNormal;
varying vec4 vPosition;
varying vec3 vEyeVec;
 
void main( void ) {
	vec4 vertex = uMVMatrix * vec4( gl_Vertex.xyz, 1.0 );
	vNormal = vec3( uNMatrix * vec4( gl_Normal, 1.0 ) );
	vEyeVec = -vec3( vertex.xyz );
	
	gl_Position = uPMatrix * uMVMatrix * vec4( gl_Vertex.xyz, 1.0 );
	vPosition = gl_Position;
}