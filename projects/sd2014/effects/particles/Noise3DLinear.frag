// Noise3DLinear.frag
uniform sampler3D t;
uniform int a;

float s(vec3 x){
	return texture3D(t,x/256.).x;
}
void main(){	float x=float(a);
	gl_FragData[0]=vec4(1)*(
		s(vec3(gl_FragCoord.xy,x))*.5+
		s(vec3(gl_FragCoord.xy,x)*2.)*.25+
		s(vec3(gl_FragCoord.xy,x)*4.)*.125+
		s(vec3(gl_FragCoord.xy,x)*8.)*.0625+
		s(vec3(gl_FragCoord.xy,x)*16.)*.03125

	);
}