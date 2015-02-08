////Our Geometery Shader
//The geometry shader is also unique amongst the shader stages
//in that it is able to increase or reduce the amount of 
//data flowing in through the pipeline in a programmatic way.

#version 400 core
layout (triangles) in;
layout (points, max_vertices = 3) out;

void main(void)
{
	int i;

	for(i = 0; i < gl_in.length(); i++)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
}