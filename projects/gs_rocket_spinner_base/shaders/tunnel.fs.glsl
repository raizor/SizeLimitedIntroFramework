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

uniform vec3 vLightPos;
uniform float fGlowAmount;

void main()
{
	vec4 lambert = lambertian(
		vec4(0.3, 0.2, 0.1, 1.0), // ambient color
		vec4(1.0), // diffuse color
		vec4(1.0), // specular color
		0.3, // diffuse factor
		0.3, // specular factor
		8.0, // specular pow
		viewspacePosition3, // frag position
		vec3(0), // eye pos (we're in view space)
		(viewMatrix * vec4(vLightPos, 1.0)).xyz, // light position (view space)
     // vec4(vLightPos, 1.0).xyz, // light position (view space)
		viewspaceNormal); // normal

	gl_FragData[0] = lambert;// * vec4(fGlowAmount);
	gl_FragData[1] = vec4(viewspacePosition3, 1.0);
	gl_FragData[2] = vec4(0.0, 0.0, 0.0, 1.0);
	//gl_FragData[0] = vec4(1);
}

