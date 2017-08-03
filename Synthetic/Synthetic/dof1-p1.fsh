//
// Fragment shader of 1st pass of depth-of-field rendering
//

varying vec2 projection;

varying float 	  z;
uniform sampler2D tex;
uniform float focalDistance, focalRange;

void main (void)
{
	float	blur = clamp ( abs ( focalDistance  + z ) / focalRange, 0.0, 1.0 );
	vec2 coord = gl_TexCoord [0].st; //vec2(gl_Position.x, gl_Position.y)
	coord = vec2(gl_FragCoord.x / 512, gl_FragCoord.y / 512);
	gl_FragData [0] = vec4 ( texture2D ( tex, coord).rgb, blur );

}
