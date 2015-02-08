#include "Render.hpp"

Render::Render()
{
    renderWindow = nullptr;
    renderContext = nullptr;
}

Render::Render(const std::string &windowTitle,
        unsigned int windowWidth,
        unsigned int windowHeight)
{
    Render();
    CreateWindow(windowTitle, windowWidth, windowHeight);
}

Render::~Render()
{
    DestroyWindow();
}

void Render::CreateWindow(const std::string &windowTitle,
                  unsigned int windowWidth,
                  unsigned int windowHeight)
{
    //Initialize SDL2 video if it is not already
    if(!SDL_WasInit(SDL_INIT_VIDEO))
        SDL_InitSubSystem(SDL_INIT_VIDEO);

    //We will create a OpenGL 4.3 context to use debug context messaging
    //system.
    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    //Create the window
    renderWindow = SDL_CreateWindow(windowTitle.c_str(), //The title of the window
                                    100, 100, //Starting X/Y position of the newly created Window
                                    windowWidth, windowHeight, //The Width and Height of the window
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE); //The window is resizeable,OpenGL and allow HIDPI
    //Fail if the program was unable to
    //be created
    assert(renderWindow);

    //Fail if we could not get an OpenGL context
    renderContext = SDL_GL_CreateContext(renderWindow);
    assert(renderContext);

    //Set the context to the window
    SDL_GL_MakeCurrent(renderWindow, renderContext);
}

void Render::DestroyWindow()
{
    //If a window/context is already there destroy it and
    //replace it with this window
    if(renderContext)
    {
        SDL_GL_DeleteContext(renderContext);
        renderContext = nullptr;
    }

    if(renderWindow)
    {
        SDL_DestroyWindow(renderWindow);
        renderWindow = nullptr;
    }
}

//OpenGL Debugging callback
void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
const GLchar *message, GLvoid *userParam)
{
    std::cout << "GL Debug:" << message << '\n';
}
