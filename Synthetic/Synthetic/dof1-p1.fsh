//
// Fragment shader of 1st pass of depth-of-field rendering
//

varying float 	  z;
uniform sampler2D tex;
uniform float focalDistance, focalRange;

void main (void)
{
	float	blur = clamp ( abs ( focalDistance  + z ) / focalRange, 0.0, 1.0 );
	gl_FragData [0] = vec4 ( texture2D ( tex, gl_TexCoord [0].st).rgb, blur );

}
