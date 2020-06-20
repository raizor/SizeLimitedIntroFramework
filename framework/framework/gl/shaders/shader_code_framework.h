/* File generated with Shader Minifier 1.1.3.r.1
 * http://www.ctrl-alt-test.fr
 */
#ifndef SHADER_CODE_FRAMEWORK_H_
#define SHADER_CODE_FRAMEWORK_H_

const char* const tv_vs_glsl =
 "varying vec2 pos;"
 "void main()"
 "{"
   "pos=gl_MultiTexCoord0.xy;"
   "gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;"
 "}";

const char* const tv_fs_glsl =
 "uniform sampler2D input0;"
 "uniform vec2 viewportSize;"
 "varying vec2 pos;"
 "uniform float fTime,fFlashAmount,fReflectionSize,fDeformValue,fScreenSize1,fScreenSize2,fScreenSize3;"
 "const float PI=3.14159;"
 "float lens=PI/fDeformValue,screenWidth=viewportSize.x,screenHeight=viewportSize.y;"
 "vec2 zoom(in vec2 p,in float radius)"
 "{"
   "float zoom=1.5-(radius*cos(p.x*PI/lens)+radius*cos(p.y*PI/lens));"
   "return vec2(p.x*zoom-.5,p.y*zoom-.5);"
 "}"
 "vec3 deform(in sampler2D tex,in vec2 p)"
 "{"
   "return p.x<-1.||p.x>0.||p.y<-1.||p.y>0.?vec3(0.,0.,0.):texture2D(tex,p).xyz;"
 "}"
 "float rand(in vec2 p)"
 "{"
   "return fract(sin(dot(p.xy,vec2(12.9898,78.233)))*43758.5);"
 "}"
 "void main()"
 "{"
   "vec2 p=vec2(0.,0.);"
   "p.x=1.-pos.x*2.;"
   "p.y=1.-pos.y*2.;"
   "vec2 q=pos,z=zoom(p,fScreenSize1),z1=zoom(p,fScreenSize2),z2=zoom(p,fScreenSize3);"
   "float g=(2.-cos(PI/lens/2.+z.x*PI/lens)-cos(PI/lens/2.+z.y*PI/lens))*32.,rnd1=rand(vec2(p.x+fTime,p.y-fTime)),rnd2=rand(vec2(p.x-fTime,p.y+fTime)),d=rnd1*2./float(screenWidth);"
   "vec3 source=deform(input0,z),glass1=deform(input0,z1),glass2=deform(input0,z2);"
   "float v=fReflectionSize*g;"
   "vec3 noise;"
   "noise.x=deform(input0,vec2(z.x-d-v,z.y-d)).x;"
   "noise.y=deform(input0,vec2(z.x,z.y)).y;"
   "noise.z=deform(input0,vec2(z.x-d+v,z.y-d)).z;"
   "vec3 color=source+glass1*glass1+glass2*.1+noise*.15;"
   "color+=fFlashAmount;"
   "color-=(vec3(rnd1,rnd1,rnd1)-vec3(rnd2,rnd2,rnd2))*.125;"
   "color*=.75+.25*sin(z.x*float(screenWidth)*10.2);"
   "color*=.9+.1*cos(z.y*float(screenHeight)*10.2)*sin(.5+z.x*float(screenWidth)*10.2);"
   "color*=q.x*(6.-q.x*6.)*q.y*(6.-q.y*6.);"
   "color*=.975+.025*sin(fTime*77.7*cos(fTime*191.1));"
   "gl_FragColor=vec4(color,1.);"
   "return;"
 "}";

const char* const ssao_vs_glsl =
 "varying vec2 vUv;"
 "void main()"
 "{"
   "vUv=gl_MultiTexCoord0.xy;"
   "gl_Position=gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;"
 "}";

const char* const ssao_fs_glsl =
 "uniform sampler2D bgl_DepthTexture,bgl_RenderedTexture;"
 "uniform float bgl_RenderedTextureWidth,bgl_RenderedTextureHeight;\n"
 "#define PI 3.14159265\n"
 "float width=bgl_RenderedTextureWidth,height=bgl_RenderedTextureHeight;"
 "varying vec2 vUv;"
 "vec2 texCoord=vUv;"
 "uniform float znear,zfar;"
 "int samples=16;"
 "float radius=3.,aoclamp=.25;"
 "bool noise=true;"
 "float noiseamount=.0002,diffarea=.4,gdisplace=.4;"
 "bool mist=true;"
 "float miststart=0.,mistend=16.;"
 "bool onlyAO=true;"
 "float lumInfluence=.2;"
 "vec2 rand(vec2 coord)"
 "{"
   "float noiseX=(fract(1.-coord.x*(width/2.))*.25+fract(coord.y*(height/2.))*.75)*2.-1.,noiseY=(fract(1.-coord.x*(width/2.))*.75+fract(coord.y*(height/2.))*.25)*2.-1.;"
   "if(noise)"
     "{"
       "noiseX=clamp(fract(sin(dot(coord,vec2(12.9898,78.233)))*43758.5),0.,1.)*2.-1.;"
       "noiseY=clamp(fract(sin(dot(coord,vec2(12.9898,78.233)*2.))*43758.5),0.,1.)*2.-1.;"
     "}"
   "return vec2(noiseX,noiseY)*noiseamount;"
 "}"
 "float doMist()"
 "{"
   "float zdepth=texture2D(bgl_DepthTexture,texCoord.xy).x,depth=-zfar*znear/(zdepth*(zfar-znear)-zfar);"
   "return clamp((depth-miststart)/mistend,0.,1.);"
 "}"
 "float readDepth(in vec2 coord)"
 "{"
   "if(vUv.x<0.||vUv.y<0.)"
     "return 1.;"
   "return 2.*znear/(zfar+znear-texture2D(bgl_DepthTexture,coord).x*(zfar-znear));"
 "}"
 "float compareDepths(in float depth1,in float depth2,inout int far)"
 "{"
   "float garea=2.,diff=(depth1-depth2)*100.;"
   "if(diff<gdisplace)"
     "{"
       "garea=diffarea;"
     "}"
   "else"
     "{"
       "far=1;"
     "}"
   "float gauss=pow(2.7182,-2.*(diff-gdisplace)*(diff-gdisplace)/(garea*garea));"
   "return gauss;"
 "}"
 "float calAO(float depth,float dw,float dh)"
 "{"
   "float dd=(1.-depth)*radius,temp=0.,temp2=0.,coordw=vUv.x+dw*dd,coordh=vUv.y+dh*dd,coordw2=vUv.x-dw*dd,coordh2=vUv.y-dh*dd;"
   "vec2 coord=vec2(coordw,coordh),coord2=vec2(coordw2,coordh2);"
   "int far=0;"
   "temp=compareDepths(depth,readDepth(coord),far);"
   "if(far>0)"
     "{"
       "temp2=compareDepths(readDepth(coord2),depth,far);"
       "temp+=(1.-temp)*temp2;"
     "}"
   "return temp;"
 "}"
 "void main()"
 "{"
   "texCoord=vUv;"
   "vec2 noise=rand(texCoord);"
   "float depth=readDepth(texCoord),w=1./width/clamp(depth,aoclamp,1.)+noise.x*(1.-noise.x),h=1./height/clamp(depth,aoclamp,1.)+noise.y*(1.-noise.y),pw,ph,ao,dl=PI*(3.-sqrt(5.)),dz=1./float(samples),l=0.,z=1.-dz/2.;"
   "for(int i=0;i<=samples;i++)"
     "{"
       "float r=sqrt(1.-z);"
       "pw=cos(l)*r;"
       "ph=sin(l)*r;"
       "ao+=calAO(depth,pw*w,ph*h);"
       "z=z-dz;"
       "l=l+dl;"
     "}"
   "ao/=float(samples);"
   "ao=1.-ao;"
   "if(mist)"
     "{"
       "ao=mix(ao,1.,doMist());"
     "}"
   "vec3 color=texture2D(bgl_RenderedTexture,texCoord).xyz,lumcoeff=vec3(.299,.587,.114);"
   "float lum=dot(color.xyz,lumcoeff);"
   "vec3 luminance=vec3(lum,lum,lum),final=vec3(color*mix(vec3(ao),vec3(1.),luminance*lumInfluence));"
   "if(onlyAO)"
     ";"
   "gl_FragColor=vec4(final,1.);"
 "}";

#endif // SHADER_CODE_FRAMEWORK_H_