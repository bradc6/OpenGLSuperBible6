//Simple OpenGL vertex shader that we will
//be using for our simple texture example.
#version 400 core

uniform sampler2D sampleTexture;

out vec4 outputColor;

void main(void)
{
	outputColor = texture(sampleTexture, gl_FragCoord.xy / textureSize(sampleTexture, 0));
}