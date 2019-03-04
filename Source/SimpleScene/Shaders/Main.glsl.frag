#version 420 core

out vec4 color;

in VertexShaderOuput
{
    vec4 vertexColor;
} fragmentShaderIn;

void main(void)
{
    color = fragmentShaderIn.vertexColor;
    //color = vec4(1.0, 0.0, 0.0, 1.0);
}
/*
in VSOutput
{
        vec2 textureCoordinate;
} fragmentShaderInput;

out vec4 color;

void main(void)
{
        color = vec4(fragmentShaderInput.textureCoordinate.x, fragmentShaderInput.textureCoordinate.y, 1.0, 1.0);
}*/
