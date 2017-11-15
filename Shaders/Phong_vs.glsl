#version 400            
uniform mat4 PVM;
uniform float time;
uniform mat4 M;
uniform mat4 V;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;
in vec3 tangent_attrib;
in vec3 mBitangent_attrib;

out vec2 tex_coord;  
out vec4 vPos_EyeSpace;
//out vec4 n_EyeSpace;


out vec3 n_EyeSpace;
out vec3 tan_EyeSpace;
out vec3 mBitan_EyeSpace;
out mat3 TBN;



void main(void)
{

	
	
   vPos_EyeSpace = V*M*vec4(pos_attrib, 1.0);

   
   //n_EyeSpace = transpose(inverse(V*M))*vec4(normal_attrib,0.0f);
   //n_EyeSpace = vec3(transpose(inverse(V*M))*vec4(normal_attrib,0.0f));

   n_EyeSpace = vec3(V*M* normalize(vec4(normal_attrib,0.0f)));
   tan_EyeSpace = vec3(V*M* normalize(vec4(tangent_attrib,0.0f)));
   mBitan_EyeSpace = vec3(V*M* normalize(vec4(mBitangent_attrib,0.0f)));

   TBN = transpose(mat3(tan_EyeSpace,mBitan_EyeSpace,n_EyeSpace));

   gl_Position = PVM*vec4(pos_attrib, 1.0);
   tex_coord = tex_coord_attrib;
}