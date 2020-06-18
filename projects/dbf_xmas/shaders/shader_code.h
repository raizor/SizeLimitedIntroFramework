/* File generated with Shader Minifier 1.1.3
 * http://www.ctrl-alt-test.fr
 */
#ifndef SHADER_CODE_H_
# define SHADER_CODE_H_

const char *ssao_fs_glsl =
 "uniform sampler2D bgl_DepthTexture,bgl_RenderedTexture;"
 "uniform float bgl_RenderedTextureWidth,bgl_RenderedTextureHeight;\n"
 "#define PI 3.14159265\n"
 "float f=bgl_RenderedTextureWidth,v=bgl_RenderedTextureHeight;"
 "vec2 b=gl_TexCoord[0].xy;"
 "float y=.3,x=40.;"
 "int t=16;"
 "float e=3.,i=.25;"
 "bool s=true;"
 "float m=.0002,g=.4,r=.4;"
 "bool d=true;"
 "float c=0.,h=16.;"
 "bool P=false;"
 "float o=.7;"
 "vec2 p(vec2 b)"
 "{"
   "float e=(fract(1.-b.x*(f/2.))*.25+fract(b.y*(v/2.))*.75)*2.-1.,i=(fract(1.-b.x*(f/2.))*.75+fract(b.y*(v/2.))*.25)*2.-1.;"
   "if(s)"
     "e=clamp(fract(sin(dot(b,vec2(12.9898,78.233)))*43758.5),0.,1.)*2.-1.,i=clamp(fract(sin(dot(b,vec2(12.9898,78.233)*2.))*43758.5),0.,1.)*2.-1.;"
   "return vec2(e,i)*m;"
 "}"
 "float p()"
 "{"
   "float f=texture2D(bgl_DepthTexture,b.xy).x,i=-x*y/(f*(x-y)-x);"
   "return clamp((i-c)/h,0.,1.);"
 "}"
 "float n(in vec2 b)"
 "{"
   "if(gl_TexCoord[0].x<0.||gl_TexCoord[0].y<0.)"
     "return 1.;"
   "return 2.*y/(x+y-texture2D(bgl_DepthTexture,b).x*(x-y));"
 "}"
 "float n(in float b,in float e,inout int f)"
 "{"
   "float y=2.,i=(b-e)*100.;"
   "if(i<r)"
     "y=g;"
   "else"
     " f=1;"
   "float t=pow(2.7182,-2.*(i-r)*(i-r)/(y*y));"
   "return t;"
 "}"
 "float p(float b,float f,float y)"
 "{"
   "float t=(1.-b)*e,i=0.,v=0.,x=gl_TexCoord[0].x+f*t,r=gl_TexCoord[0].y+y*t,h=gl_TexCoord[0].x-f*t,o=gl_TexCoord[0].y-y*t;"
   "vec2 s=vec2(x,r),d=vec2(h,o);"
   "int P=0;"
   "i=n(b,n(s),P);"
   "if(P>0)"
     "v=n(n(d),b,P),i+=(1.-i)*v;"
   "return i;"
 "}"
 "void main()"
 "{"
   "vec2 y=p(b);"
   "float e=n(b),h=1./f/clamp(e,i,1.)+y.x*(1.-y.x),r=1./v/clamp(e,i,1.)+y.y*(1.-y.y),x,g,s,m=PI*(3.-sqrt(5.)),c=1./float(t),u=0.,l=1.-c/2.;"
   "for(int a=0;a<=t;a++)"
     "{"
       "float T=sqrt(1.-l);"
       "x=cos(u)*T;"
       "g=sin(u)*T;"
       "s+=p(e,x*h,g*r);"
       "l=l-c;"
       "u=u+m;"
     "}"
   "s/=float(t);"
   "s=1.-s;"
   "if(d)"
     "s=mix(s,1.,p());"
   "vec3 a=texture2D(bgl_RenderedTexture,b).xyz,T=vec3(.299,.587,.114);"
   "float z=dot(a.xyz,T);"
   "vec3 I=vec3(z,z,z),C=vec3(a*mix(vec3(s),vec3(1.),I*o));"
   "if(P)"
     "C=vec3(mix(vec3(s),vec3(1.),I*o));"
   "gl_FragColor=vec4(C,1.);"
 "}";

const char *spinner_vs_glsl =
 "#version 330\n"
 "uniform float fArmMult;"
 "layout(location=0)in vec4 position;layout(location=1)in vec2 index;out CubeData{vec2 index;float time;float scale;float xrot;float yrot;vec3 offset;}vertex;\n"
 "#define HPI 1.57079\n"
 "#define PI 3.141592\n"
 "#define PI2 6.283185\n"
 "float t(vec2 b)"
 "{"
   "return fract(sin(dot(b.xy,vec2(12.9898,78.233)))*43758.5);"
 "}"
 "float f(vec2 b)"
 "{"
   "float f=t(b+vec2(71.3,56.4)),i=t(b+vec2(32.9,28.5));"
   "return.03+sqrt(-2.*log(f*.999+.001))*cos(6.28319*i);"
 "}"
 "void main()"
 "{"
   "float t=PI2/fArmMult,i=256.;"
   "gl_Position=position;"
   "vertex.index=index;"
   "float b=f(position.xy),e=f(position.xy*b),y=f(position.xy*e),r=f(position.xy*y);"
   "vec4 v=vec4(b,e,y,r);"
   "vertex.time=v.x*50.+50.;"
   "vertex.scale=v.y*.2+.3;"
   "vertex.xrot=floor(v.z*2.+2.)*t;"
   "vertex.yrot=floor(v.w*2.+2.)*t;"
   "vertex.offset=v.xyz*.4;"
 "}";

const char *spinner_gs_glsl =
 "#version 330\n"
 "mat4 t(float f)"
 "{"
   "float v=cos(f),i=sin(f);"
   "return mat4(1,0,0,0,0,v,i,0,0,-i,v,0,0,0,0,1);"
 "}"
 "mat4 f(float f)"
 "{"
   "float v=cos(f),i=sin(f);"
   "return mat4(v,0,-i,0,0,1,0,0,i,0,v,0,0,0,0,1);"
 "}"
 "mat4 v(float f)"
 "{"
   "float v=cos(f),i=sin(f);"
   "return mat4(v,i,0,0,-i,v,0,0,0,0,1,0,0,0,0,1);"
 "}"
 "mat4 f(float f,float v,float i)"
 "{"
   "return mat4(f,0,0,0,0,v,0,0,0,0,i,0,0,0,0,1);"
 "}"
 "mat4 t(float v,float i,float f)"
 "{"
   "return mat4(1,0,0,0,0,1,0,0,0,0,1,0,v,i,f,1);"
 "}"
 "uniform mat4 projectionMatrix,viewMatrix,modelViewMatrix;"
 "uniform float fArmLength,fScaleRotX,fScaleRotY,fScaleRotZ,fArmBend,fSeed1,fSeed2;"
 "layout(points)in;layout(triangle_strip,max_vertices=24)out;in CubeData{vec2 index;float time;float scale;float xrot;float yrot;vec3 offset;}vertex[];"
 "uniform float fTime;"
 "float n(vec2 f)"
 "{"
   "return fract(sin(dot(f.xy,vec2(12.9898,78.233)))*43758.5);"
 "}"
 "float s(vec2 f)"
 "{"
   "float v=n(f+vec2(71.3,56.4)),i=n(f+vec2(32.9,28.5));"
   "return.03+sqrt(-2.*log(v*.999+.001))*cos(6.28319*i);"
 "}"
 "vec4 i[8];"
 "out vec3 viewspaceNormal;"
 "out vec4 viewspacePosition4;"
 "out vec3 viewspacePosition3;"
 "void f(int f,int v,int b,int e,vec4 s)"
 "{"
   "viewspaceNormal=normalize(s.xyz),viewspacePosition4=i[f],viewspacePosition3=i[f].xyz,gl_Position=projectionMatrix*i[f],EmitVertex(),viewspacePosition4=i[v],viewspacePosition3=i[v].xyz,gl_Position=projectionMatrix*i[v],EmitVertex(),viewspacePosition4=i[b],viewspacePosition3=i[b].xyz,gl_Position=projectionMatrix*i[b],EmitVertex(),viewspacePosition4=i[e],viewspacePosition3=i[e].xyz,gl_Position=projectionMatrix*i[e],EmitVertex(),EndPrimitive();"
 "}"
 "void r(mat4 v)"
 "{"
   "mat4 t=modelViewMatrix*v;"
   "i[0]=t*vec4(1,1,1,1);"
   "i[1]=t*vec4(-1,1,1,1);"
   "i[2]=t*vec4(-1,-1,1,1);"
   "i[3]=t*vec4(1,-1,1,1);"
   "i[4]=t*vec4(1,1,-1,1);"
   "i[5]=t*vec4(-1,1,-1,1);"
   "i[6]=t*vec4(-1,-1,-1,1);"
   "i[7]=t*vec4(1,-1,-1,1);"
   "f(0,1,3,2,t*vec4(0,0,1,0));"
   "f(5,4,6,7,t*vec4(0,0,-1,0));"
   "f(4,0,7,3,t*vec4(1,0,0,0));"
   "f(1,5,2,6,t*vec4(-1,0,0,0));"
   "f(1,0,5,4,t*vec4(1,0,0,0));"
   "f(3,2,7,6,t*vec4(-1,0,0,0));"
 "}"
 "void main()"
 "{"
   "float i=fTime*.4-vertex[0].time;"
   "if(i>=0.&&i<1.)"
     "{"
       "mat4 b;"
       "float e=1.-i,s=vertex[0].scale*e,y=i*.2;"
       "b=t(0,0,0);"
       "b*=f(sin(fTime*.3-y));"
       "b*=t(cos(fTime*.16-y));"
       "b*=t(vertex[0].xrot);"
       "b*=f(vertex[0].yrot);"
       "b*=t(i*fArmLength+vertex[0].offset[0],vertex[0].offset[1],vertex[0].offset[2]);"
       "b*=v(fScaleRotX);"
       "b*=v(fScaleRotY);"
       "b*=v(fScaleRotZ);"
       "b*=f(s,s,s);"
       "r(b);"
     "}"
 "}";

const char *spinner_fs_glsl =
 "#version 330\n"
 "in vec3 viewspaceNormal,viewspacePosition3;"
 "in vec4 viewspacePosition4;"
 "in vec2 texturePosition;"
 "in vec3 objectPosition3,worldspacePosition3;"
 "uniform mat4 viewMatrix,modelViewMatrix,modelViewProjectionMatrix,viewMatrixMatrix;"
 "uniform mat3 normalMatrix;"
 "in float meshDistance;"
 "vec4 t(vec4 v,vec4 t,vec4 f,float e,float i,float b,vec3 y,vec3 s,vec3 m,vec3 P)"
 "{"
   "vec4 r=v;"
   "vec3 x=normalize(P),z=normalize(m-y);"
   "float n=dot(x,z);"
   "if(n>0.)"
     "{"
       "r+=t*n*e;"
       "vec3 o=normalize(s-y),h=reflect(-z,x);"
       "float c=pow(max(dot(h,o),0.),b);"
       "r+=f*c*i;"
     "}"
   "return r;"
 "}"
 "uniform vec4 cColor;"
 "uniform vec3 vLightPos;"
 "void main()"
 "{"
   "vec4 f=t(vec4(cColor),vec4(1.),vec4(1.),.3,.3,8.,viewspacePosition3,vec3(0),(viewMatrix*vec4(vLightPos,1.)).xyz,viewspaceNormal);"
   "gl_FragData[0]=vec4(f);"
   "gl_FragData[1]=vec4(viewspacePosition3,1.);"
   "gl_FragData[2]=vec4(0.);"
   "gl_FragData[3]=vec4(0.);"
 "}";

#endif // SHADER_CODE_H_
