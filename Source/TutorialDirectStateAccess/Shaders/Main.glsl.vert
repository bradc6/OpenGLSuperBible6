#version 420 core

//The Model View Matrix to convert vertice points
//from the localized model points to the Viewer space
//(Positions relative to the viewer (ie Camera View))
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

//Input vertices from a object buffer
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoordinate;
out gl_PerVertex { vec4 gl_Position; };

//A "struct" of data that will
//be passed along to the next shader.
out VSOutput
{
    vec2 textureCoordinate;
} vertexShaderOutput;

void main(void)
{
        //Pass along the texture coordinate to the fragment shader.

        vertexShaderOutput.textureCoordinate = textureCoordinate;

    gl_Position = projectionMatrix * modelViewMatrix * position;
}
