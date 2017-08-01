//
// Vertex shader for 3*3 Gaussian blur
//

void main(void)
{
	gl_Position     = ftransform();
	gl_TexCoord [0] = gl_MultiTexCoord0;
}
