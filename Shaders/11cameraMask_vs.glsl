#version 400            
uniform mat4 T;
uniform mat4 S;
uniform mat4 M;
uniform mat4 PVM;
uniform mat4 V;


in vec3 pos_attrib; //in object space
in vec2 tex_coord_attrib;

out vec2 tex_coord;
out float intensityCoff;

out vec3 eye_position;

vec3 eyeP = vec3(0.0f, 0.0f, 0.0f);

void main(void)
{
	
	eye_position = vec3 (V*M*vec4(pos_attrib, 1.0f));
	float distance = length(eye_position - eyeP);
	intensityCoff = clamp(1.2f - sqrt(distance/400.0f), 0.00, 0.5);


   //Compute clip-space vertex position
   gl_Position = PVM*vec4(pos_attrib, 1.0);     //w = 1 becase this is a point
   tex_coord = tex_coord_attrib;
}