// Trendwhore.vert
uniform mat4 World;
uniform mat4 ProjViewWorld;

varying vec3 nurmull;
varying vec3 worldPos;

void main()
{
	nurmull = (World * vec4(gl_Normal, 0)).xyz;
	worldPos = (World * gl_Vertex).xyz;
	gl_Position = ProjViewWorld * gl_Vertex;
}
