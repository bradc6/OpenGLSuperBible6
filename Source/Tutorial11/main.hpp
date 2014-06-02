#ifndef TUTORIAL11_MAIN_Header
#define TUTORIAL11_MAIN_Header

//GLEW must be included before GL.h (or SDL_opengl.h)
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


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
        throw "A filesystem error occured";
    }

    return inputFileStringStream.str();
}

#endif
