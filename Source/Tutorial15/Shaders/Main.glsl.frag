#version 400 core

out vec4 color;

in VertexShaderOuput
{
        vec4 vertexColor;
} fragmentShaderIn;

void main(void)
{
        color = fragmentShaderIn.vertexColor;
}
