#version 410 core

layout(location = 0) out vec4 outputColor;

in VertexShaderOutput {
    flat int picture;
    vec2 textureCoordinate;
} fragmentShaderInput;

uniform sampler2DArray pictureTextures;

void main(void)
{
    outputColor = texture(pictureTextures, vec3(fragmentShaderInput.textureCoordinate, float(fragmentShaderInput.picture)));
}
