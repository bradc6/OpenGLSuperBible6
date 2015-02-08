#include "main.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Simple OpenGL context creation Example\n";

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
    SDL_Window *mainWindow = SDL_CreateWindow("SDL+OpenGLContext", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

    //Check that the SDL/OpenGL window was created
    if(!mainWindow)
    {
        std::cout << "The SDL_CreateWindow method failed\n";
        exit(-1);
    }

    //The OpenGL Context (instance of OpenGL) that we will use
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);

    //Force GLEW to use expiremental draw calls, but they are supported by the card
    glewExperimental = GL_TRUE;

    //Initialize GLEW in order to ensure we don't call GL methods the driver can't understand
    if(glewInit() != GLEW_OK)
    {
        std::cout << "GLEW failed to initialize\n";
        exit(-1);
    }

    //DO SOME OPENGL STUFF HERE

    static const GLfloat redColor[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, redColor);


    SDL_GL_SwapWindow(mainWindow);
    //Create a window event in order to know when the mainWindow "Close" is pressed
    SDL_Event *windowEvent = new SDL_Event;

    while(SDL_WaitEvent(windowEvent))
    {
        if(SDL_PollEvent(windowEvent))
        {
            if(windowEvent->type == SDL_QUIT)
            {
                break;
            }
        }
    }

    //Clean up our data structures
    SDL_GL_DeleteContext(mainContext);
    return 0;
}
