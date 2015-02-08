#version 420 core

layout (location = 0) out vec4 outputColor;
layout (binding = 0) uniform sampler2D mainTexture;

in VertexShaderOutput
{
    vec2 textureCoordinate;
} fragmentShaderInput;

void main(void)
{
    outputColor = texture(mainTexture, fragmentShaderInput.textureCoordinate);
}
