//This specifies what version of OpenGL Shader
//is defined for
#version 400 core

//Input from the Vertex Shader
in vec4 triangleColor;

//Output color to the framebuffer
out vec4 outputTriangleColor;

void main(void)
{
	//We will simply output the color we wish to use,
	//that was passed in from the Vertex Shader, which in turn
	//came from our Vertex Fetch.
	outputTriangleColor = triangleColor;
}