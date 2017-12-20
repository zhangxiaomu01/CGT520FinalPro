#version 430
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform mat4 V;

in float normalCoff;
in float colorDecay;
in vec2 tex_coord;
in vec4 vPos_EyeSpace;


in vec3 eye_tangentSpace;
in vec3 light_tangentSpace;

out vec4 fragcolor;  
      

float calorTilt = 0.45f / clamp(colorDecay - 1.0f, 0.3, 0.5f);
vec3 greyScale = vec3(0.3, 0.59, 0.11); 

void main(void)
{   
	 //handle fragments
	if(texture2D(diffuseMap, tex_coord).r < 0.1)
		discard;

	//calculate diffuse and specular term
	vec3 E = normalize(eye_tangentSpace);
	vec3 l = normalize(light_tangentSpace);
	
	vec3 n = normalize(texture(normalMap, tex_coord).rgb*2.0 - 1.0);
	n.rgb = n.rgb * vec3(normalCoff, normalCoff, 1.0f);

	vec3 R = reflect(-l, n);

	float diffuseTerm = clamp(dot(n, l),0,1);
	float specularTerm =  clamp(dot(R, E),0.0f,1.0f);
	
		
	 float fogCoord = abs(vPos_EyeSpace.z / vPos_EyeSpace.w);
	 
	fragcolor = (diffuseTerm * calorTilt + 0.1) * texture2D(diffuseMap, tex_coord) + specularTerm * 0.4;
	
}




















