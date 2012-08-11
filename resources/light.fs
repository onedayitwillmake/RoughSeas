// Simple Lambertian lighting
// Mario Gonzalez
//
// Based on 'WebGL - A Beginners Guide'
#ifdef GL_SL
precision highp float;
#endif



uniform float uShininess;
uniform vec3 uLightDirection;
uniform vec4 uLightAmbient;
uniform vec4 uLightDiffuse;
uniform vec4 uLightSpecular;

uniform vec3 uRotation;
uniform vec3 uTranslation;

uniform vec4 uMaterialAmbient;
uniform vec4 uMaterialDiffuse;
uniform vec4 uMaterialSpecular;

varying vec3 vNormal;
varying vec3 vEyeVec;
varying vec4 vPosition;

void main( void ) {
	vec3 L = normalize( uLightDirection );
	vec3 N = normalize(cross(dFdx(vPosition.xyz), dFdy(vPosition.xyz)));

	float lambertTerm = dot(N,-L);
	vec4 Ia = uLightAmbient * uMaterialAmbient;
	vec4 Id = vec4(0.0, 0.0, 0.0, 1.0 );
	vec4 Is = vec4( 0.0, 0.0, 0.0, 1.0 );

	if( lambertTerm > 0.0 ) {
		Id = uLightDiffuse * uMaterialDiffuse * lambertTerm;
		vec3 E = normalize( vEyeVec );
		vec3 R = reflect( L, N );
		float specular = pow( max(dot(R, E), 0.0), uShininess );
		Is = uLightSpecular * uMaterialSpecular * specular;
	}

	vec4 finalColor = Ia + Id + Is;
	finalColor.a = 1.0;
	
	gl_FragColor = finalColor;
}
