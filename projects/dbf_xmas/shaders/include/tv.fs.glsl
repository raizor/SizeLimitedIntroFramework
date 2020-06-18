uniform sampler2D input0; 
uniform vec2 viewportSize;
varying vec2 pos;

uniform float fTime; 
uniform float fFlashAmount; 
uniform float fReflectionSize; 
uniform float fDeformValue; 
uniform float fScreenSize1;
uniform float fScreenSize2;
uniform float fScreenSize3;
 
const float PI=3.14159265358979323846; 
float lens=PI/fDeformValue; 

float screenWidth = viewportSize.x;
float screenHeight = viewportSize.y;
 
vec2 zoom(in vec2 p,in float radius) 
{ 
	float zoom=1.5-(radius*cos(p.x*PI/lens)+radius*cos(p.y*PI/lens)); 
	return vec2(p.x*zoom-0.5,p.y*zoom-0.5); 
} 
 
vec3 deform(in sampler2D tex,in vec2 p) 
{ 
	return (p.x<-1.0||p.x>0.0||p.y<-1.0||p.y>0.0)?vec3(0.0,0.0,0.0):texture2D(tex,p).xyz; 
} 
 
float rand(in vec2 p) 
{ 
	return fract(sin(dot(p.xy,vec2(12.9898,78.233)))*43758.5453); 
} 
 
void main(void) 
{ 
	vec2 p = vec2(0.0, 0.0);
	p.x = 1.0-(pos.x*2.0);
	p.y = 1.0-(pos.y*2.0);
	//vec2 p = gl_FragCoord.xy / vec2(800.0, 600.0);
	vec2 q=pos; 
	vec2 z =zoom(p,fScreenSize1); 
	vec2 z1=zoom(p,fScreenSize2); 
	vec2 z2=zoom(p,fScreenSize3); 
	float g=(2.0-cos(PI/lens/2.0+z.x*PI/lens)-cos(PI/lens/2.0+z.y*PI/lens))*32.0; 

	float rnd1=rand(vec2(p.x+fTime,p.y-fTime)); 
	float rnd2=rand(vec2(p.x-fTime,p.y+fTime)); 
	float d=rnd1*2.0/float(screenWidth); 

	vec3 source=deform(input0,z); 
	vec3 glass1=deform(input0,z1); 
	vec3 glass2=deform(input0,z2); 

	float v=fReflectionSize*g; 
 
	vec3 noise; 
	noise.x=deform(input0,vec2(z.x-d-v,z.y-d)).x; 
	noise.y=deform(input0,vec2(z.x    ,z.y  )).y; 
	noise.z=deform(input0,vec2(z.x-d+v,z.y-d)).z; 

    vec3 color=source+(glass1*glass1)+(glass2*0.1)+(noise*0.15); 

	
	color+=fFlashAmount;											// flash 	
	color-=(vec3(rnd1,rnd1,rnd1)-vec3(rnd2,rnd2,rnd2))*0.125;// noise 
	
	color*=0.75+0.25*sin(z.x*float(screenWidth)*10.2);			// scanline w
	color*=0.90+0.10*cos(z.y*float(screenHeight)*10.2)*sin(0.5+z.x*float(screenWidth)*10.2);	// scanline h 
	//color*=q.x*(6.0-q.x*6.0)*q.y*(6.0-q.y*6.0);				// vignetting 
	color*=0.975+0.025*sin(fTime*77.7*cos(fTime*191.1));	// flickering 
	
	//color = min(color, 0.0);
	//color = texture2D(input0,p).xyz; 

	gl_FragColor=vec4(color,1.0); 
	//gl_FragColor=vec4(p.x, 1.0, 1.0, 1.0); 
	return;
} 