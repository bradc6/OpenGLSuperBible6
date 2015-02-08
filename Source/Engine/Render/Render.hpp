#pragma once

//GLEW must be included before GL.h (or SDL_opengl.h)
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <assert.h>
#include <iostream>
#include <string>
#include <memory>


#include "../Camera/Camera.hpp"

class Render
{
    public:
        Render();
        Render(const std::string &windowTitle,
                unsigned int windowWidth,
                unsigned int windowHeight);

        ~Render();

        void CreateWindow(const std::string &windowTitle = "Simple Engine",
                          unsigned int windowWidth = 800,
                          unsigned int windowHeight = 600);
        void DestroyWindow();
    protected:

    private:
        SDL_Window *renderWindow;
        SDL_GLContext renderContext;
        Camera defaultCamera;
};
