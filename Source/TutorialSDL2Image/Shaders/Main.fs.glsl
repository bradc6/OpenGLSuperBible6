//Simple OpenGL vertex shader that we will
//be using for our simple texture example.
#version 400 core

uniform sampler2D sampleTexture;

out vec4 outputColor;

void main(void)
{
        //Because OpenGL considers the origin of textures to be the bottom left corner,
        //instead of the top left corner we need to flip the coordinate system around for the
        //image.
        vec2 flippedTextureCoordinate = vec2(gl_FragCoord.x, 1.0 - gl_FragCoord.y);

        //You could use the vec2 flip directly as well.
        outputColor = texture(sampleTexture, flippedTextureCoordinate.xy / textureSize(sampleTexture, 0));

        //Or you can pass the texture coordinate directly to check the result without the vertical flip
        //outputColor = texture(sampleTexture, gl_FragCoord.xy / textureSize(sampleTexture, 0));
}
