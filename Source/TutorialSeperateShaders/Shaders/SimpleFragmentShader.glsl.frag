//This specifies what version of OpenGL Shader
//is defined for
#version 420 core

in VSOutput
{
        vec2 textureCoordinate;
} fragmentShaderInput;

out vec4 color;

void main(void)
{
	color = vec4(fragmentShaderInput.textureCoordinate.x, fragmentShaderInput.textureCoordinate.y, 1.0, 1.0);
}