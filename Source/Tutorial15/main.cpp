#include "main.hpp"

int main()
{
    std::cout << "Sample code of uploading a 2D Texture from a KTX image\n";

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        exit(-1);
    }

    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow("KTX Texture Triangle", 100, 100,
                                              WINDOW_RESOLUTION_WIDTH, WINDOW_RESOLUTION_HEIGHT,
                                              SDL_WINDOW_OPENGL);

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
    //********************************************
    //Load+Compile+Link the Shaders
    //********************************************


    //********************************************
    //Create a 2D Texture and upload it to the GPU
    //********************************************
    GLuint sampleTexture = glNULL;

    //Generate a name for the texture to create.
    glGenTextures(1, &sampleTexture);

    //Bind the texture to the context, setting the texture to be 2D.
    glBindTexture(GL_TEXTURE_2D, sampleTexture);

    //First we will init a surface pointer and
    //a integer to have LoadImage get the texture data and
    //detect the texture image format. (RGB, RGBA, ect)
    SDL_Surface *sampleTextureSurface = NULL;
    GLint sampleTextureFormat = glNULL;
    LoadImage(QUOTE(TEXTUREDIR/MediumSampleTexture.png), sampleTextureSurface, sampleTextureFormat);

    //With the image loaded into system memory we will now upload it
    //to the GPU.
    // Specify the amount of storage we need for the texture
    glTexStorage2D(GL_TEXTURE_2D,                                      // 2D texture
                   8,                                                  // 8 mipmap levels
                   GL_RGBA32F,                                         // 32-bit floating-point RGBA data
                   sampleTextureSurface->w, sampleTextureSurface->h);                // imageWidth x imageHeight texels

    //Tell OpenGL the amount of storage that we need have for
    //the texture we are uploading.
    glTexSubImage2D(GL_TEXTURE_2D,                                    //The type of texture that we are uploading.
                    0,                                                //The mipmapped nth level of the texture.
                    0,0,                                              //The texel offset in the X & Y direction
                                                                      //within the texture array.
                    sampleTextureSurface->w, sampleTextureSurface->h, //The width & height of the target texture.
                    sampleTextureFormat,                              //The format of the image (Red/Green/Blue/Alpha).
                    GL_UNSIGNED_BYTE,                                 //The data format of the pixel data.
                    sampleTextureSurface->pixels);                    //The system memory location of the texture data.

    //Now that the texture has been uploaded to the GPU, we can
    //free our system memory copy
    SDL_free(sampleTextureSurface);
    sampleTextureSurface = NULL;

    //**********************************************
    //Create a vertex array and upload it to the GPU
    //**********************************************
    //We will be using TinyObjLoader, as we don't really want to
    //know the wavefront obj spec.

    //Load the shapes and materials (Even though we will not use the materials)
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string tinyObjError = tinyobj::LoadObj(shapes,
                                                materials,
                                                QUOTE(MODELDIR/Torus.obj),
                                                QUOTE(MODELDIR));

    if(!tinyObjError.empty()())
    {
        throw tinyObjError.c_str();
    }

    return 0;
}
