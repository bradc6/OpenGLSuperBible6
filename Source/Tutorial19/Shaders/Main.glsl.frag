#version 410 core

uniform sampler2D mainSampler;
out vec4 finalFragmentColor;
in vec2 textureCoordinate;

void main(void)
{
    finalFragmentColor = texture(mainSampler, textureCoordinate);
}
