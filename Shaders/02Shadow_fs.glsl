#version 330

// color to framebuffer
layout (location = 0) out vec4 resultingColor;

void main(void)
{
   // fragment shader by default is obligated to output color
   resultingColor = vec4(0, 0, 0, 1);
}