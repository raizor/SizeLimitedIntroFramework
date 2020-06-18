// simulation
varying vec2 vUv;
			
uniform vec3 origin;
uniform sampler2D tPositions;
uniform float width;
uniform float height;

uniform float timer;


float rand(vec2 co)
{
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
		
void main()
{
	vec3 pos = texture2D( tPositions, vUv ).xyz;

	float uvStep = 1.0/width;

	if (vUv.x < uvStep && vUv.y < uvStep)
	{
		pos = origin;
	} else {
		vec2 prevUv = vUv.xy;
		prevUv.x -= uvStep;

		if (prevUv.x < 0.0)
		{
			prevUv.x = 1.0-uvStep;
			prevUv.y -= uvStep;				
		}

		vec3 prevPos = texture2D( tPositions, prevUv ).xyz;

		pos.x += (prevPos.x - pos.x)/2.0;
		pos.y += (prevPos.y - pos.y)/2.0;
		pos.z += (prevPos.z - pos.z)/2.0;
	}

	// Write new position out
	gl_FragColor = vec4(pos, 1.0);


}