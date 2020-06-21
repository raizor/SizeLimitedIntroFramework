varying vec2 vUv;

void main() 
{
	vUv = vec2(uv.x, uv.y);
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

}