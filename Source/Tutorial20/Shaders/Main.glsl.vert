#version 410 core

layout(location = 0) in int pictureIndex;

out gl_PerVertex { vec4 gl_Position; };

out VertexShaderOutput
{
    flat int picture;
    vec2 textureCoordinate;
} vertexOutput;

struct picturePosition
{
    float xOffset;
    float yOffset;
    float orientation;
    float unused;
};

layout (std140) uniform picturesSet
{
    picturePosition pictures[256];
};

void main(void)
{
    //The base position that will be used
    const vec2[4] startPosition = vec2[4](vec2(-0.5, -0.5),
                                     vec2( 0.5, -0.5),
                                     vec2(-0.5,  0.5),
                                     vec2( 0.5,  0.5));

    vertexOutput.textureCoordinate = startPosition[gl_VertexID].xy + vec2(0.5);
    float cosine = cos(pictures[pictureIndex].orientation);
    float sine = sin(pictures[pictureIndex].orientation);

    mat2 rotationMatrix = mat2(vec2(cosine, sine),
                               vec2(-sine, cosine));
    vec2 position = 0.25 * rotationMatrix * startPosition[gl_VertexID];

    gl_Position = vec4(position.x + pictures[pictureIndex].xOffset,
                       position.y + pictures[pictureIndex].yOffset,
                       0.5, 1.0);

    vertexOutput.picture = pictureIndex % 64;
}
