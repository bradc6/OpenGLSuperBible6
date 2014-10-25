#version 420 core

//The Model View Matrix to convert vertice points
//from the localized model points to the Viewer space
//(Positions relative to the viewer (ie Camera View))
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;


layout (location = 0) in vec4 position;
layout (location = 4) in vec2 tc;

out VS_OUT
{
    vec2 tc;
} vs_out;

void main(void)
{
    vec4 pos_vs = modelViewMatrix * position;

    vs_out.tc = tc;

    gl_Position = projectionMatrix * pos_vs;
}
