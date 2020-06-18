// add additional, optional velocity texture for directional movement
// seconddary object buffers to store a target position for morpting?

#version 150 compatibility
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D texOldPos;
uniform sampler2D texVel;
uniform sampler2D posTex;

uniform float time;
uniform float timeStep;
uniform float damping;
uniform float velSpeed;

void main()
{

	ivec2 puv = ivec2(gl_FragCoord.xy);
	vec4 oldPosLife = texelFetch(texOldPos, puv, 0);
	vec3 pos = oldPosLife.xyz;
	pos = texelFetch(texVel, puv, 0).xyz * timeStep * velSpeed;
	gl_FragData[0] = vec4(pos, 1.0);
	//gl_FragData[0] = vec4(1.0, 0.0, 0.0, 1.0);
}