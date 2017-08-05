//
// Vertex shader of 1st pass of depth-pf-field rendering
//
//#version 330 


varying mediump float z;

varying vec2 projection;
uniform mat4 transformModelView;
uniform mat4 transformProjection;
uniform mat4 transform3;
uniform mat4 transfFromCamera;
uniform vec3 posCamera;

void main(void)
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	
	z               = pos.z;
	gl_Position     = ftransform();
	gl_TexCoord [0] = gl_MultiTexCoord0;
	
	/// OUR TRANSFORM!
	projection = -(gl_ModelViewProjectionMatrix * gl_Vertex).xy / z / 2;
	projection = -(transformProjection * gl_ModelViewMatrix * gl_Vertex).xy / z / 2  + vec2(0.5, 0.5);
	
	//vec3 projected = (transform3 * (gl_Vertex.xyz - posCamera));
	vec4 posCamera4 = vec4(posCamera, 1.0);
	float Z = (gl_ModelViewMatrix * gl_Vertex).z; //transformModelView
	vec4 projected = gl_ProjectionMatrix * gl_ModelViewMatrix * (gl_Vertex);// - posCamera4);
	projection = -projected.xy / Z / 2 + vec2(0.5, 0.5); 
}
