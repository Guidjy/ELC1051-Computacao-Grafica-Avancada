// simple toon vertex shader
// www.lighthouse3d.com

uniform vec3 lightSourcePos;

varying vec3 normal, lightDir;

void main()
{	
	//lightDir = normalize(vec3(gl_LightSource[0].position));
	lightDir = normalize( lightSourcePos );
	normal = normalize(gl_NormalMatrix * gl_Normal);
		
	gl_Position = ftransform();
}
