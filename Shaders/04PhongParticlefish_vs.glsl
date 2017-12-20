#version 430            
uniform mat4 PVM;
uniform mat4 M;
uniform mat4 V;
uniform mat4 u_shadowMat;
uniform vec3 directL;
uniform float time;
uniform float timeLoop;
uniform int behavior;
uniform float fishSpeed;

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

out vec4 o_shadowCoord;
out float colorDecay;
out float normalCoff;

vec3 eyeP = vec3(0.0f, 0.0f, 0.0f);


void main(void)
{
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

   //handle different fish behaviors
   vec4 currentPos;
   if(behavior == 1)
   {
		currentPos = PVM*( vec4(pos_attrib.x  , pos_attrib.y, pos_attrib.z  + yoffset, 1.0) 
						+ vec4(offset* clamp(5.0f* abs(sin(0.02*time)), 0.5,1.0), 0.0f) ) ; 
   }
	
	if(behavior == 2)
   {
		currentPos = PVM*( vec4(pos_attrib.x + fishSpeed * timeLoop * clamp(offset.x, 1.0f, 2.5f) , pos_attrib.y + timeLoop, pos_attrib.z  + yoffset, 1.0) 
						+ vec4(offset, 0.0f) ) ; 
						//+ 2 * timeLoop
   }
   if(behavior == 3)
   {
		currentPos = PVM*( vec4(pos_attrib.x + fishSpeed * timeLoop * clamp(offset.x, 1.0f, 2.0f) , pos_attrib.y + 
						0.5 * timeLoop, pos_attrib.z  + yoffset, 1.0) 
						+ vec4(offset, 0.0f) ) ; 
						//+ 2 * timeLoop
   }
   if(behavior == 4)
   {
		currentPos = PVM*( vec4(pos_attrib.x + fishSpeed * timeLoop * clamp(offset.x, 1.0f, 2.0f) , pos_attrib.y - 
						0.5 * sin(timeLoop), pos_attrib.z  + yoffset, 1.0) 
						+ vec4(offset, 0.0f) ) ; 
						//+ 2 * timeLoop
   }
   


   gl_Position = currentPos;
   tex_coord = tex_coord_attrib;
   o_shadowCoord = u_shadowMat * M *vec4(pos_attrib, 1.0);
}