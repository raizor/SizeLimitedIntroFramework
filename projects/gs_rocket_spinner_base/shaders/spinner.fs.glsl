/********************************************************************
** STITCH SOURCE: 'ss.fs.common' **
********************************************************************/
#version 330

in vec3 viewspaceNormal, viewspacePosition3;
in vec4 viewspacePosition4;
in vec2 texturePosition;
in vec3 objectPosition3;
in vec3 worldspacePosition3;


uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 viewMatrixMatrix;
uniform mat3 normalMatrix;
/********************************************************************
** STITCH SOURCE: 'ss.fs.mesh' **
********************************************************************/
in float meshDistance;
/********************************************************************
** STITCH SOURCE: 'ss.lambertian' **
********************************************************************/
vec4 lambertian(
	vec4 ambientColor,
	vec4 diffuseColor,
	vec4 specularColor,
	float diffuseFactor,
	float specularFactor,
	float specularPow,
	vec3 pos,
	vec3 eyePos,
	vec3 lightPos,
	vec3 normal
)
{

	lightPos = vec3(0.0, 0.0, 130.0);
	vec4 color = ambientColor;

	vec3 N = normalize(normal);
	vec3 L = normalize(lightPos - pos);
	
	float lambertTerm = dot(N,L);

	if (lambertTerm > 0.0)
	{
		color += diffuseColor * lambertTerm * diffuseFactor;
		
		vec3 E = normalize(eyePos-pos);
		vec3 R = reflect(-L, N);

		float specularTerm = pow(max(dot(R, E), 0.0), specularPow);
		color += specularColor * specularTerm * specularFactor;	
	}

	return color;
}/*******************************************************************/


uniform vec4 cColor;
uniform vec3 vLightPos;

void main()
{

	vec4 lambert = lambertian(
		vec4(cColor), // ambient color
		vec4(1.0), // diffuse color
		vec4(1.0), // specular color
		0.2, // diffuse factor
		0.5, // specular factor
		1.0, // specular pow
		viewspacePosition3, // frag position
		vec3(0), // eye pos (we're in view space)
		(viewMatrix * vec4(vLightPos, 1.0)).xyz, // light position (view space)
//		vec4(vLightPos, 1.0).xyz, // light position (view space)
		viewspaceNormal); // normal


	gl_FragData[0] = vec4(lambert.xyz, 1.0); // color
	gl_FragData[1] = vec4(viewspacePosition3, 1.0); // vs pos
	gl_FragData[2] = vec4(0.0); // glow input
	gl_FragData[3] = vec4(0.0); // godrays input
/*
	float d = length(normalizedCoord);
//	float dr = d - 0.3;
	float dr = d;
	float v = exp(-dr*dr*100.0)*0.4/sqrt(sizeScale+2.1);

	gl_FragColor = vec4(v*col*vec3(1.0, 1.0, 1.0), 1.0);
	//gl_FragColor = vec4(1.0);*/
}

