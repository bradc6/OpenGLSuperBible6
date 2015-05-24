#ifndef TUTORIAL13_MAIN_Header
#define TUTORIAL13_MAIN_Header

//GLEW must be included before GL.h (or SDL_opengl.h)
#include <GL/glew.h>

//Approx conversion from Degrees to Radians
#define degreesToRadians(targetDegree) targetDegree*(3.141592f/180.0f)

//Force the use of Radians only
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <SDL2/SDL_image.h>

#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>


#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef FILE_ARG
#include QUOTE(FILE_ARG)
#endif

#define glNULL 0

//!Load a file into a string
/*!Load a file into a string, which in turn can be
 *	put into OpenGL shader source for comilation
 *	\pre A string to a valid filesystem path
 *	\param [in] filePath The file to be loaded into a string
 *	\returns A string with the specified file
 *	\note NA*/
std::string LoadFileToString(const std::string &filePath)
{
    std::ifstream inputFile;
    std::stringstream inputFileStringStream;

    inputFile.exceptions(std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit);

    try
    {
        inputFile.open(filePath.c_str(), std::ios::binary);
        inputFileStringStream << inputFile.rdbuf();
        inputFile.close();
    }
    catch (...)
    {
        throw "A filesystem error occured";
    }

    return inputFileStringStream.str();
}

//!Loads & compiles an GLSL shader
/*!Loads a GLSL shader and compiles the shader
 * \pre The shaderFilePath must be a valid OpenGL GLSL shader for the
 *  shader type specified.
 * \param [in] shaderFilePath The filepath to the shader you wish to load & compile.
 * \param [in] shaderType The type of OpenGL shader that we are compiling.
 * \throws A filesystem/ShaderCompileError error if there are any issues.
 * \returns The handle for the newly compiled shader.
 * \note NA*/
GLuint CompileGLShader(std::string shaderFilePath, GLint shaderType)
{
    switch(shaderType)
    {
        case GL_VERTEX_SHADER:
        {
            std::cout << "Vertex Shader";
            break;
        }
        case GL_TESS_CONTROL_SHADER:
        {
            std::cout << "Tesselation Control Shader";
            break;
        }
        case GL_TESS_EVALUATION_SHADER:
        {
            std::cout << "Tesselation Evaluation Shader";
            break;
        }
        case GL_GEOMETRY_SHADER:
        {
            std::cout << "GeometryShader";
            break;
        }
        case GL_FRAGMENT_SHADER:
        {
            std::cout << "Fragment Shader";
            break;
        }
        case GL_COMPUTE_SHADER:
        {
            std::cout << "Computation Shader";
            break;
        }
        default:
        {
            throw "Unknown shader type";
            break;
        }
    }

    std::cout << " Filepath: " << shaderFilePath << '\n';

    std::string targetShaderSource = LoadFileToString(shaderFilePath);

    //Make a pointer to make glShaderSource happy (I REALLY hate this part of the solution)
    const char *sourceTargetShaderBegin = targetShaderSource.c_str();

    //Create a instance of a vertex shader (Create a shader)
    GLuint targetShader = glCreateShader(shaderType);
    //Load the shader source into our Vertex Shader instance.
    glShaderSource(targetShader, 1, (const GLchar **) &sourceTargetShaderBegin, NULL);
    //Clear the system memory copy of the Vertex Shader Source
    targetShaderSource.clear();

    //Now to compile the Vertex Shader
    glCompileShader(targetShader);
    //Lets make a GLint to ensure that the shader compilation was successful
    GLint shaderCompilerStatus;
    //Get the status of the shader compiler.
    glGetShaderiv(targetShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the vertex shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(targetShader, 1024, NULL, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    return targetShader;
}

void GenerateTextureData(float *textureData, const unsigned int textureWidth, const unsigned int textureHeight)
{
    int textureXPosition, textureYPosition;

    for (textureYPosition = 0; textureYPosition < textureHeight; textureYPosition++)
    {
        for (textureXPosition = 0; textureXPosition < textureWidth; textureXPosition++)
        {
            textureData[(textureYPosition * textureWidth + textureXPosition) * 4 + 0] = (float)((textureXPosition & textureYPosition) & 0xFF) / 255.0f;
            textureData[(textureYPosition * textureWidth + textureXPosition) * 4 + 1] = (float)((textureXPosition | textureYPosition) & 0xFF) / 255.0f;
            textureData[(textureYPosition * textureWidth + textureXPosition) * 4 + 2] = (float)((textureXPosition ^ textureYPosition) & 0xFF) / 255.0f;
            textureData[(textureYPosition * textureWidth + textureXPosition) * 4 + 3] = 1.0f;
        }
    }
}

#endif
