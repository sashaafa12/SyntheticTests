//
// Vertex shader of 2nd pass of depth-pf-field rendering
//

void main(void)
{
	gl_Position     = ftransform();
	gl_TexCoord [0] = gl_MultiTexCoord0;
}
