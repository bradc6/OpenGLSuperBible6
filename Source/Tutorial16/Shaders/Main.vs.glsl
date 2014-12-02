#version 420 core

//Input vertices from a object buffer
layout (location = 0) in vec4 position;
layout (location = 4) in vec2 textureCoordinate;

//A "struct" of data that will
//be passed along to the next shader.
out VSOutput
{
	vec2 textureCoordinate;
} vertexShaderOutput;

//The Model View Matrix to convert vertice points
//from the localized model points to the Viewer space
//(Positions relative to the viewer (ie Camera View))
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
	//Pass along the texture coordinate to the fragment shader.
	vertexShaderOutput.textureCoordinate = textureCoordinate;

    gl_Position = projectionMatrix * modelViewMatrix * position;
}
