varying vec2 pos;

void main()
{
	pos = -((gl_Vertex - 1.0)*0.5);
	gl_Position = gl_Vertex;
}