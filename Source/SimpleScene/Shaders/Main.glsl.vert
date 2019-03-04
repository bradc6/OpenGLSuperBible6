#version 460 core

//The Model View Matrix to convert vertice points
//from the localized model points to the Viewer space
//(Positions relative to the viewer (ie Camera View))
uniform mat4 cameraViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

//Input vertices from a object buffer
layout (location = 0) in vec4 position;

//The vertice color data to share with the
//fragment shader.
out gl_PerVertex { vec4 gl_Position; };

out VertexShaderOuput
{
    vec4 vertexColor;
} vertexShaderOut;



void main(void)
{
        gl_Position = projectionMatrix * cameraViewMatrix * modelMatrix * position ;
        vertexShaderOut.vertexColor = position * 2.0 + vec4(0.5, 0.5, 0.5, 1.0);
}
