#version 330

// attributes
in vec3 pos_attrib;

// matrices
uniform mat4 u_modelMat;
uniform mat4 u_shadowViewMat;
uniform mat4 u_shadowProjMat;

void main(void)
{
   // transform to screen space
   // shader will save z/w value to z-buffer
   gl_Position	= u_shadowProjMat * u_shadowViewMat * u_modelMat * vec4(pos_attrib, 1);
}