#version 400

uniform samplerCube cubemap;
uniform sampler2D diffusemap;

out vec4 fragcolor;           
in vec3 pos;
in vec3 pos_eye;
     

void main(void)
{   
   //fragcolor = vec4(pos.y/10000.0f);
   fragcolor = texture(cubemap, pos);
}




















