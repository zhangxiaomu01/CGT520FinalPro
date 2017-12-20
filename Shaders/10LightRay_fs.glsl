#version 400

uniform sampler2D diffuseMap;
uniform float time;

out vec4 fragcolor;    

in vec2 tex_coord;       
in float intensityCoff;
in vec3 eye_position;

     
void main(void)
{   

	vec2 UVMod = vec2(sin(0.1 * time ), cos(time));
	vec2 tex_coordMod = vec2(tex_coord.x + 0.05f *UVMod.x, tex_coord.y); 
	float IntensityMul = clamp(cos(0.5 * (time+ eye_position.x/ 2.0f)), 0.5f, 1.0f);
   //fragcolor = vec4(pos, 1.0);
   fragcolor = texture(diffuseMap, tex_coordMod ) * intensityCoff * vec4(1.0f, 1.0f, 0.8f, 1.0f) *IntensityMul;
   //fragcolor = vec4(time * 1.0f);
}




















