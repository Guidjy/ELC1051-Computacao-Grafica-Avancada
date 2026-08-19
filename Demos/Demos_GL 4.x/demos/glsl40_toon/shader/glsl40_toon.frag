// simple toon fragment shader
// www.lighthouse3d.com

#version 400

in vec3 Color;
in vec3 Normal;

uniform vec3 LightDir;

void main() 
{
	float intensity;
	vec4 color;

	intensity = max(dot(LightDir, Normal), 0.0); 
	
	if (intensity > 0.98)
		color = vec4(Color,1.0) * vec4(0.9,0.9,0.9,1.0);
	else if (intensity > 0.5)
		color = vec4(Color,1.0) * vec4(0.4,0.4,0.4,1.0);	
	else if (intensity > 0.25)
		color = vec4(Color,1.0) * vec4(0.2,0.2,0.2,1.0);
	else
		color = vec4(Color,1.0) * vec4(0.1,0.1,0.1,1.0);	
		
	gl_FragColor = color;
}
