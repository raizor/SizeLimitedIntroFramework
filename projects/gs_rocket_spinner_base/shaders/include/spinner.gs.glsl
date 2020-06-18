/********************************************************************
** STITCH SOURCE: 'ss.version330' **
********************************************************************/
#version 330
/********************************************************************
** STITCH SOURCE: 'ss.matTransform' **
********************************************************************/

mat4 mrotx(float angle)
{
	float c = cos(angle),s = sin(angle);

	return mat4(
		1, 0, 0, 0,
		0, c, s, 0,
		0, -s, c, 0,
		0, 0, 0, 1);
}

mat4 mroty(float angle)
{
	float c = cos(angle),s = sin(angle);

	return mat4(
		c, 0, -s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1);
}

mat4 mrotz(float angle)
{
	float c = cos(angle),s = sin(angle);

	return mat4(
		c, s, 0, 0,
		-s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

mat4 mscale(float x, float y, float z)
{
	return mat4(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
}

mat4 mtranslate(float x, float y, float z)
{
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1);
}
/*******************************************************************/
 
// geom shader for tunnel

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;

uniform float fArmLength;
uniform float fScaleRotX;
uniform float fScaleRotY;
uniform float fScaleRotZ;
uniform float fArmBend;
uniform float fSeed1;
uniform float fSeed2;

//[
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;


in CubeData
{
    vec2 index;
    float time;
    float scale;
    float xrot;
    float yrot;
    vec3 offset;    
}vertex[];
//]

uniform float fTime;

float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float randGaussian(vec2 co)
{
	float r1 = rand(co + vec2(71.3, 56.4));
	float r2 = rand(co + vec2(32.9, 28.5));

	return 0.03 + sqrt(-2.0*log(r1*0.999+0.001))*cos(6.283185*r2);
}

vec4 vertices[8];

out vec3 viewspaceNormal;
out vec4 viewspacePosition4;
out vec3 viewspacePosition3;

void emitface(int i, int j, int k, int l, vec4 normal)
{
	normal = normal * projectionMatrix;
	viewspaceNormal = normalize(normal.xyz);

	viewspacePosition4 = vertices[i];
	viewspacePosition3 = vertices[i].xyz;
	gl_Position = projectionMatrix*vertices[i];
	EmitVertex();

	viewspacePosition4 = vertices[j];
	viewspacePosition3 = vertices[j].xyz;
	gl_Position = projectionMatrix*vertices[j];
	EmitVertex();

	viewspacePosition4 = vertices[k];
	viewspacePosition3 = vertices[k].xyz;
	gl_Position = projectionMatrix*vertices[k];
	EmitVertex();

	viewspacePosition4 = vertices[l];
	viewspacePosition3 = vertices[l].xyz;
	gl_Position = projectionMatrix*vertices[l];
	EmitVertex();

	EndPrimitive();
}

void emitcube(mat4 m)
{
	mat4 vm = modelViewMatrix * m;

	vertices[0] = vm*vec4(1, 1, 1, 1);
	vertices[1] = vm*vec4(-1, 1, 1, 1);
	vertices[2] = vm*vec4(-1, -1, 1, 1);
	vertices[3] = vm*vec4(1, -1, 1, 1);
	vertices[4] = vm*vec4(1, 1, -1, 1);
	vertices[5] = vm*vec4(-1, 1, -1, 1);
	vertices[6] = vm*vec4(-1, -1, -1, 1);
	vertices[7] = vm*vec4(1, -1, -1, 1);

	emitface(0, 1, 3, 2, vm*vec4( 0,  0, -1,  1)); // back
	emitface(5, 4, 6, 7, vm*vec4( 0,  0,  1,  1)); // front
	emitface(4, 0, 7, 3, vm*vec4(-1,  0,  0,  1)); // right
	emitface(1, 5, 2, 6, vm*vec4( 1,  0,  0,  1)); // left
	emitface(1, 0, 5, 4, vm*vec4( 0, -1,  0,  1)); // top
	emitface(3, 2, 7, 6, vm*vec4( 0,  1,  0,  1)); // bottom
}

void main()
{		
	float nt = fTime * 0.4 - vertex[0].time;
	
	if(nt >= 0.0 && nt < 1.0)
	{
		mat4 t;
	
		float nt2 = 1.0 - nt;
		float scale = vertex[0].scale * nt2;
		float timeoffset = nt * 0.2;	

		t = mtranslate(0, 0, 0);
		/*
		t *= mroty(sin(fTime * .3 - timeoffset) * 1.0);
		t *= mrotx(cos(fTime * .16 - timeoffset) * 1.0);
		
		t *= mrotx(vertex[0].xrot);
		t *= mroty(vertex[0].yrot);
		
		t *= mtranslate(nt * fArmLength + vertex[0].offset[0], vertex[0].offset[1], vertex[0].offset[2]);
		
		t*= mrotz(fScaleRotX);
		t*= mrotz(fScaleRotY);
		t*= mrotz(fScaleRotZ);
		
		t *= mscale( scale, scale, scale);
		*/
		emitcube(t);
	}
	
}


