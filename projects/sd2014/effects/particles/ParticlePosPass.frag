// ParticlePosPass.frag
#version 150 compatibility
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D OldPosLifeTex;
uniform sampler2D VelTex;
uniform sampler2D PosTex;

uniform float TimeStep;

uniform float VelSpeed;
uniform float DeathSpeed;

void main()
{
	ivec2 puv = ivec2(gl_FragCoord.xy);
	vec4 oldPosLife = texelFetch(OldPosLifeTex, puv, 0);
	vec3 pos = oldPosLife.xyz;
	float life = oldPosLife.w - TimeStep * DeathSpeed;
	if (life < 0.0)
	{
        // dead, respawn
		life = min(life + 1.0, 1.0);
        // get new position
		pos = texelFetch(PosTex, puv, 0).xyz;
	}
	if (pos != vec3(0))
	{
		pos += texelFetch(VelTex, puv, 0).xyz * TimeStep * VelSpeed;
	}
	gl_FragData[0] = vec4(pos, life);
}
