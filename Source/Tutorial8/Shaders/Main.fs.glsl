//This specifies what version of OpenGL Shader
//is defined for
#version 400 core

//"VertexOuputs" is the name of the struct
//whilst the instance is fragmentInput from
//the vertex shader.
//Input from the Vertex Shader
//!!WARNING!! They can't be used to gather
//outputs

in FragmentShaderData
{
	vec4 triangleColor;
} fragmentDataInput;

//Output color to the framebuffer
out vec4 outputTriangleColor;

void main(void)
{
	//We will simply output the color we wish to use,
	//that was passed in from the Vertex Shader, which in turn
	//came from our Vertex Fetch.
	outputTriangleColor = vec4(0.0, 0.8, 1.0, 1.0);
}