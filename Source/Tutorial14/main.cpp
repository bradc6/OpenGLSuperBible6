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

    return 0;
}
