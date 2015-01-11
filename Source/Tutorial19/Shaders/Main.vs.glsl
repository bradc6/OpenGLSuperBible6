#version 410 core

uniform vec2 textureOffset;
out vec2 textureCoordinate;

void main(void)
{
    const vec4 vertices[] = vec4[](vec4(-0.45, -0.45, 0.5, 1.0),
                                   vec4( 0.45, -0.45, 0.5, 1.0),
                                   vec4(-0.45,  0.45, 0.5, 1.0),
                                   vec4( 0.45,  0.45, 0.5, 1.0));

    gl_Position = vertices[gl_VertexID] + vec4(textureOffset, 0.0, 0.0);
    textureCoordinate = vertices[gl_VertexID].xy * 3.0 + vec2(0.45 * 3);
}
