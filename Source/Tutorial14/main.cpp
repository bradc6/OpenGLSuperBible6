#include "main.hpp"

#pragma message ("This sample does not currently work, check out the SDL2_Image tutorial instead")

int main()
{
    std::cout << "Sample code of uploading a 2D Texture from a KTX image\n";
    
    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        exit(-1);
    }
    
    //Request the context be OpenGL 3.2 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow("KTX Texture Triangle", 100, 100, WINDOW_RESOLUTION_WIDTH, WINDOW_RESOLUTION_HEIGHT, SDL_WINDOW_OPENGL);
    
    //Check that the SDL/OpenGL window was created
    if(!mainWindow)
    {
        std::cout << "The SDL_CreateWindow method failed\n";
        exit(-1);
    }
    
    //The OpenGL Context (instance of OpenGL) that we will use
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    SDL_GL_MakeCurrent(mainWindow, mainContext);
    
    if(!mainContext)
    {
        
        std::cout << SDL_GetError() << '\n';
        exit(-1);
    }
    
    //Force GLEW to use experimental draw calls, but they are supported by the card
    glewExperimental = true;
    
    //Initialize GLEW in order to ensure we don't call GL methods the driver can't understand
    GLenum glewError = glewInit();
    
    if(GLEW_OK != glewError)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glewError) << '\n';
        exit(-1);
    }
    
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
    GLuint vertexShader = CompileGLShader(QUOTE(SOURCEDIR/Source/Tutorial14/Shaders/Main.glsl.vert), GL_VERTEX_SHADER);
    
    //Now lets build a fragment shader
    GLuint fragmentShader = CompileGLShader(QUOTE(SOURCEDIR/Source/Tutorial14/Shaders/Main.glsl.frag), GL_FRAGMENT_SHADER);
    
    //Now to use the shaders we just compiled, we need to create a shader program
    //(In a sense these are like different, pipelines that when linked can be switched out)
    GLuint shaderProgram = glCreateProgram();
    
    //Attach shaders to our newly created shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    //Ensure that the program linked successfully
    GLint programLinkerStatus;
    //Get the status of the shader program linker.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programLinkerStatus);
    if(programLinkerStatus != GL_TRUE)
    {
        std::cout << "Failed to linke shader program\n";
        char openGLLinkerError[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, openGLLinkerError);
        std::cout << openGLLinkerError << '\n';
        exit(-1);
    }

    
    //With the shader program created and linked, lets use the GLShader program
    glUseProgram(shaderProgram);
    
    //*********************************************
    //Loading/Generating/Viewing a KTX Texture here
    //*********************************************
    
    //We will pass these variables to ktxLoadTextureN and it
    //will fill out the different attributes associated with the image.
    //We will set some default values for sanity.
    GLuint ktxTexture = 0;                     //A default state for the texture handle
    GLenum ktxTextureTargetType;    //Default to a 2D texture (Probably what you are loading)
    GLenum textureGLError;            //We will assume no error until otherwise
    GLboolean isTexturePreMipmapped;        //We will assume the image is not pre mipmapped.
    
    //First lets load the KTX image into system memory
    std::cout << "Loading " << QUOTE(TEXTUREDIR/VeryLargeSampleTexture.ktx) << '\n';
    KTX_error_code ktxTextureError = ktxLoadTextureN(QUOTE(TEXTUREDIR/VeryLargeSampleTexture.ktx),
                                                     &ktxTexture,
                                                     &ktxTextureTargetType,
                                                     NULL,
                                                     &isTexturePreMipmapped,
                                                     &textureGLError,
                                                     0,
                                                     NULL);
    //Check that KTX image loading was NOT successful
    if(KTX_SUCCESS != ktxTextureError)
    {
        //Check out what the ktx error is
        switch (ktxTextureError)
        {
            case KTX_FILE_OPEN_FAILED:
                std::cout << "The target file could not be opened.\n";
                break;
            case KTX_FILE_WRITE_ERROR:
                std::cout << "An error occurred while writing to the file.\n";
                break;
            case KTX_GL_ERROR:
                std::cout << "GL operations resulted in an error. \n";
                break;
            case KTX_INVALID_OPERATION:
                std::cout << "The operation is not allowed in the current state.\n";
                break;
            case KTX_INVALID_VALUE:
                std::cout << "A parameter value was not valid \n";
                break;
            case KTX_NOT_FOUND:
                std::cout << "Requested key was not found \n";
                break;
            case KTX_OUT_OF_MEMORY:
                std::cout << "Not enough memory to complete the operation.\n";
                break;
            case KTX_UNEXPECTED_END_OF_FILE:
                std::cout << "The file did not contain enough data.\n";
                break;
            case KTX_UNKNOWN_FILE_FORMAT:
                std::cout << "The file is not a KTX file.\n";
                break;
            case KTX_UNSUPPORTED_TEXTURE_TYPE:
                std::cout << "The KTX file specifies an unsupported texture type.\n";
                break;
            default:
                std::cout << "Unknown Error, this should NEVER happen!!\n";
                throw "Unknown KTX file";
                break;
        }
        
        
        std::cout << "libktx failed to load the ktx image!!\n";
        exit(-1);
    }

    glEnable(ktxTexture);
    if(isTexturePreMipmapped)
    {
        //Enable bilinear mipmapping
        glTexParameteri(ktxTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
    else
    {
        glTexParameteri(ktxTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexParameteri(ktxTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    //Create a window event in order to know when the mainWindow "Close" is pressed
    SDL_Event *windowEvent = new SDL_Event;
    
    while(true)
    {
        if(SDL_PollEvent(windowEvent))
        {
            if(windowEvent->type == SDL_QUIT)
            {
                break;
            }
        }
        
        //Set the clear buffer color
        static const GLfloat greenColor[] = {0.0f, 0.25f, 0.0f, 1.0f};
        
        //Clear the frame buffer with a greenColor.
        glClearBufferfv(GL_COLOR, 0, greenColor);
        //Draw the triangle vertices.
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        SDL_GL_SwapWindow(mainWindow);
    }
    
    return 0;
}
