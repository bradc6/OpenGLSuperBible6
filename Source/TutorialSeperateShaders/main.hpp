#pragma once

//GLEW must be included before GL.h (or SDL_opengl.h)
#include <GL/glew.h>

//Force the use of Radians only
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

//Include the TinyObjLoader library.
#include <tiny_obj_loader.h>

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef FILE_ARG
#include QUOTE(FILE_ARG)
#endif

#define glNULL 0

#define WINDOW_RESOLUTION_WIDTH 800
#define WINDOW_RESOLUTION_HEIGHT 600

#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
        static const GLubyte checkerBoardPattern[] =
        {
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        };
#undef B
#undef W

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

//!Creates a serperable shader program
GLuint CreateShaderProgram(std::string shaderSourceFilePath, GLint shaderType)
{
    //Get a shader loaded->compiled
    GLuint compiledShader = CompileGLShader(shaderSourceFilePath, shaderType);

    //Create a program
    GLuint shaderProgram = glCreateProgram();

    //Set the program's attribute to be a seperated shader program
    //(Otherwise linking will fail as the program shader pipeline is incomplete)
    glProgramParameteri(shaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

    //Attach out single shader to the program
    glAttachShader(shaderProgram, compiledShader);

    //Link the resulting program
    glLinkProgram(shaderProgram);

    //Ensure that the program linked successfully
    GLint programLinkerStatus;
    //Get the status of the shader program linker.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programLinkerStatus);
    if(programLinkerStatus != GL_TRUE)
    {
        std::cout << "Failed to link shader program\n";
        char openGLLinkerError[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, openGLLinkerError);
        std::cout << openGLLinkerError << '\n';
        assert(false);
    }

    //Now that the shader program has been linked successfully we will delete our
    //shader object.
    glDeleteShader(compiledShader);

    return shaderProgram;
}

//!Loads the image to system memory,
/*!Loads a GLSL shader and compiles the shader
 * \pre The image filepath must be a valid filesystem path to a valid image
 * \param [in] imageFilePath The file path to the image file to load
 * \param [out] targetSuface The resulting surface that will be
 * \param [out] textureFormat The detected format of the loaded image.
 * \throws
 * \note NA*/
void LoadImage(const std::string &imageFilePath, SDL_Surface *&targetSurface, GLint &textureFormat)
{
    //Check that the targetSurface pointer is NULL
    if(targetSurface)
    {
        throw "SDL Surface was not NULL! Don't leak memory!";
    }

    //Load & decode a 2d image that SDL2_Image can handle
    targetSurface = IMG_Load(imageFilePath.c_str());

    if(!targetSurface)
    {
        throw ("Failed to load the target image @" + imageFilePath).c_str();
    }

   //Determine the image format (Image color channel configuration)
   //textureImageFormat is first use to query the surface for the
   //number of channels. After detection it holds the OpenGL texture
   //encoding.
   textureFormat = targetSurface->format->BytesPerPixel;
   if (textureFormat == 4)
   {
       //alpha channel
       if (targetSurface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGBA;
       else
            textureFormat = GL_BGRA;
   }
   else
   {
        if (textureFormat == 3)
        {
            //no alpha channel
            if (targetSurface->format->Rmask == 0x000000ff)
                textureFormat = GL_RGB;
            else
                textureFormat = GL_BGR;
        }
    else
    {
        throw "Unable to determine image format";
    }
   }

}

//OpenGL failure call

void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
const GLchar *message, GLvoid *userParam)
{
    std::string mainMessage = message;
    std::cout << "GL Debug:" << mainMessage << '\n';
}

//Checkerboard texture pattern.
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
        static const GLubyte checkerBoardTexture[] =
        {
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        };
#undef B
#undef W

