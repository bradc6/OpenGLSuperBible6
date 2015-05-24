//Example on the usage of texture wrapping

#include "main.hpp"
#include <iostream>

const std::string SampleDescription = "Texture Wrapping";
int main(int argc, char* argv[])
{
    std::cout << SampleDescription << '\n';

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        assert(false);
    }

    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow(SampleDescription.c_str(), 100, 100, 800, 600, SDL_WINDOW_OPENGL);

    //Check that the SDL/OpenGL window was created
    if(!mainWindow)
    {
        std::cout << "The SDL_CreateWindow method failed\n";
        assert(false);
    }

    //The OpenGL Context (instance of OpenGL) that we will use
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    SDL_GL_MakeCurrent(mainWindow, mainContext);

    if(!mainContext)
    {

        std::cout << SDL_GetError() << '\n';
        assert(false);
    }

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
    std::cout << "Shader Dir: " << QUOTE(SOURCEDIR/Source/Tutorial19/Shaders/Main.glsl.vert) << '\n';
    std::string vertexShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial19/Shaders/Main.glsl.vert));
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
    std::string fragmentShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial19/Shaders/Main.glsl.frag));
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
        std::cout << "Failed to linke shader program\n";
        char openGLLinkerError[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, openGLLinkerError);
        std::cout << openGLLinkerError << '\n';
        assert(false);
    }

    //Use the shader program that OpenGL compiled and linked.
    glUseProgram(shaderProgram);

    //*************************************
    //Load Texture to show texture wrapping
    //*************************************
    GLuint mainTexture = glNULL;
    GLint mainTextureFormat;
    SDL_Surface *sysMainTexture = nullptr;

    //Load the arrow texture via SDL2_Image
    LoadImage(QUOTE(TEXTUREDIR/arrow.png), sysMainTexture, mainTextureFormat);

    //Create a name for the arrow texture.
    glGenTextures(1, &mainTexture);

    //Bind the texture to the context (prepare to upload data).
    glBindTexture(GL_TEXTURE_2D, mainTexture);

    //Allocate the storage for the texture.
    glTexStorage2D(GL_TEXTURE_2D,                                      // 2D texture
                   1,                                                  // 0 mipmap levels (Base Image)
                   GL_RGBA32F,                                         // 32-bit floating-point RGBA data
                   sysMainTexture->w, sysMainTexture->h);              // imageWidth x imageHeight texels

    //Upload the texture data to the GPU.
    glTexSubImage2D(GL_TEXTURE_2D,                                     //The type of texture that we are uploading.
                    0,                                                 //The mipmapped nth level of the texture.
                    0,0,                                               //The texel offset in the X & Y direction within the texture array.
                    sysMainTexture->w, sysMainTexture->h,              //The width & height of the target texture.
                    mainTextureFormat,                                 //The format of the image (Red/Green/Blue/Alpha).
                    GL_UNSIGNED_BYTE,                                  //The data format of the pixel data.
                    sysMainTexture->pixels);                           //The system memory location of the texture data.


    //Free system copy of the texture
    SDL_free(sysMainTexture);
    sysMainTexture = nullptr;

    //***************************
    //Setup the projection matrix
    //***************************
    //Get the SDL window width and height
    int windowWidth, windowheight;
    SDL_GetWindowSize(mainWindow, &windowWidth, &windowheight);

    //Enable the use of Depth testing to check if certain objects
    //are too far away on the screen
    glEnable(GL_DEPTH_TEST);

    //Check the depth of vertices in on Z (Must be less then or equal to 1)
    glDepthFunc(GL_LEQUAL);

    //Create a window event in order to know when the mainWindow "Close" is pressed
    SDL_Event *windowEvent = new SDL_Event;

    //Enable wireframe mode to debug vertices
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    //Get the TextureOffset uniform location
    GLint textureUniformLocation = glGetUniformLocation(shaderProgram, "textureOffset");

    while(true)
    {
        if(SDL_PollEvent(windowEvent))
        {
            switch(windowEvent->type) {
            case SDL_QUIT:
            {
                break;
            }
            }
        }

        //Set the colors and depth that we will use for the example.
        static const GLfloat greenColor[] = {0.0f, 0.25f, 0.0f, 1.0f};
        static const GLfloat yellow[] = { 0.4f, 0.4f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;

        //Set of the OpenGL viewPort size
        //This should probably be set to the same size as the window in most situations
        glViewport(0, 0, windowWidth, windowheight);

        //Clear the screen color
        glClearBufferfv(GL_COLOR, 0, greenColor);

        //Clear the depth buffer
        glClearBufferfv(GL_DEPTH, 0, &one);

        //Set the color of a texture where the texture does is NOT big
        //enough to cover the entire area.
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, yellow);

        //The different texture wrapping modes that will be shown.
        static const GLenum textureWrapModes[] = { GL_CLAMP_TO_EDGE,
                                                   GL_REPEAT,
                                                   GL_CLAMP_TO_BORDER,
                                                   GL_MIRRORED_REPEAT };

        //A set of vertice offsets to ensure that the different repeating
        //texture wrapping modes do not overlap.
        static const float offsets[] = { -0.5f, -0.5f,
                                          0.5f, -0.5f,
                                         -0.5f,  0.5f,
                                          0.5f,  0.5f };

        for(unsigned int currentWrapMode = 0; currentWrapMode < 4; currentWrapMode++)
        {
            //Set the offset for the texture wrap mode that we will display
            glUniform2fv(textureUniformLocation, 1, &offsets[currentWrapMode * 2]);

            //Set the texture wrapping modes for the S(X) and T(Y) axis
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapModes[currentWrapMode]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapModes[currentWrapMode]);

            //Draw our results the frame buffer.
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        //Swap the framebuffer.
        SDL_GL_SwapWindow(mainWindow);
    }

    return 0;
}
