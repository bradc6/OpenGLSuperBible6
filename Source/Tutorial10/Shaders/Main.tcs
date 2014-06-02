//Our OpenGL Tesselation Control Shader
//This allows us to set the level
//of tesselation we want the fixed function engine to
//Control
#version 400 core

layout (vertices = 3) out;

void main(void)
{
	if(gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 5.0;
		gl_TessLevelOuter[0] = 5.0;
		gl_TessLevelOuter[1] = 5.0;
		gl_TessLevelOuter[2] = 5.0;  
	}
	
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}