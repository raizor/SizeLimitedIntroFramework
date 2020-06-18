#version 330

in vec2 cornerVec;
in vec4 col;
in vec3 normal;
in vec3 vert;
#define MAX_LIGHTS 1

uniform mat4 matView;
uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat3 matNormal;

vec3 light1 = vec3(.5,-.5,-2);
vec4 diffuse = vec4(0.024, 0.02, 0.01, 1.0);
vec4 ambient = vec4(0.3, 0.1, 0.1, 1.0);
vec4 specular = vec4(0.3, 0.2, 0.1, 1.0);
float shininess = 0.5;


void main()
{
	//if (col == vec4(0)) discard;
	gl_FragData[0] = vec4(1.0);//vec4(col.xyz, col.w * pow(1.0 - min(length(cornerVec), 1.0), 4.0));
/*
   vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
   for (int i=0;i<MAX_LIGHTS;i++)
   {
	   vec3 L = normalize(light1 - vert);   
	   vec3 E = normalize(-vert); // we are in ye Coordinates, so EyePos is (0,0,0)  
	   vec3 R = normalize(-reflect(L,normal));  
	 
	   //calculate Ambient Term:  
	   vec4 Iamb = ambient;    

	   //calculate Diffuse Term:  
	   vec4 Idiff = diffuse * max(dot(normal,L), 0.0);
	   Idiff = clamp(Idiff, 0.0, 1.0);     
	   
	   // calculate Specular Term:
	   vec4 Ispec = specular * pow(max(dot(R,E),0.0),0.3*shininess);
	   Ispec = clamp(Ispec, 0.0, 1.0); 

	   finalColor += Iamb + Idiff + Ispec;  
   }
   gl_FragData[0] = finalColor + col; 
   //gl_FragData[0] = colx;
   //gl_FragData[0] = vec4(1.0);*/
}