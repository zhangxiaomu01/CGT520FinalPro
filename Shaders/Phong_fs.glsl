#version 400

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform mat4 M;
uniform mat4 V;
uniform float ambientL;
uniform float shininess;



out vec4 fragcolor;  
         
in vec2 tex_coord;
in vec4 vPos_EyeSpace;

//in vec4 n_EyeSpace;
in vec3 n_EyeSpace;
in vec3 tan_EyeSpace;
in vec3 mBitan_EyeSpace;
in mat3 TBN;


vec4 directL = vec4(0.0f, 10.0f,10.0f,0.0f);

vec4 eyeP = vec4(0.0f, 0.0f, 0.0f, 1.0f);

//vec4 directL_EyeSpace = V* directL;

//vec4 viewVec = normalize(eyeP - vPos_EyeSpace);
//vec4 directL_e = normalize(directL_EyeSpace);

vec4 viewVec_Tang = vec4(TBN * vec3(normalize(eyeP - vPos_EyeSpace)), 0.0f);
vec4 directL_Tang = vec4(TBN * vec3(normalize(V* directL)), 0.0f);

//vec3 n_e = normalize(n_EyeSpace);
//vec4 n_e = normalize(n_EyeSpace);

vec3 TextureNormal_tangentspace = normalize (texture(normalMap, tex_coord).rgb*2.0 - 1.0);

      
void main(void)
{   
	//vec4 reflecV = reflect(-directL_e, n_e);
	//float diffuseTerm = max(dot(n_e,directL_e), 0);

	//vec4 reflecV = vec4( reflect(-vec3(directL_e), n_e), 0.0);
	//float diffuseTerm = max(dot(n_e,vec3(directL_e)), 0);

	vec4 reflecV =vec4( reflect(-vec3(directL_Tang), TextureNormal_tangentspace), 0.0);


	float diffuseTerm = clamp(dot(TextureNormal_tangentspace,vec3(directL_Tang)),0.0,1.0);
	float specularTerm =  clamp(dot(reflecV, viewVec_Tang),0.0f,1.0f);
	
	fragcolor = texture2D(diffuseMap, tex_coord) * (0.0 + diffuseTerm ) + specularTerm ;


	//fragcolor = texture2D(diffuseMap, tex_coord) * (ambientL + diffuseTerm);// + specularTerm ;
    //fragcolor = texture2D(diffuseMap, tex_coord)* diffuseTerm ;//+  specularTerm;
	//fragcolor = vec4(specularTerm)/100.0;
}




















