#version 430
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2DShadow u_shadowMap;
uniform mat4 M;
uniform mat4 V;
uniform float shine;
uniform vec3 LightColor;

in float normalCoff;
in float colorDecay;
in vec2 tex_coord;
in vec4 vPos_EyeSpace;

in vec4 o_shadowCoord;
in vec3 eye_tangentSpace;
in vec3 light_tangentSpace;

out vec4 fragcolor;  
      
vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float fogDensity = 0.01f;
vec4 fogColor = vec4(0.1, 0.1, 0.15, 1.0);

float getFogFactor(float fogCoord)
{
	float result = 0.0;

	result = exp(-fogDensity*fogCoord);
	result = 1.0f - clamp(result, 0.0f, 1.0f);

	return result;
}

float calorBlueTilt = clamp(colorDecay - 1.0f, 0.0, 0.05f);
vec3 greyScale = vec3(0.3, 0.59, 0.11);

void main(void)
{   
	 //handle fragments
	if(texture2D(diffuseMap, tex_coord).a < 0.1)
		discard;

	//calculate diffuse and specular term
	vec3 E = normalize(eye_tangentSpace);
	vec3 l = normalize(light_tangentSpace);
	
	vec3 n = normalize(texture(normalMap, tex_coord).rgb*2.0 - 1.0);
	n.rgb = n.rgb * vec3(normalCoff, normalCoff, 1.0f);

	vec3 R = reflect(-l, n);

	float diffuseTerm = clamp(dot(n, l),0,1);
	float specularTerm =  2.0f*clamp(pow(dot(R, E),shine),0.0f,1.0f);
	
	float shadowFactor = textureProj(u_shadowMap, o_shadowCoord);
	
	
	// Calculate shadow maps
	float bias = 0.005*tan(acos(dot(n,l)));
    bias = clamp(bias, 0.0 ,0.01);
    for (int i=0;i<4;i++){
		 int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
		shadowFactor -= 0.2*(1.0-texture( u_shadowMap, vec3(o_shadowCoord.xy + poissonDisk[index]/700.0,  (o_shadowCoord.z-bias)/o_shadowCoord.w) ));
	 }

	 shadowFactor = clamp(shadowFactor, 0.2f, 1.0f);
	 if(dot(n, l)<0)
	 {
		 shadowFactor = 0.2f;
	 }

	 float fogCoord = abs(vPos_EyeSpace.z / vPos_EyeSpace.w);
	 
	 vec4 color = (texture2D(diffuseMap, tex_coord) * 
				(0.45 * colorDecay +  colorDecay*shadowFactor * diffuseTerm  )+ 
				colorDecay *0.2 *shadowFactor * specularTerm) * vec4(LightColor, 1.0f) ;

	color.rgb = (vec3(dot(color.rgb,greyScale)) * (colorDecay-1.0f) + (2.0 - colorDecay)*color.rgb) ;
	 fragcolor =  mix(color, fogColor, clamp(getFogFactor(fogCoord),0.0, 0.6f) ) + vec4(0.0f, 0.0f, calorBlueTilt, 0.0f);
	//fragcolor = vec4(colorDecay);

}




















