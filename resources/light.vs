// Simple Lambertian lighting
// Mario Gonzalez
//
// Based on 'WebGL - A Beginners Guide'

attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;

uniform vec3 uRotation;
uniform vec3 uTranslation;

uniform mat4 uMVMatrix;
uniform mat4 uPMatrix;
uniform mat4 uNMatrix;

varying vec3 vNormal;
varying vec3 vEyeVec;
 
mat4 rotationX( in float angle ) {
	return mat4(	1.0,		0,			0,			0,
			 		0, 	cos(angle),	-sin(angle),		0,
					0, 	sin(angle),	 cos(angle),		0,
					0, 			0,			  0, 		1);
}

mat4 rotationY( in float angle ) {
	return mat4(	cos(angle),		0,		sin(angle),	0,
			 				0,		1.0,			 0,	0,
					-sin(angle),	0,		cos(angle),	0,
							0, 		0,				0,	1);
}

mat4 rotationZ( in float angle ) {
	return mat4(	cos(angle),		-sin(angle),	0,	0,
			 		sin(angle),		cos(angle),		0,	0,
							0,				0,		1,	0,
							0,				0,		0,	1);
}

mat4 translation( in vec3 position ) {
	return mat4(1, 0, 0, position.x,
				0, 1, 0, position.y,
				0, 0, 1, position.z,
				0, 0, 0, 1);
}

void main( void ) {
	vec4 vertex = vec4(gl_Vertex.xyz, 1.0);

	// vertex = vertex;//* rotationX(uRotation.x) * rotationY(uRotation.y) * rotationZ(uRotation.z) * translation(uTranslation);
	// Transformed vertex position
	vertex = uMVMatrix * vertex;
	
	// Transformed vertexPosition
	vNormal = vec3(uNMatrix * vec4(gl_Normal, 1.0));
	vEyeVec = -vec3( vertex.xyz );
	
	// * rotationX(uTimer*5.0)
	gl_Position = uPMatrix * vertex;
}
