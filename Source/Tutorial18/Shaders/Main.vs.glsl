#version 420 core

out VertexShaderOutput
{
    vec2 textureCoordinate;
} vertexOutput;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform float offset;

void main(void)
{
    const vec2[4] position = vec2[4](vec2(-0.5, -0.5),
                                     vec2( 0.5, -0.5),
                                     vec2(-0.5,  0.5),
                                     vec2( 0.5,  0.5));
    vertexOutput.textureCoordinate = (position[gl_VertexID].xy + vec2(offset, 0.5)) * vec2(30.0, 1.0);
    gl_Position = (projectionMatrix * modelViewMatrix) * vec4(position[gl_VertexID], 0.0, 1.0);
}
