varying vec2 uv;
varying vec3 cameraPosition;
varying vec3 viewDirection;
//varying float farPlane;
uniform mat4 modelviewMatrix;
uniform mat4 imvm;
varying mat4 pmat;

varying mat4 mview;


void main() 
{
	uv = gl_Vertex;
	gl_Position = gl_Vertex;

	vec4 v = gl_Vertex;		
	v = gl_ProjectionMatrixInverse*v;
	v /= v.w;
	v = gl_ModelViewMatrixInverse*v;
	vec4 nearPlanePosition = v;

	mview = gl_ModelViewMatrix;
	pmat = gl_ProjectionMatrix;
	
	cameraPosition =  gl_ModelViewMatrixInverse * vec4(0.0, 0.0, 0.0, 1.0) ;
	viewDirection = normalize(nearPlanePosition - cameraPosition);

	//mvm = gl_ModelViewMatrix;
	//imvm = inverse(mvm);
	//gl_Position = sign( gl_Position );

	/**
	vec4 v = gl_Vertex;		
	v = imvm*v;
	v /= v.w;
	v = imvm*v;
	vec3 nearPlanePosition = v;*/
		
	//cameraPosition =  imvm * vec4(0.0, 0.0, 0.0, 1.0) ;

	//vec4 v = vec4(0.0, 0.0, 0.0, 1.0);
	//v = gl_ProjectionMatrixInverse*v;
	//v /= v.w;
	//v = mvm*v;
	//cameraLookDirection = normalize(ViewMatrixBackward() - cameraPosition);

	//nearPlane = -(gl_ProjectionMatrix[3][2] / (gl_ProjectionMatrix[2][2] - 1.0));
    //farPlane = -(gl_ProjectionMatrix[3][2] / (gl_ProjectionMatrix[2][2] + 1.0));

	//viewDirection = normalize(nearPlanePosition - cameraPosition);	
}