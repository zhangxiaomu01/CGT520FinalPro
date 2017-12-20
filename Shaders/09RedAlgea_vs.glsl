#version 430            
uniform mat4 PVM;
uniform mat4 M;
uniform mat4 V;
uniform mat4 u_shadowMat;
uniform vec3 directL;
uniform float time;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;
in vec3 tangent_attrib;
in vec3 mBitangent_attrib;

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


   float offset = 0.1*sin( 0.1 *time*pos_attrib.x);
    float offset01 = 0.1*cos( 0.2 *time*pos_attrib.x);
	
	
	offset =  0.03*cos(0.7* time*pos_attrib.y) + 0.03*sin(0.9* time*pos_attrib.y);
	
   gl_Position = PVM*vec4(pos_attrib.x + offset, pos_attrib.y , pos_attrib.z + offset, 1.0);


   tex_coord = tex_coord_attrib;
   o_shadowCoord = u_shadowMat * M *vec4(pos_attrib, 1.0);
}