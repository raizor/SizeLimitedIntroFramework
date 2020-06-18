// Trendwhore.vert
uniform mat4 matView;
uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat3 matNormal;

varying vec3 norm;
varying vec3 worldPos;

void main()
{
	norm = (matModelView * vec4(gl_Normal, 0)).xyz;
	worldPos = (matModelView * gl_Vertex).xyz;
	gl_Position = matProjection * matModelView * gl_Vertex;
}
