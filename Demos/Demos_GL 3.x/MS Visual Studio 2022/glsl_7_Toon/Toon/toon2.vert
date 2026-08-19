// simple toon vertex shader
// www.lighthouse3d.com

uniform vec3 lightSourcePos;
uniform vec3 lightSourcePos2;

varying vec3 normal, lightDir;
varying vec3 lightDir2;
varying float px;
varying float py;
varying float pz;
varying float wawa;
void main()
{	
	//lightDir = normalize(vec3(gl_LightSource[0].position));
	lightDir = normalize( lightSourcePos );
	lightDir2 = normalize( lightSourcePos2 );
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	
	
	
	px = gl_Vertex.x;
	py = gl_Vertex.y;
	pz = gl_Vertex.z;

	
	
	
	

	
	
	
		
	gl_Position = ftransform();
}
