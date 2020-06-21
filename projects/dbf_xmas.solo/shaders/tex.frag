uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

const int   complexity      = 90;    // More points of color.
const float offset          = 10.0;   // Drives complexity in the amount of curls/cuves.  Zero is a single whirlpool.
const float fluid_speed     = 9.0;  // Drives speed, higher number will make it slower.
const float color_intensity = 0.3;

const float Pi = 3.14159;

float KalisetTexture(vec3 p) {
	vec3 pos=p;
	float l=1.;
	float ln=0.;
	float lnprev=0.;
	float expsmooth=0.;
	for (int i=0; i<13; i++) {
		p.xyz=abs(p.xyz);
		p=p/dot(p,p);
		p=p*2.-vec3(1.);
		if (mod(float(i),2.)>0.) {
			lnprev=ln;
			ln=length(p);
			expsmooth+=exp(-1./abs(lnprev-ln));
		}
	}
	return expsmooth;
}

void main()
{
  //vec2 p=(2.0*gl_FragCoord.xy-resolution)/max(resolution.x,resolution.y);
  vec2 p = gl_FragCoord.xy / vec2(800.0, 600.0);

  /*
  for(int i=1;i<complexity;i++)
  {
    vec2 newp=p;
    newp.x+=0.6/float(i)*sin(float(i)*p.y+time/fluid_speed+0.3*float(i))+offset;
    newp.y+=0.6/float(i)*sin(float(i)*p.x+time/fluid_speed+0.3*float(i+10))+offset;
    p=newp;
  }
  vec3 col=vec3(color_intensity*sin(3.0*p.x)+color_intensity,color_intensity*sin(3.0*p.y)+color_intensity,color_intensity*sin(p.x+p.y)+color_intensity);
  */
 
	vec3 col = vec3(1.0);
	col.r = KalisetTexture(vec3(-1.0 + (p*2.0), 1.0));
  
  gl_FragColor=vec4(col, 1.0);

 }