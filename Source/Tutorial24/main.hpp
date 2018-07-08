#pragma once

//GLEW must be included before GL.h (or SDL_opengl.h)
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <memory>

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef FILE_ARG
#include QUOTE(FILE_ARG)
#endif

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
        assert(!"A filesystem error occured");
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
    if (programLinkerStatus != GL_TRUE)
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

//OpenGL failure call
//Formatting code from https://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
    std::cout << "---------------------opengl-callback-start------------\n";
    std::cout << "message: " << message << '\n';
    std::cout << "type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "OTHER";
        break;
    default:
        assert(!"Unknown DEBUG Type!");
        break;
    }
    std::cout << '\n';

    std::cout << "id: " << id << '\n';
    std::cout << "severity: ";
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "HIGH";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "NOTIFICATION";
        break;
    default:
        assert(!"Unknown SEVERITY Type!");
        break;
    }
    std::cout << '\n';
    std::cout << "---------------------opengl-callback-end--------------" << '\n';
}
