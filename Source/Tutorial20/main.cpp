//Load up a model and wrap textures around it.

#include "main.hpp"
#include <iostream>

const std::string programDescription = "Texture Array Sample";

int main(int argc, char* argv[])
{
    std::cout << programDescription << '\n';

    //Initialize the graphics portion of SDL
    assert(SDL_InitSubSystem(SDL_INIT_VIDEO) >= 0); //"SDL was unable to initialize"

    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow(programDescription.c_str(), 100, 100, 800, 600, SDL_WINDOW_OPENGL);

    //Check that the SDL/OpenGL window was created
    assert(mainWindow); //The SDL_CreateWindow method failed

    //The OpenGL Context (instance of OpenGL) that we will use
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    SDL_GL_MakeCurrent(mainWindow, mainContext);

    assert(mainContext); //Could not create OpenGL context

    //Force GLEW to use experimental draw calls, but they are supported by the card
    glewExperimental = true;

    //Initialize GLEW in order to ensure we don't call GL methods the driver can't understand
    GLenum glewError = glewInit();

    if(GLEW_OK != glewError)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glewError) << '\n';
        assert(false);
    }

    //With the context set we will setup a OpenGL debug context callback
    glDebugMessageCallbackARB((GLDEBUGPROCARB)gl_debug_callback, NULL);

    //DO SOME OPENGL STUFF HERE

    //Declare and create a vertex array object
    //This will store the different attributes of the shaders and vertex
    GLuint savedVertexAttributes;

    //Generate the Vertex Array object  on the graphics card that we
    //will later be able to save for fast reference. This only allocates the memory
    //Next we need to bind it to be active.
    glGenVertexArrays(1, &savedVertexAttributes);

    //Bind that newly created space in order to be used
    glBindVertexArray(savedVertexAttributes);

    //Now lets build a vertex shader
    std::cout << "Shader Dir: " << QUOTE(SOURCEDIR/Source/Tutorial20/Shaders/Main.glsl.vert) << '\n';
    std::string vertexShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial20/Shaders/Main.glsl.vert));
    //Make a pointer to make glShaderSource happy (I REALLY hate this part of the solution)
    const char *sourceVertexShaderBegin = vertexShaderSource.c_str();
    //Create a instance of a vertex shader (Create a shader)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Load the shader source into our Vertex Shader instance.
    glShaderSource(vertexShader, 1, (const GLchar **) &sourceVertexShaderBegin, nullptr);
    //Clear the system memory copy of the Vertex Shader Source
    vertexShaderSource.clear();
    //Now to compile the Vertex Shader
    glCompileShader(vertexShader);
    //Lets make a GLint to ensure that the shader compilation was successful
    GLint shaderCompilerStatus;
    //Get the status of the shader compiler.
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the vertex shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(vertexShader, 1024, nullptr, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    //Now lets build a fragment shader
    std::string fragmentShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial20/Shaders/Main.glsl.frag));
    const char *sourceFragmentShaderBegin = fragmentShaderSource.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar **) &sourceFragmentShaderBegin, nullptr);
    fragmentShaderSource.clear();
    glCompileShader(fragmentShader);
    //Get the status of the shader compiler.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the fragment shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    //Now to use the shaders we just compiled, we need to create a shader program
    //(In a sense these are like different, pipelines that when linked can be switched out)
    GLuint shaderProgram = glCreateProgram();

    //Attach shaders to our newly created shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //Link the Shader Program to create a executable shader pipeline
    //for the graphics card t ouse.
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

    //Use the shader program that OpenGL compiled and linked.
    glUseProgram(shaderProgram);

    //Since we are not going to attach the shaders to any
    //additional programs they are safe to be deleted.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Generate a name for our Vertex Array object
    //and bind it for use.
    GLuint mainVertexArray;
    glGenVertexArrays(1, &mainVertexArray);
    glBindVertexArray(mainVertexArray);


    return 0;
}
