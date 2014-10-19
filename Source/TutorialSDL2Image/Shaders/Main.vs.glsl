//Our simple texture vertex shader.
//All we are doing is hard coding in vertex points.
//Otherwise we would need to make a uniform and generate
//a buffer. (Making an overly complex example)
#version 400

void main(void)
{
	const vec4 triangleVertices[] = vec4[](vec4( 0.75, -0.75, 0.5, 1.0),
										   vec4(-0.75, -0.75, 0.5, 1.0),
										   vec4( 0.75,  0.75, 0.5, 1.0));

	gl_Position = triangleVertices[gl_VertexID];
}