varying vec3 normal;
varying vec3 vert;

void main()
{
	vec4 diffuse;
	vec4 spec;
	vec4 ambient;

    vec3 L = normalize(gl_LightSource[0].position.xyz - vert);
    vec3 E = normalize(-vert);
    vec3 R = normalize(reflect(-L,normal));  

	float specPow = 1.0;
	vec4 ambientMat = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 diffuseMat = vec4(0.0, 0.5, 0.5, 1.0);
	vec4 specMat = vec4(1.0, 1.0, 0.0, 1.0);

   	ambient = ambientMat;

  	diffuse = clamp( diffuseMat * max(dot(normal,L), 0.0)  , 0.0, 1.0 ) ;
   	spec = clamp ( specMat * pow(max(dot(R,E),0.0),0.3*specPow) , 0.0, 1.0 );

    gl_FragColor = ambient + diffuse + spec;
}