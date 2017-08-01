//
// Vertex shader of 1st pass of depth-pf-field rendering
//

uniform float focalDistance, focalRange;
varying float blur;

void main(void)
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;

	blur            = clamp ( abs ( -pos.z - focalDistance) / focalRange, 0.0, 1.0 );
	gl_Position     = ftransform();
	gl_TexCoord [0] = gl_MultiTexCoord0;
}
