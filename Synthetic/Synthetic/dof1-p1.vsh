//
// Vertex shader of 1st pass of depth-pf-field rendering
//
#version 130 
precision mediump float;

varying mediump float z;

varying vec2 projection;
uniform mat4 transform;

void main(void)
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	
	z               = pos.z;
	gl_Position     = ftransform();
	gl_TexCoord [0] = gl_MultiTexCoord0;
	
	/// OUR TRANSFORM!
	//projection = -(gl_ModelViewProjectionMatrix * gl_Vertex).xy / z / 2;
	projection = -(transform * gl_Vertex).xy / z / 2;
}
