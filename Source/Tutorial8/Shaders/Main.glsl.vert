//This specifies what version of OpenGL Shader
//is defined for, in this tutorial we are going to 
//We will pass a set of attributes to the fragment shader
#version 400 core

layout (location = 0) in vec4 triangleOffset;

// "VertexOutputs" is the name of the struct
//whilst the instance is vertexOutput in the 
//Vertex Shader (Known as a Interface Block)

void main(void)
{
	//We will hard code a triangle with three vertices
	const vec4 triangleVertices[] = vec4[] (vec4( 0.25, -0.25, 0.5, 1.0), 
											vec4(-0.25, -0.25, 0.5, 1.0),
											vec4( 0.25,  0.25, 0.5, 1.0));


	//Index into our array using gl_VertexID
	//Then add the offset to our hard coded vertex position
	gl_Position = triangleVertices[gl_VertexID] + triangleOffset;
}