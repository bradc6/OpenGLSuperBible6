//Draw a OpenGL Triangle onto the screen with different offsets.
//We will create our first simple tesselation shader

#include "main.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Draw Triangle on the screen with offsets sent to the vertex shader\n"
    		  << "Then pass the Fragment Color data from the VShader to the fragment shader (in bulk)\n";

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        assert(false);
    }

    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow("Draw Triangle onto the screen", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

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

    //We set a static color with        Red   Blue  Green Alpha
    const GLfloat redColor[] = { 1.0f, 0.0f, 0.0f, 1.0f};

    //Clear the buffer with a type (eg GL_Color ie. a color) and 0 (Unknown) and the color to set
    //the buffer.
    glClearBufferfv(GL_COLOR, 0, redColor);


    //Now lets build a vertex shader
    std::string vertexShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial8/Shaders/Main.glsl.vert));
    //Make a pointer to make glShaderSource happy (I REALLY hate this part of the solution)
    const char *sourceVertexShaderBegin = vertexShaderSource.c_str();
    //Create a instance of a vertex shader (Create a shader)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Load the shader source into our Vertex Shader instance.
    glShaderSource(vertexShader, 1, (const GLchar **) &sourceVertexShaderBegin, NULL);
    //Clear the system memory copy of the Vertex Shader Source
    vertexShaderSource.clear();
    //Now to compile the Vertex Shader
    glCompileShader(vertexShader);
    //Lets make a GLint to ensure that the shader compilation was successful
    GLint shaderCompilerStatus;
    //Get the status of the shader compiler.
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the vertex shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    //Now lets build a Tesselation Control Shader
    std::string tesselationControlShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial8/Shaders/Main.glsl.tesc));
    const char *sourceTesselationControlShaderBegin = tesselationControlShaderSource.c_str();
    GLuint tesselationControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tesselationControlShader, 1, (const GLchar **) &sourceTesselationControlShaderBegin, NULL);
    tesselationControlShaderSource.clear();
    glCompileShader(tesselationControlShader);
    //Get the status of the shader compiler.
    glGetShaderiv(tesselationControlShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the Tesselation Control shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(tesselationControlShader, 1024, NULL, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    //Now lets build a Tesselation Evaluation Shader
    std::string tesselationEvaluationShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial8/Shaders/Main.glsl.tese));
    const char *sourceTesselationEvaluationShaderBegin = tesselationEvaluationShaderSource.c_str();
    GLuint tesselationEvaluationShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tesselationEvaluationShader, 1, (const GLchar **) &sourceTesselationEvaluationShaderBegin, NULL);
    tesselationEvaluationShaderSource.clear();
    glCompileShader(tesselationEvaluationShader);
    //Get the status of the shader compiler.
    glGetShaderiv(tesselationEvaluationShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the Tesselation Evaluation shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(tesselationEvaluationShader, 1024, NULL, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    //Now lets build a fragment shader
    std::string fragmentShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial8/Shaders/Main.glsl.frag));
    const char *sourceFragmentShaderBegin = fragmentShaderSource.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar **) &sourceFragmentShaderBegin, NULL);
    fragmentShaderSource.clear();
    glCompileShader(fragmentShader);
    //Get the status of the shader compiler.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the fragment shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    //Now to use the shaders we just compiled, we need to create a shader program
    //(In a sense these are like different, pipelines that when linked can be switched out)
    GLuint shaderProgram = glCreateProgram();

    //Attach shaders to our newly created shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, tesselationControlShader);
    glAttachShader(shaderProgram, tesselationEvaluationShader);
    glAttachShader(shaderProgram, fragmentShader);

    //Link the Shader Program to create a executable shader pipeline
    //for the graphics card t ouse.
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


    glUseProgram(shaderProgram);

    //Lets enable to wireframe in order to see our tesselation in action
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        GLfloat time = ((GLfloat)SDL_GetTicks() * 750) / (GLfloat)CLOCKS_PER_SEC;

        GLfloat currentTriangleOffset[] = { (float)sin(time) * 0.5f,
                                            (float)cos(time) * 0.6f,
                                            0.0f, 0.0f };

        const GLfloat background[] = {sinf(time), cosf(time), 0.5f, 1.0f};
        //const GLfloat triangleColor[] = {cosf(time), sinf(time), cosf(time), 1.0f};
        const GLfloat triangleColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, background);

        glVertexAttrib4fv(0, currentTriangleOffset);
        glVertexAttrib4fv(1, triangleColor);

        glDrawArrays(GL_PATCHES, 0, 3);

        SDL_GL_SwapWindow(mainWindow);
    }

    //Clean Up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(tesselationControlShader);
    glDeleteShader(tesselationEvaluationShader);
    glDeleteShader(fragmentShader);

    glDeleteProgram(shaderProgram);

    //Clean up our data structures
    SDL_GL_DeleteContext(mainContext);
    return 0;
}
