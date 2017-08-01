//
// Fragment shader of 2nd pass of depth-of-field rendering
//

uniform sampler2D tex;

void main (void)
{
	const vec2	d1 = vec2 ( 1.0/512.0, 1.0/512.0 );
	const vec2	d2 = vec2 ( 1.0/512.0, -1.0/512.0 );
	const vec2	d3 = vec2 ( -1.0/512.0, 1.0/512.0 );
	const vec2	d4 = vec2 ( -1.0/512.0, -1.0/512.0 );

	vec2	p = gl_TexCoord [0].st;
	
	gl_FragData [0] = (texture2D ( tex, vec2 ( p + d1 ) ) + 
                       texture2D ( tex, vec2 ( p + d2 ) ) + 
					   texture2D ( tex, vec2 ( p + d3 ) ) + 
			           texture2D ( tex, vec2 ( p + d4 ) ) ) * 0.25;
}
