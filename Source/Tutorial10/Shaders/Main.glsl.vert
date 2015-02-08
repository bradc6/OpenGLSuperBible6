//This specifies what version of OpenGL Shader
//is defined for, in this tutorial we are going to 
//We will pass a set of attributes to the fragment shader
#version 400 core

in vec4 offset;
out vec4 vertexShaderColorOut;

void main(void)
{
	const vec4 vertices[] = vec4[](vec4(0.25, -0.25, 0.5, 1.0),
								   vec4(-0.25, -0.25, 0.5, 1.0),
								   vec4(0.25, 0.25, 0.5, 1.0));

	const vec4 colorSelection[] = vec4[](vec4(1.0, 0.0, 0.0, 1.0),
										 vec4(0.0, 1.0, 0.0, 1.0),
										 vec4(0.0, 0.0, 1.0, 1.0));

	//Add the offset to the vertex position
	gl_Position = vertices[gl_VertexID] + offset;

	//output the fixed value for vertexShaderColorOut
	vertexShaderColorOut = colorSelection[gl_VertexID];
}