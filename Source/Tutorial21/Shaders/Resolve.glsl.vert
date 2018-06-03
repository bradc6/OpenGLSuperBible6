#version 430 core

out gl_PerVertex
{
    vec4 gl_Position;
};

void main(void)
{
    const vec4 vertices[] = vec4[]( vec4(-1.0, -1.0, 0.5, 1.0),
				    vec4( 1.0, -1.0, 0.5, 1.0),
				    vec4(-1.0,  1.0, 0.5, 1.0),
				    vec4( 1.0,  1.0, 0.5, 1.0) );

    gl_Position = vertices[gl_VertexID];
}
