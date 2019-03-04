#include "main.hpp"

#include <memory>
#include <iostream>
#include <cmath>

#include "glm/glm.hpp"

std::string programTitle = "Fragment List Example";

int main()
{
    std::cout << programTitle << "\n";

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        assert(false);
    }

    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow(programTitle.c_str(), 100, 100, 800, 600, SDL_WINDOW_OPENGL);

    //Check that the SDL/OpenGL window was created
    if(!mainWindow)
    {
        std::cout << "The SDL_CreateWindow method failed\n";
        assert(false);
    }

    //The OpenGL Context (instance of OpenGL) that we will use
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);

    //Force GLEW to use expiremental draw calls, but they are supported by the card
    glewExperimental = GL_TRUE;

    //Initialize GLEW in order to ensure we don't call GL methods the driver can't understand
    if(glewInit() != GLEW_OK)
    {
        std::cout << "GLEW failed to initialize\n";
        assert(false);
    }

    //Your graphics card must support seperate shader objects and debug callback
    assert(GLEW_ARB_separate_shader_objects);
    assert(GLEW_ARB_debug_output);

    //With the context set we will setup a OpenGL debug context callback
    glDebugMessageCallbackARB(gl_debug_callback, nullptr);

    //*************************
    //*************************
    //DO SOME OPENGL STUFF HERE
    //*************************
    //*************************

    //Create the shader pipeline that we will use to connect our shader objects to
    GLuint mainShaderPipeline = glNULL;
    glGenProgramPipelines(1, &mainShaderPipeline);
    assert(mainShaderPipeline != glNULL);

    enum ShaderPipeline
    {
        SHADER_PIPELINE_CLEAR = 0,
        SHADER_PIPELINE_RESOLVE,
        SHADER_PIPELINE_APPEND,
        NUMBER_OF_SHADER_PIPELINES,
    };

    const std::string c_ShaderPipelineFilenames [] =
    {
        "Clear",
        "Resolve",
        "Append",
    };

    static_assert(NUMBER_OF_SHADER_PIPELINES == (sizeof(c_ShaderPipelineFilenames) / sizeof(c_ShaderPipelineFilenames[0])), "c_ShaderPipelineFilenames and ShaderPipeline must be the same in order and size");

    std::vector<ShaderPipelinePrograms> shaderPipelines;
    for(int currentPipeline = 0; currentPipeline < NUMBER_OF_SHADER_PIPELINES; currentPipeline++)
    {
        ShaderPipelinePrograms newShaderPipeline;
        //Create the vertex program
        std::string shaderFilepath = QUOTE(SOURCEDIR/Source/Tutorial21/Shaders/);
        newShaderPipeline.vertexShaderProgram = CreateShaderProgram((shaderFilepath + c_ShaderPipelineFilenames[currentPipeline] + ".glsl.vert"), GL_VERTEX_SHADER);
        newShaderPipeline.fragmentShaderProgram = CreateShaderProgram((shaderFilepath + c_ShaderPipelineFilenames[currentPipeline] + ".glsl.frag"), GL_FRAGMENT_SHADER);
    }

    //Load the shapes and materials (Even though we will not use the materials)
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string tinyObjError = tinyobj::LoadObj(shapes,
                                                materials,
                                                QUOTE(MODELDIR/torus.obj),
                                                QUOTE(MODELDIR));

    //Check if any errors occured while loading
    //the wavefront obj
    if(!tinyObjError.empty())
    {
        std::cout << tinyObjError.c_str() << '\n';
        assert(0);
    }

    //Ensure that there is at least one shape in
    //the object.
    if(shapes.size() < 1)
    {
        throw "No shapes were found";
    }

    //Generate a buffer name to use
    GLuint vertexObjectBuffer = glNULL;

    glGenBuffers(1, &vertexObjectBuffer);

    //Now bind the newly created buffer to the context using the GL_ARRAY_BUFFER
    //binding point
    glBindBuffer(GL_ARRAY_BUFFER, vertexObjectBuffer);

    //With the buffer bound to our OpenGL context, we can now create the size
    //of the buffer to be (allocation of size), it will be of data source
    //squareVertices, which will be GL_STATIC_DRAW (const) (The data is copied from system memory
    //to graphics memory)
    glBufferData(GL_ARRAY_BUFFER, (shapes[0].mesh.positions.size() * sizeof(GLfloat)), &shapes[0].mesh.positions[0], GL_STATIC_DRAW);

    //This function has NOTHING to do with pointers (legacy)
    //Instead it is a pointer to a buffer object that the Vertex Shader
    //will fill up with the data we give it. (We initialize it to 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //Once we fill the glVertexAttrib buffer with the data we want
    //we will enable it, telling OpenGL to use it instead of glVertexAttrib*()
    glEnableVertexAttribArray(0);

    //**********************************
    //Setup the Element order to draw in
    //**********************************
    GLuint elementBufferObject = glNULL;

    //Generate a name for the buffer
    glGenBuffers(1, &elementBufferObject);

    //Bind it to the context
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    //Upload the data to the element buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (shapes[0].mesh.indices.size() * sizeof(GLuint)),
                  &shapes[0].mesh.indices[0],
                  GL_STATIC_DRAW);

    static const GLfloat redColor[] = { 1.0f, 0.0f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, redColor);


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

        SDL_GL_SwapWindow(mainWindow);
    }

    //Clean up our data structures
    SDL_GL_DeleteContext(mainContext);
    return 0;
}
