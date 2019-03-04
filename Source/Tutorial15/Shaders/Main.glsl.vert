#version 400 core

//Input vertices from a object buffer
in vec4 position;

//The vertice color data to share with the
//fragment shader.
out VertexShaderOuput
{
        vec4 vertexColor;
} vertexShaderOut;


//The Model View Matrix to convert vertice points
//from the localized model points to the Viewer space
//(Positions relative to the viewer (ie Camera View))
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main(void)
{
        gl_Position = projectionMatrix * modelViewMatrix * position;
        vertexShaderOut.vertexColor = position * 2.0 + vec4(0.5, 0.5, 0.5, 0);
}
