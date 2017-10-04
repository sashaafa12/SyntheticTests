//
// Vertex shader of 1st pass of depth-pf-field rendering
//
//#version 330 


varying mediump float z;

varying vec2 projection;
uniform mat4 transformModelView;
uniform mat4 transformProjection;
uniform mat4 transform3;
uniform mat4 transfMatrix;
uniform vec3 posCamera;
uniform float phi;

uniform mat3 modelMatrix;
#define M_PI 3.141597f

void main(void)
{
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	
	z               = pos.z;
	gl_Position     = ftransform();
	gl_TexCoord [0] = gl_MultiTexCoord0;
	
	/// OUR TRANSFORM!
	projection = -(gl_ModelViewProjectionMatrix * gl_Vertex).xy / z / 2;
	projection = -(transformProjection * gl_ModelViewMatrix * gl_Vertex).xy / z / 2  + vec2(0.5, 0.5);
	
	vec3 camCoord = modelMatrix * (gl_Vertex.xyz - posCamera);
	float fov = M_PI / 2;
	float asp = 1.0f;
	float x = camCoord.x / tan(fov/2)  + 0.5;// / (camCoord.z * tan(fov/2)) + 0.5;
	float y = camCoord.y / asp / tan(fov/2)  + 0.5;// / (camCoord.z * tan(fov/2) * asp) + 0.5;
	//x = 512 * x;
	//y = 512 * y;
		
	vec4 projected = gl_ProjectionMatrix * gl_ModelViewMatrix * (gl_Vertex) * mat4(cos(phi),-sin(phi),0,0,sin(phi),cos(phi),0,0,0,0,1,0,0,0,0,1);// - posCamera4);   mat4(cos(phi),0,sin(phi),0,0,1,0,0,-sin(phi),0,cos(phi),0,0,0,0,1);//
	//projected.xy = vec2(x,y);
	projection = -projected.xy / z / 2 + vec2(0.5, 0.5); 
}
