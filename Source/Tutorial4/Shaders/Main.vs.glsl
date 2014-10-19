//This specifies what version of OpenGL Shader
//is defined for
#version 400 core

void main(void)
{
	//We will hard code a triangle with three vertices
	const vec4 triangleVertices[3] = vec4[3] (vec4( 0.25, -0.25, 0.5, 1.0), 
											  vec4(-0.25, -0.25, 0.5, 1.0),
											  vec4( 0.25,  0.25, 0.5, 1.0));

	//Index into our array using gl_VertexID
	gl_Position = triangleVertices[gl_VertexID];
}