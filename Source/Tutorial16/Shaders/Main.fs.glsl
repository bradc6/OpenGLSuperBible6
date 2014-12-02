//Simple OpenGL vertex shader that we will
//be using for our simple texture example.
#version 420 core

layout (binding = 0) uniform sampler2D sampleTexture;

//A "struct" of data that will
//be passed along to the next shader.
in VSOutput
{
	vec2 textureCoordinate;
} fragmentShaderInput;


out vec4 outputColor;

void main(void)
{
	outputColor = texture(sampleTexture, fragmentShaderInput.textureCoordinate * vec2(3.0, 1.0));
}