#include "main.hpp"

#define WINDOW_RESOLUTION_WIDTH 800
#define WINDOW_RESOLUTION_HEIGHT 600

int main()
{
    std::cout << "Sample code of uploading a 2D Texturer\n"
              << "Note: We are using a simple hard coded texture.\n";

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        assert(false);
    }

    //Request the context be OpenGL 3.2 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow("Simple Texture Triangle (SDL2_Image)", 100, 100, WINDOW_RESOLUTION_WIDTH, WINDOW_RESOLUTION_HEIGHT, SDL_WINDOW_OPENGL);

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
    GLuint vertexShader = CompileGLShader(QUOTE(SOURCEDIR/Source/TutorialSDL2Image/Shaders/Main.glsl.vert), GL_VERTEX_SHADER);

    //Now lets build a fragment shader
    GLuint fragmentShader = CompileGLShader(QUOTE(SOURCEDIR/Source/TutorialSDL2Image/Shaders/Main.glsl.frag), GL_FRAGMENT_SHADER);

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
        assert(false);
    }

    //With the shader program created and linked, lets use the GLShader program
    glUseProgram(shaderProgram);

    //********************************************
    //Create a 2D Texture and upload it to the GPU
    //********************************************
    GLuint triangleTexture = glNULL;

    //Load & decode a 2d image that SDL2_Image can handle
    SDL_Surface *triangleTextureData = IMG_Load(QUOTE(TEXTUREDIR/MediumSampleTexture.png));

    //Generate a name for the texture to create.
    glGenTextures(1, &triangleTexture);

    //Bind the texture to the context, setting the texture to be 2D.
    glBindTexture(GL_TEXTURE_2D, triangleTexture);

    //Determine the mode (Color channels) for the image.
    GLint textureImageFormat = 0;

   //Determine the image format (Image color channel configuration)
   //textureImageFormat is first use to query the surface for the
   //number of channels. After detection it holds the OpenGL texture
   //encoding.
   textureImageFormat = triangleTextureData->format->BytesPerPixel;
   if (textureImageFormat == 4)
   {
       //alpha channel
       if (triangleTextureData->format->Rmask == 0x000000ff)
            textureImageFormat = GL_RGBA;
       else
            textureImageFormat = GL_BGRA;
   }
   else
   {
        if (textureImageFormat == 3)
        {
            //no alpha channel
            if (triangleTextureData->format->Rmask == 0x000000ff)
                textureImageFormat = GL_RGB;
            else
                textureImageFormat = GL_BGR;
        }
    else
    {
        throw "Unable to determine image format";
    }
   }

    // Specify the amount of storage we need for the texture
    glTexStorage2D(GL_TEXTURE_2D,                                      // 2D texture
                   8,                                                  // 8 mipmap levels
                   GL_RGBA32F,                                         // 32-bit floating-point RGBA data
                   triangleTextureData->w, triangleTextureData->h);    // imageWidth x imageHeight texels

    //Tell OpenGL the amount of storage that we need have for
    //the texture we are uploading.
    glTexSubImage2D(GL_TEXTURE_2D,                                              //The type of texture that we are uploading.
                    0,                                                          //The mipmapped nth level of the texture.
                    0,0,                                                        //The texel offset in the X & Y direction within the texture array.
                    triangleTextureData->w, triangleTextureData->h,             //The width & height of the target texture.
                    textureImageFormat,                                         //The format of the image (Red/Green/Blue/Alpha).
                    GL_UNSIGNED_BYTE,                                           //The data format of the pixel data.
                    triangleTextureData->pixels);                               //The system memory location of the texture data.

    //Free the SDL surface we used to load the image
    SDL_FreeSurface(triangleTextureData);
    triangleTextureData = NULL;

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
}
