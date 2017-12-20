#version 400            
uniform mat4 PVM;
uniform mat4 M;
uniform mat4 V;

in vec3 pos_attrib; //in object space
out vec3 pos;
out vec3 pos_eye;

void main(void)
{
   //Compute clip-space vertex position
   gl_Position = PVM*vec4(pos_attrib, 1.0);     //w = 1 becase this is a point
   pos = pos_attrib;
   pos_eye = vec3(V*M*vec4(pos_attrib, 1.0f));
}