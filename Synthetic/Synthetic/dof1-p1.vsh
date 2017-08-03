//
// Vertex shader of 1st pass of depth-pf-field rendering
//

varying float z;
varying vec2 projection;

void main(void)
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;

	z               = pos.z;
	gl_Position     = ftransform();
	gl_TexCoord [0] = gl_MultiTexCoord0;

	projection = vec2(pos.x, pos.y);
	//gl_TexCoord [0].x = pos.x;
	//gl_TexCoord [0].y = pos.y;
}
