#version 330

in vec2 quadCoord; // position on quad
in float quadSize;

uniform vec2 viewportSize;
uniform float aspectRatio;

void main(void)
{
	float hres = (viewportSize.y-(viewportSize.y / aspectRatio)) * 0.5;
	if (gl_FragCoord.y < hres || gl_FragCoord.y > viewportSize.y - hres)
	{
		discard;
	}

	float centerd = length(quadCoord);
	float clampd = max(centerd-0.2, 0.0);
	float alphaFade = exp(-clampd*clampd*4.0);

	if (alphaFade < 0.001)
		discard;

	gl_FragData[0] = vec4(1.0);
	gl_FragData[0] = vec4(vec3(1.0), alphaFade * 2.0 * quadSize);
	//gl_FragData[1] = vec4(0);

//	gl_FragColor = vec4(1.0);
}
