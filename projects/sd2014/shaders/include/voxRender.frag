// ParticleRender.frag
varying vec2 CornerVec;

varying vec4 Color;

void main()
{
	if (Color == vec4(0)) discard;
	gl_FragData[0] = vec4(Color.xyz, Color.w * pow(1.0 - min(length(CornerVec), 1.0), 4.0));
}
