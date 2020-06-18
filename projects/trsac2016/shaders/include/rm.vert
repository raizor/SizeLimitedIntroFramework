varying vec3 cameraPosition;
varying vec3 viewDirection;

varying mat4 pmat;
varying mat4 mview;
varying mat4 mviewproj;

void main() 
{
	gl_Position = gl_Vertex;

	vec4 v = gl_Vertex;		
	v = gl_ProjectionMatrixInverse*v;
	v /= v.w;
	v = gl_ModelViewMatrixInverse*v;
	vec4 nearPlanePosition = v / v.w;

	mview = gl_ModelViewMatrix;
	mviewproj = gl_ModelViewProjectionMatrix;
	pmat = gl_ProjectionMatrix;
	
	cameraPosition =  (gl_ModelViewMatrixInverse * vec4(0.0, 0.0, 0.0, 1.0)).xyz ;
	viewDirection = normalize(nearPlanePosition - cameraPosition).xyz;
}