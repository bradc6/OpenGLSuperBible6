#include "main.hpp"
#define glNULL 0
const std::string programDescription = "Simple instancing";

int main()
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

    //***************************************
    //DO SOME OPENGL STUFF HERE
    //***************************************

    //**************************************
    //**************************************
    //Create the OpenGL shader pipeline
    //**************************************
    //**************************************
    //Create the shader pipeline that we will use to connect our shader objects to
    GLuint mainShaderPipeline = glNULL;
    glGenProgramPipelines(1, &mainShaderPipeline);
    assert(mainShaderPipeline != glNULL);

    //Compile and link our shader programs that we will attach to the pipeline
    GLuint vertexShaderProgram = CreateShaderProgram(QUOTE(SOURCEDIR/Source/Tutorial24/Shaders/Main.glsl.vert), GL_VERTEX_SHADER);
    GLuint fragmentShaderProgram = CreateShaderProgram(QUOTE(SOURCEDIR/Source/Tutorial24/Shaders/Main.glsl.frag), GL_FRAGMENT_SHADER);

    //With the shader programs compiled and linked lets attach them to the pipeline
    glUseProgramStages(mainShaderPipeline, GL_VERTEX_SHADER_BIT, vertexShaderProgram);
    glUseProgramStages(mainShaderPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShaderProgram);

    //Use the pipeline
    glBindProgramPipeline(mainShaderPipeline);

    //Verify the pipeline is good to go
    glValidateProgramPipeline(mainShaderPipeline);

    //**************************************
    //**************************************
    //Create the vertice buffer/bind/update
    //**************************************
    //**************************************
    //Lets create the grass vertices
    static const GLfloat grassBladeVertices[] =
   {
       -0.3f, 0.0f,
        0.3f, 0.0f,
       -0.20f, 1.0f,
        0.1f, 1.3f,
       -0.05f, 2.3f,
        0.0f, 3.3f
   };

    //Upload the vertices to VRAM
    GLuint grassVertexBuffer = glNULL;
    glGenBuffers(1, &grassVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, grassVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassBladeVertices), grassBladeVertices, GL_STATIC_DRAW);

    GLuint vertexArrayObject = glNULL;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);



    //Create a window event in order to know when the mainWindow "Close" is pressed
    std::unique_ptr<SDL_Event> windowEvent = std::make_unique<SDL_Event>();

    while(SDL_WaitEvent(windowEvent.get()))
    {
        if(SDL_PollEvent(windowEvent.get()) && windowEvent->type == SDL_QUIT)
            break;


        static const GLfloat redColor[] = { 1.0f, 0.0f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, redColor);

        SDL_GL_SwapWindow(mainWindow);
    }

    //Clean up our data structures
    SDL_GL_DeleteContext(mainContext);

}
