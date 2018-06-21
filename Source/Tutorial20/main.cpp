//Alien rain using texture arrays

//****************************NOTICE*********************************
//NOTE: I am going to start using ShaderProgram pipelines here on out
//the old way of creating the shader programs and compiling each permutation
//feels old and unnessesary because seperate_shader is even supported on
//macOS. (Crazy right) It was also making a lot of extra code that is simply
//not nessesary. (You should know how to create a shader program at this point.
//*******************************************************************

#include "main.hpp"
#include <memory>
#include <iostream>
#include <cmath>

#include "glm/glm.hpp"

#include "sb6ktx.h"

const std::string programDescription = "Texture Array Sample";

// Random number generator
static unsigned int seed = 0x13371337;
static inline float random_float()
{
    float res;
    unsigned int tmp;

    seed *= 16807;

    tmp = seed ^ (seed >> 4) ^ (seed << 15);

    *((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}


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

    //Your graphics card must support seperate shader objects and debug callback
    assert(GLEW_ARB_separate_shader_objects);
    assert(GLEW_ARB_debug_output);

    //With the context set we will setup a OpenGL debug context callback
    glDebugMessageCallbackARB(gl_debug_callback, nullptr);
    //DO SOME OPENGL STUFF HERE

    //Create the shader pipeline that we will use to connect our shader objects to
    GLuint mainShaderPipeline = glNULL;
    glGenProgramPipelines(1, &mainShaderPipeline);
    assert(mainShaderPipeline != glNULL);

    //Compile and link our shader programs that we will attach to the pipeline
    GLuint vertexShaderProgram = CreateShaderProgram(QUOTE(SOURCEDIR/Source/Tutorial20/Shaders/Main.glsl.vert), GL_VERTEX_SHADER);
    GLuint fragmentShaderProgram = CreateShaderProgram(QUOTE(SOURCEDIR/Source/Tutorial20/Shaders/Main.glsl.frag), GL_FRAGMENT_SHADER);

    //With the shader programs compiled and linked lets attach them to the pipeline
    glUseProgramStages(mainShaderPipeline, GL_VERTEX_SHADER_BIT, vertexShaderProgram);
    glUseProgramStages(mainShaderPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShaderProgram);

    //Use the pipeline
    glBindProgramPipeline(mainShaderPipeline);

    //Verify the pipeline is good to go
    glValidateProgramPipeline(mainShaderPipeline);

    //Declare and create a vertex array object
    GLuint savedVertexAttributes;

    //Generate the Vertex Array object  on the graphics card that we
    //will later be able to save for fast reference. This only allocates the memory
    //Next we need to bind it to be active.
    glGenVertexArrays(1, &savedVertexAttributes);

    //Bind that newly created space in order to be used
    glBindVertexArray(savedVertexAttributes);

    //*********************************************
    //Load up the AlienRain texture array
    //*********************************************
    GLuint tex_alien_array = sb6::ktx::file::load(QUOTE(TEXTUREDIR/Aliens.ktx));
    assert(tex_alien_array != 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex_alien_array);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    const unsigned int c_numberOfTextures = 256;

    GLuint rain_buffer;
    glGenBuffers(1, &rain_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, rain_buffer);
    glBufferData(GL_UNIFORM_BUFFER, c_numberOfTextures * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    
    float           droplet_x_offset[c_numberOfTextures];
    float           droplet_rot_speed[c_numberOfTextures];
    float           droplet_fall_speed[c_numberOfTextures];

    for (unsigned int i = 0; i < c_numberOfTextures; i++)
    {
        droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
        droplet_rot_speed[i] = (random_float() + 0.5f) * ((i & 1) ? -3.0f : 3.0f);
        droplet_fall_speed[i] = random_float() + 0.2f;
    }

    glBindVertexArray(savedVertexAttributes);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Create a window event in order to know when the mainWindow "Close" is pressed
    std::unique_ptr<SDL_Event> windowEvent = std::make_unique<SDL_Event>();

    //Get the SDL window width and height
    int windowWidth, windowHeight;
    SDL_GetWindowSize(mainWindow, &windowWidth, &windowHeight);

    while (true)
    {
        if (SDL_PollEvent(windowEvent.get()))
        {
            if (windowEvent->type == SDL_QUIT)
            {
                break;
            }
        }

        const GLfloat milisecondsPerSecond = 1000;
        GLfloat time = static_cast<GLfloat>(SDL_GetTicks()) / milisecondsPerSecond;

        glViewport(0, 0, windowWidth, windowHeight);

        //Clear the backbuffer to black
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, black);

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, rain_buffer);

        glm::vec4 *droplet = static_cast<glm::vec4*>(glMapBufferRange(GL_UNIFORM_BUFFER, 0, c_numberOfTextures * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        assert(droplet != nullptr);
        for (int i = 0; i < 256; i++)
        {
            droplet[i][0] = droplet_x_offset[i];
            droplet[i][1] = 2.0f - fmodf((time + float(i)) * droplet_fall_speed[i], 4.31f);
            droplet[i][2] = time * droplet_rot_speed[i];
            droplet[i][3] = 0.0f;
        }
        glUnmapBuffer(GL_UNIFORM_BUFFER);

        for (unsigned int currentAlien = 0; currentAlien < c_numberOfTextures; currentAlien++)
        {
            glVertexAttribI1i(0, currentAlien);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        SDL_GL_SwapWindow(mainWindow);
    }

    return 0;
}
