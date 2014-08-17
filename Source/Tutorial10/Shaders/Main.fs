//This specifies what version of OpenGL Fragment Shader
//gl_FragCoord is an input variable that contains the
//window relative coordinate (x, y, z, 1/w) values for the fragment
#version 400 core

//A color that is produced by the  vertex shader
in vec4 vertexShaderColorOut;
out vec4 color;

void main(void)
{
	//Pass through the color from teh vertex shader to the fragment shader
	color = vertexShaderColorOut;
}