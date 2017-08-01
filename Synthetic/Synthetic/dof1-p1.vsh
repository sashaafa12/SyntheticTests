//
// Vertex shader of 1st pass of depth-pf-field rendering
//

varying float z;

void main(void)
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;

	z               = pos.z;
	gl_Position     = ftransform();
	gl_TexCoord [0] = gl_MultiTexCoord0;
}
