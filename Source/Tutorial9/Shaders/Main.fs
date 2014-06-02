//This specifies what version of OpenGL Fragment Shader
//gl_FragCoord is an input variable that contains the
//window relative coordinate (x, y, z, 1/w) values for the fragment
#version 400 core

//"VertexOuputs" is the name of the struct
//whilst the instance is fragmentInput from
//the vertex shader.
//Input from the Vertex Shader
//!!WARNING!! They can't be used to gather
//outputs

//Output color to the framebuffer
out vec4 outputTriangleColor;

void main(void)
{
	//We will simply output the color we wish to use,
	//that was passed in from the Vertex Shader, which in turn
	//came from our Vertex Fetch.
	outputTriangleColor = vec4((sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5),
							   (cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5),
							   (sin(gl_FragCoord.x * 0.25) * cos(gl_FragCoord.y * 0.15)),
							   	1.0);
}