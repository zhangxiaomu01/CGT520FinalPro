#version 430            
uniform mat4 P;
uniform mat4 T;
uniform mat4 S;
uniform mat4 R;
uniform mat4 V;
uniform vec3 directL;
uniform float time;
uniform float timeLoop;


in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;
in vec3 tangent_attrib;
in vec3 mBitangent_attrib;
in vec3 offset;

out vec2 tex_coord;  
out vec4 vPos_EyeSpace;

out vec3 eye_tangentSpace;
out vec3 light_tangentSpace;


out float colorDecay;
out float normalCoff;

vec3 eyeP = vec3(0.0f, 0.0f, 0.0f);
mat4 M; 
mat4 PVM;

void main(void)
{
	M = T*S*R;
	PVM = P*V*M;
	//calculate vertex coordinate in eye space
   vPos_EyeSpace = V*M*vec4(pos_attrib, 1.0);
   float distance = length(vec3(vPos_EyeSpace) - eyeP);
   //colorDecay = clamp(1.2f - sqrt(distance/2000.0f), 0.2, 1.0);
   colorDecay = clamp(sqrt(distance/1000.0f)+1.0, 1.0, 1.5);
   normalCoff = clamp(1.2f - sqrt(distance/400.0f), 0.00, 0.5);

   //calculate tangent space frame
   vec3 n_EyeSpace = vec3(transpose(inverse(V*M))* vec4(normal_attrib,0.0f));
   vec3 tan_EyeSpace = vec3(V*M* vec4(tangent_attrib,0.0f));
   vec3 mBitan_EyeSpace = vec3(V*M* vec4(mBitangent_attrib,0.0f));
   mat3 TBN = transpose(mat3(tan_EyeSpace,mBitan_EyeSpace,n_EyeSpace));

   eye_tangentSpace = TBN * (eyeP - vec3(vPos_EyeSpace));
   light_tangentSpace = TBN * vec3(V *vec4(directL, 0.0f));

   float yoffset = 0.05 * sin(10*time + 10* pos_attrib.x) + 0.02*sin(10* time*pos_attrib.y) ;

   
   vec4 currentPos = PVM*( vec4(pos_attrib.x  , pos_attrib.y + 2 * timeLoop + 2 *offset.y, pos_attrib.z  + yoffset, 1.0) 
						+ vec4(offset, 0.0f) ) ; 
   
   gl_Position = currentPos;

   float textScale = abs(cos(0.2*timeLoop)) + 1.0f;

   tex_coord = textScale * tex_coord_attrib;
   
}